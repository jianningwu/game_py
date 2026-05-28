# C 语言常用库速查手册

> 按分类整理的 C 标准库和常用第三方库，含用途和编译说明。
> 更新日期：2026年5月

---

## 一、C 标准库（随编译器一起安装，无需下载）

### <stdio.h> — 标准输入输出

| 函数 | 用途 | 示例 |
|------|------|------|
| `printf(fmt, ...)` | 格式化输出到 stdout | `printf("x=%d\n", x)` |
| `scanf(fmt, ...)` | 格式化输入 | `scanf("%d", &x)` (注意 &) |
| `fprintf(fp, fmt, ...)` | 格式化输出到文件 | `fprintf(fp, "x=%d", x)` |
| `fscanf(fp, fmt, ...)` | 从文件格式化读取 | `fscanf(fp, "%d", &x)` |
| `sprintf(buf, fmt, ...)` | 格式化输出到字符串 | `sprintf(buf, "x=%d", x)` |
| `snprintf(buf, n, fmt, ...)` | 安全版 sprintf(限制长度) | 推荐替代 sprintf |
| `fopen(path, mode)` | 打开文件 | `fopen("a.txt", "r")` |
| `fclose(fp)` | 关闭文件 | **必须调用** |
| `fgets(buf, n, fp)` | 读取一行(安全) | 替代危险的 `gets()` |
| `fputs(s, fp)` | 写入字符串 | `fputs("hello\n", fp)` |
| `fread(buf, sz, n, fp)` | 读取二进制数据 | `fread(arr, sizeof(int), 5, fp)` |
| `fwrite(buf, sz, n, fp)` | 写入二进制数据 | `fwrite(arr, sizeof(int), 5, fp)` |
| `fseek(fp, off, whence)` | 移动文件指针 | SEEK_SET/SEEK_CUR/SEEK_END |
| `ftell(fp)` | 获取当前位置 | 返回距文件头的字节数 |
| `rewind(fp)` | 回到文件开头 | 等价 `fseek(fp, 0, SEEK_SET)` |
| `feof(fp)` | 到达文件末尾? | 常用于循环判断 |
| `perror(msg)` | 打印系统错误信息 | 配合 errno 使用 |
| `remove(path)` | 删除文件 | `remove("temp.txt")` |
| `rename(old, new)` | 重命名文件 | `rename("a.txt", "b.txt")` |
| `tmpfile()` | 创建临时文件 | 关闭时自动删除 |

### <stdlib.h> — 标准库

| 函数 | 用途 | 示例 |
|------|------|------|
| `malloc(size)` | 分配内存(不初始化) | `malloc(10 * sizeof(int))` |
| `calloc(n, size)` | 分配并清零 | `calloc(10, sizeof(int))` |
| `realloc(ptr, size)` | 重新分配大小 | `realloc(ptr, 20 * sizeof(int))` |
| `free(ptr)` | 释放内存 | **必须调用** |
| `atoi(s)` | 字符串转 int | `atoi("42")` -> 42 |
| `atof(s)` | 字符串转 double | `atof("3.14")` -> 3.14 |
| `atol(s)` | 字符串转 long | - |
| `strtol(s, &end, base)` | 字符串转 long(带错误检查) | 比 atoi 更安全 |
| `strtod(s, &end)` | 字符串转 double(带错误检查) | 比 atof 更安全 |
| `rand()` | 生成随机数(0 ~ RAND_MAX) | 先用 `srand(time(NULL))` 设置种子 |
| `srand(seed)` | 设置随机种子 | `srand((unsigned)time(NULL))` |
| `qsort(arr, n, sz, cmp)` | 快速排序 | 需要提供比较函数 |
| `bsearch(&key, arr, n, sz, cmp)` | 二分查找 | 前提：数组已排序 |
| `abs(n)` | int 绝对值 | `abs(-5)` -> 5 |
| `labs(n)` | long 绝对值 | - |
| `exit(code)` | 终止程序 | `exit(0)` 成功, `exit(1)` 失败 |
| `system(cmd)` | 执行系统命令 | `system("ls")` |
| `getenv(var)` | 获取环境变量 | `getenv("PATH")` |

