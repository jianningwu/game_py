#!/usr/bin/env python3
"""
================================================================================
脚本功能：Python 全知识点教学程序
- 覆盖 Python 编程语言几乎所有核心知识点（23个模块）
- 每个模块以独立函数组织，附带详细中文注释
- 每个知识点包含：概念说明 + 示例代码 + 运行结果展示
- 可直接运行：python python_full_tutorial.py
- 适合 Python 零基础到进阶的系统学习
================================================================================
"""
import sys
import os

# ============================================================================
# 全局辅助函数：用于打印章节标题
# ============================================================================

def print_section(title, level=1):
    """打印格式化的章节标题，让输出更清晰易读"""
    if level == 1:
        # 一级标题用等号和星号包围
        border = "=" * 70
        print(f"\n\n{border}")
        print(f"  {title}")
        print(f"{border}\n")
    elif level == 2:
        # 二级标题用星号线
        print(f"\n{'*' * 50}")
        print(f"  {title}")
        print(f"{'*' * 50}\n")
    else:
        # 三级标题用短横线
        print(f"\n--- {title} ---")

# ============================================================================
# 第1章：基础语法 —— 变量、数据类型、运算符、类型转换
# ============================================================================

def chapter_01_basics():
    """
    [第1章] Python 基础语法
    涵盖：变量定义、基本数据类型、算术/比较/逻辑运算符、类型转换
    Python 是动态类型语言，变量不需要声明类型，运行时自动推断
    """
    print_section("第1章：基础语法 —— 变量、数据类型、运算符")

    # ----- 变量与数据类型 -----
    print_section("1.1 变量与数据类型", level=2)

    # 整数(int)：Python 3 的整数理论上无上限，只受内存限制
    age = 25
    # 浮点数(float)：IEEE 754 双精度，约15-17位有效数字
    price = 19.99
    # 字符串(str)：用单引号或双引号包裹，两者等价
    name = "张三"
    # 布尔值(bool)：True 和 False 首字母必须大写
    is_student = True
    # 空值(NoneType)：表示"无"或"空"，类似其他语言的 null
    result = None

    # type() 函数查看变量类型
    print(f"age = {age}, 类型: {type(age).__name__}")          # int
    print(f"price = {price}, 类型: {type(price).__name__}")    # float
    print(f"name = '{name}', 类型: {type(name).__name__}")     # str
    print(f"is_student = {is_student}, 类型: {type(is_student).__name__}")  # bool
    print(f"result = {result}, 类型: {type(result).__name__}") # NoneType

    # ----- 算术运算符 -----
    print_section("1.2 算术运算符", level=2)

    a, b = 10, 3
    print(f"a = {a}, b = {b}")
    print(f"加法 a + b = {a + b}")       # 13
    print(f"减法 a - b = {a - b}")       # 7
    print(f"乘法 a * b = {a * b}")       # 30
    print(f"除法 a / b = {a / b}")       # 3.333...(总是返回float)
    print(f"整除 a // b = {a // b}")     # 3 (向下取整)
    print(f"取余 a % b = {a % b}")       # 1
    print(f"幂运算 a ** b = {a ** b}")   # 1000

    # ----- 比较运算符 -----
    print_section("1.3 比较运算符", level=2)
    x, y = 5, 8
    print(f"x = {x}, y = {y}")
    print(f"x == y : {x == y}")  # 等于
    print(f"x != y : {x != y}")  # 不等于
    print(f"x > y  : {x > y}")   # 大于
    print(f"x < y  : {x < y}")   # 小于
    print(f"x >= y : {x >= y}")  # 大于等于
    print(f"x <= y : {x <= y}")  # 小于等于

    # ----- 逻辑运算符 -----
    print_section("1.4 逻辑运算符", level=2)
    p, q = True, False
    print(f"p = {p}, q = {q}")
    print(f"p and q = {p and q}")  # 与：两个都为True才True
    print(f"p or q  = {p or q}")   # 或：任一为True即True
    print(f"not p   = {not p}")    # 非：取反

    # 短路求值：and 遇到 False 就停止，or 遇到 True 就停止
    print(f"'hello' and 0 and 'world' = {'hello' and 0 and 'world'}")  # 返回0(第一个False)
    print(f"0 or '' or 'hello' = {0 or '' or 'hello'}")                # 返回'hello'(第一个True)

    # ----- 类型转换 -----
    print_section("1.5 类型转换", level=2)
    # int() 转整数：截断小数部分（不是四舍五入）
    print(f"int(3.14) = {int(3.14)}")        # 3
    print(f"int('123') = {int('123')}")      # 123 (字符串转整数)
    # float() 转浮点数
    print(f"float(10) = {float(10)}")        # 10.0
    print(f"float('3.14') = {float('3.14')}") # 3.14
    # str() 转字符串
    print(f"str(100) = '{str(100)}'")        # '100'
    # bool() 转布尔：0、空字符串、None、空容器为False，其他为True
    print(f"bool(0) = {bool(0)}, bool(42) = {bool(42)}")
    print(f"bool('') = {bool('')}, bool('hello') = {bool('hello')}")


# ============================================================================
# 第2章：字符串操作 —— 索引、切片、格式化、常用方法
# ============================================================================

def chapter_02_strings():
    """
    [第2章] 字符串操作
    字符串是 Python 中最常用的数据类型之一。它是不可变序列（一旦创建不能修改）。
    """
    print_section("第2章：字符串操作")

    # ----- 索引与切片 -----
    print_section("2.1 索引与切片", level=2)
    s = "Hello, Python!"
    # 索引：正数从0开始从左到右，负数从-1开始从右到左
    print(f"原始字符串: '{s}'")
    print(f"s[0]  = '{s[0]}'")     # 第一个字符 'H'
    print(f"s[-1] = '{s[-1]}'")    # 倒数第一个字符 '!'
    print(f"s[7]  = '{s[7]}'")     # 第8个字符 'P'

    # 切片语法: s[start:end:step]
    # start 包含，end 不包含，step 步长（默认1）
    print(f"s[0:5]   = '{s[0:5]}'")     # 'Hello' (索引0到4)
    print(f"s[7:]    = '{s[7:]}'")      # 'Python!' (从索引7到最后)
    print(f"s[:5]    = '{s[:5]}'")      # 'Hello' (从开头到索引4)
    print(f"s[::2]   = '{s[::2]}'")     # 'Hlo yhn' (每2个取1个)
    print(f"s[::-1]  = '{s[::-1]}'")    # '!nohtyP ,olleH' (反转字符串)

    # ----- 字符串格式化 -----
    print_section("2.2 字符串格式化", level=2)
    name, age = "李明", 28
    # f-string (Python 3.6+，最推荐的方式)
    print(f"f-string: 我叫{name}，今年{age}岁，明年{age + 1}岁")
    # str.format() (Python 2.6+)
    print("format(): 我叫{}，今年{}岁".format(name, age))
    print("format(): 我叫{0}，今年{1}岁，{0}说你好".format(name, age))
    print("format(): 我叫{n}，今年{a}岁".format(n=name, a=age))
    # % 格式化 (旧式风格，兼容老代码)
    print("百分号: 我叫%s，今年%d岁" % (name, age))
    # 数字格式化
    pi = 3.1415926
    print(f"pi 保留2位小数: {pi:.2f}")
    print(f"百分比格式: {0.856:.1%}")
    print(f"千分位分隔: {1000000:,}")

    # ----- 常用字符串方法 -----
    print_section("2.3 常用字符串方法", level=2)
    text = "  Hello World!  "
    print(f"原始: '{text}'")
    print(f".strip()  去首尾空格: '{text.strip()}'")
    print(f".upper()  转大写: '{text.upper()}'")
    print(f".lower()  转小写: '{text.lower()}'")
    print(f".replace('World', 'Python') 替换: '{text.replace('World', 'Python')}'")
    print(f".split(' ') 按空格分割: {text.split(' ')}")
    # join 是 split 的逆操作
    words = ['Python', 'is', 'great']
    print(f"' '.join(words) 用空格连接: '{' '.join(words)}'")
    print(f"'Hello' in text 是否包含: {'Hello' in text}")
    print(f".startswith('  He') 是否以xx开头: {text.startswith('  He')}")
    print(f".endswith('! ') 是否以xx结尾: {text.endswith('! ')}")
    # find 返回子串位置，找不到返回 -1
    print(f".find('World') 查找位置: {text.find('World')}")


# ============================================================================
# 第3章：列表与元组 —— 序列类型
# ============================================================================

