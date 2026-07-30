# 定義編譯器為 gcc
CC = gcc

# 編譯選項，加入 -Wall 可以顯示所有的警告訊息，並將 include 目錄加入 include path
CFLAGS = -Wall -Iinclude

# 目標執行檔名稱
TARGET = main

# 原始碼目錄
SRC_DIR = src

# 來源檔案
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/ring_buffer.c

# 預設編譯目標
all: $(TARGET)

# 編譯目標執行檔的規則
# 依賴於所有的來源檔案
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# 清除編譯產生的檔案的規則
clean:
	rm -f $(TARGET)
