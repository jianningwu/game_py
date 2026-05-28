#!/usr/bin/env make -f
# ============================================================================
# Makefile 全知识点教学
# - 覆盖 Makefile 核心语法：规则、变量、函数、模式、条件判断
# - 所有知识点通过 make 命令实际运行验证
# - 运行方式: make -f makefile_tutorial.mk <target>
# - 配套文档: Makefile详解与速查.md
# ============================================================================

# ============================================================================
# 0. Makefile 本质：规则驱动，依赖决定执行
# ============================================================================
# Makefile 由一系列"规则"组成，每条规则格式:
#
#   目标(target): 前置条件(prerequisites)
#   <TAB>  配方(recipe)
#
# 核心思想:
#   - 目标通常是文件名（要生成的文件）
#   - 前置条件是目标依赖的文件
#   - 配方是生成目标的 shell 命令
#   - 每个配方行必须以 TAB 开头（不是空格！）
#   - make 会比较目标与依赖的时间戳，仅当依赖更新时才重新构建
#
# 执行流程:
#   1. make 默认执行第一个目标
#   2. 检查依赖是否是最新的（递归检查）
#   3. 如果目标不存在或依赖更新，执行配方

# ============================================================================
# 1. 变量定义与使用
# ============================================================================

# --- 普通变量（递归展开）---
# 用 = 定义，使用时才展开（可能导致重复计算）
NAME         = Makefile教程
VERSION      = 1.0
AUTHOR       = 张三

# --- 立即展开变量 ---
# 用 := 定义，定义时就展开值（推荐，和常规编程语言一致）
CURRENT_TIME := $(shell date "+%Y-%m-%d %H:%M:%S")
CURRENT_DIR  := $(shell pwd)

# --- 条件赋值 ---
# ?= 仅在变量未定义时才赋值
CC          ?= gcc
CFLAGS      ?= -Wall -Wextra -O2

# --- 追加赋值 ---
# += 往变量后追加内容（自动加空格）
CFLAGS      += -std=c11

# --- 多行变量 ---
define MULTILINE_HELP
这是多行帮助信息。
第一行。
第二行。
可以包含变量引用: $(NAME) v$(VERSION)
endef

# ============================================================================
# 2. 自动变量（Make 预设的特殊变量）
# ============================================================================
# $@  当前规则的目标文件名
# $<  第一个依赖文件名
# $^  所有依赖文件列表（去重）
# $?  所有比目标新的依赖文件列表
# $*  模式规则中 % 匹配的部分（不含后缀）
# $(@D)  目标文件所在目录
# $(@F)  目标文件名（不含目录）

# 演示自动变量的规则
demo_automatic_vars:
	@echo "目标(\$$@)  : $@"
	@echo "命令示例: gcc -c \$< -o \$@   # 编译第一个依赖生成目标"

# ============================================================================
# 3. 伪目标 (.PHONY)
# ============================================================================
# .PHONY 声明目标不是文件名，每次都会执行（不检查文件时间戳）
.PHONY: help clean demo_all demo_variables demo_functions demo_pattern \
        demo_condition demo_file demo_phony demo_all_targets

# 默认目标（第一个目标）
help:
	@echo "=========================================="
	@echo "  $(NAME) v$(VERSION) by $(AUTHOR)"
	@echo "=========================================="
	@echo ""
	@echo "可用目标:"
	@echo "  make demo_all         - 运行所有演示"
	@echo "  make demo_variables   - 变量演示"
	@echo "  make demo_functions   - 函数演示"
	@echo "  make demo_pattern     - 模式规则演示"
	@echo "  make demo_condition   - 条件判断演示"
	@echo "  make demo_file        - 文件依赖演示"
	@echo "  make demo_phony       - .PHONY 演示"
	@echo "  make info             - 显示变量信息"
	@echo "  make clean            - 清理演示文件"

# ============================================================================
# 4. 常用函数演示
# ============================================================================

