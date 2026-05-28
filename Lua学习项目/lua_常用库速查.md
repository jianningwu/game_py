# Lua 常用库速查手册

> 按分类整理的 Lua 常用标准库和第三方库，含用途说明。
> 更新日期：2026年5月

---

## 一、Lua 标准库（随 Lua 一起安装）

### string — 字符串操作

| 函数 | 用途 | 示例 |
|------|------|------|
| `string.len(s)` 或 `#s` | 获取字符串长度 | `#"hello"` -> 5 |
| `string.upper(s)` | 转大写 | `string.upper("hello")` -> "HELLO" |
| `string.lower(s)` | 转小写 | `string.lower("HELLO")` -> "hello" |
| `string.sub(s, i, j)` | 截取子串(索引从1开始) | `string.sub("hello", 2, 4)` -> "ell" |
| `string.find(s, pattern)` | 查找模式，返回起始位置 | `string.find("hello", "ll")` -> 3 |
| `string.gsub(s, pattern, repl)` | 全局替换 | `string.gsub("lol", "l", "x")` -> "xox" |
| `string.gmatch(s, pattern)` | 遍历所有匹配 | 用于 for 循环遍历 |
| `string.match(s, pattern)` | 返回第一个匹配 | `string.match("a123", "%d+")` -> "123" |
| `string.format(fmt, ...)` | 格式化输出 | `string.format("age=%d", 25)` |
| `string.reverse(s)` | 反转字符串 | `string.reverse("abc")` -> "cba" |
| `string.rep(s, n)` | 重复 n 次 | `string.rep("ha", 3)` -> "hahaha" |
| `string.byte(s, i)` | 字符转 ASCII 码 | `string.byte("A")` -> 65 |
| `string.char(...)` | ASCII 码转字符 | `string.char(65)` -> "A" |

### table — Table 操作

| 函数 | 用途 | 示例 |
|------|------|------|
| `table.insert(t, val)` | 末尾插入 | `table.insert(t, "x")` |
| `table.insert(t, pos, val)` | 指定位置插入 | `table.insert(t, 2, "x")` |
| `table.remove(t)` | 移除末尾(返回) | `table.remove(t)` |
| `table.remove(t, pos)` | 移除指定位置(返回) | `table.remove(t, 1)` |
| `table.sort(t)` | 升序排序 | `table.sort({3,1,2})` |
| `table.sort(t, comp)` | 自定义排序 | `table.sort(t, function(a,b) return a>b end)` |
| `table.concat(t, sep)` | 合并为字符串 | `table.concat({"a","b"}, ",")` -> "a,b" |
| `table.move(a,f,t,b)` | 移动元素 (Lua 5.3+) | 高效复制 table 元素 |
| `table.pack(...)` | 将参数打包为 table | `table.pack(1,2,3)` |

### math — 数学运算

| 函数 | 用途 | 示例 |
|------|------|------|
| `math.abs(x)` | 绝对值 | `math.abs(-5)` -> 5 |
| `math.ceil(x)` | 向上取整 | `math.ceil(3.2)` -> 4 |
| `math.floor(x)` | 向下取整 | `math.floor(3.8)` -> 3 |
| `math.max(...)` | 最大值 | `math.max(3,7,2)` -> 7 |
| `math.min(...)` | 最小值 | `math.min(3,7,2)` -> 2 |
| `math.random()` | 随机数 [0,1) | - |
| `math.random(n)` | 随机整数 [1,n] | `math.random(10)` |
| `math.random(m, n)` | 随机整数 [m,n] | `math.random(1, 100)` |
| `math.randomseed(n)` | 设置随机种子 | `math.randomseed(os.time())` |
| `math.sqrt(x)` | 平方根 | `math.sqrt(16)` -> 4 |
| `math.pi` | 圆周率常量 | 3.1415926535898 |
| `math.sin/cos/tan` | 三角函数 | 参数是弧度 |
| `math.deg(r)` | 弧度转角度 | `math.deg(math.pi)` -> 180 |
| `math.rad(d)` | 角度转弧度 | `math.rad(180)` -> pi |
| `math.log(x)` | 自然对数 | `math.log(math.exp(1))` -> 1 |
| `math.tointeger(x)` | 转为整数 (Lua 5.3+) | `math.tointeger("123")` -> 123 |