def chapter_03_lists_tuples():
    """
    [第3章] 列表与元组
    列表(list) 是可变的序列，元组(tuple) 是不可变的序列。
    """
    print_section("第3章：列表与元组")

    # ----- 列表基本操作 -----
    print_section("3.1 列表(List)基本操作", level=2)

    # 创建列表：用方括号，元素可以是不同类型（但不推荐混用）
    fruits = ["苹果", "香蕉", "橘子", "葡萄"]
    print(f"原始列表: {fruits}")

    # 增
    fruits.append("西瓜")           # 末尾追加
    fruits.insert(1, "草莓")        # 在指定位置插入
    print(f"增加后: {fruits}")

    # 删
    fruits.remove("香蕉")           # 按值删除（找不到会报错）
    popped = fruits.pop()           # 弹出末尾元素（也可以 pop(index)）
    popped2 = fruits.pop(0)         # 弹出指定位置元素
    print(f"删除的元素: popped={popped}, popped(0)={popped2}")
    print(f"删除后: {fruits}")

    # 改
    fruits[0] = "芒果"              # 直接赋值修改
    print(f"修改后: {fruits}")

    # 查
    print(f"fruits[0] = {fruits[0]}")
    print(f"fruits[-1] = {fruits[-1]}")
    print(f"'橘子'的索引: {fruits.index('橘子')}")  # index 返回第一次出现的位置
    print(f"列表中元素个数: {len(fruits)}")

    # 列表合并与复制
    more = ["菠萝", "樱桃"]
    combined = fruits + more        # + 创建新列表，不改变原列表
    fruits.extend(more)             # extend 在原列表上扩展
    print(f"合并后: {fruits}")
    # 注意：repeated = fruits * 2 创建新列表，元素重复两次

    # ----- 列表排序 -----
    print_section("3.2 列表排序", level=2)
    nums = [3, 1, 4, 1, 5, 9, 2, 6]
    # sorted() 返回新的排序列表，不改变原列表
    print(f"sorted(nums)升序: {sorted(nums)}")
    print(f"sorted(nums, reverse=True)降序: {sorted(nums, reverse=True)}")
    # .sort() 原地排序，改变原列表
    nums.sort()
    print(f"nums.sort()后: {nums}")
    # 按自定义规则排序：key 参数接收一个函数
    words = ["apple", "banana", "pear", "kiwi"]
    print(f"按长度排序: {sorted(words, key=len)}")

    # ----- 列表遍历 -----
    print_section("3.3 列表遍历", level=2)
    for i, fruit in enumerate(fruits, start=1):
        # enumerate 同时获取索引和值，start 指定起始序号
        print(f"  {i}. {fruit}")

    # ----- 元组 -----
    print_section("3.4 元组(Tuple)", level=2)
    # 元组用圆括号定义，不可修改（immutable）
    point = (3, 4)
    print(f"坐标: {point}")
    print(f"x={point[0]}, y={point[1]}")

    # 元组解包（unpacking）
    x, y = point
    print(f"解包后 x={x}, y={y}")

    # 单元素元组需要在后面加逗号
    single = (42,)
    print(f"单元素元组: {single}, 类型: {type(single).__name__}")

    # 为什么用元组？
    # 1. 元组不可变，适合作为字典的 key
    # 2. 元组比列表更省内存
    # 3. 函数返回多个值时，Python 自动打包为元组

# ============================================================================
# 第4章：字典与集合
# ============================================================================

def chapter_04_dicts_sets():
    """
    [第4章] 字典与集合
    字典(dict) 是键值对的映射，基于哈希表实现，查找速度 O(1)。
    集合(set) 是无序不重复元素的集合，同样基于哈希表。
    """
    print_section("第4章：字典与集合")

    # ----- 字典基本操作 -----
    print_section("4.1 字典(Dict)基本操作", level=2)

    # 创建字典的几种方式
    student = {"name": "张三", "age": 20, "score": 95.5}
    # 也可是用 dict() 构造：dict(name="张三", age=20)
    print(f"学生信息: {student}")

    # 访问元素
    print(f"姓名: {student['name']}")           # 直接访问（key不存在会报错）
    print(f"电话: {student.get('phone', 'N/A')}")  # get 方法（key不存在返回默认值）

    # 增/改（写法和上面一样，key 存在就修改，不存在就新增）
    student["score"] = 98.0          # 修改已有 key
    student["email"] = "zhang@example.com"  # 新增 key
    print(f"增/改后: {student}")

    # 删
    removed = student.pop("age")     # 删除并返回 value
    del student["email"]             # 直接删除（不返回）
    print(f"删除后: {student}")

    # ----- 字典遍历 -----
    print_section("4.2 字典遍历", level=2)
    info = {"name": "李四", "age": 25, "city": "北京"}
    print("遍历键值对(.items()):")
    for key, value in info.items():
        print(f"  {key} -> {value}")
    print("遍历键(.keys()):", list(info.keys()))
    print("遍历值(.values()):", list(info.values()))

    # ----- 字典进阶 -----
    print_section("4.3 字典进阶技巧", level=2)
    # 字典合并 (Python 3.9+ 支持 | 运算符)
    a = {"x": 1, "y": 2}
    b = {"y": 3, "z": 4}
    merged = a | b  # 相同 key 以后者为准
    print(f"合并 a|b: {merged}")
    # 也可以用 ** 解包 (Python 3.5+)
    print(f"合并 {{**a, **b}}: {{**a, **b}} = { {**a, **b} }")

    # ----- 集合 -----
    print_section("4.4 集合(Set)基本操作", level=2)
    # 集合中的元素是唯一的，常用于去重
    nums_list = [1, 2, 2, 3, 3, 3, 4]
    unique_nums = set(nums_list)
    print(f"列表去重: {nums_list} -> {unique_nums}")

    # 集合运算
    s1 = {1, 2, 3, 4}
    s2 = {3, 4, 5, 6}
    print(f"s1={s1}, s2={s2}")
    print(f"并集 s1|s2 = {s1 | s2}")          # 所有元素
    print(f"交集 s1&s2 = {s1 & s2}")          # 共同元素
    print(f"差集 s1-s2 = {s1 - s2}")          # s1有s2没有
    print(f"对称差 s1^s2 = {s1 ^ s2}")        # 只在其中一个集合中的元素

    # 集合增删
    s = {1, 2, 3}
    s.add(4)        # 添加元素
    s.remove(1)     # 删除元素（不存在会报错）
    s.discard(999)  # 安全删除（不存在不报错）
    print(f"操作后的集合: {s}")


# ============================================================================
# 第5章：条件判断 —— if/elif/else, match/case
# ============================================================================

def chapter_05_conditionals():
    """
    [第5章] 条件判断
    Python 通过缩进来定义代码块（通常4个空格）。
    """
    print_section("第5章：条件判断")

    # ----- if/elif/else -----
    print_section("5.1 if / elif / else", level=2)

    score = 85
    print(f"成绩: {score}")
    if score >= 90:
        grade = "A"
    elif score >= 80:
        grade = "B"      # 85 走这个分支
    elif score >= 70:
        grade = "C"
    elif score >= 60:
        grade = "D"
    else:
        grade = "F"
    print(f"等级: {grade}")

    # if 条件的"真值测试"：0、空字符串、None、空容器都是 False
    name = ""
    if name:
        print("名字不为空")
    else:
        print("名字为空，走 else 分支")   # 这个会输出

    # ----- 三元表达式(条件表达式) -----
    print_section("5.2 三元表达式", level=2)
    age = 18
    status = "成年" if age >= 18 else "未成年"
    print(f"年龄{age}: {status}")

    # ----- match / case (Python 3.10+) -----
    print_section("5.3 match / case 模式匹配 (Python 3.10+)", level=2)
    # 类似其他语言的 switch-case，但更强大（支持模式匹配）

    def http_status_desc(code):
        """将 HTTP 状态码转为描述"""
        match code:
            case 200:
                return "成功"
            case 301 | 302:         # | 表示"或"，匹配多个值
                return "重定向"
            case 404:
                return "未找到"
            case 500:
                return "服务器错误"
            case _:                 # _ 是通配符，匹配任何值（类似 default）
                return "未知状态码"

    for code in [200, 302, 404, 500, 418]:
        print(f"  HTTP {code}: {http_status_desc(code)}")

    # match 也可以匹配数据结构
    def describe_point(pt):
        match pt:
            case (0, 0):
                return "原点"
            case (0, y):
                return f"Y轴上的点(y={y})"
            case (x, 0):
                return f"X轴上的点(x={x})"
            case (x, y):
                return f"普通点({x}, {y})"

    print(f"  (0, 0): {describe_point((0, 0))}")
    print(f"  (0, 5): {describe_point((0, 5))}")
    print(f"  (3, 0): {describe_point((3, 0))}")
    print(f"  (3, 5): {describe_point((3, 5))}")


# ============================================================================
# 第6章：循环 —— for/while, break/continue
# ============================================================================

