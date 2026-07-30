# 定義編譯器為 gcc
CC = gcc

# 編譯選項，加入 -Wall 可以顯示所有的警告訊息，-Iinclude 包含標頭檔路徑
CFLAGS = -Wall -Iinclude

# 目標執行檔名稱
TARGET = main

# 原始碼目錄
SRC_DIR = src
INC_DIR = include

# 預設編譯目標
all: $(TARGET)

# 編譯目標執行檔的規則
# 依賴於 src 目錄下的原始碼檔案
$(TARGET): $(SRC_DIR)/main.c $(SRC_DIR)/protocol.c $(INC_DIR)/protocol.h $(SRC_DIR)/ring_buffer.c $(INC_DIR)/ring_buffer.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC_DIR)/main.c $(SRC_DIR)/protocol.c $(SRC_DIR)/ring_buffer.c

# 清除編譯產生的檔案的規則
clean:
	rm -f $(TARGET)
