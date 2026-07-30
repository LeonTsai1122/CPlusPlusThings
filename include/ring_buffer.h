#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// 定義環形緩衝區的固定大小
#define RING_BUFFER_SIZE 256

/**
 * 環形緩衝區結構體
 */
typedef struct {
    uint8_t buffer[RING_BUFFER_SIZE]; // 固定大小的陣列用來存放資料
    size_t head;                      // 寫入指標（下次寫入的索引位置）
    size_t tail;                      // 讀取指標（下次讀取的索引位置）
    size_t size;                      // 目前緩衝區內儲存的資料數量
} RingBuffer;

/**
 * 初始化環形緩衝區
 * @param rb 緩衝區結構體的指標
 */
void ring_buffer_init(RingBuffer *rb);

/**
 * 檢查環形緩衝區是否已滿
 * @param rb 緩衝區結構體的指標
 * @return true 如果滿了，false 如果未滿
 */
bool ring_buffer_is_full(const RingBuffer *rb);

/**
 * 檢查環形緩衝區是否為空
 * @param rb 緩衝區結構體的指標
 * @return true 如果空了，false 如果未空
 */
bool ring_buffer_is_empty(const RingBuffer *rb);

/**
 * 寫入資料到環形緩衝區
 * @param rb 緩衝區結構體的指標
 * @param data 要寫入的資料
 * @return true 表示寫入成功，false 表示寫入失敗（例如已滿，防止溢位）
 */
bool ring_buffer_push(RingBuffer *rb, uint8_t data);

/**
 * 從環形緩衝區讀取資料
 * @param rb 緩衝區結構體的指標
 * @param data 用來接收讀取資料的指標
 * @return true 表示讀取成功，false 表示讀取失敗（例如為空）
 */
bool ring_buffer_pop(RingBuffer *rb, uint8_t *data);

#endif // RING_BUFFER_H
