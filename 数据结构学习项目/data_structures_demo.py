#!/usr/bin/env python3
"""
================================================================================
脚本功能：数据结构 Python 实现演示
- 用 Python 实现常用数据结构：链表、栈、队列、堆、哈希表、BST、Trie、图等
- 每个数据结构含详细中文注释、复杂度分析、典型应用示例
- 运行方式：python data_structures_demo.py
- 配套文档：数据结构详解与对比.md
================================================================================
"""

# ============================================================================
# 1. 单向链表 (Singly Linked List)
# ============================================================================

class ListNode:
    """链表节点"""
    def __init__(self, val=0, next_node=None):
        self.val = val
        self.next = next_node    # 指向下一个节点的引用


class SinglyLinkedList:
    """单向链表 -- 适合频繁在头部插入/删除的场景"""

    def __init__(self):
        self.head = None         # 头节点（哨兵或第一个节点）

    def prepend(self, val):
        """头部插入 O(1)：新节点指向原头，更新头指针"""
        new_node = ListNode(val, self.head)
        self.head = new_node

    def append(self, val):
        """尾部追加 O(n)：需遍历到末尾"""
        if not self.head:
            self.head = ListNode(val)
            return
        cur = self.head
        while cur.next:          # 走到最后一个节点
            cur = cur.next
        cur.next = ListNode(val)

    def delete(self, val):
        """删除第一个值为val的节点 O(n)：需要找到前驱"""
        if not self.head:
            return
        if self.head.val == val:           # 删的是头节点
            self.head = self.head.next
            return
        cur = self.head
        while cur.next and cur.next.val != val:
            cur = cur.next                 # 找前驱
        if cur.next:
            cur.next = cur.next.next       # 跳过被删节点

    def find(self, val):
        """查找 O(n)"""
        cur = self.head
        while cur:
            if cur.val == val:
                return cur
            cur = cur.next
        return None

    def to_list(self):
        """转为 Python list 便于打印"""
        result = []
        cur = self.head
        while cur:
            result.append(cur.val)
            cur = cur.next
        return result


# ============================================================================
# 2. 栈 (Stack) -- 数组实现
# ============================================================================

class Stack:
    """栈 -- LIFO，用 Python list 实现（list 天然支持 push/pop 末尾 O(1)）"""

    def __init__(self):
        self._data = []          # 用 list 尾部作为栈顶

    def push(self, val):
        """入栈 O(1)"""
        self._data.append(val)

    def pop(self):
        """出栈 O(1)，栈空时抛异常"""
        if self.is_empty():
            raise IndexError("栈为空!")
        return self._data.pop()

    def peek(self):
        """查看栈顶 O(1)"""
        if self.is_empty():
            raise IndexError("栈为空!")
        return self._data[-1]

    def is_empty(self):
        return len(self._data) == 0

    def size(self):
        return len(self._data)


def demo_stack_use_case():
    """栈的经典应用：括号匹配"""
    pairs = {')': '(', ']': '[', '}': '{'}
    test_cases = ["()", "()[]{}", "(]", "([)]", "{[]}"]

    for s in test_cases:
        stack = []
        valid = True
        for ch in s:
            if ch in "([{":
                stack.append(ch)               # 左括号入栈
            elif ch in ")]}":
                if not stack or stack.pop() != pairs[ch]:
                    valid = False              # 栈空或不匹配
                    break
        if stack:                               # 还有未匹配的左括号
            valid = False
        print(f"  '{s}': {'合法' if valid else '不合法'}")


# ============================================================================
# 3. 队列 (Queue) -- 双端队列实现
# ============================================================================

from collections import deque


class Queue:
    """队列 -- FIFO，用 collections.deque 实现（O(1) 两端操作）"""

    def __init__(self):
        self._data = deque()

    def enqueue(self, val):
        """入队 O(1)"""
        self._data.append(val)

    def dequeue(self):
        """出队 O(1)"""
        if self.is_empty():
            raise IndexError("队列为空!")
        return self._data.popleft()    # 左端弹出

    def peek(self):
        """查看队头 O(1)"""
        if self.is_empty():
            raise IndexError("队列为空!")
        return self._data[0]

    def is_empty(self):
        return len(self._data) == 0

    def size(self):
        return len(self._data)


# ============================================================================
# 4. 堆 (Heap) -- 用 Python heapq 模块
# ============================================================================

import heapq


def demo_heap():
    """堆的经典应用：Top K 问题"""
    print("  [Top K] 从 10 个数字中找出最大的 3 个:")

    nums = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3]
    k = 3

    # 方法：维护一个大小为 K 的小顶堆
    # 堆顶是最小的，如果新数 > 堆顶，弹出堆顶并插入新数
    heap = []
    for num in nums:
        if len(heap) < k:
            heapq.heappush(heap, num)           # 直接入堆
        elif num > heap[0]:
            heapq.heapreplace(heap, num)        # 弹出堆顶并入新数 O(log k)

    result = sorted(heap, reverse=True)          # 从大到小
    print(f"  数据: {nums}")
    print(f"  Top {k}: {result} (时间复杂度 O(n log k)，比排序的 O(n log n) 更优)")


