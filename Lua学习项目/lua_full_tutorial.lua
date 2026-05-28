#!/usr/bin/env lua
--[[
================================================================================
脚本功能：Lua 全知识点教学程序
- 覆盖 Lua 编程语言几乎所有核心知识点（14个模块）
- 每个模块以独立函数组织，附带详细中文注释
- 每个知识点包含：概念说明 + 示例代码 + 运行结果展示
- 可直接运行：lua lua_full_tutorial.lua
- 适合 Lua 零基础到进阶的系统学习
================================================================================
--]]

-- ============================================================================
-- 全局辅助函数：用于打印章节标题
-- ============================================================================

local function print_section(title, level)
    -- 打印格式化的章节标题，让输出更清晰易读
    level = level or 1
    if level == 1 then
        local border = string.rep("=", 60)
        print("\n\n" .. border)
        print("  " .. title)
        print(border .. "\n")
    elseif level == 2 then
        print("\n" .. string.rep("*", 45))
        print("  " .. title)
        print(string.rep("*", 45) .. "\n")
    else
        print("\n--- " .. title .. " ---")
    end
end

-- ============================================================================
-- 第1章：基础语法 —— 变量、数据类型、运算符
-- ============================================================================

local function chapter_01_basics()
    --[[
    [第1章] Lua 基础语法
    Lua 是动态类型语言，只有 8 种基本类型：nil, boolean, number, string,
    function, table, thread, userdata
    ]]
    print_section("第1章：基础语法 -- 变量、数据类型、运算符")

    ---- 变量与数据类型 ----
    print_section("1.1 变量与数据类型", 2)

    -- Lua 变量默认是全局的，使用 local 关键字声明局部变量（推荐！）
    local age = 25           -- number：Lua 5.3+ 区分整数和浮点数
    local price = 19.99      -- number：双精度浮点数
    local name = "张三"       -- string：单引号或双引号都可
    local is_student = true  -- boolean：true 和 false（小写）
    local result = nil       -- nil：表示"无"或"空"，类似其他语言的 null
    -- table 和 function 是引用类型，后面章节会详细讲解

    -- type() 函数查看变量类型
    print(string.format("age = %d, 类型: %s", age, type(age)))
    print(string.format("price = %.2f, 类型: %s", price, type(price)))
    print(string.format("name = '%s', 类型: %s", name, type(name)))
    print(string.format("is_student = %s, 类型: %s", tostring(is_student), type(is_student)))
    print(string.format("result = %s, 类型: %s", tostring(result), type(result)))

    -- nil 的特殊性：访问不存在的 table 字段返回 nil，未赋值的变量也是 nil
    local not_initialized     -- 值为 nil
    print("未初始化的变量类型: " .. type(not_initialized))

    ---- 算术运算符 ----
    print_section("1.2 算术运算符", 2)

    local a, b = 10, 3        -- Lua 支持多重赋值
    print(string.format("a = %d, b = %d", a, b))
    print(string.format("加法 a + b = %d", a + b))
    print(string.format("减法 a - b = %d", a - b))
    print(string.format("乘法 a * b = %d", a * b))
    print(string.format("除法 a / b = %.3f", a / b))     -- 总是返回浮点数
    print(string.format("整除 a // b = %d", a // b))     -- Lua 5.3+ 整除运算符
    print(string.format("取余 a %% b = %d", a % b))       -- %% 是转义，输出 %
    print(string.format("幂运算 a ^ b = %d", a ^ b))      -- ^ 是幂运算符（不是 **）

    ---- Lua 特有：字符串自动转数字 ----
    print_section("1.3 Lua 特有 -- 字符串自动转数字", 2)
    -- Lua 在算术运算中会自动尝试将字符串转为数字（coercion）
    print(string.format('"10" + "5" = %d', "10" + "5"))   -- 15
    -- 但比较时不自动转换：
    print(string.format('"10" > 5  = %s', tostring("10" > 5)))  -- true (Lua自动转换)
    -- 推荐显式转换：tonumber()
    print(string.format("tonumber('10') = %s", tonumber("10")))

    ---- 比较运算符 ----
    print_section("1.4 比较运算符", 2)
    local x, y = 5, 8
    print(string.format("x = %d, y = %d", x, y))
    -- Lua 用 ~= 表示不等于（不是 !=）
    print(string.format("x == y : %s", tostring(x == y)))
    print(string.format("x ~= y : %s", tostring(x ~= y)))   -- 不等于
    print(string.format("x > y  : %s", tostring(x > y)))
    print(string.format("x < y  : %s", tostring(x < y)))
    print(string.format("x >= y : %s", tostring(x >= y)))
    print(string.format("x <= y : %s", tostring(x <= y)))

    ---- 逻辑运算符 ----
    print_section("1.5 逻辑运算符", 2)
    local p, q = true, false
    -- Lua 用 and/or/not，不是 &&/||/!
    print(string.format("p and q = %s", tostring(p and q)))
    print(string.format("p or q  = %s", tostring(p or q)))
    print(string.format("not p   = %s", tostring(not p)))

    -- and/or 的"短路返回"特性（Lua 常用技巧）：
    -- and 返回第一个 false 值或最后一个值
    -- or  返回第一个 true 值或最后一个值
    print(string.format("5 and 10 = %d", 5 and 10))     -- 10
    print(string.format("nil and 10 = %s", tostring(nil and 10)))  -- nil
    print(string.format("nil or 10 = %d", nil or 10))   -- 10（类似 Python 的 or 默认值模式）
    print(string.format("false or 'hi' = %s", false or 'hi'))

    -- Lua 的常用技巧：用 and/or 实现三元表达式
    -- 相当于: condition and true_value or false_value
    local score = 85
    local grade = score >= 90 and "A" or "B"
    print(string.format("成绩 %d 的等级: %s", score, grade))
    -- 注意：true_value 不能为 false/nil，否则这个技巧会失效
end

-- ============================================================================
-- 第2章：字符串操作
-- ============================================================================

local function chapter_02_strings()
    --[[
    [第2章] 字符串操作
    Lua 字符串是不可变的。字符串可以用单引号、双引号或长括号 [[ ]] 表示。
    Lua 有自己的"模式匹配"（pattern），不是完整的正则表达式，但更轻量。
    ]]
    print_section("第2章：字符串操作")

    ---- 字符串定义方式 ----
    print_section("2.1 字符串定义方式", 2)

    local s1 = '单引号字符串'
    local s2 = "双引号字符串"
    -- 长括号：适合多行字符串，不会转义任何字符
    local s3 = [[
        这是一个
        多行字符串
        不需要转义\n
    ]]
    -- 带等号的长括号：用于字符串内包含 ]] 的情况
    local s4 = [=[包含 ]] 的字符串]=]
    print("单引号: " .. s1)
    print("双引号: " .. s2)
    print("长括号: " .. s3)
    print("带等号长括号: " .. s4)

    ---- 字符串拼接与长度 ----
    print_section("2.2 拼接与长度", 2)

    -- Lua 用 .. 拼接字符串（不是 +）
    local greeting = "Hello" .. ", " .. "Lua!"
    print("拼接结果: " .. greeting)
    -- # 获取字符串/table 长度
    print(string.format("字符串长度: %d", #greeting))
    print(string.format("'你好' 的长度: %d", #"你好"))  -- 注意：是字节数，不是字符数！

    ---- 字符串格式化 ----
    print_section("2.3 字符串格式化(string.format)", 2)

    local name, age = "李明", 28
    print(string.format("我叫%s，今年%d岁", name, age))
    print(string.format("浮点数: %.2f", 3.14159))
    print(string.format("十六进制: %x", 255))
    print(string.format("百分比: %d%%", 80))          -- %% 输出 %

    ---- 常用字符串方法 ----
    print_section("2.4 常用字符串方法", 2)
    -- string.xxx 也可以写成 str:xxx（Lua 的方法调用语法糖）
    local text = "  Hello Lua!  "

    print(string.format("原始: '%s'", text))
    -- string.match 用于模式匹配，返回匹配到的字符串
    print(string.format("去掉首尾(%s): '%s'", "空格",
        string.gsub(text, "^%s*(.-)%s*$", "%1")))  -- 模拟 trim

    print(string.format("转大写: '%s'", string.upper(text)))
    print(string.format("转小写: '%s'", string.lower(text)))
    print(string.format("查找 Lua: %d", string.find(text, "Lua")))  -- 返回起始位置
    print(string.format("替换 Lua -> World: '%s'",
        string.gsub(text, "Lua", "World")))  -- gsub 返回替换后字符串和次数
    -- string.sub 截取子串（索引从1开始！）
    print(string.format("截取 sub(4,7): '%s'", string.sub(text, 4, 7)))
    -- string.reverse 反转
    print(string.format("反转: '%s'", string.reverse("Hello")))

    ---- Lua 模式匹配(Pattern) ----
    print_section("2.5 Lua 模式匹配(不是正则!)", 2)
    -- Lua 的模式匹配比正则表达式简单，但效率更高
    -- 常用模式字符：
    -- %d 数字, %w 字母数字, %s 空白, %a 字母, %l 小写, %u 大写
    -- .  任意字符, % 转义
    -- +  1个或多个, * 0个或多个, - 懒惰匹配, ? 0或1个

    local phone_str = "电话: 13812345678"
    -- string.match 返回匹配到的捕获
    local phone = string.match(phone_str, "%d+")
    print(string.format("提取数字: %s", phone))

    -- 用 () 捕获分组
    local date = "2024-03-15"
    local year, month, day = string.match(date, "(%d+)%-(%d+)%-(%d+)")
    print(string.format("年=%s, 月=%s, 日=%s", year, month, day))

    -- gmatch 遍历所有匹配
    local s = "apple=5, banana=3"
    print("遍历所有键值对:")
    for k, v in string.gmatch(s, "(%w+)=(%d+)") do
        print(string.format("  %s -> %s", k, v))
    end
end

-- ============================================================================
-- 第3章：Table（Lua 的核心数据结构）
-- ============================================================================

local function chapter_03_tables()
    --[[
    [第3章] Table -- Lua 核心数据结构
    Table 是 Lua 唯一的数据结构，同时扮演：数组、字典、对象、集合、模块等角色。
    Table 索引从 1 开始（不是 0！），这是 Lua 最重要的设计之一。
    ]]
    print_section("第3章：Table -- Lua 核心数据结构")

    ---- Table 作为数组 ----
    print_section("3.1 Table 作为数组(索引从1开始)", 2)

    -- 创建数组（索引自动从1开始）
    local fruits = {"苹果", "香蕉", "橘子", "葡萄"}
    print("数组 fruits:")
    for i = 1, #fruits do   -- # 获取 table 长度
        print(string.format("  [%d] = %s", i, fruits[i]))
    end

    -- 增删操作
    table.insert(fruits, "西瓜")      -- 末尾追加
    table.insert(fruits, 2, "草莓")   -- 在索引2处插入（后面的元素后移）
    print("insert 后:")
    for i, v in ipairs(fruits) do    -- ipairs 按索引1,2,3...顺序遍历
        print(string.format("  [%d] = %s", i, v))
    end

    local removed = table.remove(fruits, 1)  -- 移除索引1的元素
    print(string.format("remove 索引1: %s", removed))
    table.remove(fruits)             -- 不指定则移除最后一个

    ---- Table 作为字典 ----
    print_section("3.2 Table 作为字典(Hash Map)", 2)

    local student = {
        name = "张三",               -- 等价于 ["name"] = "张三"
        age = 20,
        score = 95.5,
        ["class"] = "一班",          -- 含特殊字符的 key 必须用 ["key"]
    }
    print(string.format("姓名: %s", student.name))
    print(string.format("年龄: %d", student.age))
    print(string.format("分数: %.1f", student["score"]))  -- 也可以用方括号

    -- 增删改
    student.score = 98.0            -- 修改
    student.email = "zhang@test.com" -- 添加
    student.age = nil               -- 设为 nil 即可删除（Lua 的方式）
    print("\n修改后:")
    for k, v in pairs(student) do   -- pairs 遍历所有键值对
        print(string.format("  %s = %s", k, tostring(v)))
    end

    ---- Table 混合使用 ----
    print_section("3.3 Table 混合使用(数组+字典)", 2)

    local mixed = {10, 20, 30, name = "混合表", active = true}
    -- ipairs 只遍历数组部分（连续整数索引）
    print("ipairs 遍历(只取数组部分):")
    for i, v in ipairs(mixed) do
        print(string.format("  [%d] = %s", i, tostring(v)))
    end
    print("pairs 遍历(取全部):")
    for k, v in pairs(mixed) do
        print(string.format("  [%s] = %s", tostring(k), tostring(v)))
    end

    ---- Table 的引用特性 ----
    print_section("3.4 Table 引用与拷贝", 2)
    -- Table 是引用类型，赋值不会复制内容
    local a = {1, 2, 3}
    local b = a              -- b 和 a 指向同一个 table
    b[1] = 999
    print(string.format("a[1] = %d (也被修改了!)", a[1]))  -- 999

    -- 浅拷贝（只复制第一层）
    local shallow = {}
    for k, v in pairs(a) do
        shallow[k] = v
    end
    shallow[1] = 0
    print(string.format("浅拷贝后 a[1]不变: %d", a[1]))
end

-- ============================================================================
-- 第4章：控制流 -- if/for/while/repeat
-- ============================================================================

local function chapter_04_control_flow()
    --[[
    [第4章] 控制流
    Lua 有 if/then、while、repeat/until、for（数值for和泛型for）四种控制结构。
    没有 switch/case，但可以用 table 模拟。
    ]]
    print_section("第4章：控制流")

    ---- if / then / elseif / else ----
    print_section("4.1 if / then / elseif / else", 2)

    local score = 85
    local grade
    if score >= 90 then
        grade = "A"
    elseif score >= 80 then        -- 注意：是 elseif，不是 elif 也不是 else if
        grade = "B"
    elseif score >= 70 then
        grade = "C"
    elseif score >= 60 then
        grade = "D"
    else
        grade = "F"
    end   -- 必须以 end 结尾！
    print(string.format("成绩 %d: %s", score, grade))

    -- AND/OR 实现三元表达式
    print(string.format("是否及格: %s", score >= 60 and "是" or "否"))

    ---- while 循环 ----
    print_section("4.2 while 循环", 2)

    local count = 3
    while count > 0 do
        print(string.format("  倒计时: %d", count))
        count = count - 1
    end

    ---- repeat / until ----
    print_section("4.3 repeat / until (类似 do-while)", 2)
    -- repeat/until 至少执行一次，条件为真时退出
    local i = 0
    repeat
        i = i + 1
        print(string.format("  i = %d", i))
    until i >= 3   -- 条件为真时退出

    ---- for 循环（数值型） ----
    print_section("4.4 for 循环 -- 数值型", 2)
    -- for var = start, end, step do ... end
    -- step 默认为 1，可以是负数

    print("for i=1,5:")
    for i = 1, 5 do
        print(string.format("  %d", i))
    end

    print("for i=10,1,-2:")
    for i = 10, 1, -2 do
        print(string.format("  %d", i))
    end

    ---- for 循环（泛型型 -- 遍历 table） ----
    print_section("4.5 for 循环 -- 泛型型(遍历)", 2)

    local colors = {"red", "green", "blue"}
    -- ipairs：按数字索引依次遍历（从1开始直到遇到nil）
    print("ipairs 遍历数组:")
    for idx, val in ipairs(colors) do
        print(string.format("  %d: %s", idx, val))
    end

    -- pairs：遍历所有键值对（不限顺序）
    local info = {name = "李四", age = 25, city = "北京"}
    print("pairs 遍历字典:")
    for key, val in pairs(info) do
        print(string.format("  %s = %s", key, tostring(val)))
    end

    ---- break 和 goto ----
    print_section("4.6 break 和 goto", 2)
    -- Lua 没有 continue，但可以用 goto 模拟

    print("break 示例:")
    for i = 1, 10 do
        if i > 3 then
            break     -- 跳出循环
        end
        print(string.format("  %d", i))
    end

    -- Lua 5.2+ 支持 goto（标签用 ::label::）
    print("goto 模拟 continue(跳过偶数):")
    for i = 1, 6 do
        if i % 2 == 0 then
            goto continue    -- 跳转到标签
        end
        print(string.format("  %d", i))
        ::continue::          -- 标签定义
    end
end

-- ============================================================================
-- 第5章：函数 -- 多返回值、变长参数、闭包
-- ============================================================================

local function chapter_05_functions()
    --[[
    [第5章] 函数
    Lua 函数是一等公民，可以被赋值、作为参数传递、作为返回值。
    Lua 函数独特之处：多返回值、变长参数、尾调用优化。
    ]]
    print_section("第5章：函数")

    ---- 函数定义与调用 ----
    print_section("5.1 函数定义与调用", 2)

    local function greet(name)
        -- 默认参数在 Lua 中需要手动实现
        name = name or "世界"    -- 如果 name 为 nil/false，使用默认值
        return "你好, " .. name .. "!"
    end

    print(greet("小明"))
    print(greet())                 -- 不传参数，使用默认值

    ---- 多返回值 ----
    print_section("5.2 Lua 特性 -- 多返回值", 2)
    -- Lua 函数可以返回多个值（不需要打包成数组/tuple）

    local function stats(nums)
        local total, count = 0, 0
        for _, n in ipairs(nums) do
            total = total + n
            count = count + 1
        end
        return total, total / count, count  -- 返回三个值
    end

    local sum, avg, cnt = stats({85, 92, 78, 90})
    print(string.format("总分=%d, 均分=%.1f, 人数=%d", sum, avg, cnt))

    -- 多返回值可以直接作为函数参数
    local function add3(a, b, c)
        return a + b + c
    end
    print(string.format("字符串 'a,b,c' 解包为3个参数: %d",
        add3(string.match("10,20,30", "(%d+),(%d+),(%d+)"))))

    ---- 变长参数(...) ----
    print_section("5.3 变长参数(...)", 2)

    local function sum_all(...)
        local args = {...}    -- 将所有参数打包为 table（不推荐大量参数时使用）
        -- 更好的方式是用 table.pack 或 select
        local total = 0
        for i = 1, select('#', ...) do  -- select('#', ...) 获取参数数量
            total = total + select(i, ...)  -- select(n, ...) 获取第n个参数
        end
        return total
    end

    print(string.format("sum_all(1,2,3) = %d", sum_all(1, 2, 3)))
    print(string.format("sum_all(10,20,30,40) = %d", sum_all(10, 20, 30, 40)))

    ---- 闭包 ----
    print_section("5.4 闭包(Closure)", 2)
    -- 闭包 = 函数 + 它捕获的外部变量
    -- 是 Lua 最强大的特性之一，广泛用于回调、迭代器等场景

    local function make_counter()
        local count = 0       -- 这个变量被闭包捕获
        return function()
            count = count + 1 -- 修改外部变量
            return count
        end
    end

    local c1 = make_counter()
    local c2 = make_counter()
    print(string.format("c1: %d, %d, %d", c1(), c1(), c1()))
    print(string.format("c2: %d (独立的!)", c2()))

    ---- 尾调用优化(Tail Call Optimization) ----
    print_section("5.5 尾调用优化(TCO)", 2)
    -- 当函数最后一个动作是调用另一个函数时，Lua 会回收当前栈帧
    -- 这使得递归可以无限进行而不栈溢出

    local function factorial_tail(n, acc)
        acc = acc or 1
        if n <= 1 then
            return acc
        end
        return factorial_tail(n - 1, acc * n) -- 尾调用，不增加栈帧
    end

    print(string.format("尾调用阶乘 factorial_tail(1000) 不会栈溢出: done"))
    -- 这里不实际计算1000的阶乘，只是说明语法
end

-- ============================================================================
-- 第6章：元表(Metatable) -- Lua 最强大的特性
-- ============================================================================

local function chapter_06_metatables()
    --[[
    [第6章] 元表(Metatable) -- Lua 最独特的特性
    元表允许你改变 table 的行为，实现操作符重载、OOP、代理模式等。
    这是 Lua 被誉为"可编程的语言"的核心原因。
    ]]
    print_section("第6章：元表(Metatable)")

    ---- 元表基础 ----
    print_section("6.1 元表基础", 2)

    -- setmetatable 设置元表，getmetatable 获取元表
    local t = {}
    local mt = {}
    setmetatable(t, mt)
    print(string.format("有元表: %s", tostring(getmetatable(t) ~= nil)))

    -- 保护元表：__metatable 可以隐藏元表
    local protected = {}
    local pmt = {__metatable = "不许看!"}
    setmetatable(protected, pmt)
    local ok, err = pcall(getmetatable, protected)
    print(string.format("保护元表访问: %s", err))

    ---- __index -- 读取时的默认值 ----
    print_section("6.2 __index -- 访问不存在字段时的回调", 2)

    -- __index 可以是函数或 table
    local defaults = {color = "未知", size = "中"}
    local obj = {name = "物品1"}
    setmetatable(obj, {__index = defaults})  -- 索引不存在时查 defaults

    print(string.format("obj.name = %s (自身字段)", obj.name))
    print(string.format("obj.color = %s (来自元表)", obj.color))  -- 自身没有color
    print(string.format("obj.size = %s (来自元表)", obj.size))

    -- __index 为函数时
    local t2 = {a = 1, b = 2}
    setmetatable(t2, {
        __index = function(tbl, key)
            print(string.format("  字段 '%s' 不存在，返回默认值", key))
            return "默认值"
        end
    })
    print(string.format("t2.a = %s", tostring(t2.a)))
    print(string.format("t2.xyz = %s", t2.xyz))  -- 触发 __index 函数

    ---- __newindex -- 写入时的拦截 ----
    print_section("6.3 __newindex -- 拦截写入操作", 2)

    local readonly = {}
    setmetatable(readonly, {
        __newindex = function(tbl, key, value)
            print(string.format("  警告: 不允许修改 '%s'!", key))
        end,
        __index = function() return "常量" end,
    })
    readonly.safe_var = "尝试修改"   -- 被 __newindex 拦截！
    print(string.format("readonly.safe_var = %s", readonly.safe_var))

    ---- 操作符重载 ----
    print_section("6.4 操作符重载", 2)

    -- 创建一个"向量"类型，支持加法
    local Vector = {}     -- 元表，也是"类"

    function Vector.new(x, y)
        return setmetatable({x = x, y = y}, Vector)
    end

    -- __add 定义 + 运算
    Vector.__add = function(a, b)
        return Vector.new(a.x + b.x, a.y + b.y)
    end

    -- __tostring 定义 tostring() 和 print() 的输出
    Vector.__tostring = function(v)
        return string.format("Vector(%.1f, %.1f)", v.x, v.y)
    end

    -- __eq 定义 == 比较
    Vector.__eq = function(a, b)
        return a.x == b.x and a.y == b.y
    end

    local v1 = Vector.new(3, 4)
    local v2 = Vector.new(1, 2)
    print(string.format("v1 = %s", tostring(v1)))
    print(string.format("v2 = %s", tostring(v2)))
    print(string.format("v1 + v2 = %s", tostring(v1 + v2)))
    print(string.format("v1 == v2: %s", tostring(v1 == v2)))

    -- 完整操作符元方法列表：
    -- __add(+), __sub(-), __mul(*), __div(/), __mod(%), __pow(^), __unm(-)
    -- __concat(..), __len(#), __eq(==), __lt(<), __le(<=)
    -- __call(当table作为函数调用), __index, __newindex

    ---- __call -- 将 table 当作函数调用 ----
    print_section("6.5 __call -- 像函数一样调用 table", 2)

    local Multiplier = {}
    function Multiplier.new(factor)
        return setmetatable({factor = factor}, {__call = function(tbl, x)
            return tbl.factor * x
        end})
    end

    local triple = Multiplier.new(3)
    print(string.format("triple(7) = %d", triple(7)))  -- 像函数一样调用！
end

-- ============================================================================
-- 第7章：面向对象编程（Lua 风格）
-- ============================================================================

local function chapter_07_oop()
    --[[
    [第7章] 面向对象编程 -- Lua 风格
    Lua 没有内置的 class 关键字，但通过 table + 元表可以优雅地实现
    类、继承、多态等 OOP 概念。社区也有 class 库（如 middleclass）。
    ]]
    print_section("第7章：面向对象编程 -- Lua 风格")

    ---- 基本"类"实现 ----
    print_section("7.1 基本类实现", 2)

    -- 定义一个 Animal "类"
    local Animal = {}
    Animal.__index = Animal    -- 元表的 __index 指向自己

    -- 构造器
    function Animal.new(name)
        -- 创建实例，设置元表为 Animal
        local self = setmetatable({}, Animal)
        self.name = name
        self.energy = 100
        return self
    end

    -- 实例方法
    function Animal:speak()
        -- 冒号语法糖：function Animal:speak() 等价于 function Animal.speak(self)
        return self.name .. "发出了声音"
    end

    function Animal:eat(food)
        self.energy = self.energy + 10
        return self.name .. "吃了" .. food .. "，能量:" .. self.energy
    end

    local cat = Animal.new("小花")
    print(cat:speak())        -- 冒号调用，自动传入 self
    print(cat:eat("鱼"))

    ---- 继承 ----
    print_section("7.2 继承(Inheritance)", 2)

    -- Dog 继承自 Animal
    local Dog = {}
    Dog.__index = Dog
    setmetatable(Dog, {__index = Animal})  -- Dog 的方法不存在时查 Animal

    function Dog.new(name, breed)
        local self = setmetatable(Animal.new(name), Dog)  -- 调用父类构造器
        self.breed = breed or "土狗"
        return self
    end

    function Dog:speak()
        -- 重写父类方法
        return self.name .. "说: 汪汪!"
    end

    function Dog:wag_tail()
        return self.name .. "摇尾巴"
    end

    local dog = Dog.new("旺财", "金毛")
    print(dog:speak())          -- Dog 自己的方法
    print(dog:wag_tail())       -- Dog 独有的方法
    print(dog:eat("骨头"))      -- 继承自 Animal 的方法

    -- 多态：同一个方法调用，不同对象表现不同
    local animals = {cat, dog}
    print("\n多态示例:")
    for _, animal in ipairs(animals) do
        print("  " .. animal:speak())
    end

    ---- 私有性实现（闭包方式） ----
    print_section("7.3 私有成员实现(闭包方式)", 2)

    local function createBankAccount(initial_balance)
        -- balance 不暴露在返回的 table 中，只能通过方法访问
        local balance = initial_balance or 0

        return {
            deposit = function(amount)
                balance = balance + amount
                return balance
            end,
            withdraw = function(amount)
                if amount > balance then
                    error("余额不足!")
                end
                balance = balance - amount
                return balance
            end,
            get_balance = function()
                return balance
            end,
        }
    end

    local account = createBankAccount(1000)
    account.deposit(500)
    account.withdraw(200)
    print(string.format("余额: %d (无法直接从外部访问balance字段)", account.get_balance()))
    print(string.format("account.balance = %s (无法直接访问)", tostring(account.balance)))
end

-- ============================================================================
-- 第8章：模块与包管理
-- ============================================================================

local function chapter_08_modules()
    --[[
    [第8章] 模块与包管理
    Lua 使用 require 加载模块，通过 package.path 和 package.cpath 定位文件。
    ]]
    print_section("第8章：模块与包管理")

    ---- require 基础 ----
    print_section("8.1 require 与 package.path", 2)

    -- require 查找路径
    print("package.path (Lua 模块搜索路径):")
    -- 简化显示，实际路径很长
    local paths = {}
    for p in string.gmatch(package.path, "[^;]+") do
        table.insert(paths, p)
    end
    for i = 1, math.min(3, #paths) do
        print(string.format("  %s", paths[i]))
    end

    -- require 工作流程:
    -- 1. 检查 package.loaded[modname] 是否已加载
    -- 2. 在 package.path 中搜索 .lua 文件
    -- 3. 在 package.cpath 中搜索 .so/.dll 文件(C模块)
    -- 4. 找到后加载并执行，将返回值存入 package.loaded

    -- 已经加载的模块样例
    print(string.format("\n已加载模块数: %d", #({next, package.loaded})[1] or 0))

    ---- 创建模块的标准模式 ----
    print_section("8.2 创建模块的标准模式", 2)

    -- 典型模块文件结构（此处用代码演示，不创建实际文件）:
    print([[
    -- mymodule.lua 的标准写法:
    local M = {}          -- 模块 table（局部变量）

    -- 私有函数（不导出）
    local function private_helper()
        -- 只在模块内部使用
    end

    -- 公共函数（导出）
    function M.public_func()
        return "hello from mymodule"
    end

    return M              -- 最后返回模块 table
    ]])

    ---- 常用标准模块 ----
    print_section("8.3 常用标准模块速览", 2)

    local modules = {
        {"string", "字符串操作", [[string.format("hello %s", "world")]]},
        {"table", "table 操作", [[table.insert(t, "val")]]},
        {"math", "数学函数", [[math.sin(math.pi / 2)]]},
        {"io", "文件 I/O", [[io.open("test.txt", "r")]]},
        {"os", "操作系统接口", [[os.date("%Y-%m-%d")]]},
        {"coroutine", "协程", [[coroutine.create(func)]]},
        {"debug", "调试工具", [[debug.traceback()]]},
        {"package", "包管理", [[package.path]]},
    }
    for _, mod in ipairs(modules) do
        print(string.format("  %-12s -- %-12s 示例: %s", mod[1], mod[2], mod[3]))
    end
end

-- ============================================================================
-- 第9章：错误处理 -- pcall, xpcall
-- ============================================================================

local function chapter_09_error_handling()
    --[[
    [第9章] 错误处理
    Lua 使用 error() 抛出错误，pcall() 和 xpcall() 捕获错误。
    没有 try/catch 语法，但保护调用(protected call)提供了相同的功能。
    ]]
    print_section("第9章：错误处理")

    ---- pcall -- 保护调用 ----
    print_section("9.1 pcall -- 保护调用(类似 try/catch)", 2)

    local function divide(a, b)
        if b == 0 then
            error("除数不能为零!", 2)   -- 第二个参数是错误级别
        end
        return a / b
    end

    -- pcall 返回两个值：成功标志 和 结果/错误信息
    local ok, result = pcall(divide, 10, 2)
    if ok then
        print(string.format("10/2 = %.1f", result))
    else
        print("错误: " .. result)
    end

    local ok2, err = pcall(divide, 10, 0)
    if not ok2 then
        print(string.format("10/0 错误: %s", err))
    end

    ---- xpcall -- 带错误处理函数的保护调用 ----
    print_section("9.2 xpcall -- 带错误处理器的保护调用", 2)
    -- xpcall 的第二个参数是错误处理函数（类似 finally/debug traceback）

    local function error_handler(err)
        return "自定义错误处理器: " .. tostring(err)
    end

    local function risky_func()
        error("某个错误发生了!")
    end

    local ok3, err3 = xpcall(risky_func, debug.traceback)
    if not ok3 then
        print("xpcall 捕获(带traceback):\n" .. err3)
    end

    ---- assert -- 断言 ----
    print_section("9.3 assert -- 断言", 2)

    local function set_age(age)
        assert(type(age) == "number", "age 必须是数字!")
        assert(age >= 0 and age <= 150, "年龄必须在 0-150 之间!")
        return "年龄设为 " .. age
    end

    local ok4, rs = pcall(set_age, 25)
    print(string.format("set_age(25): %s", rs))

    local ok5, err5 = pcall(set_age, -5)
    print(string.format("set_age(-5): %s", err5))
end

-- ============================================================================
-- 第10章：协程(Coroutine)
-- ============================================================================

local function chapter_10_coroutines()
    --[[
    [第10章] 协程(Coroutine)
    协程是 Lua 最强大的特性之一。与线程不同，协程是协作式的，
    由程序员控制何时切换执行权。适合游戏逻辑、状态机、迭代器等场景。
    ]]
    print_section("第10章：协程(Coroutine)")

    ---- 协程基础 ----
    print_section("10.1 协程创建与执行", 2)

    -- 使用 coroutine.create 创建协程
    local co = coroutine.create(function()
        print("  协程开始")
        coroutine.yield("第一步完成")    -- 暂停并返回值给 resume
        print("  协程继续")
        coroutine.yield("第二步完成")
        print("  协程结束")
        return "全部完成"
    end)

    -- coroutine.resume 恢复执行
    -- 返回: 成功标志, yield 的参数 / 错误信息
    print("主线程: 第一次 resume")
    local ok1, val1 = coroutine.resume(co)
    print(string.format("主线程: 收到 '%s'", tostring(val1)))

    print("主线程: 第二次 resume")
    local ok2, val2 = coroutine.resume(co)
    print(string.format("主线程: 收到 '%s'", tostring(val2)))

    print("主线程: 第三次 resume(协程结束)")
    local ok3, val3 = coroutine.resume(co)
    print(string.format("主线程: 收到 '%s'", tostring(val3)))

    -- 检查协程状态
    print(string.format("协程状态: %s (正常结束为 dead)", coroutine.status(co)))
    -- 状态: running, suspended, normal, dead

    ---- 生产者-消费者模式 ----
    print_section("10.2 生产者-消费者模式", 2)

    local function producer()
        for i = 1, 4 do
            coroutine.yield(i)    -- 每次 yield 一个产品
        end
    end

    local co2 = coroutine.create(producer)
    print("消费协程产生的值:")
    while coroutine.status(co2) ~= "dead" do
        local ok, value = coroutine.resume(co2)
        if ok and value then
            print(string.format("  消费: %d", value))
        end
    end

    ---- coroutine.wrap -- 更简洁的方式 ----
    print_section("10.3 coroutine.wrap -- 返回函数而非协程对象", 2)
    -- wrap 返回一个函数，每次调用等于 resume
    local function generator()
        for i = 1, 3 do
            coroutine.yield(i)
        end
    end

    local gen = coroutine.wrap(generator)
    print("wrap 方式: " .. gen())
    print("wrap 方式: " .. gen())
    print("wrap 方式: " .. gen())
end

-- ============================================================================
-- 第11章：文件操作
-- ============================================================================

local function chapter_11_file_io()
    --[[
    [第11章] 文件操作
    Lua 提供 io 模块进行文件读写。有两种风格：隐式句柄(io.xxx)和显式句柄(file:xxx)。
    ]]
    print_section("第11章：文件操作")

    ---- 写入文件 ----
    print_section("11.1 写入文件", 2)

    local filename = "_test_lua.txt"
    -- io.open 打开文件，模式: r(读) w(写) a(追加) b(二进制)
    local file, err = io.open(filename, "w")
    if not file then
        print("打开文件失败: " .. err)
        return
    end

    file:write("第一行: Hello Lua!\n")
    file:write("第二行: 文件操作示例\n")
    file:write(string.format("第三行: 当前时间 %s\n", os.date()))
    file:close()                    -- 关闭文件
    print("已写入: " .. filename)

    ---- 读取文件 ----
    print_section("11.2 读取文件", 2)

    -- 逐行读取
    local file2 = io.open(filename, "r")
    if file2 then
        print("逐行读取:")
        for line in file2:lines() do  -- lines() 返回迭代器
            print("  " .. line)
        end
        file2:close()
    end

    -- 一次性读取全部内容
    local content = io.open(filename, "r"):read("*a")
    print(string.format("\n全部内容: %d 字节", #content))

    -- 读取模式:
    -- "*a" 全部, "*l" 一行(默认), "*n" 一个数字, n 读取n个字符

    os.remove(filename)  -- 删除测试文件
end

-- ============================================================================
-- 第12章：标准库速览
-- ============================================================================

local function chapter_12_stdlib()
    --[[
    [第12章] Lua 标准库速览
    Lua 标准库非常精简，但足够实用。本节通过实例展示常用函数。
    ]]
    print_section("第12章：标准库速览")

    ---- string 库 ----
    print_section("12.1 string 库", 2)

    print("string.byte('A') = " .. string.byte('A'))           -- 字符 -> ASCII
    print("string.char(65) = " .. string.char(65))             -- ASCII -> 字符
    print("string.rep('ha', 3) = " .. string.rep("ha", 3))     -- 重复
    print("string.len('hello') = " .. string.len("hello"))     -- 长度
    print("#string.len语法糖 = " .. #"hello")

    ---- table 库 ----
    print_section("12.2 table 库", 2)

    local t = {3, 1, 4, 1, 5, 9, 2}
    print("table.sort 升序: " .. table.concat(t, ", "))
    table.sort(t)
    print("table.sort 后: " .. table.concat(t, ", "))

    local t2 = {"a", "b", "c"}
    print("table.concat(t2, '-'): " .. table.concat(t2, "-"))

    -- table.move 移动元素 (Lua 5.3+)
    local src = {10, 20, 30, 40}
    local dst = {}
    table.move(src, 1, 3, 1, dst)   -- 从src的1-3移到dst从1开始
    print("table.move: " .. table.concat(dst, ", "))

    ---- math 库 ----
    print_section("12.3 math 库", 2)
    print(string.format("math.pi = %.5f", math.pi))
    print(string.format("math.abs(-5) = %d", math.abs(-5)))
    print(string.format("math.max(3,7,2) = %d", math.max(3, 7, 2)))
    print(string.format("math.min(3,7,2) = %d", math.min(3, 7, 2)))
    print(string.format("math.ceil(3.2) = %d", math.ceil(3.2)))
    print(string.format("math.floor(3.8) = %d", math.floor(3.8)))
    print(string.format("math.random(1, 10) = %d", math.random(1, 10)))
    print(string.format("math.randomseed 设随机种子: done"))

    ---- os 库 ----
    print_section("12.4 os 库", 2)
    print("os.date('%Y-%m-%d %H:%M:%S'): " .. os.date("%Y-%m-%d %H:%M:%S"))
    print(string.format("os.time(): %d (Unix时间戳)", os.time()))
    print(string.format("os.clock(): %.3f (CPU时间)", os.clock()))
    -- os.execute 执行系统命令
    -- os.getenv 获取环境变量
    -- os.tmpname 临时文件名
end

-- ============================================================================
-- 第13章：Lua 编程技巧与惯用法
-- ============================================================================

local function chapter_13_idioms()
    --[[
    [第13章] Lua 编程技巧与惯用法
    掌握 Lua 的惯用写法，写出地道、高效的 Lua 代码。
    ]]
    print_section("第13章：Lua 编程技巧与惯用法")

    ---- 默认参数 ----
    print_section("13.1 默认参数", 2)
    local function greet(name)
        name = name or "匿名"        -- 利用 or 的短路特性
        return "你好, " .. name
    end
    print(greet())
    print(greet("张三"))

    ---- 安全的表访问 ----
    print_section("13.2 安全的嵌套表访问", 2)
    -- 访问深层嵌套的表容易遇到 nil 错误
    local config = {server = {host = "localhost"}}
    -- 使用 and 短路来安全访问
    local port = ((config or {}).server or {}).port or 8080
    print(string.format("port = %d (安全默认值)", port))

    ---- table 作为 switch-case ----
    print_section("13.3 table 模拟 switch/case", 2)

    local actions = {
        add = function(a, b) return a + b end,
        sub = function(a, b) return a - b end,
        mul = function(a, b) return a * b end,
        div = function(a, b) return a / b end,
    }

    local function calculate(op, a, b)
        local action = actions[op]
        if action then
            return action(a, b)
        else
            error("不支持的操作: " .. op)
        end
    end

    print(string.format("add: %d", calculate("add", 10, 5)))
    print(string.format("mul: %d", calculate("mul", 10, 5)))

    ---- 条件初始化（一次性加载） ----
    print_section("13.4 条件初始化模式", 2)
    -- 利用 or 的短路特性实现懒加载
    local heavy_data = nil
    local function get_data()
        heavy_data = heavy_data or {expensive = "计算耗时数据"}  -- 只计算一次
        return heavy_data
    end
    print("第一次调用: " .. get_data().expensive)
    print("第二次调用(直接返回缓存): " .. get_data().expensive)

    ---- 内存化(Memoization) ----
    print_section("13.5 内存化(Memoization)", 2)

    local function memoize(func)
        local cache = {}
        return function(n)
            cache[n] = cache[n] or func(n)
            return cache[n]
        end
    end

    local expensive_func = memoize(function(n)
        -- 模拟耗时计算
        local result = 0
        for i = 1, n do
            result = result + i
        end
        return result
    end)

    print(string.format("sum(1000000) 第一次: %d (计算)", expensive_func(1000000)))
    print(string.format("sum(1000000) 第二次: %d (缓存命中)", expensive_func(1000000)))
end

-- ============================================================================
-- 第14章：Lua 应用场景概述
-- ============================================================================

local function chapter_14_applications()
    --[[
    [第14章] Lua 应用场景
    Lua 因其轻量(约200KB)、快速(LuaJIT可接近C速度)、易嵌入等特点，
    在多个领域有着广泛应用。
    ]]
    print_section("第14章：Lua 应用场景概述")

    local applications = {
        {name = "游戏开发", desc = "Lua 是最流行的游戏脚本语言之一",
         examples = "World of Warcraft, Roblox, 愤怒的小鸟, Love2D 引擎"},

        {name = "嵌入式/物联网", desc = "Lua 体积小(<200KB)，适合资源受限设备",
         examples = "NodeMCU (ESP8266), eLua, OpenWrt"},

        {name = "Nginx/OpenResty", desc = "用 Lua 编写高性能 Web 服务",
         examples = "OpenResty (淘宝), Kong API网关, Cloudflare"},

        {name = "Redis 脚本", desc = "Redis 内置 Lua 解释器，原子执行脚本",
         examples = "EVAL命令, 分布式锁, 限流算法"},

        {name = "Neovim 插件", desc = "Neovim 使用 Lua 作为主要配置和插件语言",
         examples = "Lazy.nvim, Telescope, 各种nvim插件"},

        {name = "Wireshark", desc = "Wireshark 使用 Lua 编写数据包解析插件",
         examples = "协议解析器, 流量分析脚本"},

        {name = "配置语言", desc = "Lua table 语法天然适合做配置文件",
         examples = "Neovim init.lua, Awesome WM 配置, Hammerspoon"},

        {name = "LuaJIT 高性能", desc = "LuaJIT 的 JIT 编译可达接近 C 的性能",
         examples = "数值计算, OpenResty 高性能 Web, 游戏引擎"},
    }

    for _, app in ipairs(applications) do
        print(string.format("[%s]", app.name))
        print(string.format("  描述: %s", app.desc))
        print(string.format("  实例: %s", app.examples))
        print()
    end

    ---- Lua 优缺点总结 ----
    print_section("14.2 Lua 优缺点总结", 2)
    print([[
优点:
  - 轻量级: 完整解释器 < 200KB
  - 速度快: LuaJIT 接近 C 语言性能
  - 易嵌入: C API 简单清晰，容易与 C/C++ 交互
  - 语法简洁: 仅 1 种数据结构(table)，学习曲线平缓
  - 协程: 内置协作式多任务支持
  - 元表: 强大的元编程能力

缺点:
  - 标准库小: 缺少网络/线程/正则等，需第三方库
  - 索引从 1 开始: 让其他语言用户不习惯
  - 社区较小: 相比 Python/JS 生态较弱
  - 没有原生 OOP: 需用 table + 元表模拟
  - 动态类型: 大型项目维护困难
  ]])
end

-- ============================================================================
-- 主函数：串联所有章节
-- ============================================================================

local function main()
    print(string.rep("=", 60))
    print("    Lua 全知识点教学程序")
    print("    覆盖 Lua 编程几乎所有核心知识点（14个模块）")
    print(string.rep("=", 60))
    print("\n提示：每个章节都可以独立学习，直接跳到感兴趣的内容即可。")

    -- 所有章节函数列表
    local chapters = {
        {"基础语法", chapter_01_basics},
        {"字符串操作", chapter_02_strings},
        {"Table核心", chapter_03_tables},
        {"控制流", chapter_04_control_flow},
        {"函数", chapter_05_functions},
        {"元表", chapter_06_metatables},
        {"OOP", chapter_07_oop},
        {"模块与包", chapter_08_modules},
        {"错误处理", chapter_09_error_handling},
        {"协程", chapter_10_coroutines},
        {"文件操作", chapter_11_file_io},
        {"标准库", chapter_12_stdlib},
        {"编程技巧", chapter_13_idioms},
        {"应用场景", chapter_14_applications},
    }

    -- 运行所有章节
    for _, ch in ipairs(chapters) do
        local name, func = ch[1], ch[2]
        local ok, err = pcall(func)
        if not ok then
            print(string.format("\n[警告] 章节 '%s' 执行出错: %s", name, tostring(err)))
        end
    end

    -- 结束语
    print("\n" .. string.rep("=", 60))
    print("    全部章节学习完毕！")
    print("    建议与 Python 对照学习，理解两种语言的异同。")
    print(string.rep("=", 60))
end

-- ============================================================================
-- 程序入口
-- ============================================================================

-- 直接运行此文件时执行 main()
main()