def chapter_06_loops():
    """
    [第6章] 循环
    for 循环遍历可迭代对象，while 循环在条件为真时重复执行。
    """
    print_section("第6章：循环")

    # ----- for 循环 -----
    print_section("6.1 for 循环基础", level=2)

    # 遍历列表
    print("遍历列表:")
    for color in ["红", "绿", "蓝"]:
        print(f"  颜色: {color}")

    # range(start, stop, step)：生成整数序列
    print("range(5):", list(range(5)))           # 0到4
    print("range(2, 6):", list(range(2, 6)))     # 2到5
    print("range(0, 10, 3):", list(range(0, 10, 3)))  # 0,3,6,9

    for i in range(3):
        print(f"  循环第{i}次")

    # ----- enumerate, zip -----
    print_section("6.2 enumerate 和 zip", level=2)

    # enumerate 同时获取索引和值
    names = ["Alice", "Bob", "Charlie"]
    print("enumerate 示例:")
    for idx, name in enumerate(names, start=1):  # start 指定起始编号
        print(f"  {idx}. {name}")

    # zip 将多个可迭代对象"压缩"在一起，按最短的截断
    print("zip 示例:")
    subjects = ["数学", "语文", "英语"]
    scores = [95, 87, 92]
    for subj, score in zip(subjects, scores):
        print(f"  {subj}: {score}分")

    # ----- while 循环 -----
    print_section("6.3 while 循环", level=2)
    count = 3
    while count > 0:
        print(f"  倒计时: {count}")
        count -= 1

    # ----- break / continue / pass -----
    print_section("6.4 break / continue / pass", level=2)
    print("break: 终止整个循环")
    for i in range(10):
        if i == 5:
            break
        print(f"  {i}", end=" ")
    print()

    print("continue: 跳过当前迭代，继续下一次循环")
    for i in range(10):
        if i % 2 == 0:
            continue       # 跳过偶数
        print(f"  {i}", end=" ")
    print()

    # pass 是不做任何事的占位符，用于语法上需要语句但逻辑上暂不处理的地方
    for i in range(3):
        if i == 1:
            pass  # TODO: 以后处理
        else:
            print(f"  处理 i={i}")

    # ----- for-else / while-else -----
    print_section("6.5 for/while-else (Python 独有特性)", level=2)
    # else 块在循环正常结束（没有被 break 打断）时执行
    for i in range(3):
        if i == 10:    # 永远不会触发
            break
    else:
        print("  for 循环正常结束（没被 break）")
    # 这个特性常用于"搜索"场景：搜到了 break，没搜到走 else


# ============================================================================
# 第7章：函数
# ============================================================================

def chapter_07_functions():
    """
    [第7章] 函数
    函数是组织代码的基本单位，用 def 关键字定义。
    Python 函数是一等公民：可以赋值给变量、作为参数传递、作为返回值。
    """
    print_section("第7章：函数基础")

    # ----- 函数定义与调用 -----
    print_section("7.1 函数定义与调用", level=2)

    def greet(name, greeting="你好"):
        """
        一个简单的问候函数
        Args:
            name: 姓名
            greeting: 问候语（有默认值）
        Returns:
            完整的问候字符串
        """
        return f"{greeting}, {name}!"

    # 位置参数调用
    print(greet("小明"))
    # 关键字参数调用（可以不按顺序）
    print(greet(greeting="Hello", name="Tom"))

    # ----- 参数形式 -----
    print_section("7.2 各种参数形式", level=2)

    # *args：接收任意数量的位置参数，打包为元组
    def sum_all(*args):
        """求所有参数的和"""
        return sum(args)

    print(f"sum_all(1,2,3) = {sum_all(1, 2, 3)}")
    print(f"sum_all(10,20,30,40) = {sum_all(10, 20, 30, 40)}")

    # **kwargs：接收任意数量的关键字参数，打包为字典
    def print_info(**kwargs):
        """打印所有传入的关键字参数"""
        for key, value in kwargs.items():
            print(f"  {key} = {value}")

    print("print_info(name='张三', age=20, city='上海'):")
    print_info(name="张三", age=20, city="上海")

    # 组合使用：位置参数 -> *args -> 关键字参数 -> **kwargs
    def demo(a, b, *args, default="默认值", **kwargs):
        print(f"  a={a}, b={b}, args={args}, default={default}, kwargs={kwargs}")

    print("当位置/可变/关键字参数全部使用时:")
    demo(1, 2, 3, 4, default="新值", x=10, y=20)

    # ----- 作用域 -----
    print_section("7.3 变量作用域", level=2)
    # LEGB 规则：Local -> Enclosing -> Global -> Built-in
    x = "全局变量"

    def outer():
        x = "外层函数的局部变量"

        def inner():
            x = "内层函数的局部变量"  # 这是全新的局部变量
            print(f"  inner 中的 x: {x}")

        inner()
        print(f"  outer 中的 x: {x}")

    outer()
    print(f"  全局的 x: {x}")

    # 如需在函数内修改全局变量，使用 global 关键字（谨慎使用）
    counter = 0

    def increment():
        global counter      # 声明要修改全局变量
        counter += 1

    increment()
    increment()
    print(f"全局 counter 修改后: {counter}")

    # 如需修改外层（非全局）函数的变量，使用 nonlocal
    def make_counter():
        count = 0
        def increment():
            nonlocal count  # 声明要修改外层函数的变量
            count += 1
            return count
        return increment    # 返回内部函数（闭包）

    c = make_counter()
    print(f"nonlocal示例: {c()}, {c()}, {c()}")

    # ----- 类型提示 -----
    print_section("7.4 类型提示(Type Hints, Python 3.5+)", level=2)
    # 类型提示不影响运行时行为，但有助于 IDE 检查和文档生成

    def add(a: int, b: int) -> int:
        """类型提示示例：两个整数相加，返回整数"""
        return a + b

    print(f"add(3, 5) = {add(3, 5)}")
    # Python 不强制执行类型标注，传字符串也能运行
    print(f"add('hello', 'world') = {add('hello', 'world')}")  # 虽然标注了int，但str也能+

    # 更多类型提示示例
    from typing import List, Dict, Optional, Union

    def process_items(items: List[str], default: Optional[str] = None) -> Dict[str, str]:
        """处理字符串列表，返回字典。Optional[str] 表示可以是 str 或 None"""
        result = {}
        for item in items:
            result[item] = default or "未设置"
        return result

    print(f"process_items: {process_items(['a', 'b'], 'value')}")


# ============================================================================
# 第8章：高级函数 —— lambda, map/filter/reduce, 闭包
# ============================================================================

def chapter_08_advanced_functions():
    """
    [第8章] 高级函数
    函数在 Python 中是一等公民，可以像变量一样传递和使用。
    """
    print_section("第8章：高级函数")

    # ----- lambda 匿名函数 -----
    print_section("8.1 lambda 表达式", level=2)
    # lambda 参数: 返回值（只能包含一个表达式，不能有语句）
    square = lambda x: x ** 2
    print(f"square(5) = {square(5)}")

    # lambda 常用于排序、过滤等需要临时函数的地方
    pairs = [(1, "one"), (4, "four"), (2, "two"), (3, "three")]
    # 按第二个元素（字符串）排序
    sorted_pairs = sorted(pairs, key=lambda pair: pair[1])
    print(f"按字符串排序: {sorted_pairs}")

    # ----- map / filter / reduce -----
    print_section("8.2 map / filter / reduce", level=2)

    # map(func, iterable)：对可迭代对象的每个元素执行 func
    nums = [1, 2, 3, 4, 5]
    # map 返回迭代器，用 list() 转换为列表
    doubled = list(map(lambda x: x * 2, nums))
    print(f"map 翻倍: {doubled}")

    # filter(func, iterable)：保留 func 返回 True 的元素
    evens = list(filter(lambda x: x % 2 == 0, nums))
    print(f"filter 偶数: {evens}")

    # reduce(func, iterable)：累积地将 func 应用于序列
    # Python 3 中 reduce 移到了 functools 模块
    from functools import reduce
    total = reduce(lambda a, b: a + b, nums)  # 计算 1+2+3+4+5
    print(f"reduce 求和: {total}")

    # 注意：map/filter 在很多场景下可以用推导式代替，更 Pythonic
    doubled_lc = [x * 2 for x in nums]   # 等同 map
    evens_lc = [x for x in nums if x % 2 == 0]  # 等同 filter
    print(f"列表推导式 翻倍: {doubled_lc}")
    print(f"列表推导式 偶数: {evens_lc}")

    # ----- 闭包 -----
    print_section("8.3 闭包(Closure)", level=2)
    # 闭包 = 函数 + 它捕获的外部变量（自由变量）

    def make_multiplier(factor):
        """创建一个乘法器函数，factor 被闭包捕获"""
        def multiplier(x):
            return x * factor  # factor 来自外部函数作用域
        return multiplier

    double = make_multiplier(2)
    triple = make_multiplier(3)
    print(f"double(10) = {double(10)}")
    print(f"triple(10) = {triple(10)}")

    # 使用闭包可以实现"带状态的函数"
    def make_averager():
        """创建一个平均值计算器，保存历史数值"""
        numbers = []            # 这个列表被闭包捕获
        def averager(value):
            numbers.append(value)
            return sum(numbers) / len(numbers)
        return averager

    avg = make_averager()
    print(f"avg(10) = {avg(10)}")   # 10.0
    print(f"avg(20) = {avg(20)}")   # 15.0
    print(f"avg(30) = {avg(30)}")   # 20.0


# ============================================================================
# 第9章：装饰器
# ============================================================================