# ============================================================================
# 5. 哈希表 (Hash Table)
# ============================================================================

class SimpleHashTable:
    """
    简化版哈希表实现 -- 展示链地址法解决冲突
    真正的 Python dict 更复杂（开放地址 + 伪随机探测）
    """

    def __init__(self, capacity=8):
        self.capacity = capacity
        self.size = 0
        # 每个桶存一个链表 [(key, value), ...]
        self.buckets = [[] for _ in range(capacity)]

    def _hash(self, key):
        """哈希函数：Python 内置 hash，取模得到桶索引"""
        return hash(key) % self.capacity

    def put(self, key, value):
        """插入/更新 O(1) 平均"""
        idx = self._hash(key)
        bucket = self.buckets[idx]
        # 检查 key 是否已存在
        for i, (k, v) in enumerate(bucket):
            if k == key:
                bucket[i] = (key, value)   # 更新
                return
        bucket.append((key, value))        # 新键值对
        self.size += 1

    def get(self, key, default=None):
        """查找 O(1) 平均"""
        idx = self._hash(key)
        for k, v in self.buckets[idx]:
            if k == key:
                return v
        return default

    def remove(self, key):
        """删除 O(1) 平均"""
        idx = self._hash(key)
        bucket = self.buckets[idx]
        for i, (k, v) in enumerate(bucket):
            if k == key:
                del bucket[i]
                self.size -= 1
                return True
        return False

    def __repr__(self):
        items = []
        for bucket in self.buckets:
            for k, v in bucket:
                items.append(f"{k!r}: {v!r}")
        return "{" + ", ".join(items) + "}"


# ============================================================================
# 6. 二叉搜索树 (BST)
# ============================================================================

class TreeNode:
    """二叉树节点"""
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right


class BST:
    """二叉搜索树 -- 左 < 根 < 右"""

    def __init__(self):
        self.root = None

    def insert(self, val):
        """插入 O(h)，h 为树高（平均 O(log n)，最坏 O(n)）"""
        if not self.root:
            self.root = TreeNode(val)
            return

        cur = self.root
        while True:
            if val < cur.val:
                if cur.left:
                    cur = cur.left
                else:
                    cur.left = TreeNode(val)
                    return
            elif val > cur.val:
                if cur.right:
                    cur = cur.right
                else:
                    cur.right = TreeNode(val)
                    return
            else:
                return   # 重复值不插入

    def search(self, val):
        """查找 O(h)"""
        cur = self.root
        while cur:
            if val == cur.val:
                return cur
            elif val < cur.val:
                cur = cur.left
            else:
                cur = cur.right
        return None

    def inorder(self):
        """中序遍历 -- 得到有序序列（BST 的核心价值）"""
        result = []

        def dfs(node):
            if not node:
                return
            dfs(node.left)
            result.append(node.val)       # 中序位置（左->根->右）
            dfs(node.right)

        dfs(self.root)
        return result


# ============================================================================
# 7. 前缀树 (Trie)
# ============================================================================

class TrieNode:
    """Trie 节点"""
    def __init__(self):
        self.children = {}               # 子节点字典 {字符: TrieNode}
        self.is_end = False              # 是否是单词结尾


class Trie:
    """前缀树 -- O(L) 插入/查找，L = 字符串长度"""

    def insert(self, word):
        """插入单词 O(L)"""
        node = self.root
        for ch in word:
            if ch not in node.children:
                node.children[ch] = TrieNode()
            node = node.children[ch]
        node.is_end = True

    def __init__(self):
        self.root = TrieNode()

    def search(self, word):
        """精确查找单词是否存在 O(L)"""
        node = self._find_node(word)
        return node is not None and node.is_end

    def starts_with(self, prefix):
        """前缀匹配 -- 检查是否有以此前缀开头的单词 O(L)"""
        return self._find_node(prefix) is not None

    def _find_node(self, s):
        """找到 s 对应的节点，不存在则返回 None"""
        node = self.root
        for ch in s:
            if ch not in node.children:
                return None
            node = node.children[ch]
        return node


# ============================================================================
# 8. 图 (Graph) -- 邻接表实现
# ============================================================================

from collections import defaultdict, deque as _deque


