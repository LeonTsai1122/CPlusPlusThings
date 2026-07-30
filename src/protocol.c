#include "protocol.h"
#include <stddef.h>

void protocol_parser_init(ProtocolParser *parser) {
    if (parser == NULL) return;
    parser->state = STATE_WAIT_HEADER;
    parser->checksum = 0;
    parser->data_index = 0;
    parser->packet.cmd_id = 0;
    parser->packet.payload_len = 0;
}

bool protocol_parser_feed_byte(ProtocolParser *parser, uint8_t byte) {
    if (parser == NULL) return false;

    switch (parser->state) {
        case STATE_WAIT_HEADER:
            if (byte == PROTOCOL_HEADER) {
                parser->state = STATE_READ_CMD;
                // Header 開始參與 checksum 計算
                parser->checksum = byte;
            }
            break;

        case STATE_READ_CMD:
            parser->packet.cmd_id = byte;
            parser->checksum ^= byte;
            parser->state = STATE_READ_LEN;
            break;

        case STATE_READ_LEN:
            parser->packet.payload_len = byte;
            parser->checksum ^= byte;
            // 防呆：長度若超過最大限制，則丟棄並重置
            if (parser->packet.payload_len > PROTOCOL_MAX_PAYLOAD_LEN) {
                parser->state = STATE_WAIT_HEADER;
            } else if (parser->packet.payload_len == 0) {
                // 如果長度為 0，直接進 checksum
                parser->state = STATE_CHECK_SUM;
            } else {
                parser->data_index = 0;
                parser->state = STATE_READ_DATA;
            }
            break;

        case STATE_READ_DATA:
            parser->packet.data[parser->data_index++] = byte;
            parser->checksum ^= byte;
            if (parser->data_index >= parser->packet.payload_len) {
                parser->state = STATE_CHECK_SUM;
            }
            break;

        case STATE_CHECK_SUM:
            // 收到最後一個 byte 並與我們計算的 checksum 進行比對
            if (byte == parser->checksum) {
                // 成功解析
                parser->state = STATE_WAIT_HEADER; // 準備接下一個
                return true;
            } else {
                // 錯誤，重置
                parser->state = STATE_WAIT_HEADER;
            }
            break;

        default:
            parser->state = STATE_WAIT_HEADER;
            break;
    }

    return false;
}