def chapter_09_decorators():
    """
    [第9章] 装饰器
    装饰器是一种在运行时修改函数或类行为的设计模式。
    本质是一个接受函数作为参数并返回新函数的高阶函数。
    用途：日志、计时、权限检查、缓存、注册等。
    """
    print_section("第9章：装饰器")

    import time
    import functools

    # ----- 无参装饰器 -----
    print_section("9.1 无参装饰器", level=2)

    def timer(func):
        """装饰器：测量函数执行时间"""
        @functools.wraps(func)  # 保留原函数的元信息(__name__, __doc__等)
        def wrapper(*args, **kwargs):
            start = time.perf_counter()
            result = func(*args, **kwargs)
            elapsed = time.perf_counter() - start
            print(f"  [计时] {func.__name__} 执行耗时: {elapsed:.6f}秒")
            return result
        return wrapper

    @timer  # 等价于: slow_function = timer(slow_function)
    def slow_function():
        """模拟耗时操作"""
        total = sum(range(1000000))
        return total

    result = slow_function()
    print(f"  结果: {result}")

    # 不加 @functools.wraps 的后果：函数名和文档会丢失
    def bad_decorator(func):
        def wrapper(*args, **kwargs):
            return func(*args, **kwargs)
        return wrapper

    @bad_decorator
    def test_func():
        """这是测试函数"""
        pass

    print(f"没有 @wraps: __name__ = {test_func.__name__}")  # wrapper

    # ----- 带参装饰器 -----
    print_section("9.2 带参装饰器", level=2)
    # 带参装饰器 = 返回装饰器的函数（多嵌套一层）

    def retry(max_attempts=3):
        """装饰器：函数执行失败时自动重试"""
        def decorator(func):
            @functools.wraps(func)
            def wrapper(*args, **kwargs):
                for attempt in range(1, max_attempts + 1):
                    try:
                        return func(*args, **kwargs)
                    except Exception as e:
                        if attempt == max_attempts:
                            raise  # 最后一次仍失败，抛出异常
                        print(f"  [重试] {func.__name__} 第{attempt}次失败: {e}")
            return wrapper
        return decorator

    import random

    @retry(max_attempts=5)
    def flaky_function():
        """模拟不稳定函数，40%概率失败"""
        if random.random() < 0.4:
            raise ValueError("随机失败!")
        return "成功"

    print(f"不稳定函数结果: {flaky_function()}")

    # ----- 常用内置装饰器 -----
    print_section("9.3 常用内置装饰器", level=2)

    class MyClass:
        class_var = "类变量"

        @staticmethod   # 静态方法：不需要 self，不访问实例和类
        def static_example():
            return "我是静态方法"

        @classmethod    # 类方法：第一个参数是类本身(cls)，可访问类变量
        def class_example(cls):
            return f"我是类方法，可以访问 {cls.class_var}"

        @property       # 属性装饰器：让方法像属性一样访问
        def full_name(self):
            return f"{self.first}_{self.last}"

        def __init__(self):
            self.first = "张"
            self.last = "三"

    print(f"@staticmethod: {MyClass.static_example()}")
    print(f"@classmethod: {MyClass.class_example()}")
    obj = MyClass()
    print(f"@property: {obj.full_name}")  # 不需要加括号调用


# ============================================================================
# 第10章：生成器与迭代器
# ============================================================================

def chapter_10_generators_iterators():
    """
    [第10章] 生成器与迭代器
    迭代器是实现了 __iter__ 和 __next__ 的对象。
    生成器是一种特殊的迭代器，用 yield 关键字定义，惰性计算，省内存。
    """
    print_section("第10章：生成器与迭代器")

    # ----- 迭代器基础 -----
    print_section("10.1 迭代器(Iterator)基础", level=2)
    # 可迭代对象(Iterable)：能用 for 循环遍历的对象(list/str/dict等)
    # 迭代器(Iterator)：能被 next() 调用的对象

    my_list = [1, 2, 3]
    iterator = iter(my_list)     # iter() 从可迭代对象获取迭代器
    print(f"next: {next(iterator)}")  # 1
    print(f"next: {next(iterator)}")  # 2
    print(f"next: {next(iterator)}")  # 3
    try:
        next(iterator)                 # 没有元素了，抛出 StopIteration
    except StopIteration:
        print("迭代器已耗尽(StopIteration)")

    # ----- 生成器 -----
    print_section("10.2 生成器(Generator)", level=2)

    def fibonacci(n):
        """生成斐波那契数列的前n个数（使用yield）"""
        a, b = 0, 1
        for _ in range(n):
            yield a      # yield 暂停函数，返回一个值
            a, b = b, a + b  # 下次调用时从 yield 之后继续执行

    print("斐波那契数列(前10个):", list(fibonacci(10)))

    # 生成器的核心优势：惰性计算(懒加载)，不一次性占内存
    import sys

    # 列表：一次性创建1千万个元素（占用大量内存）
    # nums_list = [i for i in range(10000000)]  # ~80MB
    # 生成器：仅在需要时计算下一个值（几乎不占内存）
    nums_gen = (i for i in range(10000000))  # 生成器表达式
    print(f"生成器大小: {sys.getsizeof(nums_gen)} 字节")

    # ----- yield from -----
    print_section("10.3 yield from (委托子生成器)", level=2)
    # yield from 将一个生成器的迭代委托给另一个生成器

    def chain_generators(*iterables):
        """串联多个可迭代对象（类似 itertools.chain）"""
        for it in iterables:
            yield from it  # 逐个yield子生成器的元素

    combined = chain_generators([1, 2], [3, 4, 5], [6])
    print(f"yield from 串联: {list(combined)}")

    # ----- itertools 常用工具 -----
    print_section("10.4 itertools 常用工具", level=2)
    from itertools import chain, cycle, islice, permutations, combinations

    # count: 无限计数器
    # cycle: 无限循环
    # islice: 对迭代器切片
    print(f"islice(cycle('ABC'), 7): {list(islice(cycle('ABC'), 7))}")

    # 排列组合
    items = ['A', 'B', 'C']
    print(f"permutations(排列,2个): {list(permutations(items, 2))}")
    print(f"combinations(组合,2个): {list(combinations(items, 2))}")


# ============================================================================
# 第11章：面向对象编程(上) —— 类基础
# ============================================================================

def chapter_11_oop_basics():
    """
    [第11章] 面向对象编程基础
    类(Class) 是创建对象的蓝图，对象(Object) 是类的实例。
    Python 的一切都是对象，包括数字、函数、模块。
    """
    print_section("第11章：面向对象编程基础")

    # ----- 类定义与实例化 -----
    print_section("11.1 类定义与实例化", level=2)

    class Dog:
        """狗类 —— OOP入门示例"""
        # 类变量：所有实例共享
        species = "犬科"

        # __init__ 是构造方法，创建实例时自动调用
        # self 代表实例本身，必须是第一个参数（名字约定为self）
        def __init__(self, name: str, age: int):
            # 实例变量：每个实例独有
            self.name = name
            self.age = age

        # 实例方法：操作实例数据
        def bark(self):
            """叫一声"""
            return f"{self.name}说: 汪汪!"

        def celebrate_birthday(self):
            """过生日"""
            self.age += 1
            return f"{self.name}现在{self.age}岁了"

    # 创建实例
    dog1 = Dog("旺财", 3)
    dog2 = Dog("小白", 1)

    print(f"dog1: {dog1.name}, {dog1.age}岁, 物种: {dog1.species}")
    print(f"dog2: {dog2.name}, {dog2.age}岁, 物种: {dog2.species}")
    print(f"dog1: {dog1.bark()}")
    print(f"dog2: {dog2.bark()}")
    print(f"dog1: {dog1.celebrate_birthday()}")

    # 可以动态给实例添加属性（但不推荐，破坏封装性）
    dog1.color = "棕色"
    print(f"dog1 动态添加了 color: {dog1.color}")

    # ----- 类方法、静态方法、属性方法 -----
    print_section("11.2 三种特殊方法", level=2)

    class Calculator:
        """计算器类，演示三种方法类型"""
        precision = 2  # 类变量

        def __init__(self, name=""):
            self.name = name  # 实例变量

        # 实例方法(self)：访问实例和类
        def add(self, a, b):
            """实例方法：可以访问 self 和类变量"""
            return round(a + b, self.precision)

        # 类方法(cls)：通过类调用，第一个参数是类本身
        @classmethod
        def set_precision(cls, value):
            """类方法：修改类变量，影响所有实例"""
            cls.precision = value

        # 静态方法：不需要 self 或 cls，就是放在类里的普通函数
        @staticmethod
        def info():
            """静态方法：和实例、类都没有直接关系"""
            return "这是一个计算器工具"

    calc = Calculator("我的计算器")
    print(f"add: {calc.add(3.14159, 2.71828)}")
    Calculator.set_precision(4)  # 通过类调用类方法
    print(f"调整精度后 add: {calc.add(3.14159, 2.71828)}")
    print(f"info: {Calculator.info()}")

    # ----- __slots__ -----
    print_section("11.3 __slots__ (内存优化)", level=2)

    class SlottedClass:
        """使用 __slots__ 的类不能动态添加属性，但更省内存"""
        __slots__ = ['x', 'y']  # 只允许有 x 和 y 两个属性

        def __init__(self, x, y):
            self.x = x
            self.y = y

    obj = SlottedClass(1, 2)
    print(f"x={obj.x}, y={obj.y}")
    # obj.z = 3  # AttributeError! __slots__ 禁止动态添加属性


# ============================================================================
# 第12章：面向对象编程(下) —— 继承、多态、魔术方法
# ============================================================================

