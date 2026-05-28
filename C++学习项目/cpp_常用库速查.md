# C++ 常用库速查手册

> 按分类整理的 C++ 标准库和常用第三方库，含用途和说明。
> 更新日期：2026年5月

---

## 一、C++ 标准库（STL）

### 容器（Containers）

| 容器 | 头文件 | 底层实现 | 特点 |
|------|--------|----------|------|
| `vector<T>` | `<vector>` | 动态数组 | 随机访问O(1)，尾部插入O(1)，最常用 |
| `list<T>` | `<list>` | 双向链表 | 任意位置插入删除O(1)，不支持下[] |
| `deque<T>` | `<deque>` | 分段数组 | 双端插入O(1)，随机访问O(1) |
| `map<K,V>` | `<map>` | 红黑树 | 有序键值对，查找O(log n) |
| `unordered_map<K,V>` | `<unordered_map>` | 哈希表 | 无序键值对，查找O(1)平均 |
| `set<T>` | `<set>` | 红黑树 | 有序集合，元素唯一 |
| `unordered_set<T>` | `<unordered_set>` | 哈希表 | 无序集合，查找O(1) |
| `stack<T>` | `<stack>` | 适配器 | LIFO |

### 算法（Algorithms）

| 算法 | 头文件 | 用途 |
|------|--------|------|
| `sort(first, last)` | `<algorithm>` | 排序 O(n log n) |
| `stable_sort(...)` | `<algorithm>` | 稳定排序 |
| `find(first, last, val)` | `<algorithm>` | 线性查找 |
| `binary_search(...)` | `<algorithm>` | 二分查找(要求已排序) |
| `count(first, last, val)` | `<algorithm>` | 计数 |
| `count_if(...)` | `<algorithm>` | 条件计数 |
| `transform(...)` | `<algorithm>` | 对每个元素执行操作 |
| `for_each(...)` | `<algorithm>` | 遍历（类似 range-for） |
| `remove_if(...)` | `<algorithm>` | 移除满足条件的元素 |
| `reverse(...)` | `<algorithm>` | 反转 |
| `shuffle(...)` | `<algorithm>` | 随机打乱 |
| `min/max/minmax` | `<algorithm>` | 极值 |

### 智能指针（Smart Pointers）

| 类型 | 头文件 | 用途 |
|------|--------|------|
| `unique_ptr<T>` | `<memory>` | 独占所有权，不能拷贝只能移动，零开销 |
| `shared_ptr<T>` | `<memory>` | 共享所有权，引用计数 |
| `weak_ptr<T>` | `<memory>` | 弱引用，不增加计数，打破循环引用 |
| `make_unique<T>(args...)` | `<memory>` | 创建 unique_ptr（C++14+） |
| `make_shared<T>(args...)` | `<memory>` | 创建 shared_ptr（一次分配，更高效） |

### 字符串与流

| 类 | 头文件 | 用途 |
|----|--------|------|
| `string` | `<string>` | 动态字符串（替代 char*） |
| `string_view` | `<string_view>` | 字符串视图（不拥有数据，C++17） |
| `ostringstream` | `<sstream>` | 写入到字符串 |
| `istringstream` | `<sstream>` | 从字符串读取 |
| `ifstream` | `<fstream>` | 文件输入流 |
| `ofstream` | `<fstream>` | 文件输出流 |

### 多线程（C++11+）

| 类/函数 | 头文件 | 用途 |
|----------|--------|------|
| `thread` | `<thread>` | 线程对象 |
| `mutex` | `<mutex>` | 互斥锁 |
| `lock_guard<M>` | `<mutex>` | RAII 锁（自动释放） |
| `unique_lock<M>` | `<mutex>` | 更灵活的锁（可延迟/转移） |
| `condition_variable` | `<condition_variable>` | 条件变量（等待-通知） |
| `future<T>` | `<future>` | 异步结果 |
| `async(...)` | `<future>` | 异步执行任务 |
| `atomic<T>` | `<atomic>` | 原子操作（无锁） |

### 函数式与工具

| 类/函数 | 头文件 | 用途 |
|----------|--------|------|
| `function<Signature>` | `<functional>` | 通用函数包装器 |
| `bind(...)` | `<functional>` | 参数绑定 |
| `Lambda 表达式` | 语言特性 | `[capture](args){body}` |
| `optional<T>` | `<optional>` | 可选值（C++17） |
| `variant<Ts...>` | `<variant>` | 类型安全联合体（C++17） |
| `any` | `<any>` | 存储任意类型（C++17） |
| `tuple<Ts...>` | `<tuple>` | 固定大小异构集合 |
| `pair<T1,T2>` | `<utility>` | 二元组 |

---

## 二、C++ 第三方库生态

### 通用工具

