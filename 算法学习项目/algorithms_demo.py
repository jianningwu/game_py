#!/usr/bin/env python3
"""
================================================================================
脚本功能：算法 Python 实现演示
- 涵盖排序、搜索、递归回溯、DP、贪心、图算法、字符串匹配、双指针等
- 每个算法含详细中文注释、复杂度分析、典型应用示例
- 运行方式：python algorithms_demo.py
- 配套文档：算法详解与对比.md
================================================================================
"""
import heapq
from collections import deque as _deque, defaultdict
from functools import lru_cache
from itertools import permutations, combinations

SEP = "\n" + "=" * 60

###############################################################################
# 1. 排序算法
###############################################################################

def quick_sort(arr):
    """快速排序 O(n log n) -- 原地，不稳定，实际最快"""
    def _partition(low, high):
        # 三数取中选 pivot（避免最坏 O(n²)）
        mid = (low + high) // 2
        pivot = sorted([arr[low], arr[mid], arr[high]])[1]
        if pivot == arr[mid]:
            arr[mid], arr[high] = arr[high], arr[mid]
        elif pivot == arr[low]:
            pass
        else:
            arr[low], arr[high] = arr[high], arr[low]
        pivot = arr[high]

        i = low                              # i 指向第一个 >= pivot 的位置
        for j in range(low, high):
            if arr[j] < pivot:
                arr[i], arr[j] = arr[j], arr[i]
                i += 1
        arr[i], arr[high] = arr[high], arr[i]  # pivot 归位
        return i

    def _sort(low, high):
        if low >= high:
            return
        p = _partition(low, high)
        _sort(low, p - 1)
        _sort(p + 1, high)

    _sort(0, len(arr) - 1)
    return arr


def merge_sort(arr):
    """归并排序 O(n log n) -- 稳定，适合链表/外部排序"""
    if len(arr) <= 1:
        return arr

    mid = len(arr) // 2
    left = merge_sort(arr[:mid])
    right = merge_sort(arr[mid:])

    # 合并两个有序数组
    result = []
    i = j = 0
    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1
    result.extend(left[i:])
    result.extend(right[j:])
    return result


