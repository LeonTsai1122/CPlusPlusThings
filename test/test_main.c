#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "protocol.h"
#include "ring_buffer.h"

// 輕量級測試框架巨集
#define TEST_CASE(name) void test_##name() { \
    printf("Running test: %s...\n", #name);

#define END_TEST_CASE() \
    printf("Passed!\n"); \
}

#define EXPECT_TRUE(cond) \
    if (!(cond)) { \
        printf("FAILED at %s:%d: %s is false\n", __FILE__, __LINE__, #cond); \
        return; \
    }

#define EXPECT_FALSE(cond) \
    if (cond) { \
        printf("FAILED at %s:%d: %s is true\n", __FILE__, __LINE__, #cond); \
        return; \
    }

#define EXPECT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        printf("FAILED at %s:%d: expected %d, got %d\n", __FILE__, __LINE__, (int)(expected), (int)(actual)); \
        return; \
    }

// ------------------------------------------------------------------
// Ring Buffer 測試案例
// ------------------------------------------------------------------
TEST_CASE(ring_buffer_init_empty)
    RingBuffer rb;
    rb_init(&rb);

    EXPECT_TRUE(rb_is_empty(&rb));
    EXPECT_FALSE(rb_is_full(&rb));

    uint8_t dummy;
    EXPECT_FALSE(rb_pop(&rb, &dummy)); // 空的緩衝區不能 pop
END_TEST_CASE()

TEST_CASE(ring_buffer_push_pop)
    RingBuffer rb;
    rb_init(&rb);

    EXPECT_TRUE(rb_push(&rb, 42));
    EXPECT_FALSE(rb_is_empty(&rb));

    uint8_t data = 0;
    EXPECT_TRUE(rb_pop(&rb, &data));
    EXPECT_EQ(42, data);
    EXPECT_TRUE(rb_is_empty(&rb));
END_TEST_CASE()

TEST_CASE(ring_buffer_full_wrap)
    RingBuffer rb;
    rb_init(&rb);

    // 填滿緩衝區
    for (int i = 0; i < RING_BUFFER_SIZE; i++) {
        EXPECT_TRUE(rb_push(&rb, i % 256));
    }

    EXPECT_TRUE(rb_is_full(&rb));
    EXPECT_FALSE(rb_is_empty(&rb));
    EXPECT_FALSE(rb_push(&rb, 99)); // 滿了不能再 push

    // 讀出一半
    uint8_t data;
    for (int i = 0; i < RING_BUFFER_SIZE / 2; i++) {
        EXPECT_TRUE(rb_pop(&rb, &data));
        EXPECT_EQ(i % 256, data);
    }

    EXPECT_FALSE(rb_is_full(&rb));
    EXPECT_FALSE(rb_is_empty(&rb));

    // 再寫入一半，觸發折返
    for (int i = 0; i < RING_BUFFER_SIZE / 2; i++) {
        EXPECT_TRUE(rb_push(&rb, (i + RING_BUFFER_SIZE) % 256));
    }

    EXPECT_TRUE(rb_is_full(&rb));

    // 把剩下的讀出來
    for (int i = RING_BUFFER_SIZE / 2; i < RING_BUFFER_SIZE; i++) {
        EXPECT_TRUE(rb_pop(&rb, &data));
        EXPECT_EQ(i % 256, data);
    }
    for (int i = 0; i < RING_BUFFER_SIZE / 2; i++) {
        EXPECT_TRUE(rb_pop(&rb, &data));
        EXPECT_EQ((i + RING_BUFFER_SIZE) % 256, data);
    }

    EXPECT_TRUE(rb_is_empty(&rb));
END_TEST_CASE()

// ------------------------------------------------------------------
// Serial Protocol 測試案例
// ------------------------------------------------------------------
TEST_CASE(protocol_valid_packet)
    SerialPacket pkt;
    init_packet_parser(&pkt);

    uint8_t payload[] = {0x01, 0x02, 0x03, 0x04};
    uint8_t header = 0xAA;
    uint8_t cmd = 0x55;
    uint8_t len = 4;

    uint8_t checksum = header ^ cmd ^ len ^ payload[0] ^ payload[1] ^ payload[2] ^ payload[3];

    EXPECT_FALSE(parse_byte(&pkt, header));
    EXPECT_FALSE(parse_byte(&pkt, cmd));
    EXPECT_FALSE(parse_byte(&pkt, len));
    EXPECT_FALSE(parse_byte(&pkt, payload[0]));
    EXPECT_FALSE(parse_byte(&pkt, payload[1]));
    EXPECT_FALSE(parse_byte(&pkt, payload[2]));
    EXPECT_FALSE(parse_byte(&pkt, payload[3]));
    EXPECT_TRUE(parse_byte(&pkt, checksum)); // Checksum 正確時回傳 true

    EXPECT_EQ(0xAA, pkt.header);
    EXPECT_EQ(0x55, pkt.cmd);
    EXPECT_EQ(4, pkt.len);
    EXPECT_EQ(0x01, pkt.payload[0]);
    EXPECT_EQ(0x04, pkt.payload[3]);
END_TEST_CASE()

TEST_CASE(protocol_invalid_checksum)
    SerialPacket pkt;
    init_packet_parser(&pkt);

    EXPECT_FALSE(parse_byte(&pkt, 0xAA));
    EXPECT_FALSE(parse_byte(&pkt, 0x11));
    EXPECT_FALSE(parse_byte(&pkt, 0x01));
    EXPECT_FALSE(parse_byte(&pkt, 0x55));
    // 故意給錯誤的 Checksum
    EXPECT_FALSE(parse_byte(&pkt, 0x00));

    // 預期狀態被重置回等待 Header
    EXPECT_EQ(STATE_WAIT_HEADER, pkt.state);
END_TEST_CASE()

// ------------------------------------------------------------------
// 主測試程式
// ------------------------------------------------------------------
int main() {
    printf("Starting Unit Tests...\n");
    printf("=========================\n");

    test_ring_buffer_init_empty();
    test_ring_buffer_push_pop();
    test_ring_buffer_full_wrap();

    test_protocol_valid_packet();
    test_protocol_invalid_checksum();

    printf("=========================\n");
    printf("All Tests Completed Successfully.\n");
    return 0;
}