def chapter_12_oop_advanced():
    """
    [第12章] 面向对象编程进阶
    涵盖继承、多态、魔术方法、property 等高级OOP特性。
    """
    print_section("第12章：面向对象编程进阶")

    # ----- 继承 -----
    print_section("12.1 继承(Inheritance)", level=2)

    class Animal:
        """动物基类（父类）"""
        def __init__(self, name):
            self.name = name

        def speak(self):
            """基类的默认行为"""
            return f"{self.name}发出了声音"

    class Cat(Animal):
        """猫类，继承自 Animal"""
        def speak(self):
            """重写(override)父类方法"""
            return f"{self.name}说: 喵喵!"

        def purr(self):
            """子类独有的方法"""
            return f"{self.name}发出咕噜声"

    class Dog2(Animal):
        """狗类，继承自 Animal"""
        def speak(self):
            return f"{self.name}说: 汪汪!"

    animals = [Cat("小花"), Dog2("旺财"), Animal("未知生物")]
    for animal in animals:
        # 多态：同一方法调用，不同对象表现不同行为
        print(f"  {animal.speak()}")

    # 检查继承关系
    print(f"Cat 是 Animal 的子类? {issubclass(Cat, Animal)}")
    print(f"cat 是 Cat 的实例? {isinstance(Cat('小花'), Cat)}")
    print(f"cat 是 Animal 的实例? {isinstance(Cat('小花'), Animal)}")  # 也是!

    # ----- 多重继承 -----
    print_section("12.2 多重继承与 MRO", level=2)

    class A:
        def method(self): return "A"

    class B(A):
        def method(self): return "B"

    class C(A):
        def method(self): return "C"

    class D(B, C):
        pass  # 不重写 method

    d = D()
    print(f"D 的 MRO(方法解析顺序): {[c.__name__ for c in D.__mro__]}")
    print(f"d.method() = {d.method()}")  # 输出 'B'，因为 B 排在 C 前面

    # ----- @property -----
    print_section("12.3 @property (属性装饰器)", level=2)

    class Temperature:
        """温度类，使用 property 实现属性的 getter/setter"""
        def __init__(self, celsius=0):
            self._celsius = celsius  # 下划线前缀约定为"受保护的"属性

        @property
        def celsius(self):
            """获取摄氏度"""
            return self._celsius

        @celsius.setter
        def celsius(self, value):
            """设置摄氏度，带验证"""
            if value < -273.15:
                raise ValueError("温度不能低于绝对零度(-273.15C)!")
            self._celsius = value

        @property
        def fahrenheit(self):
            """计算华氏度(只读属性)"""
            return self._celsius * 9 / 5 + 32

    t = Temperature(37)
    print(f"摄氏度: {t.celsius}, 华氏度: {t.fahrenheit}")
    t.celsius = 100     # 使用 setter
    print(f"修改后: 摄氏度={t.celsius}, 华氏度={t.fahrenheit}")
    # t.fahrenheit = 212  # AttributeError! 没有 setter

    # ----- 魔术方法 -----
    print_section("12.4 魔术方法(Dunder Methods)", level=2)

    class Vector:
        """二维向量，演示常用魔术方法"""

        def __init__(self, x, y):
            self.x = x
            self.y = y

        def __repr__(self):
            """给开发者看的字符串表示（调试用）"""
            return f"Vector({self.x}, {self.y})"

        def __str__(self):
            """给用户看的字符串表示（print 用）"""
            return f"({self.x}, {self.y})"

        def __add__(self, other):
            """定义 + 运算：向量加法"""
            return Vector(self.x + other.x, self.y + other.y)

        def __eq__(self, other):
            """定义 == 比较"""
            return self.x == other.x and self.y == other.y

        def __len__(self):
            """定义 len()，这里返回向量的模（取整）"""
            return int((self.x ** 2 + self.y ** 2) ** 0.5)

        def __getitem__(self, index):
            """支持索引访问：v[0], v[1]"""
            return [self.x, self.y][index]

        def __bool__(self):
            """定义 bool() 行为：零向量为 False"""
            return self.x != 0 or self.y != 0

    v1 = Vector(3, 4)
    v2 = Vector(1, 2)
    print(f"v1 = {v1}")                         # __str__
    print(f"repr(v1) = {repr(v1)}")             # __repr__
    print(f"v1 + v2 = {v1 + v2}")               # __add__
    print(f"v1 == v2: {v1 == v2}")              # __eq__
    print(f"len(v1) = {len(v1)}")               # __len__
    print(f"v1[0]={v1[0]}, v1[1]={v1[1]}")      # __getitem__
    print(f"bool(Vector(0,0)) = {bool(Vector(0, 0))}")  # __bool__


# ============================================================================
# 第13章：异常处理与上下文管理器
# ============================================================================

def chapter_13_exceptions():
    """
    [第13章] 异常处理与上下文管理器
    Python 使用 try/except 捕获异常，而不是返回错误码。
    上下文管理器 with 语句自动管理资源的获取与释放。
    """
    print_section("第13章：异常处理与上下文管理器")

    # ----- try/except/else/finally -----
    print_section("13.1 try/except/else/finally", level=2)

    def divide(a, b):
        """安全除法，演示完整的异常处理流程"""
        try:
            result = a / b
        except ZeroDivisionError:
            print("  错误: 除数不能为零!")
            return None
        except TypeError as e:
            # as 可以捕获异常对象，获取详细信息
            print(f"  错误: 类型不正确 - {e}")
            return None
        else:
            # else 在 try 块没有异常时执行
            print("  (计算成功)")
            return result
        finally:
            # finally 无论如何都会执行（常用于释放资源、关闭文件）
            print("  (finally: 本次除法调用结束)")

    print(f"divide(10, 2) = {divide(10, 2)}")
    print(f"divide(10, 0) = {divide(10, 0)}")
    print(f"divide('a', 2) = {divide('a', 2)}")

    # ----- 自定义异常 -----
    print_section("13.2 自定义异常", level=2)

    class InvalidAgeError(ValueError):
        """自定义异常：无效年龄"""
        def __init__(self, age, message="年龄必须在0到150之间"):
            self.age = age
            self.message = f"{message}，收到: {age}"
            super().__init__(self.message)

    def set_age(age):
        if not 0 <= age <= 150:
            raise InvalidAgeError(age)
        return f"年龄设为 {age}"

    try:
        print(set_age(200))
    except InvalidAgeError as e:
        print(f"捕获到自定义异常: {e}")

    # ----- 上下文管理器 -----
    print_section("13.3 上下文管理器(with 语句)", level=2)
    # with 确保资源被正确释放，即使中间出错也能执行清理

    # 文件操作是最常见的 with 用法
    # with open('test.txt', 'w') as f:
    #     f.write('hello')
    # 离开 with 块时，文件自动关闭

    # 自定义上下文管理器（使用类）
    class ManagedFile:
        """自定义上下文管理器 —— 用 __enter__ 和 __exit__"""
        def __init__(self, filename, mode='w'):
            self.filename = filename
            self.mode = mode

        def __enter__(self):
            """进入 with 块时调用，返回值赋给 as 后面的变量"""
            print(f"  打开文件: {self.filename}")
            self.file = open(self.filename, self.mode)
            return self.file

        def __exit__(self, exc_type, exc_val, exc_tb):
            """离开 with 块时调用，无论是否发生异常"""
            print(f"  关闭文件: {self.filename}")
            self.file.close()
            # 返回 True 可以抑制异常（通常不这样做）
            return False

    # 使用自定义上下文管理器
    with ManagedFile("_test_context.txt", "w") as f:
        f.write("Hello, Context Manager!")
    import os
    os.remove("_test_context.txt")  # 清理测试文件

    # 更简单的自定义上下文管理器（使用 contextlib）
    from contextlib import contextmanager

    @contextmanager
    def managed_resource(name):
        """用生成器函数创建上下文管理器（更简洁）"""
        print(f"  获取资源: {name}")
        try:
            yield name  # yield 之前是 __enter__，yield 之后是 __exit__
        finally:
            print(f"  释放资源: {name}")

    with managed_resource("数据库连接") as resource:
        print(f"  正在使用 {resource}...")


# ============================================================================
# 第14章：文件操作
# ============================================================================

def chapter_14_files():
    """
    [第14章] 文件操作
    涵盖文本文件、二进制文件的读写，以及 pathlib 路径操作。
    """
    print_section("第14章：文件操作")
    import json  # 第21章会详细讲

    # ----- 文件读写基础 -----
    print_section("14.1 文件读写基础", level=2)

    filename = "_test_file.txt"

    # 写入文件
    with open(filename, "w", encoding="utf-8") as f:
        f.write("第一行: Hello Python!\n")
        f.write("第二行: 文件操作示例\n")
        f.writelines(["第三行: writelines示例\n", "第四行: 批量写入\n"])
    print(f"已写入: {filename}")

    # 读取文件
    with open(filename, "r", encoding="utf-8") as f:
        print(f"read()读取全部:\n{f.read()}")

    with open(filename, "r", encoding="utf-8") as f:
        print(f"readline()逐行读取:")
        for i, line in enumerate(f, 1):
            print(f"  第{i}行: {line.strip()}")

    # 文件打开模式说明：
    # 'r'  只读（默认，文件必须存在）
    # 'w'  只写（文件不存在则创建，存在则清空）
    # 'a'  追加（文件不存在则创建，存在则在末尾添加）
    # 'x'  独占创建（文件已存在则报错）
    # 'b'  二进制模式（如 'rb'）
    # '+'  读写模式（如 'r+'）

    # ----- pathlib (Python 3.4+) -----
    print_section("14.2 pathlib 路径操作(推荐替代 os.path)", level=2)
    from pathlib import Path

    # 当前文件路径
    current = Path(filename)
    print(f"文件路径: {current}")
    print(f"绝对路径: {current.absolute()}")
    print(f"文件名: {current.name}")
    print(f"后缀: {current.suffix}")
    print(f"是否存在: {current.exists()}")
    print(f"是否文件: {current.is_file()}")
    print(f"父目录: {current.parent}")

    # 路径拼接（运算符 / 比 os.path.join 更优雅）
    base = Path("D:/test")
    full = base / "subdir" / "file.txt"
    print(f"路径拼接: {full}")

    os.remove(filename)  # 清理