def heap_sort(arr):
    """堆排序 O(n log n) -- 原地 O(1) 额外空间"""
    n = len(arr)

    # 建堆 O(n)：从最后一个非叶节点往前 heapify
    def heapify(size, root):
        largest = root
        left = 2 * root + 1
        right = 2 * root + 2
        if left < size and arr[left] > arr[largest]:
            largest = left
        if right < size and arr[right] > arr[largest]:
            largest = right
        if largest != root:
            arr[root], arr[largest] = arr[largest], arr[root]
            heapify(size, largest)

    for i in range(n // 2 - 1, -1, -1):
        heapify(n, i)

    # 排序：反复取堆顶
    for i in range(n - 1, 0, -1):
        arr[0], arr[i] = arr[i], arr[0]
        heapify(i, 0)

    return arr


###############################################################################
# 2. 搜索算法
###############################################################################

def binary_search(arr, target):
    """二分查找 O(log n) -- 经典版（左闭右闭）"""
    left, right = 0, len(arr) - 1
    while left <= right:
        mid = left + (right - left) // 2
        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1
    return -1


def binary_search_leftmost(arr, target):
    """二分查找变体：找第一个等于 target 的位置（左边界）"""
    left, right = 0, len(arr)
    while left < right:
        mid = left + (right - left) // 2
        if arr[mid] < target:
            left = mid + 1
        else:
            right = mid
    return left if left < len(arr) and arr[left] == target else -1


def binary_search_rightmost(arr, target):
    """二分查找变体：找最后一个等于 target 的位置（右边界）"""
    left, right = 0, len(arr)
    while left < right:
        mid = left + (right - left) // 2
        if arr[mid] <= target:
            left = mid + 1
        else:
            right = mid
    return left - 1 if left > 0 and arr[left - 1] == target else -1


###############################################################################
# 3. 回溯算法
###############################################################################

def backtrack_permutations(nums):
    """回溯：全排列 O(n!)"""
    result = []
    used = [False] * len(nums)

    def backtrack(path):
        if len(path) == len(nums):
            result.append(path[:])
            return
        for i, num in enumerate(nums):
            if used[i]:
                continue
            used[i] = True           # 做选择
            path.append(num)
            backtrack(path)           # 递归
            path.pop()                # 撤销选择
            used[i] = False

    backtrack([])
    return result


def backtrack_n_queens(n):
    """回溯：N 皇后 -- 返回所有解"""
    result = []
    # 用三个集合记录被攻击的位置（避免每次 O(n) 检查）
    cols = set()          # 列
    diag1 = set()         # 主对角线 (row - col)
    diag2 = set()         # 副对角线 (row + col)
    board = [['.'] * n for _ in range(n)]

    def backtrack(row):
        if row == n:
            result.append([''.join(r) for r in board])
            return
        for col in range(n):
            if col in cols or (row - col) in diag1 or (row + col) in diag2:
                continue
            # 做选择
            board[row][col] = 'Q'
            cols.add(col)
            diag1.add(row - col)
            diag2.add(row + col)

            backtrack(row + 1)

            # 撤销
            board[row][col] = '.'
            cols.remove(col)
            diag1.remove(row - col)
            diag2.remove(row + col)

    backtrack(0)
    return result


###############################################################################
# 4. 动态规划
###############################################################################

def dp_knapsack_01(weights, values, capacity):
    """
    0-1 背包 O(n*W): 每件物品只能选一次
    dp[w] = 容量为 w 时的最大价值
    """
    dp = [0] * (capacity + 1)
    # 从后往前遍历容量，保证每件物品只选一次
    for w_i, v_i in zip(weights, values):
        for w in range(capacity, w_i - 1, -1):
            dp[w] = max(dp[w], dp[w - w_i] + v_i)
    return dp[capacity]


def dp_longest_common_subsequence(text1, text2):
    """
    LCS 最长公共子序列 O(m*n)
    dp[i][j] = text1[:i] 和 text2[:j] 的最长公共子序列长度
    """
    m, n = len(text1), len(text2)
    dp = [[0] * (n + 1) for _ in range(m + 1)]

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if text1[i - 1] == text2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1  # 匹配，双方都往前
            else:
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])  # 跳过一方

    # 回溯得到具体子序列
    i, j = m, n
    lcs = []
    while i > 0 and j > 0:
        if text1[i - 1] == text2[j - 1]:
            lcs.append(text1[i - 1])
            i -= 1
            j -= 1
        elif dp[i - 1][j] > dp[i][j - 1]:
            i -= 1
        else:
            j -= 1

    return dp[m][n], ''.join(reversed(lcs))


def dp_longest_increasing_subsequence(arr):
    """
    LIS 最长递增子序列 O(n log n)
    用 patience sorting (耐心排序) + 二分查找
    """
    import bisect
    if not arr:
        return 0, []

    # tails[i] = 长度为 i+1 的递增子序列的最小末尾值
    tails = []
    # prev[i] = arr[i] 在 LIS 中的前驱索引
    prev = [-1] * len(arr)
    # tail_indices[i] = tails[i] 在原数组的索引
    tail_indices = []

    for i, num in enumerate(arr):
        # 二分查找第一个 >= num 的位置（用 bisect_left）
        idx = bisect.bisect_left(tails, num)
        if idx >= len(tails):
            # num 比所有 tails 都大，扩展 tails
            if tails:
                prev[i] = tail_indices[-1]
            tails.append(num)
            tail_indices.append(i)
        else:
            # 替换第一个 >= num 的值，缩小末尾
            tails[idx] = num
            tail_indices[idx] = i
            if idx > 0:
                prev[i] = tail_indices[idx - 1]

    # 回溯构造 LIS
    lis = []
    idx = tail_indices[-1]
    while idx != -1:
        lis.append(arr[idx])
        idx = prev[idx]
    lis.reverse()
    return len(lis), lis


###############################################################################
# 5. 贪心算法
###############################################################################

