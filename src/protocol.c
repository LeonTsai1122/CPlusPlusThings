#include "protocol.h"
#include <stddef.h>

void init_packet_parser(SerialPacket *pkt) {
    if (pkt == NULL) return;

    pkt->state = STATE_WAIT_HEADER;
    pkt->payload_idx = 0;
    pkt->calc_checksum = 0;
}

bool parse_byte(SerialPacket *pkt, uint8_t b) {
    if (pkt == NULL) return false;

    switch (pkt->state) {
        case STATE_WAIT_HEADER:
            if (b == PACKET_HEADER) {
                pkt->header = b;
                pkt->calc_checksum = b;
                pkt->state = STATE_WAIT_CMD;
            }
            break;

        case STATE_WAIT_CMD:
            pkt->cmd = b;
            pkt->calc_checksum ^= b;
            pkt->state = STATE_WAIT_LEN;
            break;

        case STATE_WAIT_LEN:
            pkt->len = b;
            pkt->calc_checksum ^= b;
            pkt->payload_idx = 0;
            if (pkt->len > 0) {
                pkt->state = STATE_WAIT_PAYLOAD;
            } else {
                pkt->state = STATE_WAIT_CHECKSUM;
            }
            break;

        case STATE_WAIT_PAYLOAD:
            if (pkt->payload_idx < MAX_PAYLOAD_LEN && pkt->payload_idx < pkt->len) {
                pkt->payload[pkt->payload_idx] = b;
                pkt->calc_checksum ^= b;
                pkt->payload_idx++;

                if (pkt->payload_idx == pkt->len) {
                    pkt->state = STATE_WAIT_CHECKSUM;
                }
            } else {
                // 長度超過緩衝區大小，直接重置
                pkt->state = STATE_WAIT_HEADER;
            }
            break;

        case STATE_WAIT_CHECKSUM:
            pkt->checksum = b;
            if (pkt->checksum == pkt->calc_checksum) {
                pkt->state = STATE_PACKET_COMPLETE;
                return true; // 成功解析出一個完整封包
            } else {
                pkt->state = STATE_WAIT_HEADER; // Checksum error, reset
            }
            break;

        case STATE_PACKET_COMPLETE:
        case STATE_PACKET_ERROR:
            // 等待下一次解析，這裡應該在呼叫端呼叫 init_packet_parser
            // 但如果直接傳新資料過來，我們也可以嘗試當作 header
            if (b == PACKET_HEADER) {
                pkt->header = b;
                pkt->calc_checksum = b;
                pkt->state = STATE_WAIT_CMD;
            } else {
                pkt->state = STATE_WAIT_HEADER;
            }
            break;
    }

    return false;
}
