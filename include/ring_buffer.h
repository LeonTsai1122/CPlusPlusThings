#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

// 定義環形緩衝區的容量
#define RING_BUFFER_SIZE 256

/**
 * 環形緩衝區結構體
 */
typedef struct {
    uint8_t buffer[RING_BUFFER_SIZE]; // 儲存資料的陣列
    uint16_t head;                    // 寫入指標
    uint16_t tail;                    // 讀取指標
    uint16_t count;                   // 目前緩衝區內的資料數量
} RingBuffer;

/**
 * 初始化環形緩衝區
 * @param rb 指向 RingBuffer 結構體的指標
 */
void rb_init(RingBuffer *rb);

/**
 * 檢查環形緩衝區是否已滿
 * @param rb 指向 RingBuffer 結構體的指標
 * @return 若滿則回傳 true，否則回傳 false
 */
bool rb_is_full(const RingBuffer *rb);

/**
 * 檢查環形緩衝區是否為空
 * @param rb 指向 RingBuffer 結構體的指標
 * @return 若空則回傳 true，否則回傳 false
 */
bool rb_is_empty(const RingBuffer *rb);

/**
 * 寫入一筆資料到環形緩衝區
 * @param rb 指向 RingBuffer 結構體的指標
 * @param data 要寫入的資料
 * @return 成功回傳 true，緩衝區已滿則回傳 false
 */
bool rb_push(RingBuffer *rb, uint8_t data);

/**
 * 從環形緩衝區讀取一筆資料
 * @param rb 指向 RingBuffer 結構體的指標
 * @param data 用來儲存讀取結果的指標
 * @return 成功回傳 true，緩衝區為空則回傳 false
 */
bool rb_pop(RingBuffer *rb, uint8_t *data);

#endif // RING_BUFFER_H
