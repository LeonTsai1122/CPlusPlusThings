#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>

// 封包常數定義
#define PACKET_HEADER 0xAA
#define MAX_PAYLOAD_LEN 256

// 解析器狀態機定義
typedef enum {
    STATE_WAIT_HEADER = 0,
    STATE_WAIT_CMD,
    STATE_WAIT_LEN,
    STATE_WAIT_PAYLOAD,
    STATE_WAIT_CHECKSUM,
    STATE_PACKET_COMPLETE,
    STATE_PACKET_ERROR
} ParserState;

// 封包結構體定義
typedef struct {
    uint8_t header;
    uint8_t cmd;
    uint8_t len;
    uint8_t payload[MAX_PAYLOAD_LEN];
    uint8_t checksum;

    // 以下為內部解析狀態使用
    ParserState state;
    uint16_t payload_idx;
    uint8_t calc_checksum;
} SerialPacket;

// 初始化封包解析器
void init_packet_parser(SerialPacket *pkt);

// 逐字元解析函式
// 收到完整且校驗正確的封包時回傳 true
bool parse_byte(SerialPacket *pkt, uint8_t b);

#endif // PROTOCOL_H
