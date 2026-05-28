# ==========================================================================
#  数据结构库 Makefile
#  支持 MinGW (gcc) 和 MSVC (cl)
# ==========================================================================

# ---------- 工具链检测 ----------
ifdef ComSpec
    # Windows 环境
    RM = del /Q
    ifeq ($(CC),)
        CC = gcc
    endif
else
    RM = rm -f
endif

# ---------- 目录 ----------
SRC_DIR  = src
INC_DIR  = include
TEST_DIR = tests
BUILD_DIR = build

# ---------- 源文件 ----------
SRCS     = $(wildcard $(SRC_DIR)/*.c)
OBJS     = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TEST_SRCS = $(wildcard $(TEST_DIR)/test_*.c)
TEST_OBJS = $(TEST_SRCS:$(TEST_DIR)/%.c=$(BUILD_DIR)/%.o)

# ---------- 编译选项 ----------
CFLAGS   = -Wall -Wextra -Werror -Wpedantic -std=c99 -O2 -I$(INC_DIR)
LDFLAGS  =

# 平台特定链接库
# Windows: 多媒体库 (timeGetTime)
# Linux:   pthread 线程库
ifdef ComSpec
    LDFLAGS += -lwinmm
else
    LDFLAGS += -lpthread
endif

# ---------- 目标 ----------
.PHONY: all clean test

all: $(BUILD_DIR) libds.a test_runner

# 创建构建目录
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# ---------- 静态库 ----------
libds.a: $(OBJS)
	ar rcs $@ $(OBJS)

# ---------- 测试运行器 ----------
TEST_RUNNER_OBJS = $(TEST_OBJS) $(BUILD_DIR)/run_tests.o $(filter-out $(BUILD_DIR)/main.o, $(OBJS))

test_runner: $(TEST_RUNNER_OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

# ---------- 编译规则 ----------
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# ---------- 运行测试 ----------
test: test_runner
	./test_runner

# ---------- 清理 ----------
clean:
	rm -rf $(BUILD_DIR) libds.a test_runner.exe test_runner

# ==========================================================================
#  示例：编译并使用
# ==========================================================================
#  gcc -Iinclude src/darray.c src/dlist.c src/mqueue.c src/htable.c src/btree.c src/graph.c -o libds_test
