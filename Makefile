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
	lib.o \
	riscv.o \
	vm.o

# 把核心物件檔的路徑加入到 $(OUT) 目錄下
OBJS_core := $(addprefix $(OUT)/, $(OBJS_core))

# 所有物件檔的總和
OBJS := $(OBJS_core)

# 依賴文件 (.d 檔) 的清單，用於追蹤依賴關係
deps := $(OBJS:%.o=%.o.d)

# 建立輸出目錄，確保目錄存在
SHELL_HACK := $(shell mkdir -p $(OUT))

# 可執行檔清單
EXEC = $(OUT)/rv0 $(OUT)/dump0 $(OUT)/vm0 

# 預設目標：編譯所有可執行檔
all: $(EXEC)


# 規則：將 C 源文件編譯為目標文件，並生成依賴文件 (.d)
$(OUT)/%.o: $(SRC)/%.c
	$(CC) -o $@ $(CFLAGS) -c -MMD -MF $@.d $<

# 規則：編譯 vm0 可執行檔，依賴於核心物件檔
$(OUT)/rv0: $(OBJS_core) src/rv0.c
	$(CC) $(CFLAGS) -o $@ $^
	cp $(OUT)/rv0 rv0

# 規則：編譯 vm0 可執行檔，依賴於核心物件檔
$(OUT)/vm0: $(OBJS_core) src/vm0.c
	$(CC) $(CFLAGS) -o $@ $^

# 規則：編譯 dump0，可執行檔，並且指定 source 文件
$(OUT)/dump0: $(OBJS_core) src/dump0.c
	$(CC) $(CFLAGS) -o $@ $^

dump_test:
	./$(OUT)/dump0 test/test1.o

vm_test:
	./$(OUT)/vm0 test/test1.o

rv_test:
	./$(OUT)/rv0 run0 test/test1.c

test: dump_test vm_test rv_test

# 清理目標：刪除可執行檔、物件檔與依賴文件
clean:
	$(RM) $(EXEC) $(OBJS) $(deps)

# 徹底清理：除了 clean 動作外，還刪除輸出目錄
distclean: clean
	$(RM) -r $(OUT)

# 包含依賴文件，確保編譯時的依賴關係正確處理
-include $(deps)