class Graph:
    """
    图 -- 邻接表表示（最常用）
    支持：添加边、DFS、BFS、拓扑排序
    """

    def __init__(self):
        self.adj = defaultdict(list)     # 邻接表 {节点: [邻居列表]}

    def add_edge(self, u, v, directed=False):
        """添加边 O(1)"""
        self.adj[u].append(v)
        if not directed:
            self.adj[v].append(u)

    def dfs(self, start):
        """深度优先搜索 O(V+E) -- 递归实现"""
        visited = set()
        result = []

        def _dfs(node):
            visited.add(node)
            result.append(node)
            for neighbor in self.adj[node]:
                if neighbor not in visited:
                    _dfs(neighbor)

        _dfs(start)
        return result

    def bfs(self, start):
        """广度优先搜索 O(V+E) -- 队列实现，求无权图最短路径"""
        visited = {start}
        result = []
        q = _deque([start])

        while q:
            node = q.popleft()
            result.append(node)
            for neighbor in self.adj[node]:
                if neighbor not in visited:
                    visited.add(neighbor)
                    q.append(neighbor)

        return result

    def shortest_path(self, start, end):
        """BFS 求无权图最短路径（边数最少）"""
        if start == end:
            return [start]

        visited = {start}
        parent = {start: None}           # 记录父节点，用于回溯路径
        q = _deque([start])

        while q:
            node = q.popleft()
            for neighbor in self.adj[node]:
                if neighbor not in visited:
                    visited.add(neighbor)
                    parent[neighbor] = node
                    q.append(neighbor)
                    if neighbor == end:   # 找到目标
                        # 回溯路径
                        path = []
                        cur = end
                        while cur is not None:
                            path.append(cur)
                            cur = parent[cur]
                        return path[::-1]  # 反转得到 start->end
        return None                       # 不可达

    def topological_sort(self):
        """
        拓扑排序 O(V+E) -- 仅适用于有向无环图(DAG)
        应用：课程选修顺序、构建依赖解析
        """
        indegree = defaultdict(int)       # 入度表
        for u in self.adj:
            for v in self.adj[u]:
                indegree[v] += 1

        # 入度为 0 的节点入队
        q = _deque([node for node in self.adj if indegree[node] == 0])
        result = []

        while q:
            node = q.popleft()
            result.append(node)
            for neighbor in self.adj[node]:
                indegree[neighbor] -= 1
                if indegree[neighbor] == 0:
                    q.append(neighbor)

        return result


# ============================================================================
# 9. LRU 缓存
# ============================================================================

class LRUCache:
    """
    LRU 缓存 -- 双向链表 + 哈希表，所有操作 O(1)
    Python 3.2+ 有内置的 @functools.lru_cache，
    或 Python 3.7+ 可用 collections.OrderedDict 简化实现
    """

    class _Node:
        def __init__(self, key=0, val=0):
            self.key = key
            self.val = val
            self.prev = None
            self.next = None

    def __init__(self, capacity):
        self.capacity = capacity
        self.cache = {}  # key -> Node

        # 虚拟头尾节点，简化边界处理
        self.head = self._Node()
        self.tail = self._Node()
        self.head.next = self.tail
        self.tail.prev = self.head

    def _remove(self, node):
        """从链表中移除节点 O(1)"""
        node.prev.next = node.next
        node.next.prev = node.prev

    def _add_to_front(self, node):
        """将节点插入到头部(最近使用) O(1)"""
        node.next = self.head.next
        node.prev = self.head
        self.head.next.prev = node
        self.head.next = node

    def get(self, key):
        """获取值，并标记为最近使用 O(1)"""
        if key in self.cache:
            node = self.cache[key]
            self._remove(node)
            self._add_to_front(node)       # 移到最前
            return node.val
        return -1

    def put(self, key, value):
        """插入/更新 O(1)"""
        if key in self.cache:
            node = self.cache[key]
            node.val = value
            self._remove(node)
            self._add_to_front(node)
            return

        if len(self.cache) >= self.capacity:
            # 淘汰最久未使用的（链表尾部的前一个）
            lru = self.tail.prev
            self._remove(lru)
            del self.cache[lru.key]

        node = self._Node(key, value)
        self.cache[key] = node
        self._add_to_front(node)


# ============================================================================
# 10. 并查集 (Union-Find)
# ============================================================================

class UnionFind:
    """并查集 -- 近乎 O(1) 的 find/union，含路径压缩和按秩合并"""

    def __init__(self, n):
        # parent[i] = i 的父节点（初始指向自己）
        self.parent = list(range(n))
        # rank[i] = 以 i 为根的树的高度上界
        self.rank = [0] * n
        self.count = n           # 连通分量数

    def find(self, x):
        """查找 x 的代表元，同时路径压缩"""
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])  # 递归压缩
        return self.parent[x]

    def union(self, x, y):
        """合并 x 和 y 所在的集合（按秩合并）"""
        rx, ry = self.find(x), self.find(y)
        if rx == ry:
            return False         # 已经在同一集合

        # 把小树接在大树下面
        if self.rank[rx] < self.rank[ry]:
            self.parent[rx] = ry
        elif self.rank[rx] > self.rank[ry]:
            self.parent[ry] = rx
        else:
            self.parent[ry] = rx
            self.rank[rx] += 1
        self.count -= 1
        return True

    def connected(self, x, y):
        """判断 x 和 y 是否在同一集合"""
        return self.find(x) == self.find(y)