# ============================================================================
# 第15章：模块与包
# ============================================================================

def chapter_15_modules_packages():
    """
    [第15章] 模块与包
    模块(.py文件)是 Python 代码组织的基本单位。
    包是包含 __init__.py 的目录，用于组织多个模块。
    """
    print_section("第15章：模块与包")

    # ----- import 机制 -----
    print_section("15.1 import 语句", level=2)

    # 标准导入方式
    import math                    # 导入整个模块
    print(f"math.sqrt(16) = {math.sqrt(16)}")

    # 导入特定函数/类
    from math import sin, cos, pi  # 从模块导入指定内容
    print(f"sin(pi/2) = {sin(pi/2)}")

    # 导入并重命名（常用于处理长模块名）
    import numpy as np             # 行业约定别名
    from collections import defaultdict as dd

    # 导入模块的所有内容（不推荐，会污染命名空间）
    # from math import *

    # ----- __name__ -----
    print_section("15.2 __name__ 变量", level=2)
    # 当模块被直接运行时，__name__ 为 "__main__"
    # 当模块被导入时，__name__ 为模块文件名
    print(f"当前模块的 __name__: {__name__}")

    # 标准写法（放在文件末尾）：
    # if __name__ == "__main__":
    #     main()

    # ----- 常用标准库模块 -----
    print_section("15.3 常用标准库速览", level=2)
    examples = [
        ("math", "数学函数", "math.sqrt(16)"),
        ("random", "随机数生成", "random.randint(1, 6)"),
        ("datetime", "日期时间", "datetime.datetime.now()"),
        ("os", "操作系统接口", "os.getcwd()"),
        ("sys", "Python解释器", "sys.version"),
        ("json", "JSON编解码", "json.dumps({'a': 1})"),
        ("re", "正则表达式", "re.search(r'\\d+', 'a123')"),
        ("collections", "容器数据类型", "collections.Counter"),
        ("itertools", "迭代器工具", "itertools.permutations"),
        ("functools", "高阶函数工具", "functools.reduce"),
        ("pathlib", "路径操作", "Path('.').absolute()"),
        ("typing", "类型提示", "List[int]"),
        ("argparse", "命令行参数", "argparse.ArgumentParser"),
        ("logging", "日志", "logging.info()"),
    ]
    for module, purpose, example in examples:
        print(f"  {module:15s} - {purpose:12s} 示例: {example}")


# ============================================================================
# 第16章：推导式
# ============================================================================

def chapter_16_comprehensions():
    """
    [第16章] 推导式
    推导式是 Python 特有的简洁语法，用于快速创建列表/字典/集合。
    比传统的 for 循环更简洁、更快速（在 C 层面优化过）。
    """
    print_section("第16章：推导式(Comprehensions)")

    # ----- 列表推导式 -----
    print_section("16.1 列表推导式(List Comprehension)", level=2)

    # 基本语法: [表达式 for 变量 in 可迭代对象 if 条件]
    squares = [x ** 2 for x in range(10)]
    print(f"平方数: {squares}")

    # 带条件的推导式
    even_squares = [x ** 2 for x in range(10) if x % 2 == 0]
    print(f"偶数的平方: {even_squares}")

    # if-else 在表达式位置（注意和上面 filter 的区别）
    labels = ["偶数" if x % 2 == 0 else "奇数" for x in range(5)]
    print(f"奇偶标签: {labels}")

    # ----- 字典推导式 -----
    print_section("16.2 字典推导式(Dict Comprehension)", level=2)
    # 语法: {键表达式: 值表达式 for 变量 in 可迭代对象}

    word_lengths = {word: len(word) for word in ["apple", "banana", "pear"]}
    print(f"单词长度映射: {word_lengths}")

    # 交换键值
    swapped = {v: k for k, v in word_lengths.items()}
    print(f"键值交换: {swapped}")

    # ----- 集合推导式 -----
    print_section("16.3 集合推导式(Set Comprehension)", level=2)
    # 语法: {表达式 for 变量 in 可迭代对象}

    unique_lengths = {len(word) for word in ["a", "ab", "abc", "a", "ab"]}
    print(f"唯一长度: {unique_lengths}")

    # ----- 嵌套推导式 -----
    print_section("16.4 嵌套推导式", level=2)
    # 多层嵌套推导式，等价于嵌套 for 循环（注意可读性）

    # 展平二维列表
    matrix = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
    flattened = [num for row in matrix for num in row]
    print(f"展平矩阵: {flattened}")

    # 生成3x3乘法表
    mul_table = [[i * j for j in range(1, 4)] for i in range(1, 4)]
    print("乘法表:")
    for row in mul_table:
        print(f"  {row}")

    # ----- 生成器表达式(类似推导式但惰性计算) -----
    print_section("16.5 生成器表达式", level=2)
    # 用圆括号代替方括号，生成的是一个迭代器而非列表
    gen = (x ** 2 for x in range(1000000))  # 不占内存
    print(f"生成器表达式类型: {type(gen).__name__}")

    # 推导式 vs 传统循环的性能差异
    # 推导式在 CPython 中是 C 层面实现的，比纯 Python 循环快 20-50%


# ============================================================================
# 第17章：常用内置函数
# ============================================================================

def chapter_17_builtin_functions():
    """
    [第17章] 常用内置函数
    Python 有约70个内置函数，无需导入即可使用。这里按类别介绍最常用的。
    """
    print_section("第17章：常用内置函数")

    # ----- 类型转换类 -----
    print_section("17.1 类型转换类", level=2)
    print(f"int('42') = {int('42')}")
    print(f"float('3.14') = {float('3.14')}")
    print(f"str(100) = '{str(100)}'")
    print(f"bool([]) = {bool([])}")
    print(f"list('abc') = {list('abc')}")
    print(f"tuple([1,2,3]) = {tuple([1, 2, 3])}")
    print(f"set([1,2,2,3]) = {set([1, 2, 2, 3])}")
    print(f"dict([('a',1),('b',2)]) = {dict([('a', 1), ('b', 2)])}")

    # ----- 数学运算类 -----
    print_section("17.2 数学运算类", level=2)
    nums = [5, 3, 8, 1, 9, 2]
    print(f"abs(-5) = {abs(-5)}")
    print(f"round(3.14159, 2) = {round(3.14159, 2)}")
    print(f"sum({nums}) = {sum(nums)}")
    print(f"min({nums}) = {min(nums)}")
    print(f"max({nums}) = {max(nums)}")
    print(f"pow(2, 10) = {pow(2, 10)}")     # 2的10次方 = 1024
    print(f"divmod(17, 5) = {divmod(17, 5)}")  # (商, 余数) = (3, 2)

    # ----- 序列操作类 -----
    print_section("17.3 序列操作类", level=2)
    print(f"len({nums}) = {len(nums)}")
    print(f"sorted({nums}) = {sorted(nums)}")
    print(f"sorted({nums}, reverse=True) = {sorted(nums, reverse=True)}")
    print(f"reversed([1,2,3]) = {list(reversed([1, 2, 3]))}")

    # enumerate, zip, range
    print("enumerate(['a','b','c']):", list(enumerate(['a', 'b', 'c'])))
    print("zip([1,2],['a','b']):", list(zip([1, 2], ['a', 'b'])))
    print("range(5):", list(range(5)))

    # ----- 判断类 -----
    print_section("17.4 判断类", level=2)
    print(f"all([True,1,'a']) = {all([True, 1, 'a'])}")  # 全为真 -> True
    print(f"all([True,0]) = {all([True, 0])}")            # 有假 -> False
    print(f"any([0,'',True]) = {any([0, '', True])}")     # 有真 -> True
    print(f"any([0,'']) = {any([0, ''])}")               # 全为假 -> False
    print(f"isinstance(42, int) = {isinstance(42, int)}")
    print(f"isinstance(42, str) = {isinstance(42, str)}")
    print(f"issubclass(bool, int) = {issubclass(bool, int)}")  # True! bool是int的子类

    # ----- 高阶函数类 -----
    print_section("17.5 高阶函数类", level=2)
    # map, filter (虽然推导式更常用)
    doubled = list(map(str, [1, 2, 3]))
    print(f"map(str, [1,2,3]) = {doubled}")

    # zip 的高级用法：同时遍历多个序列
    names = ["Alice", "Bob", "Charlie"]
    scores = [95, 87, 92]
    print("成绩单:")
    for name, score in zip(names, scores):
        print(f"  {name}: {score}")

    # ----- 其他实用函数 -----
    print_section("17.6 其他实用函数", level=2)
    # dir() 列出对象的属性和方法
    print(f"dir([])[:5] (list的前5个属性): {dir([])[:5]}")
    # help() 获取帮助（在交互式环境中使用）
    # id() 获取对象的内存地址
    a = [1, 2, 3]
    print(f"id(a) = {id(a)}")
    # hash() 获取哈希值（不可变对象才有）
    print(f"hash('hello') = {hash('hello')}")
    # repr() 获取开发者字符串表示
    print(f"repr('hello\\n') = {repr('hello\\n')}")  # 显示转义符
    # eval() 执行字符串表达式（谨慎使用，有安全风险）
    print(f"eval('1+2*3') = {eval('1+2*3')}")