### <string.h> — 字符串与内存操作

| 函数 | 用途 | 注意 |
|------|------|------|
| `strlen(s)` | 获取字符串长度 | 不含 `\0` |
| `strcpy(dst, src)` | 复制字符串 | **不检查缓冲区大小** |
| `strncpy(dst, src, n)` | 安全复制(限长) | 可能不补 `\0` |
| `strcat(dst, src)` | 拼接字符串 | **不检查缓冲区大小** |
| `strncat(dst, src, n)` | 安全拼接(限长) | - |
| `strcmp(a, b)` | 比较字符串 | 相等返回 0 |
| `strncmp(a, b, n)` | 比较前 n 个字符 | - |
| `strchr(s, c)` | 查找字符(首次) | 返回指针或 NULL |
| `strrchr(s, c)` | 查找字符(末次) | - |
| `strstr(hay, needle)` | 查找子串 | `strstr("hello", "ll")` -> "llo" |
| `strtok(s, delim)` | 分割字符串 | **会修改原字符串** |
| `strerror(errno)` | 错误码转字符串 | - |
| `memcpy(dst, src, n)` | 复制内存(不重叠) | 更快，不依赖 `\0` |
| `memmove(dst, src, n)` | 复制内存(支持重叠) | 稍慢但更安全 |
| `memset(ptr, val, n)` | 填充内存 | `memset(arr, 0, sizeof(arr))` |
| `memcmp(a, b, n)` | 比较内存 | - |
| `memchr(ptr, val, n)` | 在内存中查找字节 | - |

### <math.h> — 数学函数（编译时加 `-lm`）

| 函数 | 用途 | 示例 |
|------|------|------|
| `sqrt(x)` | 平方根 | `sqrt(16)` -> 4.0 |
| `pow(x, y)` | x 的 y 次方 | `pow(2, 10)` -> 1024.0 |
| `exp(x)` | e 的 x 次方 | `exp(1)` -> 2.718 |
| `log(x)` | 自然对数 | `log(exp(1))` -> 1.0 |
| `log10(x)` | 以10为底对数 | `log10(100)` -> 2.0 |
| `fabs(x)` | 浮点绝对值 | `fabs(-3.14)` -> 3.14 |
| `ceil(x)` | 向上取整 | `ceil(3.2)` -> 4.0 |
| `floor(x)` | 向下取整 | `floor(3.8)` -> 3.0 |
| `round(x)` | 四舍五入 | `round(3.5)` -> 4.0 |
| `fmod(x, y)` | 浮点取余 | `fmod(10.5, 3)` -> 1.5 |
| `sin(x) / cos(x) / tan(x)` | 三角函数 | 参数是**弧度** |
| `asin(x) / acos(x) / atan(x)` | 反三角函数 | - |
| `sinh(x) / cosh(x) / tanh(x)` | 双曲函数 | - |
| `M_PI` | 圆周率常量 | GCC 需 `_GNU_SOURCE` |

### <time.h> — 日期时间

| 函数 | 用途 | 示例 |
|------|------|------|
| `time(&t)` | 获取当前 Unix 时间戳 | `time(NULL)` |
| `localtime(&t)` | 时间戳转本地时间 | 返回 `struct tm*` |
| `gmtime(&t)` | 时间戳转 UTC 时间 | - |
| `strftime(buf, n, fmt, tm)` | 格式化时间字符串 | `strftime(buf, 100, "%Y-%m-%d", tm)` |
| `mktime(tm)` | `struct tm` 转时间戳 | - |
| `difftime(t1, t2)` | 时间差(秒) | - |
| `clock()` | CPU 时间(用于性能测量) | `(end-start)/CLOCKS_PER_SEC` |