def greedy_activity_selection(activities):
    """
    活动选择问题：选出最多不重叠的活动
    贪心策略：每次选结束时间最早的
    activities: [(start, end), ...]
    """
    # 按结束时间排序
    sorted_acts = sorted(activities, key=lambda x: x[1])
    selected = [sorted_acts[0]]
    last_end = sorted_acts[0][1]

    for start, end in sorted_acts[1:]:
        if start >= last_end:          # 不重叠
            selected.append((start, end))
            last_end = end

    return selected


def greedy_jump_game(nums):
    """
    跳跃游戏：判断能否到达最后一个位置
    贪心策略：维护最远可达位置
    """
    max_reach = 0
    for i, jump in enumerate(nums):
        if i > max_reach:              # 当前位置不可达
            return False
        max_reach = max(max_reach, i + jump)
        if max_reach >= len(nums) - 1:
            return True
    return False


###############################################################################
# 6. 图算法
###############################################################################

def dijkstra(graph, start):
    """
    Dijkstra 最短路径 O((V+E)log V)
    graph: 邻接表 {节点: [(邻居, 权重), ...]}
    返回: {节点: 最短距离}
    """
    dist = {start: 0}
    # 优先队列: (距离, 节点)
    pq = [(0, start)]
    visited = set()

    while pq:
        d, node = heapq.heappop(pq)
        if node in visited:
            continue
        visited.add(node)

        for neighbor, weight in graph.get(node, []):
            new_dist = d + weight
            if neighbor not in dist or new_dist < dist[neighbor]:
                dist[neighbor] = new_dist
                heapq.heappush(pq, (new_dist, neighbor))

    return dist


def topological_sort(graph):
    """
    拓扑排序 O(V+E) -- Kahn 算法
    graph: 邻接表 (有向图)
    """
    indegree = defaultdict(int)
    for u in graph:
        for v in graph[u]:
            indegree[v] += 1

    q = _deque([node for node in graph if indegree[node] == 0])
    result = []

    while q:
        node = q.popleft()
        result.append(node)
        for neighbor in graph[node]:
            indegree[neighbor] -= 1
            if indegree[neighbor] == 0:
                q.append(neighbor)

    return result


def kruskal_mst(n, edges):
    """
    Kruskal 最小生成树 O(E log E)
    n: 节点数 (0..n-1)
    edges: [(u, v, weight), ...]
    """
    parent = list(range(n))
    rank = [0] * n

    def find(x):
        if parent[x] != x:
            parent[x] = find(parent[x])
        return parent[x]

    def union(x, y):
        rx, ry = find(x), find(y)
        if rx == ry:
            return False
        if rank[rx] < rank[ry]:
            parent[rx] = ry
        elif rank[rx] > rank[ry]:
            parent[ry] = rx
        else:
            parent[ry] = rx
            rank[rx] += 1
        return True

    # 按权重排序
    sorted_edges = sorted(edges, key=lambda x: x[2])
    mst = []
    total_weight = 0

    for u, v, w in sorted_edges:
        if union(u, v):                    # 不形成环
            mst.append((u, v, w))
            total_weight += w
            if len(mst) == n - 1:           # MST 有 n-1 条边
                break

    return mst, total_weight


###############################################################################
# 7. 字符串匹配 KMP
###############################################################################

def kmp_search(text, pattern):
    """
    KMP 字符串匹配 O(n+m)
    返回所有匹配位置的列表
    """
    if not pattern:
        return []

    # 构建 next 数组 (PMT 部分匹配表)
    # next[j] = pattern[0:j] 的最长相等前后缀长度
    m = len(pattern)
    next_arr = [0] * m
    j = 0                                    # j = 已匹配的前缀长度
    for i in range(1, m):
        while j > 0 and pattern[i] != pattern[j]:
            j = next_arr[j - 1]              # 回退
        if pattern[i] == pattern[j]:
            j += 1
        next_arr[i] = j

    # 在 text 中搜索 pattern
    result = []
    j = 0                                    # j = 已经匹配的 pattern 长度
    n = len(text)
    for i in range(n):
        while j > 0 and text[i] != pattern[j]:
            j = next_arr[j - 1]              # 利用已匹配的信息，不回退 i
        if text[i] == pattern[j]:
            j += 1
        if j == m:
            result.append(i - m + 1)          # 找到匹配
            j = next_arr[j - 1]               # 继续找下一个

    return result