# ============================================================================
# 第18章：正则表达式
# ============================================================================

def chapter_18_regex():
    """
    [第18章] 正则表达式
    正则表达式(RegEx)是一种强大的文本模式匹配工具。
    Python 通过 re 模块提供正则表达式支持。
    """
    print_section("第18章：正则表达式")
    import re

    # ----- 基本匹配 -----
    print_section("18.1 基本匹配", level=2)
    text = "联系方式: 邮箱 zhangsan@example.com, 手机 13812345678"
    # r-string (原始字符串) 避免反斜杠转义问题
    # 匹配邮箱地址
    pattern = r'[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}'
    match = re.search(pattern, text)
    if match:
        print(f"找到邮箱: {match.group()}")

    # ----- 常用模式符号 -----
    print_section("18.2 常用正则符号", level=2)
    print("常用模式符号说明:")
    patterns = [
        (r'\d', "匹配任意数字(0-9)"),
        (r'\w', "匹配字母、数字、下划线"),
        (r'\s', "匹配空白字符"),
        (r'.', "匹配任意字符(除换行符)"),
        (r'^', "匹配字符串开头"),
        (r'$', "匹配字符串结尾"),
        (r'*', "匹配0次或多次"),
        (r'+', "匹配1次或多次"),
        (r'?', "匹配0次或1次"),
        (r'{n}', "匹配恰好n次"),
        (r'{n,m}', "匹配n到m次"),
        (r'[abc]', "匹配a、b、c中的任一个"),
        (r'[^abc]', "匹配不是a、b、c的任意字符"),
        (r'(abc|def)', "匹配abc或def"),
        (r'()', "捕获分组"),
    ]
    for pattern, desc in patterns:
        print(f"  {pattern:15s}  {desc}")

    # ----- re 模块函数 -----
    print_section("18.3 re 模块常用函数", level=2)

    s = "apple=5, banana=3, apple=2, orange=4"

    # re.search() 搜索第一个匹配
    result = re.search(r'\d+', s)
    print(f"search: {result.group()}")  # 5

    # re.findall() 找到所有匹配，返回列表
    nums = re.findall(r'\d+', s)
    print(f"findall: {nums}")  # ['5', '3', '2', '4']

    # re.sub() 替换所有匹配
    replaced = re.sub(r'\d+', 'X', s)
    print(f"sub: {replaced}")  # apple=X, banana=X, apple=X, orange=X

    # re.split() 按模式分割
    parts = re.split(r'[,;]', "a,b;c,d")
    print(f"split: {parts}")  # ['a', 'b', 'c', 'd']

    # ----- 分组 -----
    print_section("18.4 捕获分组", level=2)
    # 用 () 创建分组，用 group(n) 或 groups() 提取
    date_str = "2024-03-15"
    match = re.match(r'(\d{4})-(\d{2})-(\d{2})', date_str)
    if match:
        print(f"完整匹配: {match.group()}")
        print(f"年月日: {match.groups()}")
        print(f"年: {match.group(1)}, 月: {match.group(2)}, 日: {match.group(3)}")

    # 命名分组: (?P<name>...)
    m = re.match(r'(?P<year>\d{4})-(?P<month>\d{2})-(?P<day>\d{2})', date_str)
    if m:
        print(f"命名分组: year={m.group('year')}, month={m.group('month')}")


# ============================================================================
# 第19章：日期时间
# ============================================================================

def chapter_19_datetime():
    """
    [第19章] 日期时间处理
    datetime 模块是最常用的日期时间处理标准库。
    """
    print_section("第19章：日期时间处理")
    from datetime import datetime, date, time, timedelta
    import time as _time

    # ----- 获取当前时间 -----
    print_section("19.1 获取当前时间", level=2)
    now = datetime.now()
    today = date.today()
    print(f"当前完整时间: {now}")
    print(f"当前日期: {today}")
    print(f"当前时间戳(秒): {_time.time()}")

    # ----- 格式化输出 -----
    print_section("19.2 格式化输出(strftime/strptime)", level=2)
    now = datetime.now()
    # strftime: datetime -> 字符串
    print(f"strftime示例:")
    print(f"  %Y-%m-%d: {now.strftime('%Y-%m-%d')}")
    print(f"  %Y年%m月%d日: {now.strftime('%Y年%m月%d日')}")
    print(f"  %H:%M:%S: {now.strftime('%H:%M:%S')}")
    print(f"  ISO格式: {now.isoformat()}")

    # strptime: 字符串 -> datetime
    date_str = "2026-05-28 14:30:00"
    parsed = datetime.strptime(date_str, "%Y-%m-%d %H:%M:%S")
    print(f"strptime解析: {parsed}, 类型: {type(parsed).__name__}")

    # ----- 时间计算(timedelta) -----
    print_section("19.3 时间计算(timedelta)", level=2)
    today = date.today()
    # timedelta 表示两个时间点之间的差
    one_week = timedelta(weeks=1)
    ten_days = timedelta(days=10)
    print(f"今天: {today}")
    print(f"一周后: {today + one_week}")
    print(f"10天前: {today - ten_days}")
    print(f"距离元旦还有: {date(today.year + 1, 1, 1) - today}")

    # 计算年龄
    birth = date(1995, 8, 15)
    age_days = (today - birth).days
    print(f"从{birth}到今天共 {age_days} 天，约 {age_days // 365} 岁")


# ============================================================================
# 第20章：并发编程 —— 多线程、多进程、协程
# ============================================================================

def chapter_20_concurrency():
    """
    [第20章] 并发编程
    - 多线程(threading)：适合 I/O 密集型任务，共享内存，有 GIL 限制
    - 多进程(multiprocessing)：适合 CPU 密集型任务，独立内存，绕过 GIL
    - 协程(asyncio)：单线程并发，适合高并发 I/O，最轻量
    """
    print_section("第20章：并发编程")
    import threading
    import time

    # ----- 多线程 -----
    print_section("20.1 多线程(threading)", level=2)

    def worker(name, delay):
        """模拟 I/O 密集型任务（比如网络请求、文件读写）"""
        print(f"  [线程-{name}] 开始工作")
        time.sleep(delay)   # 模拟 I/O 等待
        print(f"  [线程-{name}] 完成")

    # 创建并启动线程
    t1 = threading.Thread(target=worker, args=("A", 0.5))
    t2 = threading.Thread(target=worker, args=("B", 0.3))

    start = time.time()
    t1.start()
    t2.start()
    t1.join()  # 等待线程结束
    t2.join()
    # 串行需要0.8秒，两个线程并行仅需约0.5秒
    print(f"两个线程总耗时: {time.time() - start:.2f}秒")
    # 注意：Python 有 GIL（全局解释器锁），多线程不能真正并行执行 CPU 密集任务
    # 但 I/O 操作会释放 GIL，所以多线程对 I/O 密集型任务仍然有效

    # ----- 多进程 -----
    print_section("20.2 多进程(multiprocessing)", level=2)

    def cpu_heavy(n):
        """模拟 CPU 密集型任务"""
        return sum(i * i for i in range(n))

    from multiprocessing import Pool

    with Pool(processes=2) as pool:
        results = pool.map(cpu_heavy, [1000000, 2000000])
    print(f"多进程计算结果: {results}")

    # ----- 协程 -----
    print_section("20.3 协程(asyncio 基础)", level=2)
    # 注意：asyncio 需要异步环境，这里展示基本语法

    import asyncio

    async def async_worker(name, delay):
        """异步工作函数（使用 async def 定义）"""
        print(f"  [协程-{name}] 开始")
        await asyncio.sleep(delay)  # await 等待异步操作（非阻塞）
        print(f"  [协程-{name}] 完成")
        return f"结果-{name}"

    async def main():
        """异步入口函数"""
        # 并发运行多个协程
        results = await asyncio.gather(
            async_worker("X", 0.2),
            async_worker("Y", 0.1),
            async_worker("Z", 0.3),
        )
        print(f"  协程结果: {results}")

    asyncio.run(main())
    # 协程的优势：单线程 + 事件循环，处理成千上万个并发连接
    # 适合 Web 服务器、API 调用等高并发 I/O 场景


# ============================================================================
# 第21章：序列化 —— JSON, pickle, CSV
# ============================================================================