### io — 文件 I/O

| 函数 | 用途 | 示例 |
|------|------|------|
| `io.open(path, mode)` | 打开文件 | `io.open("test.txt", "r")` |
| `file:read("*a")` | 读取全部内容 | - |
| `file:read("*l")` | 读取一行 | - |
| `file:read("*n")` | 读取一个数字 | - |
| `file:write(...)` | 写入内容 | `file:write("hello\n")` |
| `file:lines()` | 返回行迭代器 | 用于 for 循环 |
| `file:seek(whence)` | 移动文件指针 | "set"/"cur"/"end" |
| `file:close()` | 关闭文件 | - |
| `io.lines(path)` | 打开文件并返回行迭代器 | - |
| `io.type(obj)` | 检查是否为文件句柄 | - |

### os — 操作系统接口

| 函数 | 用途 | 示例 |
|------|------|------|
| `os.date(fmt)` | 格式化当前时间 | `os.date("%Y-%m-%d")` |
| `os.time()` | Unix 时间戳 | - |
| `os.time(table)` | table 转时间戳 | `os.time({year=2024,month=3,day=15})` |
| `os.clock()` | CPU 时间(秒) | 用于性能测量 |
| `os.execute(cmd)` | 执行系统命令 | `os.execute("ls")` |
| `os.getenv(var)` | 获取环境变量 | `os.getenv("HOME")` |
| `os.exit(code)` | 退出程序 | `os.exit(0)` |
| `os.tmpname()` | 生成临时文件名 | - |
| `os.rename(a, b)` | 重命名文件 | - |
| `os.remove(path)` | 删除文件 | - |

### coroutine — 协程

| 函数 | 用途 | 示例 |
|------|------|------|
| `coroutine.create(f)` | 创建协程 | 返回协程对象 |
| `coroutine.resume(co, ...)` | 恢复/启动协程 | 返回 ok, yield值 |
| `coroutine.yield(...)` | 暂停协程 | 返回参数给 resume |
| `coroutine.status(co)` | 查看状态 | running/suspended/normal/dead |
| `coroutine.wrap(f)` | 返回包装函数 | 无需手动 resume |
| `coroutine.running()` | 返回当前协程 | - |

### debug — 调试

| 函数 | 用途 |
|------|------|
| `debug.traceback()` | 获取调用栈信息 |
| `debug.getinfo()` | 获取函数信息 |
| `debug.getlocal()` | 获取局部变量 |
| `debug.setmetatable()` | 设置元表(即使有 __metatable 保护) |

---

## 二、第三方库生态

### 网络与 Web

| 库名 | 用途 | 来源 |
|------|------|------|
| **lua-http** | 现代 HTTP 客户端/服务器库 | `luarocks install lua-http` |
| **lua-resty-http** | OpenResty 的 HTTP 客户端 | OpenResty 内置 |
| **lua-socket** (luasocket) | 最常用的 TCP/UDP 网络库 | `luarocks install luasocket` |
| **lua-websockets** | WebSocket 客户端/服务器 | `luarocks install lua-websockets` |
| **cqueues** | 事件循环库(类似 libuv) | `luarocks install cqueues` |
| **lua-curl** | libcurl 的 Lua 绑定 | `luarocks install lua-curl` |

### Web 框架 (OpenResty 生态)

| 库名 | 用途 | 来源 |
|------|------|------|
| **OpenResty** | 基于 Nginx 的高性能 Web 平台 | `brew/openresty` 或源码安装 |
| **Kong** | 基于 OpenResty 的 API 网关 | `luarocks install kong` |
| **Lapis** | MoonScript/Lua Web 框架 | `luarocks install lapis` |
| **Sailor** | MVC Web 框架 | `luarocks install sailor` |
| **lor** | 轻量级 Web 框架(类似 Flask/Sinatra) | `luarocks install lor` |

### 数据库