###############################################################################
# 8. 滑动窗口
###############################################################################

def sliding_window_longest_unique(s):
    """
    无重复字符的最长子串 O(n)
    滑动窗口 + 哈希表记录字符最后出现位置
    """
    last_seen = {}                           # 字符 -> 最后出现的索引
    left = 0
    max_len = 0
    best_start = 0

    for right, ch in enumerate(s):
        if ch in last_seen and last_seen[ch] >= left:
            left = last_seen[ch] + 1         # 出现重复，收缩窗口
        last_seen[ch] = right
        if right - left + 1 > max_len:
            max_len = right - left + 1
            best_start = left

    return max_len, s[best_start:best_start + max_len]


def sliding_window_min_subarray(arr, target):
    """
    长度最小的子数组(和 >= target) O(n)
    """
    left = 0
    current_sum = 0
    min_len = float('inf')

    for right, num in enumerate(arr):
        current_sum += num
        while current_sum >= target:         # 满足条件，尝试收缩
            min_len = min(min_len, right - left + 1)
            current_sum -= arr[left]
            left += 1

    return min_len if min_len != float('inf') else 0


###############################################################################
# 9. 双指针
###############################################################################

def two_pointer_two_sum_sorted(arr, target):
    """
    有序数组的两数之和 O(n)
    左右指针从两端逼近
    """
    left, right = 0, len(arr) - 1
    while left < right:
        s = arr[left] + arr[right]
        if s == target:
            return [left, right]
        elif s < target:
            left += 1
        else:
            right -= 1
    return []


def two_pointer_three_sum(arr):
    """
    三数之和 = 0 O(n^2)
    排序 + 固定一个 + 双指针找另外两个
    """
    arr = sorted(arr)
    result = []
    n = len(arr)

    for i in range(n - 2):
        if i > 0 and arr[i] == arr[i - 1]:    # 跳过重复
            continue
        left, right = i + 1, n - 1
        while left < right:
            s = arr[i] + arr[left] + arr[right]
            if s == 0:
                result.append([arr[i], arr[left], arr[right]])
                left += 1
                right -= 1
                # 跳过重复
                while left < right and arr[left] == arr[left - 1]:
                    left += 1
                while left < right and arr[right] == arr[right + 1]:
                    right -= 1
            elif s < 0:
                left += 1
            else:
                right -= 1

    return result


###############################################################################
# 10. 快速幂（位运算）
###############################################################################

def fast_pow(base, exp):
    """
    快速幂 O(log n)：计算 base^exp
    核心: a^13 = a^(1101b) = a^8 * a^4 * a^1
    """
    result = 1
    while exp > 0:
        if exp & 1:                  # 当前最低位是 1
            result *= base
        base *= base                 # base = base^2, base^4, base^8...
        exp >>= 1                    # 指数右移
    return result


###############################################################################
# 主函数
###############################################################################

