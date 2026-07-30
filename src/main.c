#include <stdio.h>
#include "ring_buffer.h"
#include "protocol.h"

void print_buffer_status(const RingBuffer *rb) {
    printf("當前狀態 -> size: %zu, head: %zu, tail: %zu\n", rb->size, rb->head, rb->tail);
}

void print_packet(const Packet *pkt) {
    printf("解析成功! Cmd ID: 0x%02X, Length: %d, Data: ", pkt->cmd_id, pkt->payload_len);
    for (int i = 0; i < pkt->payload_len; i++) {
        printf("0x%02X ", pkt->data[i]);
    }
    printf("\n");
}

int main() {
    RingBuffer rb;

    // 1. 初始化
    printf("=== 初始化 RingBuffer ===\n");
    ring_buffer_init(&rb);
    print_buffer_status(&rb);
    printf("\n");

    // 2. 模擬寫入 5 個 Byte
    printf("=== 寫入 5 個 Byte ===\n");
    for (uint8_t i = 1; i <= 5; ++i) {
        if (ring_buffer_push(&rb, i)) {
            printf("成功寫入: %d\n", i);
        } else {
            printf("寫入失敗 (滿了)\n");
        }
    }
    print_buffer_status(&rb);
    printf("\n");

    // 3. 模擬讀取 3 個 Byte
    printf("=== 讀取 3 個 Byte ===\n");
    for (int i = 0; i < 3; ++i) {
        uint8_t data = 0;
        if (ring_buffer_pop(&rb, &data)) {
            printf("成功讀取: %d\n", data);
        } else {
            printf("讀取失敗 (空的)\n");
        }
    }
    print_buffer_status(&rb);
    printf("\n");

    // 再寫入測試溢位機制
    printf("=== 寫入至滿以測試溢位防護 (寫入 254 個 Byte) ===\n");
    for (uint16_t i = 0; i < 254; ++i) {
        ring_buffer_push(&rb, (uint8_t)(i % 256));
    }
    print_buffer_status(&rb);

    printf("=== 嘗試再寫入 1 個 Byte (應該會失敗) ===\n");
    if (ring_buffer_push(&rb, 99)) {
        printf("成功寫入: 99\n");
    } else {
        printf("寫入失敗 (滿了)，成功防止溢位！\n");
    }
    print_buffer_status(&rb);

    // ==========================================
    // 新增：演示 Protocol Parser 結合 RingBuffer
    // ==========================================
    printf("\n=== Protocol Parser 與 RingBuffer 整合測試 ===\n");

    // 清空 buffer
    ring_buffer_init(&rb);

    ProtocolParser parser;
    protocol_parser_init(&parser);

    // 構造一個測試封包: [Header 0xAA][Cmd 0x01][Len 0x03][Data 0x11, 0x22, 0x33][Checksum]
    // Checksum = 0xAA ^ 0x01 ^ 0x03 ^ 0x11 ^ 0x22 ^ 0x33 = 0xA8
    uint8_t test_packet[] = {0xAA, 0x01, 0x03, 0x11, 0x22, 0x33, 0xA8};

    // 加上一些雜訊在前面
    uint8_t noise[] = {0x12, 0x34, 0x56};
    for (int i = 0; i < sizeof(noise); i++) {
        ring_buffer_push(&rb, noise[i]);
    }

    // 將完整封包寫入 RingBuffer
    for (int i = 0; i < sizeof(test_packet); i++) {
        ring_buffer_push(&rb, test_packet[i]);
    }

    printf("RingBuffer 寫入雜訊與完整封包後：\n");
    print_buffer_status(&rb);

    printf("\n開始從 RingBuffer 讀取並解析...\n");
    uint8_t byte;
    while (ring_buffer_pop(&rb, &byte)) {
        printf("取出: 0x%02X -> ", byte);
        if (protocol_parser_feed_byte(&parser, byte)) {
            print_packet(&parser.packet);
        } else {
            printf("解析中...\n");
        }
    }

    return 0;
}