demo_functions:
	@echo "=== Makefile 常用函数演示 ==="

	@echo ""
	@echo "[字符串函数]"
	@echo "subst(替换):    $(subst ee,XX,keep)"
	@echo "patsubst(模式): $(patsubst %.c,%.o,main.c utils.c)"
	@echo "strip(去空格):  '$(strip    hello   world   )'"
	@echo "findstring:     $(findstring world,hello world)"
	@echo "filter(过滤):   $(filter %.c,main.c utils.h lib.c)"
	@echo "filter-out(反): $(filter-out %.c,main.c utils.h lib.c)"
	@echo "sort(排序去重): $(sort d b a c b a)"
	@echo "word(取第n词):  $(word 2,hello world C语言)"
	@echo "words(词数):    $(words hello world C语言)"
	@echo "firstword:      $(firstword hello world)"

	@echo ""
	@echo "[文件名函数]"
	@echo "dir(目录部分):   $(dir src/main.c inc/utils.h)"
	@echo "notdir(文件名):  $(notdir src/main.c inc/utils.h)"
	@echo "suffix(后缀):    $(suffix main.c utils.h)"
	@echo "basename(去后缀):$(basename main.c utils.h)"
	@echo "addsuffix:       $(addsuffix .bak,main utils)"
	@echo "addprefix:       $(addprefix src/,main.c utils.c)"
	@echo "join(拼接):      $(join a b c,.c .h .o)"

	@echo ""
	@echo "[shell 函数]"
	@echo "shell:           $(shell echo '执行 shell 命令')"

	@echo ""
	@echo "[foreach 循环]"
	@echo "$(foreach f,main utils,$(f).o <- demo)"

	@echo ""
	@echo "[call 调用自定义函数]"
	# 自定义函数（类似宏）
	reverse = $(2) $(1)
	@echo "reverse(a,b): $(call reverse,a,b)"

# ============================================================================
# 5. 条件判断演示
# ============================================================================

demo_condition:
	@echo "=== 条件判断演示 ==="

ifeq ($(CC),gcc)
	@echo "编译器是 GCC: $(CC)"
else
	@echo "编译器不是 GCC: $(CC)"
endif

ifdef CFLAGS
	@echo "CFLAGS 已定义: $(CFLAGS)"
endif

ifndef UNDEFINED_VAR
	@echo "UNDEFINED_VAR 未定义"
endif

# 条件判断可以放在规则内或规则外
# 放在规则外=全局生效，放在规则内=仅该规则生效

# ============================================================================
# 6. 模式规则与隐式规则
# ============================================================================

# 模式规则: %.o: %.c 任何 .o 文件都可以从同名的 .c 文件编译
# % 是通配符，匹配任意字符串
# 这是"隐式规则"的基础，避免为每个文件重复写类似的规则

# 示例：编译所有 .c 到 .o 的模式规则
%.o: %.c
	@echo "  编译 $< -> $@ (模式规则)"
	$(CC) $(CFLAGS) -c $< -o $@

# 静态模式规则：只对特定的目标应用模式
# OBJS: %.o: %.c  限定 $(OBJS) 中的 .o 文件使用此规则

# 自动生成依赖的演示
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
DEPS := $(SRCS:.c=.d)

demo_pattern:
	@echo "=== 模式规则演示 ==="
	@echo "源文件: $(SRCS)"
	@echo "目标文件: $(OBJS)"
	@echo "依赖文件: $(DEPS)"
	@echo ""

	@echo "模式规则: %.o : %.c"
	@echo "  任何 .o 匹配此规则，$< = 对应的 .c, $@ = .o"
	@echo ""
	@echo "静态模式规则: \$$(OBJS): %.o : %.c"
	@echo "  仅 \$$(OBJS) 中的 .o 匹配"

# ============================================================================
# 7. 文件依赖链演示
# ============================================================================

# 这是一个完整的构建依赖链
demo_file: main.o utils.o
	@echo ""
	@echo "=== 链接步骤 ==="
	@echo "链接 main.o + utils.o -> demo_file(可执行文件)"
	@echo "当 main.o 或 utils.o 比 demo_file 新时才重新链接"

main.o: main.c utils.h
	@echo "main.o 依赖: main.c, utils.h"

utils.o: utils.c utils.h
	@echo "utils.o 依赖: utils.c, utils.h"

