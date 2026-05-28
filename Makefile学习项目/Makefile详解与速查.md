# Makefile 详解与速查

> Makefile 是 C/C++ 项目的构建标准，也是 CI/CD 和自动化任务的常用工具。
> 更新日期：2026年5月

---

## 一、Makefile 是什么

Makefile 是一个**构建规则文件**，告诉 `make` 如何编译和链接程序。

```
核心原理: 依赖驱动的增量构建
  - 每个"规则"定义了: 目标 <- 依赖（原料）
  - make 比较目标和依赖的时间戳
  - 仅当依赖比目标新（或目标不存在）时才重新构建
```

---

## 二、基本语法

### 规则格式

```makefile
# 每条规则的格式
目标(target): 前置条件(prerequisites)
<TAB>   配方(recipe)
<TAB>   更多命令...

# 例子
main.o: main.c utils.h
    gcc -c main.c -o main.o         # <- 这行前面是 TAB，不是空格！
```

### 关键规则

- **目标**通常是文件名，如 `main.o`、`program`
- **前置条件**是目标依赖的文件列表，空格分隔
- **配方**是 shell 命令，每行**必须以 TAB 开头**（绝不能是空格）
- make 会比较目标与依赖的时间戳，仅当依赖更新时才重新构建

---

## 三、变量

### 变量定义

```makefile
# = 递归展开（延迟求值）
NAME = $(shell date)    # 每次使用 NAME 时都会重新执行 date

# := 立即展开（推荐！）
NOW := $(shell date)    # 定义时就求值，后面不变

# ?= 条件赋值（仅当未定义时）
CC ?= gcc

# += 追加
CFLAGS += -O2

# define ... endef  多行变量
define HELP_TEXT
这是第一行
这是第二行
endef
```

### 自动变量（最重要的几个）

| 变量 | 含义 | 示例 |
|------|------|------|
| `$@` | 当前目标文件名 | `prog: main.o; echo $@` -> `prog` |
| `$<` | 第一个依赖文件名 | `prog: main.o utils.o; echo $<` -> `main.o` |
| `$^` | 所有依赖文件（去重） | `prog: a.o b.o; gcc $^ -o $@` |
| `$?` | 所有比目标新的依赖 | 增量构建时有用 |
| `$*` | 模式规则中 `%` 匹配的部分 | `%.o: %.c; echo $*` -> `main` |

### 特殊变量

| 变量 | 含义 |
|------|------|
| `$(MAKE)` | make 程序名，递归调用用 |
| `$(MAKECMDGOALS)` | 命令行指定的目标列表 |
| `$(CURDIR)` | 当前工作目录（绝对路径） |
| `$(MAKEFILE_LIST)` | 已解析的 Makefile 列表 |

---

## 四、伪目标 (.PHONY)

```makefile
.PHONY: clean all install

clean:
    rm -f *.o program

all: program
```

**为什么需要 .PHONY？** 如果目录下碰巧有一个文件叫 `clean`，`make clean` 会认为目标已是最新而跳过。`.PHONY` 强制每次都执行。

常用伪目标: `all`, `clean`, `install`, `test`, `help`, `dist`

---

## 五、函数

### 字符串处理

| 函数 | 作用 | 示例 |
|------|------|------|
| `$(subst from,to,text)` | 字面替换 | `$(subst ee,XX,keep)` -> `kXXp` |
| `$(patsubst pat,rep,text)` | 模式替换 | `$(patsubst %.c,%.o,main.c)` -> `main.o` |
| `$(strip string)` | 去除首尾空格 | - |
| `$(findstring find,in)` | 查找子串 | 找到返回子串，否则空 |
| `$(filter pat,text)` | 保留匹配项 | `$(filter %.c,*.c *.h)` -> 只有 .c |
| `$(filter-out pat,text)` | 移除匹配项 | `$(filter-out %.h,*.c *.h)` -> 只有 .c |
| `$(sort list)` | 排序并去重 | - |
| `$(word n,text)` | 取第 n 个词 | `$(word 2,a b c)` -> `b` |
| `$(words text)` | 统计词数 | - |
| `$(firstword text)` | 取第一个词 | - |

### 文件名处理

| 函数 | 作用 | 示例 |
|------|------|------|
| `$(dir names...)` | 提取目录部分 | `$(dir src/main.c)` -> `src/` |
| `$(notdir names...)` | 提取文件名 | `$(notdir src/main.c)` -> `main.c` |
| `$(suffix names...)` | 提取后缀 | `$(suffix main.c)` -> `.c` |
| `$(basename names...)` | 去后缀 | `$(basename main.c)` -> `main` |
| `$(addsuffix suf,names)` | 加后缀 | `$(addsuffix .o,main)` -> `main.o` |
| `$(addprefix pre,names)` | 加前缀 | `$(addprefix src/,main.c)` -> `src/main.c` |
| `$(join list1,list2)` | 逐对拼接 | `$(join a b,.c .h)` -> `a.c b.h` |
| `$(wildcard pat)` | 通配符展开 | `$(wildcard src/*.c)` |
| `$(realpath names)` | 转绝对路径 | - |

### 流程控制

| 函数 | 作用 | 示例 |
|------|------|------|
| `$(foreach var,list,text)` | 循环 | `$(foreach f,a b,$(f).o)` -> `a.o b.o` |
| `$(if cond,then,else)` | 条件 | `$(if $(CC),$(CC),gcc)` |
| `$(call var,param,...)` | 调用宏 | `reverse=$(2) $(1); $(call reverse,a,b)` -> `b a` |
| `$(eval text)` | 动态求值 | 运行时生成 Makefile 内容 |
| `$(shell cmd)` | 执行 shell 命令 | `$(shell date)` -> 当前日期 |
| `$(error text)` | 报错退出 | - |
| `$(warning text)` | 警告（继续） | - |
| `$(info text)` | 打印信息 | - |