### <ctype.h> — 字符判断

| 函数 | 用途 |
|------|------|
| `isdigit(c)` | 是否为数字 '0'-'9' |
| `isalpha(c)` | 是否为字母 a-z/A-Z |
| `isalnum(c)` | 是否为字母或数字 |
| `isspace(c)` | 是否为空白字符 |
| `islower(c) / isupper(c)` | 是否为小写/大写 |
| `toupper(c) / tolower(c)` | 转大写/转小写 |
| `isxdigit(c)` | 是否为十六进制数字 |

### <stdarg.h> — 可变参数

| 宏 | 用途 |
|----|------|
| `va_list` | 声明参数列表变量 |
| `va_start(ap, last)` | 初始化(从 last 之后开始) |
| `va_arg(ap, type)` | 获取下一个参数 |
| `va_end(ap)` | 清理 |
| `va_copy(dst, src)` | 复制参数列表 |

### <stddef.h> — 标准定义

| 定义 | 含义 |
|------|------|
| `NULL` | 空指针 ((void*)0) |
| `size_t` | 无符号整数类型(sizeof 返回) |
| `ptrdiff_t` | 有符号指针差值类型 |
| `offsetof(type, member)` | 结构体成员偏移量 |

### <assert.h> — 断言

```c
assert(x > 0);  // x<=0 时终止程序，打印文件和行号
// 定义 NDEBUG 可禁用所有 assert
```

### <errno.h> — 错误处理

| 用法 | 说明 |
|------|------|
| `errno` | 全局错误码变量 |
| `perror(msg)` | 打印错误描述 |
| `strerror(errno)` | 获取错误描述字符串 |

### <setjmp.h> — 非本地跳转

| 函数 | 用途 |
|------|------|
| `setjmp(env)` | 设置跳转点 |
| `longjmp(env, val)` | 跳回 setjmp 处 |

### <signal.h> — 信号处理

| 函数 | 用途 |
|------|------|
| `signal(sig, handler)` | 注册信号处理函数 |
| `raise(sig)` | 发送信号给当前进程 |
| SIGINT | Ctrl+C 中断 |
| SIGSEGV | 段错误 |
| SIGTERM | 终止信号 |

---

## 二、常用第三方库

### 数据结构与算法

| 库名 | 用途 | 安装 |
|------|------|------|
| **glib** | GNOME 的 C 工具库(链表/哈希/字符串/线程) | 包管理器安装 |
| **uthash** | 仅头文件实现的哈希表 | 下载 `uthash.h` |
| **kvec** | 动态数组(类似 C++ vector) | 单头文件 |
| **stb** | Sean Barrett 的单头文件库集合(图像/字体/哈希等) | GitHub 下载 |
| **libuv** | 跨平台异步 I/O(Node.js 的底层) | 包管理器安装 |

### 网络

| 库名 | 用途 | 安装 |
|------|------|------|
| **libcurl** | HTTP/FTP/多协议网络传输 | `sudo apt install libcurl-dev` |
| **libwebsockets** | WebSocket C 库 | 包管理器或源码编译 |
| **mongoose** | 嵌入式 Web 服务器 + 网络库 | 单文件，下载即可用 |
| **libevent** | 事件驱动的网络库 | `sudo apt install libevent-dev` |
| **zeromq (libzmq)** | 高性能消息队列 | `sudo apt install libzmq3-dev` |

### 数据库

| 库名 | 用途 | 安装 |
|------|------|------|
| **libpq** | PostgreSQL 客户端 | `sudo apt install libpq-dev` |
| **libmysqlclient** | MySQL 客户端 | `sudo apt install libmysqlclient-dev` |
| **hiredis** | Redis C 客户端 | `sudo apt install libhiredis-dev` |
| **sqlite3** | SQLite C 接口 | 通常随系统自带 |