# ============================================================================
# 主函数：展示所有数据结构
# ============================================================================

def main():
    print("=" * 60)
    print("  数据结构 Python 实现演示")
    print("=" * 60)

    # 1. 链表
    print("\n[1] 单向链表:")
    sll = SinglyLinkedList()
    sll.append(10); sll.append(20); sll.prepend(5)
    print(f"  链表: {sll.to_list()}")
    sll.delete(20)
    print(f"  删除 20 后: {sll.to_list()}")

    # 2. 栈
    print("\n[2] 栈 -- 括号匹配:")
    demo_stack_use_case()

    # 3. 队列
    print("\n[3] 队列:")
    q = Queue()
    q.enqueue("A"); q.enqueue("B"); q.enqueue("C")
    print(f"  入队 A,B,C -> 出队: {q.dequeue()}, {q.dequeue()}, 剩余: {q.size()}")

    # 4. 堆 (Top K)
    print("\n[4] 堆 -- Top K:")
    demo_heap()

    # 5. 哈希表
    print("\n[5] 哈希表:")
    ht = SimpleHashTable()
    ht.put("name", "张三")
    ht.put("age", 25)
    print(f"  自定义哈希表: {ht}")
    # Python 内置 dict
    d = {"name": "张三", "age": 25}
    print(f"  Python dict(内置): {d}")

    # 6. BST
    print("\n[6] 二叉搜索树:")
    bst = BST()
    for v in [8, 3, 10, 1, 6, 14, 4, 7, 13]:
        bst.insert(v)
    print(f"  插入 [8,3,10,1,6,14,4,7,13]")
    print(f"  中序遍历(有序): {bst.inorder()}")
    print(f"  查找 6: {'找到' if bst.search(6) else '未找到'}")
    print(f"  查找 99: {'找到' if bst.search(99) else '未找到'}")

    # 7. Trie
    print("\n[7] 前缀树:")
    trie = Trie()
    for w in ["cat", "car", "dog", "dot", "door"]:
        trie.insert(w)
    print(f"  插入: cat, car, dog, dot, door")
    print(f"  search('cat'): {trie.search('cat')}")
    print(f"  search('do'): {trie.search('do')}")
    print(f"  startsWith('do'): {trie.starts_with('do')}")

    # 8. 图
    print("\n[8] 图:")
    g = Graph()
    for u, v in [(1, 2), (1, 3), (2, 4), (2, 5), (3, 6)]:
        g.add_edge(u, v)
    print(f"  边: 1-2, 1-3, 2-4, 2-5, 3-6")
    print(f"  DFS(1): {g.dfs(1)}")
    print(f"  BFS(1): {g.bfs(1)}")
    print(f"  最短路径 1->5: {g.shortest_path(1, 5)}")

    # 拓扑排序
    dag = Graph()
    dag.add_edge("A", "C", directed=True)
    dag.add_edge("B", "C", directed=True)
    dag.add_edge("C", "D", directed=True)
    dag.add_edge("B", "E", directed=True)
    print(f"  DAG 拓扑排序: {dag.topological_sort()}")

    # 9. LRU
    print("\n[9] LRU 缓存 (容量=3):")
    lru = LRUCache(3)
    lru.put(1, 1); lru.put(2, 2); lru.put(3, 3)
    print(f"  put(1,1) put(2,2) put(3,3)")
    lru.get(1)    # 使 key=1 变为最近使用
    print(f"  get(1) -> 1 (变为最近使用)")
    lru.put(4, 4) # key=2 被淘汰(最久未使用)
    print(f"  put(4,4) -> key=2 被淘汰")
    print(f"  get(2): {lru.get(2)} (已淘汰)")
    print(f"  get(1): {lru.get(1)}, get(3): {lru.get(3)}, get(4): {lru.get(4)}")

    # 10. 并查集
    print("\n[10] 并查集:")
    uf = UnionFind(5)
    uf.union(0, 1)
    uf.union(1, 2)
    uf.union(3, 4)
    print(f"  union(0,1) union(1,2) union(3,4)")
    print(f"  connected(0,2): {uf.connected(0, 2)} (应 True)")
    print(f"  connected(0,3): {uf.connected(0, 3)} (应 False)")
    print(f"  连通分量数: {uf.count}")

    print("\n" + "=" * 60)
    print("  全部数据结构演示完毕！")
    print("=" * 60)


if __name__ == "__main__":
    main()