def chapter_21_serialization():
    """
    [第21章] 序列化
    序列化：将内存中的数据结构转换为可存储或传输的格式。
    反序列化：将存储的格式还原为内存中的数据结构。
    """
    print_section("第21章：序列化")
    import json
    import pickle
    import csv
    import io

    # ----- JSON -----
    print_section("21.1 JSON (最通用的数据交换格式)", level=2)

    # Python -> JSON (序列化)
    data = {
        "name": "张三",
        "age": 25,
        "skills": ["Python", "SQL"],
        "active": True,
        "score": None
    }
    json_str = json.dumps(data, ensure_ascii=False, indent=2)
    # ensure_ascii=False 保证中文正常显示
    # indent=2 让输出更易读
    print(f"Python dict -> JSON:\n{json_str}")

    # JSON -> Python (反序列化)
    parsed = json.loads(json_str)
    print(f"JSON -> Python dict: {parsed}")

    # 文件读写
    # json.dump(data, open('data.json','w'), ensure_ascii=False)
    # data = json.load(open('data.json','r'))

    # ----- pickle -----
    print_section("21.2 pickle — Python 专用序列化", level=2)
    # pickle 可以序列化几乎所有 Python 对象，但只能在 Python 中使用
    complex_data = {"set": {1, 2, 3}, "tuple": (4, 5), "nested": [6, {7: 8}]}
    pickled = pickle.dumps(complex_data)
    print(f"pickle序列化(二进制): {pickled[:30]}...")
    restored = pickle.loads(pickled)
    print(f"pickle还原: {restored}")
    # 注意：不要 unpickle 不信任的数据，存在安全风险

    # ----- CSV -----
    print_section("21.3 CSV (逗号分隔值)", level=2)
    # CSV 是表格数据最通用的交换格式

    # 写入 CSV
    output = io.StringIO()
    writer = csv.writer(output)
    writer.writerow(["姓名", "年龄", "城市"])
    writer.writerows([
        ["张三", 25, "北京"],
        ["李四", 30, "上海"],
        ["王五", 28, "广州"],
    ])
    csv_content = output.getvalue()
    print("CSV内容:")
    print(csv_content)

    # 读取 CSV
    reader = csv.reader(io.StringIO(csv_content))
    print("解析CSV:")
    for row in reader:
        print(f"  {row}")

    # DictWriter / DictReader (用字典方式操作列名)
    import io as _io
    buf = _io.StringIO()
    dw = csv.DictWriter(buf, fieldnames=["name", "score"])
    dw.writeheader()
    dw.writerows([{"name": "Alice", "score": 95}, {"name": "Bob", "score": 87}])
    print(f"DictWriter输出: {buf.getvalue().strip()}")


# ============================================================================
# 第22章：单元测试
# ============================================================================

def chapter_22_testing():
    """
    [第22章] 单元测试
    单元测试验证代码的每个"单元"（通常是一个函数或方法）是否按预期工作。
    unittest 是 Python 标准库内置的测试框架。
    pytest 是社区最流行的第三方测试框架（更简洁）。
    """
    print_section("第22章：单元测试")

    # ----- 被测函数 -----
    print_section("22.1 被测代码示例", level=2)

    def calculator(operation, a, b):
        """简单计算器（这是我们要测试的代码）"""
        if operation == "add":
            return a + b
        elif operation == "subtract":
            return a - b
        elif operation == "multiply":
            return a * b
        elif operation == "divide":
            if b == 0:
                raise ValueError("除数不能为零")
            return a / b
        else:
            raise ValueError(f"未知操作: {operation}")

    # ----- unittest 示例 -----
    print_section("22.2 unittest 示例", level=2)

    import unittest

    class TestCalculator(unittest.TestCase):
        """计算器函数的测试用例（命名惯例: Test + 被测名称）"""

        def setUp(self):
            """每个测试方法执行前都会调用 setUp，准备测试数据"""
            self.a = 10
            self.b = 3

        def test_add(self):
            """测试加法（方法名必须以 test_ 开头）"""
            self.assertEqual(calculator("add", self.a, self.b), 13)
            self.assertEqual(calculator("add", -1, 1), 0)

        def test_subtract(self):
            """测试减法"""
            self.assertEqual(calculator("subtract", self.a, self.b), 7)

        def test_divide_normal(self):
            """测试正常除法"""
            self.assertAlmostEqual(calculator("divide", 10, 3), 3.333, places=2)

        def test_divide_by_zero(self):
            """测试除以零应抛出异常"""
            with self.assertRaises(ValueError):
                calculator("divide", 10, 0)

        def test_unknown_operation(self):
            """测试未知操作"""
            with self.assertRaises(ValueError):
                calculator("power", 2, 3)

        # 常用断言方法:
        # assertEqual(a, b)       a == b
        # assertNotEqual(a, b)    a != b
        # assertTrue(x)           bool(x) is True
        # assertFalse(x)          bool(x) is False
        # assertIs(a, b)          a is b
        # assertIsNone(x)         x is None
        # assertIn(a, b)          a in b
        # assertRaises(Exc, func) func() 抛出 Exc

    # 运行测试
    # 在命令行中: python -m unittest test_module.py
    # 这里在代码中运行，仅演示结果
    suite = unittest.TestLoader().loadTestsFromTestCase(TestCalculator)
    runner = unittest.TextTestRunner(verbosity=2)
    result = runner.run(suite)
    print(f"\n测试结果: 运行{result.testsRun}个, 成功{result.testsRun - len(result.failures) - len(result.errors)}个, 失败{len(result.failures)}个")


# ============================================================================
# 第23章：日志
# ============================================================================

def chapter_23_logging():
    """
    [第23章] 日志(logging)
    日志是程序运行时记录信息的机制，比 print 更灵活、更专业。
    可以控制输出级别、输出格式、输出目标（控制台/文件/网络等）。
    """
    print_section("第23章：日志(logging)")

    import logging

    # ----- 日志级别（从低到高） -----
    print_section("23.1 日志级别", level=2)
    # DEBUG < INFO < WARNING < ERROR < CRITICAL
    print("日志级别(从低到高):")
    levels = [
        ("DEBUG", logging.DEBUG, "调试信息，开发时使用"),
        ("INFO", logging.INFO, "常规信息，确认一切正常"),
        ("WARNING", logging.WARNING, "警告信息，可能有问题但程序仍运行"),
        ("ERROR", logging.ERROR, "错误信息，某部分功能失败"),
        ("CRITICAL", logging.CRITICAL, "严重错误，程序可能无法继续"),
    ]
    for name, level, desc in levels:
        print(f"  {name:8s} (value={level}): {desc}")

    # ----- 基础配置 -----
    print_section("23.2 基础配置", level=2)

    # 配置日志格式和级别
    logging.basicConfig(
        level=logging.DEBUG,          # 设置最低日志级别
        format='%(asctime)s [%(levelname)-8s] %(name)s: %(message)s',
        datefmt='%H:%M:%S'
    )
    logger = logging.getLogger(__name__)

    logger.debug("这是调试信息（通常生产环境不显示）")
    logger.info("用户登录成功")
    logger.warning("磁盘空间不足")
    # logger.error("数据库连接失败")
    # logger.critical("系统崩溃!")

    # ----- 日志输出到文件 -----
    print_section("23.3 日志输出到文件", level=2)
    # 可以在 basicConfig 中指定 filename
    # logging.basicConfig(filename='app.log', level=logging.INFO)
    # 也可以同时输出到控制台和文件（使用多个 handler）
    print("(日志文件配置示例，此处不实际创建文件)")

    # ----- 最佳实践 -----
    print_section("23.4 日志最佳实践", level=2)
    tips = [
        "每个模块用 logging.getLogger(__name__) 获取自己的logger",
        "用 f-string 时注意：logger.debug 中避免提前求值，用 % 格式化延迟求值",
        "敏感信息（密码、token）不要写入日志",
        "开发环境用 DEBUG 级别，生产环境用 INFO 或 WARNING",
        "长时间运行的程序考虑日志轮转（RotatingFileHandler）",
    ]
    for tip in tips:
        print(f"  - {tip}")


# ============================================================================
# 主函数：串联所有章节
# ============================================================================

def main():
    """运行所有教学章节"""
    print("=" * 70)
    print("    Python 全知识点教学程序")
    print("    覆盖 Python 编程几乎所有核心知识点（23个模块）")
    print("=" * 70)
    print("\n提示：每个章节都可以独立学习，直接跳到感兴趣的内容即可。")

    # 所有章节函数列表
    chapters = [
        ("基础语法", chapter_01_basics),
        ("字符串操作", chapter_02_strings),
        ("列表与元组", chapter_03_lists_tuples),
        ("字典与集合", chapter_04_dicts_sets),
        ("条件判断", chapter_05_conditionals),
        ("循环", chapter_06_loops),
        ("函数基础", chapter_07_functions),
        ("高级函数", chapter_08_advanced_functions),
        ("装饰器", chapter_09_decorators),
        ("生成器与迭代器", chapter_10_generators_iterators),
        ("面向对象基础", chapter_11_oop_basics),
        ("面向对象进阶", chapter_12_oop_advanced),
        ("异常处理", chapter_13_exceptions),
        ("文件操作", chapter_14_files),
        ("模块与包", chapter_15_modules_packages),
        ("推导式", chapter_16_comprehensions),
        ("常用内置函数", chapter_17_builtin_functions),
        ("正则表达式", chapter_18_regex),
        ("日期时间", chapter_19_datetime),
        ("并发编程", chapter_20_concurrency),
        ("序列化", chapter_21_serialization),
        ("单元测试", chapter_22_testing),
        ("日志", chapter_23_logging),
    ]

    # 运行所有章节
    for name, func in chapters:
        try:
            func()
        except Exception as e:
            print(f"\n[警告] 章节 '{name}' 执行出错: {e}")
            import traceback
            traceback.print_exc()

    # 结束语
    print("\n" + "=" * 70)
    print("    全部章节学习完毕！")
    print("    建议按顺序学习，也可以直接运行感兴趣的章节函数。")
    print("    例如: python -c 'from python_full_tutorial import chapter_09_decorators; chapter_09_decorators()'")
    print("=" * 70)


# ============================================================================
# 程序入口
# ============================================================================

# 如果直接运行此文件（而非导入），则执行 main()
if __name__ == "__main__":
    main()
