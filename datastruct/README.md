# 通用 C 数据结构库 (DataStruct)

商业级 C 语言常用数据结构实现。

## 包含的数据结构

| 结构 | 头文件 | 特性 |
|------|--------|------|
| 动态数组 | `darray.h` | 泛型、自动扩容、O(1) 随机访问 |
| 双向链表 | `dlist.h` | 哨兵节点、迭代器、O(1) 头尾操作 |
| 消息队列 | `mqueue.h` | 线程安全、MPMC、超时机制、紧急消息 |
| 哈希表 | `htable.h` | 链地址法、自动 rehash、自定义哈希 |
| AVL 树 | `btree.h` | 严格平衡 O(log n)、4 种遍历、范围查询 |
| 图 | `graph.h` | 邻接表、BFS/DFS/Dijkstra/拓扑排序 |

## 快速开始

```bash
# MinGW-w64 编译测试
gcc -std=c99 -Iinclude src/*.c tests/run_tests.c tests/test_*.c -o test_runner.exe -lwinmm
./test_runner.exe

# 或使用 Visual Studio
cl /Iinclude src/*.c tests/run_tests.c tests/test_*.c /Fe:test_runner.exe
```

## 设计原则

- **泛型**：通过 `void*` + 元素大小实现类型无关
- **错误码**：所有操作返回 `0` 成功 / `-1` 失败，无隐藏 abort
- **内存安全**：完整生命周期管理，无泄漏
- **C99 标准**：头文件兼容 C++

详细设计文档见 [docs/设计文档.md](docs/设计文档.md)。