| 库名 | 用途 | 说明 |
|------|------|------|
| **Boost** | "准标准库"，涵盖文件系统/网络/序列化/数学等 | 很多 Boost 库后来进入了 C++ 标准 |
| **fmt** | 类型安全的字符串格式化 | C++20 `std::format` 的前身 |
| **spdlog** | 高性能日志库 | 基于 fmt，支持多线程和异步 |
| **abseil** | Google 的 C++ 工具库 | Google 内部使用的基础库 |
| **folly** | Facebook 的 C++ 工具库 | 高性能组件集合 |

### 网络与 Web

| 库名 | 用途 | 说明 |
|------|------|------|
| **Boost.Asio** | 异步网络 I/O | 即将进入 C++26 标准 |
| **libcurl** | HTTP/FTP 多协议传输 | 最流行的网络传输库 |
| **cpp-httplib** | 单头文件 HTTP 服务器/客户端 | 极简易用，适合嵌入 |
| **gRPC** | 高性能 RPC 框架 | Google 出品，支持多语言 |
| **WebSocket++** | WebSocket C++ 库 | - |
| **Drogon** | 高性能 C++ Web 框架 | 异步，类似 Python 的 FastAPI |

### 数据库

| 库名 | 用途 | 说明 |
|------|------|------|
| **SQLite** (sqlite3) | 嵌入式数据库 | C 接口，C++ 有封装 |
| **MySQL Connector/C++** | MySQL 官方客户端 | - |
| **libpqxx** | PostgreSQL C++ 客户端 | 官方推荐 |
| **redis-plus-plus** | Redis C++ 客户端 | 基于 hiredis |
| **mongocxx** | MongoDB C++ 驱动 | 官方驱动 |

### JSON / 序列化

| 库名 | 用途 | 说明 |
|------|------|------|
| **nlohmann/json** | JSON 解析（单头文件） | C++ JSON 事实标准，语法极优雅 |
| **simdjson** | 超高速 JSON 解析 | 利用 SIMD 指令，每秒解析 GB 级 |
| **Protobuf** | Protocol Buffers C++ | Google 序列化格式 |
| **msgpack-c** | MessagePack C++ 实现 | 二进制 JSON |

### 测试

| 库名 | 用途 | 说明 |
|------|------|------|
| **Google Test** | xUnit 风格测试框架 | Google 出品，最流行 |
| **Catch2** | BDD 风格测试框架 | 单头文件，语法简洁 |
| **doctest** | 轻量测试框架 | 编译极快 |
| **Google Mock** | C++ Mock 框架 | 配合 Google Test |

### GUI

| 库名 | 用途 | 说明 |
|------|------|------|
| **Qt 6** | 跨平台 GUI 框架 | 最强大的 C++ GUI 库 |
| **wxWidgets** | 跨平台 GUI | 原生风格 |
| **Dear ImGui** | 即时模式 GUI（单头文件） | 游戏/工具开发首选 |
| **FLTK** | 轻量 GUI | 静态链接后约 1MB |

### 多媒体与游戏

| 库名 | 用途 | 说明 |
|------|------|------|
| **SDL2** | 跨平台多媒体/游戏 | Valve/Source 引擎都在用 |
| **SFML** | 简单多媒体库 | 比 SDL 更 C++ 风格 |
| **OpenCV** | 计算机视觉库 | 图像处理/人脸识别 |
| **FFmpeg** | 音视频处理 | 几乎万能 |

### AI / 机器学习

| 库名 | 用途 | 说明 |
|------|------|------|
| **libtorch** | PyTorch 的 C++ 前端 | 模型训练和推理 |
| **ONNX Runtime** | 跨平台模型推理 | 支持 PyTorch/TF 导出的模型 |
| **TensorFlow C++** | TensorFlow C++ API | - |
| **mlpack** | C++ 机器学习库 | 头文件使用，类似 sklearn |

---

## 三、C++ 构建工具

| 工具 | 用途 |
|------|------|
| **CMake** | 跨平台构建系统（最流行） |
| **vcpkg** | 微软 C/C++ 包管理器 |
| **Conan** | C/C++ 包管理器（类似 pip） |
| **Ninja** | 高速构建系统（配合 CMake） |
| **ccache** | 编译缓存（大幅加速重复编译） |
| **Clang-Format** | 代码格式化 |
| **Clang-Tidy** | 静态分析/Linter |
| **GDB/LLDB** | 调试器 |

---

## 四、编译速查

```bash
# C++17 基本编译
g++ -std=c++17 source.cpp -o program

# 严格警告 + 优化 + 调试
g++ -std=c++17 -Wall -Wextra -O2 -g source.cpp -o program

# 多文件编译
g++ -std=c++17 -c main.cpp -o main.o
g++ -std=c++17 -c utils.cpp -o utils.o
g++ main.o utils.o -o program

# 链接线程库
g++ -std=c++17 source.cpp -o program -pthread

# 地址检查（检测内存错误）
g++ -std=c++17 -fsanitize=address -g source.cpp -o program
```