def main():
    print("=" * 60)
    print("  算法 Python 实现演示")
    print("=" * 60)

    # ---- 1. 排序 ----
    print(f"{SEP}\n[1] 排序算法对比")
    test = [5, 3, 8, 1, 9, 2, 7, 4, 6]
    print(f"  原数组: {test}")
    print(f"  快速排序: {quick_sort(test.copy())}")
    print(f"  归并排序: {merge_sort(test.copy())}")
    print(f"  堆排序:   {heap_sort(test.copy())}")

    # ---- 2. 二分查找 ----
    print(f"{SEP}\n[2] 二分查找及变体")
    arr = [1, 2, 2, 2, 3, 4, 5]
    print(f"  有序数组: {arr}")
    print(f"  binary_search(2): 索引 {binary_search(arr, 2)}")
    print(f"  左边界(第一个2): 索引 {binary_search_leftmost(arr, 2)}")
    print(f"  右边界(最后一个2): 索引 {binary_search_rightmost(arr, 2)}")

    # ---- 3. 回溯 ----
    print(f"{SEP}\n[3] 回溯算法")
    print(f"  全排列 [1,2,3]: {backtrack_permutations([1, 2, 3])[:3]}...")
    queens = backtrack_n_queens(4)
    print(f"  4皇后解的数量: {len(queens)}")
    for sol in queens[:2]:
        for row in sol:
            print(f"    {row}")

    # ---- 4. 动态规划 ----
    print(f"{SEP}\n[4] 动态规划")
    # 0-1 背包
    w = [2, 3, 4, 5]
    v = [3, 4, 5, 6]
    cap = 8
    print(f"  0-1背包: weight={w}, value={v}, capacity={cap}")
    print(f"  最大价值: {dp_knapsack_01(w, v, cap)}")

    # LCS
    lcs_len, lcs_str = dp_longest_common_subsequence("abcde", "ace")
    print(f"  LCS('abcde', 'ace'): 长度={lcs_len}, 子序列='{lcs_str}'")

    # LIS
    lis_len, lis_seq = dp_longest_increasing_subsequence([10, 9, 2, 5, 3, 7, 101, 18])
    print(f"  LIS([10,9,2,5,3,7,101,18]): 长度={lis_len}, 序列={lis_seq}")

    # ---- 5. 贪心 ----
    print(f"{SEP}\n[5] 贪心算法")
    acts = [(1, 4), (3, 5), (0, 6), (5, 7), (3, 8), (5, 9), (6, 10), (8, 11)]
    print(f"  活动选择(共{len(acts)}个): {sorted(acts, key=lambda x: x[1])}")
    print(f"  最多可选: {len(greedy_activity_selection(acts))} 个")
    print(f"  跳跃游戏 [2,3,1,1,4]: {greedy_jump_game([2, 3, 1, 1, 4])}")
    print(f"  跳跃游戏 [3,2,1,0,4]: {greedy_jump_game([3, 2, 1, 0, 4])}")

    # ---- 6. 图算法 ----
    print(f"{SEP}\n[6] 图算法")
    # Dijkstra
    graph = {
        'A': [('B', 4), ('C', 2)],
        'B': [('C', 1), ('D', 5)],
        'C': [('D', 8), ('E', 10)],
        'D': [('E', 2)],
        'E': [],
    }
    dist = dijkstra(graph, 'A')
    print(f"  Dijkstra 最短路径(A为起点): {dict(sorted(dist.items()))}")

    # Kruskal
    edges = [(0, 1, 4), (0, 2, 2), (1, 2, 1), (1, 3, 5), (2, 3, 8), (2, 4, 10), (3, 4, 2)]
    mst, total = kruskal_mst(5, edges)
    print(f"  Kruskal MST: {mst}, 总权重={total}")

    # 拓扑排序
    dag = {'A': ['C'], 'B': ['C', 'E'], 'C': ['D'], 'D': [], 'E': []}
    print(f"  拓扑排序: {topological_sort(dag)}")

    # ---- 7. KMP ----
    print(f"{SEP}\n[7] KMP 字符串匹配")
    text = "ABABDABACDABABCABAB"
    pattern = "ABABC"
    print(f"  text='{text}', pattern='{pattern}'")
    print(f"  匹配位置: {kmp_search(text, pattern)}")

    # ---- 8. 滑动窗口 ----
    print(f"{SEP}\n[8] 滑动窗口")
    s = "abcabcbb"
    max_len, substr = sliding_window_longest_unique(s)
    print(f"  无重复字符最长子串 '{s}': 长度={max_len}, 子串='{substr}'")
    print(f"  最短子数组(和>=7) [2,3,1,2,4,3]: {sliding_window_min_subarray([2,3,1,2,4,3], 7)}")

    # ---- 9. 双指针 ----
    print(f"{SEP}\n[9] 双指针")
    print(f"  有序数组两数之和 [2,7,11,15], target=9: {two_pointer_two_sum_sorted([2,7,11,15], 9)}")
    print(f"  三数之和 [-1,0,1,2,-1,-4]: {two_pointer_three_sum([-1, 0, 1, 2, -1, -4])}")

    # ---- 10. 快速幂 ----
    print(f"{SEP}\n[10] 快速幂")
    print(f"  2^10 = {fast_pow(2, 10)}")
    print(f"  3^5  = {fast_pow(3, 5)}")

    print(f"\n{'=' * 60}")
    print("  全部算法演示完毕！")
    print("=" * 60)


if __name__ == "__main__":
    main()