---

## 六、条件判断

```makefile
# 条件判断必须在规则外（或配方中）
ifeq ($(CC),gcc)
    CFLAGS += -Wall        # 仅 GCC 时生效
else
    CFLAGS += /W4          # 其他编译器
endif

ifdef DEBUG
    CFLAGS += -g -DDEBUG
endif

ifndef VERSION
    VERSION := 0.0.1
endif

# 也可以在配方中使用（注意缩进格式）
target:
ifeq ($(OS),Windows_NT)
    @echo "Windows"
else
    @echo "Linux/Mac"
endif
```

---

## 七、模式规则与隐式规则

### 模式规则

```makefile
# %.o: %.c  任何 .o 文件都可以从同名 .c 编译
%.o: %.c
    $(CC) -c $< -o $@

# 有了这条规则，不需要为每个 .c 写单独的规则
# main.o 会自动匹配 -> gcc -c main.c -o main.o
```

### 静态模式规则

```makefile
# 只对特定目标生效
OBJS = main.o utils.o
$(OBJS): %.o: %.c
    $(CC) -c $< -o $@

# 等价于:
# main.o: main.c
# utils.o: utils.c
# 但更简洁
```

### 自动生成头文件依赖

```makefile
# gcc -MM 生成 .d 依赖文件
%.d: %.c
    @$(CC) -MM $< > $@

# 包含所有 .d 文件
-include $(SRCS:.c=.d)
```

---

## 八、C/C++ 项目标准模板

```makefile
# ===== 编译器配置 =====
CC       := gcc
CXX      := g++
CFLAGS   := -Wall -Wextra -O2 -std=c11
CXXFLAGS := -Wall -Wextra -O2 -std=c++17
LDFLAGS  := -lm

# ===== 目录 =====
SRC_DIR  := src
INC_DIR  := include
BUILD_DIR:= build
TARGET   := $(BUILD_DIR)/program

# ===== 源文件自动收集 =====
SRCS     := $(wildcard $(SRC_DIR)/*.c)
OBJS     := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS     := $(OBJS:.o=.d)

# ===== 默认目标 =====
.PHONY: all clean
all: $(TARGET)

# ===== 链接 =====
$(TARGET): $(OBJS)
    @mkdir -p $(BUILD_DIR)
    $(CC) $^ -o $@ $(LDFLAGS)

# ===== 编译（模式规则） =====
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
    @mkdir -p $(BUILD_DIR)
    $(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# ===== 依赖文件 =====
-include $(DEPS)
$(BUILD_DIR)/%.d: $(SRC_DIR)/%.c
    @mkdir -p $(BUILD_DIR)
    @$(CC) -MM -I$(INC_DIR) $< > $@

# ===== 清理 =====
clean:
    rm -rf $(BUILD_DIR)
```

---

## 九、常见技巧

### 1. 静默执行

```makefile
# 配方前加 @ 不打印命令本身
target:
    @echo "这行不打印命令"    # 只输出: 这行不打印命令
    echo "这行打印命令"       # 输出: echo 这行打印命令\n这行打印命令
# 全局静默: .SILENT: 或 make -s
```

### 2. 忽略错误

```makefile
# 命令前加 - 忽略失败
clean:
    -rm *.o        # 没有 .o 文件也不报错
    -rm program
```

### 3. 递归 make

```makefile
# 在子目录中执行 make
subdirs:
    $(MAKE) -C lib
    $(MAKE) -C app
# 用 $(MAKE) 而不是 make，确保传递正确的参数
```

### 4. 并行构建

```bash
make -j$(nproc)    # 并行编译，速度大幅提升
```

### 5. 生成帮助

```makefile
help: ## 显示帮助
    @grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) \
        | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[36m%-15s\033[0m %s\n", $$1, $$2}'

clean: ## 清理构建文件
    rm -rf build
```

---

## 十、Make vs CMake

| | Make | CMake |
|---|------|-------|
| **类型** | 构建工具 | 构建系统生成器（生成 Makefile/Ninja 等） |
| **跨平台** | 需手动处理 | 原生跨平台 |
| **依赖管理** | 手动 | find_package 自动查找 |
| **语法** | 简单但怪异 | 相对现代化 |
| **适合场景** | 小型项目、快速脚本 | 中大型项目、多平台发布 |
| **学习曲线** | 低（基础）高（进阶） | 中 |
| **典型项目** | Linux 内核(部分) | LLVM, KDE, 大多数 C++ 项目 |

---

## 十一、特殊目标速查

| 特殊目标 | 作用 |
|----------|------|
| `.PHONY: target` | 声明为伪目标（每次强制执行） |
| `.DEFAULT_GOAL := target` | 指定默认目标 |
| `.SUFFIXES: .c .o` | 定义后缀规则 |
| `.INTERMEDIATE: file` | 中间文件（完成后自动删除） |
| `.SECONDARY:` | 中间文件不自动删除 |
| `.PRECIOUS: %` | 即使配方失败也保留目标 |
| `.DELETE_ON_ERROR:` | 配方失败时删除目标文件 |
| `.SILENT:` | 不打印命令（全局 @） |
| `.IGNORE:` | 忽略命令错误（全局 -） |

---

## 十二、调试 Makefile

```bash
make -n          # 只打印命令，不执行（dry-run）
make -d          # 详细调试输出（非常多信息！）
make -p          # 打印所有变量和规则
make --warn-undefined-variables  # 警告未定义变量
```

在 Makefile 内部:

```makefile
$(info VAR = $(VAR))       # 打印变量值
$(warning 警告信息)         # 打印警告（继续执行）
$(error 致命错误)            # 打印错误并退出
```