### 加密与安全

| 库名 | 用途 | 安装 |
|------|------|------|
| **OpenSSL / LibreSSL** | TLS/SSL/加密算法 | `sudo apt install libssl-dev` |
| **libsodium** | 现代加密库 | `sudo apt install libsodium-dev` |
| **GnuTLS** | TLS 实现 | `sudo apt install libgnutls28-dev` |

### 解析与序列化

| 库名 | 用途 | 安装 |
|------|------|------|
| **libxml2** | XML 解析 | `sudo apt install libxml2-dev` |
| **cJSON** | 轻量 JSON 解析(单文件) | GitHub 下载 |
| **jansson** | JSON 解析库 | `sudo apt install libjansson-dev` |
| **yaml-cpp** | YAML 解析(C 接口可用) | 包管理器安装 |
| **protobuf-c** | Protocol Buffers C 实现 | `sudo apt install libprotobuf-c-dev` |

### 测试与调试

| 库名 | 用途 | 安装 |
|------|------|------|
| **CUnit** | 单元测试框架 | 包管理器安装 |
| **Check** | 单元测试框架 | `sudo apt install check` |
| **CMocka** | 轻量测试+Mock 框架 | 包管理器安装 |
| **Valgrind** | 内存泄漏/错误检测(不是库) | `sudo apt install valgrind` |

### 多媒体

| 库名 | 用途 | 安装 |
|------|------|------|
| **SDL2** | 跨平台多媒体/游戏开发 | `sudo apt install libsdl2-dev` |
| **raylib** | 简单易用的游戏开发库 | 包管理器安装 |
| **FFmpeg (libav\*)** | 音视频处理 | `sudo apt install libavcodec-dev` |
| **Cairo** | 2D 矢量图形 | `sudo apt install libcairo2-dev` |

### GUI

| 库名 | 用途 | 安装 |
|------|------|------|
| **GTK4 / GTK3** | GNOME 桌面 GUI 框架 | `sudo apt install libgtk-4-dev` |
| **Nuklear** | 即时模式 GUI(单头文件) | GitHub 下载 |
| **Dear ImGui** | 即时模式 GUI(游戏/工具) | GitHub 下载 |

---

## 三、编译链接速查

```bash
# 基本编译
gcc source.c -o program

# 严格检查 + 调试
gcc -Wall -Wextra -g source.c -o program

# 优化 + 指定标准
gcc -O2 -std=c11 source.c -o program

# 链接数学库（使用 <math.h> 时必需）
gcc source.c -o program -lm

# 链接 pthread 线程库
gcc source.c -o program -lpthread

# 多文件编译
gcc -c main.c -o main.o          # 编译为目标文件
gcc -c utils.c -o utils.o
gcc main.o utils.o -o program    # 链接

# 地址检查(检测内存越界/泄漏)
gcc -fsanitize=address -g source.c -o program
```

---

## 四、C 语言核心概念速记

| 概念 | 关键点 |
|------|--------|
| **数组索引** | 从 **0** 开始，C 不检查越界 |
| **字符串** | 以 `'\0'` 结尾的 char 数组，`strlen` 不含 `\0` |
| **指针** | `&` 取地址，`*` 解引用，`->` 访问结构体成员 |
| **内存** | `malloc` 分配 → `free` 释放（必须配对） |
| **头文件** | `.h` 放声明，`.c` 放实现，`#ifndef` 防重复 |
| **static** | 函数内=持久变量；文件级=私有函数/变量 |
| **const** | 声明不变量，编译器可优化 |
| **void\*** | 通用指针类型，可指向任何类型 |
| **宏** | 文本替换，参数必须加括号 `((x)*(x))` |
| **enum** | 命名整数常量，默认从 0 递增 |
| **结构体** | `struct Name {...};` 注意分号 |
| **编译** | 预处理 → 编译 → 汇编 → 链接 |
