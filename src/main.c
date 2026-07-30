#include <stdio.h>
#include "ring_buffer.h"

void print_buffer_status(const RingBuffer *rb) {
    printf("當前狀態 -> size: %zu, head: %zu, tail: %zu\n", rb->size, rb->head, rb->tail);
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

    return 0;
}
