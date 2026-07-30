#include <stdio.h>
#include "protocol.h"
#include "ring_buffer.h"

/**
 * 測試環形緩衝區功能
 */
void test_ring_buffer() {
    printf("Ring Buffer Test\n");
    printf("=========================\n");

    RingBuffer rb;
    rb_init(&rb);

    printf("1. 初始狀態:\n");
    printf("   Is Empty? %s\n", rb_is_empty(&rb) ? "Yes" : "No");
    printf("   Is Full? %s\n", rb_is_full(&rb) ? "Yes" : "No");

    printf("\n2. 寫入資料 (Push):\n");
    for (uint8_t i = 1; i <= 5; i++) {
        if (rb_push(&rb, i * 10)) {
            printf("   成功寫入: %d\n", i * 10);
        } else {
            printf("   寫入失敗: 緩衝區已滿\n");
        }
    }

    printf("\n3. 讀取部分資料 (Pop):\n");
    uint8_t data;
    for (int i = 0; i < 3; i++) {
        if (rb_pop(&rb, &data)) {
            printf("   成功讀取: %d\n", data);
        } else {
            printf("   讀取失敗: 緩衝區為空\n");
        }
    }

    printf("\n4. 再寫入資料 (觸發環形折返):\n");
    for (uint8_t i = 6; i <= 8; i++) {
        if (rb_push(&rb, i * 10)) {
            printf("   成功寫入: %d\n", i * 10);
        }
    }

    printf("\n5. 讀出剩餘所有資料:\n");
    while (!rb_is_empty(&rb)) {
        if (rb_pop(&rb, &data)) {
            printf("   成功讀取: %d\n", data);
        }
    }

    printf("\n");
}

/**
 * 程式進入點 (Main function)
 *
 * 這是 C 程式的起點，當程式執行時，會從這裡開始執行。
 *
 * @return 回傳 0 代表程式正常結束。
 */
int main() {
    // 執行環形緩衝區測試
    test_ring_buffer();

    printf("Serial Packet Parser Test\n");
    printf("=========================\n");

    // 準備一個測試封包
    // Header(0xAA), CMD(0x01), LEN(0x03), Payload(0x11, 0x22, 0x33), Checksum
    uint8_t test_data[] = {
        0xAA, // Header
        0x01, // Command
        0x03, // Length
        0x11, 0x22, 0x33 // Payload
    };

    // 計算預期的 Checksum: 0xAA ^ 0x01 ^ 0x03 ^ 0x11 ^ 0x22 ^ 0x33 = 0x88
    uint8_t expected_checksum = 0xAA ^ 0x01 ^ 0x03 ^ 0x11 ^ 0x22 ^ 0x33;

    // 將完整的封包組裝好
    uint8_t full_packet[7];
    for(int i=0; i<6; i++) full_packet[i] = test_data[i];
    full_packet[6] = expected_checksum;

    printf("Simulating receiving bytes: ");
    for(int i=0; i<7; i++) {
        printf("%02X ", full_packet[i]);
    }
    printf("\n\n");

    // 建立解析器實例並初始化
    SerialPacket packet;
    init_packet_parser(&packet);

    // 模擬逐字元接收並解析
    for (int i = 0; i < sizeof(full_packet); i++) {
        if (parse_byte(&packet, full_packet[i])) {
            printf(">>> Packet successfully parsed!\n");
            printf("Header: 0x%02X\n", packet.header);
            printf("CMD: 0x%02X\n", packet.cmd);
            printf("LEN: %d\n", packet.len);
            printf("Payload: ");
            for (int j = 0; j < packet.len; j++) {
                printf("0x%02X ", packet.payload[j]);
            }
            printf("\nChecksum: 0x%02X\n", packet.checksum);
        }
    }

    return 0;
}
