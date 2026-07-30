#include <stdio.h>
#include "protocol.h"

/**
 * 程式進入點 (Main function)
 *
 * 這是 C 程式的起點，當程式執行時，會從這裡開始執行。
 *
 * @return 回傳 0 代表程式正常結束。
 */
int main() {
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
