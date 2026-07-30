#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>

// 定義 Header 特徵值
#define PROTOCOL_HEADER 0xAA
// 定義 Payload 最大長度，避免溢位
#define PROTOCOL_MAX_PAYLOAD_LEN 128

/**
 * 封包解析器的狀態定義
 */
typedef enum {
    STATE_WAIT_HEADER,
    STATE_READ_CMD,
    STATE_READ_LEN,
    STATE_READ_DATA,
    STATE_CHECK_SUM
} ParserState;

/**
 * 封包結構體 (解析出來的結果)
 */
typedef struct {
    uint8_t cmd_id;
    uint8_t payload_len;
    uint8_t data[PROTOCOL_MAX_PAYLOAD_LEN];
} Packet;

/**
 * 封包解析器結構體 (FSM)
 */
typedef struct {
    ParserState state;
    uint8_t checksum;      // 計算中的 XOR checksum
    uint8_t data_index;    // 接收 data 的索引
    Packet packet;         // 存放解析中的封包內容
} ProtocolParser;

/**
 * 初始化封包解析器
 * @param parser 解析器指標
 */
void protocol_parser_init(ProtocolParser *parser);

/**
 * 送入單個 byte 進行解析
 * @param parser 解析器指標
 * @param byte 傳入的 byte
 * @return true 表示成功解析出一個完整的合法封包，false 表示還在解析中或資料有誤
 */
bool protocol_parser_feed_byte(ProtocolParser *parser, uint8_t byte);

#endif // PROTOCOL_H