| 库名 | 用途 | 来源 |
|------|------|------|
| **lua-resty-redis** | OpenResty Redis 客户端 | OpenResty 内置 |
| **lua-resty-mysql** | OpenResty MySQL 客户端 | OpenResty 内置 |
| **pgmoon** | PostgreSQL 客户端(支持协程) | `luarocks install pgmoon` |
| **lua-resty-mongol** | MongoDB 客户端 | `luarocks install lua-resty-mongol` |

### JSON / 序列化

| 库名 | 用途 | 来源 |
|------|------|------|
| **dkjson** | 纯 Lua JSON 编解码 | `luarocks install dkjson` |
| **lua-cjson** | C 实现的高速 JSON 库 | `luarocks install lua-cjson` |
| **rapidjson** | 高速 JSON 库 | `luarocks install rapidjson` |
| **lyaml** | YAML 解析 | `luarocks install lyaml` |
| **lua-MessagePack** | MessagePack 序列化 | `luarocks install lua-messagepack` |

### 测试

| 库名 | 用途 | 来源 |
|------|------|------|
| **busted** | BDD 风格测试框架(Lua 社区首选) | `luarocks install busted` |
| **luassert** | 断言库(配合 busted) | `luarocks install luassert` |
| **luacheck** | 静态代码检查(Linter) | `luarocks install luacheck` |
| **luaunit** | 类似 xUnit 的测试框架 | `luarocks install luaunit` |

### 高性能 / LuaJIT

| 库名 | 用途 | 来源 |
|------|------|------|
| **LuaJIT** | JIT 编译的 Lua 实现(比标准 Lua 快 10-50 倍) | `brew/luajit` 或源码编译 |
| **FFI** | LuaJIT 内置的 C 函数调用接口(无需写 C 绑定!) | LuaJIT 内置 |
| **lua-resty-core** | OpenResty 的 Nginx C API 绑定 | OpenResty 内置 |

### 工具与杂项

| 库名 | 用途 | 来源 |
|------|------|------|
| **luarocks** | Lua 包管理器(最常用) | `brew/luarocks` 或官网安装 |
| **inspect** | 漂亮打印任意 Lua 值(调试用) | `luarocks install inspect` |
| **penlight** | Python 风格的实用工具库(类似 Python 标准库) | `luarocks install penlight` |
| **lua-filesystem** (lfs) | 跨平台文件系统操作 | `luarocks install luafilesystem` |
| **date** | 日期时间扩展(比 os.date 强) | `luarocks install date` |
| **lua-term** | 终端颜色和样式 | `luarocks install lua-term` |
| **argparse** | 命令行参数解析 | `luarocks install argparse` |
| **ansicolors** | ANSI 颜色输出 | `luarocks install ansicolors` |
| **lua-crypto/luaossl** | 加密函数(MD5/SHA/AES等) | `luarocks install luaossl` |
| **lfs** | 文件系统操作(遍历目录等) | `luarocks install luafilesystem` |

---

## 三、Lua 常用 mode 说明

- **Lua 模式匹配不是正则！** Lua 有自己的模式语法，比正则简单但功能有限：
  - `%d` 数字, `%w` 字母数字, `%s` 空白, `%a` 字母, `%l` 小写, `%u` 大写, `%p` 标点
  - `.` 任意字符, `%` 转义, `+` 1+, `*` 0+, `-` 懒惰, `?` 0/1
  - `[abc]` 字符集, `[^abc]` 取反, `()` 捕获分组

- **Lua 索引从 1 开始**（不是 0）

- **Lua 不等于用 `~=`**（不是 `!=`）

- **Lua 幂运算用 `^`**（不是 `**`）

- **Lua 逻辑运算符: `and`, `or`, `not`**（不是 `&&`, `||`, `!`）

- **Lua 注释: `--` 单行, `--[[ ]]` 多行**

- **Lua 拼接字符串用 `..`**（不是 `+`）

- **Lua 获取长度用 `#`**（如 `#"hello"` = 5, `#{1,2,3}` = 3）

---

> **提示：** LuaRocks 是 Lua 的包管理器，类似 Python 的 pip。
> ```bash
> luarocks install <包名>     # 安装
> luarocks list               # 列出已安装
> luarocks search <关键词>    # 搜索
> ```