# make 的依赖解析:
# demo_file -> main.o + utils.o
# main.o -> main.c + utils.h
# utils.o -> utils.c + utils.h
# 任一依赖变了，沿链条重新编译

# ============================================================================
# 8. 变量信息展示
# ============================================================================

info:
	@echo "=== 项目信息 ==="
	@echo "名称:       $(NAME)"
	@echo "版本:       $(VERSION)"
	@echo "作者:       $(AUTHOR)"
	@echo "编译时间:   $(CURRENT_TIME)"
	@echo "当前目录:   $(CURRENT_DIR)"
	@echo ""
	@echo "=== 编译器配置 ==="
	@echo "CC:         $(CC)"
	@echo "CFLAGS:     $(CFLAGS)"
	@echo ""
	@echo "=== 隐式变量(默认值) ==="
	@echo "AR(静态库): $(AR)"
	@echo "RM(删除):   $(RM)"

# ============================================================================
# 9. .PHONY 防冲突演示
# ============================================================================

demo_phony:
	@echo "=== .PHONY 演示 ==="
	@echo ""
	@echo "问题: 如果目录下有一个文件叫 'clean'，"
	@echo "      make clean 会认为 clean 已是最新，跳过执行"
	@echo ""
	@echo "解决: .PHONY: clean"
	@echo "      声明 clean 是伪目标，每次都执行配方"
	@echo ""
	@echo "常用伪目标: all, clean, install, test, help"

# ============================================================================
# 10. 综合示例：C 项目构建
# ============================================================================

# 变量定义
BUILD_DIR   := build
TARGET      := $(BUILD_DIR)/app
SRC_FILES   := $(wildcard *.c)
OBJ_FILES   := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
DEP_FILES   := $(OBJ_FILES:.o=.d)

# C 项目构建的完整示例
# 注意：这里只展示结构，实际需要 .c 文件存在
demo_all_targets:
	@echo "=== C 项目构建结构 ==="
	@echo ""
	@echo "all: \$$(TARGET)                    # 默认目标"
	@echo ""
	@echo "\$$(TARGET): \$$(OBJ_FILES)"
	@echo "	\$$(CC) \$$^ -o \$$@ \$$(LDFLAGS)   # 链接"
	@echo ""
	@echo "\$$(BUILD_DIR)/%.o: %.c"
	@echo "	@mkdir -p \$$(BUILD_DIR)"
	@echo "	\$$(CC) \$$(CFLAGS) -c \$$< -o \$$@    # 编译"
	@echo ""
	@echo "clean:"
	@echo "	rm -rf \$$(BUILD_DIR)"
	@echo ""
	@echo "# 自动生成依赖 .d 文件（含头文件依赖）"
	@echo "\$$(BUILD_DIR)/%.d: %.c"
	@echo "	\$$(CC) -MM \$$< > \$$@"

# ============================================================================
# 11. clean 清理
# ============================================================================

clean:
	@echo "清理演示文件..."
	rm -f demo_file main.o utils.o
	rm -rf $(BUILD_DIR)

# ============================================================================
# 12. 运行所有演示
# ============================================================================

demo_all: help demo_functions demo_condition demo_pattern demo_file demo_phony demo_all_targets
	@echo ""
	@echo "=== 全部演示完成 ==="

# ============================================================================
# 附录：特殊目标与内置变量
# ============================================================================

# 常用特殊目标:
# .PHONY:        声明伪目标
# .DEFAULT_GOAL: 指定默认目标（覆盖第一个目标）
# .SUFFIXES:     定义后缀规则
# .INTERMEDIATE: 声明中间文件（完成后自动删除）
# .SECONDARY:    声明中间文件不自动删除
# .PRECIOUS:     即使配方失败也保留目标文件
# .DELETE_ON_ERROR: 配方失败时删除目标
# .SILENT:       不打印执行的命令

# 常用内置变量:
# $(MAKE)       make 程序名
# $(MAKECMDGOALS)  命令行指定的目标
# $(MAKEFILE_LIST) 已解析的 Makefile 列表
# $(MAKELEVEL)     make 递归调用的层级
# $(CURDIR)       当前工作目录（绝对路径）
