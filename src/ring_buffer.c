#include "ring_buffer.h"
#include <stddef.h> // size_t, NULL

void ring_buffer_init(RingBuffer *rb) {
    if (rb == NULL) {
        return;
    }
    // 初始化指標與大小皆為 0
    rb->head = 0;
    rb->tail = 0;
    rb->size = 0;
}

bool ring_buffer_is_full(const RingBuffer *rb) {
    if (rb == NULL) {
        return false; // 或者視為一種錯誤狀態，這裡回傳 false 方便處理
    }
    // 當緩衝區內的資料量達到陣列的最大容量，代表已滿
    return rb->size == RING_BUFFER_SIZE;
}

bool ring_buffer_is_empty(const RingBuffer *rb) {
    if (rb == NULL) {
        return true; // 視為空
    }
    // 當緩衝區內的資料量為 0，代表是空的
    return rb->size == 0;
}

bool ring_buffer_push(RingBuffer *rb, uint8_t data) {
    if (rb == NULL) {
        return false;
    }

    // 如果滿了，防止溢位，拒絕寫入
    if (ring_buffer_is_full(rb)) {
        return false;
    }

    // 將資料寫入目前的 head 指標位置
    rb->buffer[rb->head] = data;

    // 將 head 指標往前移，並且透過 modulo 運算來達到環繞 (wrap-around) 的效果
    rb->head = (rb->head + 1) % RING_BUFFER_SIZE;

    // 更新資料量
    rb->size++;

    return true;
}

bool ring_buffer_pop(RingBuffer *rb, uint8_t *data) {
    if (rb == NULL || data == NULL) {
        return false;
    }

    // 如果空的，沒有資料可以讀取
    if (ring_buffer_is_empty(rb)) {
        return false;
    }

    // 將目前的 tail 指標位置的資料讀出
    *data = rb->buffer[rb->tail];

    // 將 tail 指標往前移，並且透過 modulo 運算來達到環繞 (wrap-around) 的效果
    rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE;

    // 更新資料量
    rb->size--;

    return true;
}
