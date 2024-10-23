# 如果 CC 沒有被指定，預設使用 gcc 編譯器
CC ?= gcc

# 指定輸出目錄，預設為 build
OUT ?= build
TEST ?= test

# 基本的編譯選項
CFLAGS := -Wall -Wextra -Wno-unused-parameter -g  # 警告選項和除錯選項
CFLAGS += --std=gnu99 -pthread                   # 使用 GNU99 標準與 pthread 庫
CFLAGS += -I include           # 預先包含 config.h 並加入 include 目錄作為頭文件路徑

# 原始碼目錄
SRC = src

# 核心部分的物件檔清單
OBJS_core := \
	dasm.o \
	elf32lib.o \
	lib.o \
	riscv.o \
	vm.o

# 把核心物件檔的路徑加入到 $(OUT) 目錄下
OBJS_core := $(addprefix $(OUT)/, $(OBJS_core))

# 套接字部分的物件檔清單
OBJS_test := \
	test1.o \
	test2.o

# 把 test *.o 的路徑加入到 $(OUT) 目錄下
OBJS_test := $(addprefix $(TEST)/, $(OBJS_test))

# 所有物件檔的總和
OBJS := $(OBJS_core) # $(OBJS_test)

# 依賴文件 (.d 檔) 的清單，用於追蹤依賴關係
deps := $(OBJS:%.o=%.o.d)

# 建立輸出目錄，確保目錄存在
SHELL_HACK := $(shell mkdir -p $(OUT))

# 可執行檔清單
EXEC = $(OUT)/dump32 # $(OUT)/vm32 

# 預設目標：編譯所有可執行檔
all: $(EXEC)

# 規則：將 C 源文件編譯為目標文件，並生成依賴文件 (.d)
$(OUT)/%.o: $(SRC)/%.c
	$(CC) -o $@ $(CFLAGS) -c -MMD -MF $@.d $<

# 規則：編譯 vm32 可執行檔，依賴於核心物件檔
$(OUT)/vm32: $(OBJS_core)
	$(CC) $(CFLAGS) -o $@ $^

# 規則：編譯 dump32，可執行檔，並且指定 source 文件
$(OUT)/dump32: $(OBJS_core)
	$(CC) $(CFLAGS) -o $@ src/dump32.c $^

# 規則：編譯 dump32，可執行檔，並且指定 source 文件
$(OUT)/vm32: $(OBJS_core)
	$(CC) $(CFLAGS) -o $@ src/vm32.c $^

dump_test:
	./$(OUT)/dump32 test/test1.o

# 清理目標：刪除可執行檔、物件檔與依賴文件
clean:
	$(RM) $(EXEC) $(OBJS) $(deps)

# 徹底清理：除了 clean 動作外，還刪除輸出目錄
distclean: clean
	$(RM) -r $(OUT)

# 包含依賴文件，確保編譯時的依賴關係正確處理
-include $(deps)
