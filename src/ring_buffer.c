#include "ring_buffer.h"
#include <stddef.h>

void rb_init(RingBuffer *rb) {
    if (rb == NULL) return;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

bool rb_is_full(const RingBuffer *rb) {
    if (rb == NULL) return false;
    return rb->count == RING_BUFFER_SIZE;
}

bool rb_is_empty(const RingBuffer *rb) {
    if (rb == NULL) return true;
    return rb->count == 0;
}

bool rb_push(RingBuffer *rb, uint8_t data) {
    if (rb == NULL) return false;
    if (rb_is_full(rb)) {
        return false; // 緩衝區已滿，無法寫入
    }

    rb->buffer[rb->head] = data;
    rb->head = (rb->head + 1) % RING_BUFFER_SIZE; // 推進寫入指標，並處理環形折返
    rb->count++;

    return true;
}

bool rb_pop(RingBuffer *rb, uint8_t *data) {
    if (rb == NULL || data == NULL) return false;
    if (rb_is_empty(rb)) {
        return false; // 緩衝區為空，無法讀取
    }

    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE; // 推進讀取指標，並處理環形折返
    rb->count--;

    return true;
}
