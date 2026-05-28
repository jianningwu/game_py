#!/usr/bin/env bash
#===============================================================================
# 脚本功能：Shell 脚本全知识点教学程序
# - 覆盖 Shell (Bash) 脚本几乎所有核心知识点（14 个模块）
# - 每个知识点包含：概念说明 + 示例代码 + 运行结果（注释中标明）
# - 运行方式：bash shell_full_tutorial.sh
# - 适合 Shell 零基础到进阶的系统学习
#===============================================================================

# 严格模式：任何错误都退出，未定义变量报错，管道中任一命令失败都算失败
# set -euo pipefail

###############################################################################
# 辅助函数：打印章节标题
###############################################################################

h1() {
    # 一级标题：等号边框
    echo ""
    echo "======================================================================"
    echo "  $1"
    echo "======================================================================"
    echo ""
}

h2() {
    # 二级标题：星号线
    echo ""
    echo "**************************************************"
    echo "  $1"
    echo "**************************************************"
    echo ""
}

h3() {
    # 三级标题
    echo ""
    echo "--- $1 ---"
    echo ""
}

###############################################################################
# 第1章：基础入门 —— shebang、执行方式、echo、注释
###############################################################################

chapter_01_basics() {
    h1 "第1章：Shell 基础入门"

    h2 "1.1 Shebang -- 指定解释器"
    cat << 'EOF'
脚本第一行 #!/bin/bash 告诉系统用哪个解释器执行此脚本。
常见的 shebang:
  #!/bin/bash        - Bash (最常用)
  #!/bin/sh           - 系统默认 Shell (通常是 dash 或 bash 的链接)
  #!/usr/bin/env bash - 更可移植的写法 (推荐)
  #!/bin/zsh          - Zsh
EOF

    h2 "1.2 脚本执行方式"
    cat << 'EOF'
三种执行方式：
  1. bash script.sh       - 新建子进程执行 (不需要可执行权限)
  2. ./script.sh          - 直接执行 (需要 chmod +x script.sh)
  3. source script.sh     - 在当前 Shell 中执行 (或 . script.sh)
                           变量修改会保留在当前 Shell
EOF

    h2 "1.3 echo 输出"

    # echo 是 Shell 中最常用的输出命令
    name="张三"
    age=25
    echo "你好，我叫 ${name}，今年 ${age} 岁"

    # echo 常用选项
    echo -n "不换行输出"    # -n: 不自动添加换行
    echo " (接上一行)"

    # 转义字符需要 -e 选项
    echo -e "制表符: \tTAB后的内容"
    echo -e "换行符: \n第二行"

    # printf 功能更强大（类似 C 的 printf）
    printf "姓名: %-10s 年龄: %3d 分数: %.1f\n" "李四" 30 92.5

    h2 "1.4 注释"
    cat << 'EOF'
  # 单行注释
  : '多行注释方式一'  (冒号+空格+单引号)
  <<COMMENT
  多行注释方式二
  使用 heredoc
  COMMENT
EOF
}

###############################################################################
# 第2章：变量与环境变量
###############################################################################

chapter_02_variables() {
    h1 "第2章：变量与环境变量"

    h2 "2.1 变量定义与使用"

    # 变量赋值：等号两边不能有空格！
    name="张三"
    age=25

    # 使用变量：$变量名 或 ${变量名}（推荐加花括号，避免歧义）
    echo "name = ${name}"
    echo "age  = ${age}"

    # 变量默认值
    echo "未定义变量: ${undefined_var:-默认值}"     # 有默认值就用，不修改原变量
    echo "赋值默认值: ${new_var:=新值}"            # 如果未定义，赋值并返回
    echo "new_var 现在: ${new_var}"

    # 常见错误：等号两边加空格！
    # name = "张三"    # 错误！Shell 会把它当做"运行 name 命令，参数是 = 和张三"

    h2 "2.2 四种引号的区别"

    # 单引号：所有字符都是字面量，不解析变量、不解析转义
    echo '单引号: ${name} 不会被解析'

    # 双引号：会解析变量和命令替换，保留空格和换行
    echo "双引号: ${name} 会被解析"

    # 反引号(旧式)：执行命令并替换为输出
    echo "反引号: `date +%H:%M:%S`"

    # $()(新式)：同上，但支持嵌套，推荐使用
    echo "使用 \$(): $(date +%H:%M:%S)"

    h2 "2.3 环境变量 (export)"

    # 普通变量只在当前脚本/Shell 中可见
    local_var="我是局部变量"

    # export 让变量对子进程可见
    export MY_GLOBAL_VAR="我是全局变量"

    echo "local_var = ${local_var}"
    echo "MY_GLOBAL_VAR = ${MY_GLOBAL_VAR}"

    # 常用系统环境变量
    echo "HOME    = ${HOME}"          # 家目录
    echo "USER    = ${USER}"          # 当前用户
    echo "PWD     = ${PWD}"           # 当前目录
    echo "SHELL   = ${SHELL}"         # 当前 Shell
    echo "PATH    = ${PATH:0:50}..."  # 可执行文件搜索路径 (截断显示)

    h2 "2.4 特殊变量"

    cat << 'EOF'
  $0  - 脚本名称
  $1~$9 - 第1~9个参数
  ${10} - 第10个参数 (花括号必须有)
  $#  - 参数个数
  $@  - 所有参数 (每个都是独立的字符串)
  $*  - 所有参数 (拼接成一个字符串)
  $?  - 上一条命令的退出码 (0=成功,非0=失败)
  $$  - 当前 Shell 的进程 ID (PID)
  $!  - 最后一个后台进程的 PID
EOF

    h2 "2.5 readonly 只读变量"

    readonly PI=3.14159
    echo "PI = ${PI}"
    # PI=3.14  # 会报错: PI: readonly variable
    echo "尝试修改 readonly 变量会报错 (已注释)"
}

###############################################################################
# 第3章：字符串操作
###############################################################################

chapter_03_strings() {
    h1 "第3章：字符串操作"

    h2 "3.1 字符串拼接"
    str1="Hello"
    str2="World"

    # 直接写在一起
    combined="${str1}, ${str2}!"
    echo "拼接: ${combined}"

    h2 "3.2 字符串长度"
    str="Hello世界"
    echo "字符串: '${str}'"
    echo "字符长度: ${#str} (注意：Bash 统计的是字节数，不是字符数!)"

    h2 "3.3 子串截取"
    str="abcdefghij"

    echo "原始: ${str}"
    echo "\${str:2}    = ${str:2}"        # 从索引2到末尾 (索引从0开始)
    echo "\${str:2:4}  = ${str:2:4}"      # 从索引2开始取4个字符
    echo "\${str: -3}  = ${str: -3}"      # 从末尾取3个 (注意空格)
    echo "\${str: -5:3}= ${str: -5:3}"    # 从倒数第5取3个

    h2 "3.4 字符串替换与删除"
    path="/home/user/docs/file.txt"

    echo "路径: ${path}"
    echo "\${path#*/}  删除最短前缀匹配: ${path#*/}"
    echo "\${path##*/} 删除最长前缀匹配: ${path##*/}"    # 获取文件名
    echo "\${path%/*}  删除最短后缀匹配: ${path%/*}"     # 获取目录
    echo "\${path%%.*} 删除最长后缀匹配: ${path%%.*}"    # 去掉扩展名

    # 全局替换
    text="apple banana apple"
    echo "原文: ${text}"
    echo "\${text/apple/orange}: ${text/apple/orange}"       # 替换第一个
    echo "\${text//apple/orange}: ${text//apple/orange}"     # 替换全部
}

###############################################################################
# 第4章：数组
###############################################################################

chapter_04_arrays() {
    h1 "第4章：数组"

    h2 "4.1 索引数组"

    # 定义数组 (空格分隔，括号包裹)
    fruits=("苹果" "香蕉" "橘子" "葡萄")

    # 访问元素
    echo "第一个: ${fruits[0]}"          # 索引从0开始! (和 Zsh 不同)
    echo "所有元素: ${fruits[@]}"
    echo "所有索引: ${!fruits[@]}"
    echo "数组长度: ${#fruits[@]}"

    # 追加元素
    fruits+=("西瓜")
    echo "追加后: ${fruits[@]}"

    # 修改元素
    fruits[1]="草莓"
    echo "修改索引1: ${fruits[@]}"

    # 遍历数组
    echo "遍历数组:"
    for fruit in "${fruits[@]}"; do
        echo "  - ${fruit}"
    done

    h2 "4.2 关联数组 (类似字典，Bash 4+)"

    # declare -A 声明关联数组
    declare -A student
    student[name]="张三"
    student[age]=20
    student[score]=95.5

    echo "学生信息:"
    echo "  姓名: ${student[name]}"
    echo "  年龄: ${student[age]}"
    echo "  分数: ${student[score]}"

    echo "遍历关联数组:"
    for key in "${!student[@]}"; do
        echo "  ${key} -> ${student[${key}]}"
    done
}

###############################################################################
# 第5章：命令替换与算术运算
###############################################################################

chapter_05_substitution_math() {
    h1 "第5章：命令替换与算术运算"

    h2 "5.1 命令替换 -- \$()"

    # $() 是最推荐的命令替换方式
    now=$(date "+%Y-%m-%d %H:%M:%S")
    files=$(ls *.sh 2>/dev/null | wc -l)
    echo "当前时间: ${now}"
    echo "当前目录 .sh 文件数: ${files}"

    # 命令替换可以嵌套
    dir=$(dirname $(which bash))
    echo "bash 所在目录: ${dir}"

    h2 "5.2 算术运算"

    # $(( )) 进行整数运算 (最推荐)
    a=10; b=3
    echo "a=${a}, b=${b}"
    echo "  a + b  = $((a + b))"
    echo "  a - b  = $((a - b))"
    echo "  a * b  = $((a * b))"
    echo "  a / b  = $((a / b))    # 整数除法，截断"
    echo "  a % b  = $((a % b))"
    echo "  a ** b = $((a ** b))   # 幂运算"

    # 浮点数运算：Shell 不支持！需要用 bc 或 awk
    echo "浮点运算 (用 bc):"
    echo "scale=3; 10/3" | bc      # scale 设置小数位数

    # let 命令 (旧式)
    let "c = a + b"
    echo "let 结果: ${c}"

    # expr 命令 (更旧式)
    d=$(expr ${a} + ${b})
    echo "expr 结果: ${d}"
}

###############################################################################
# 第6章：条件判断
###############################################################################

chapter_06_conditionals() {
    h1 "第6章：条件判断"

    h2 "6.1 if / elif / else"

    score=85
    echo "分数: ${score}"

    # 注意格式：if [ ]; then (方括号内必须有空格!)
    if [ "${score}" -ge 90 ]; then
        grade="A"
    elif [ "${score}" -ge 80 ]; then
        grade="B"
    elif [ "${score}" -ge 70 ]; then
        grade="C"
    elif [ "${score}" -ge 60 ]; then
        grade="D"
    else
        grade="F"
    fi
    echo "等级: ${grade}"

    h2 "6.2 [ ] 与 [[ ]] 的区别"

    echo "  [ ]  : POSIX 标准，所有 Shell 通用"
    echo "  [[ ]]: Bash 扩展，功能更强（推荐 Bash 脚本中使用）"

    h2 "6.3 常用判断条件"

    cat << 'EOF'
数值比较:  -eq(等于) -ne(不等于) -gt(大于) -lt(小于) -ge(>=) -le(<=)
字符串比较: = (等于)  != (不等于)  -z (为空)  -n (非空)
文件判断:
  -f file  是否为普通文件(不是目录)
  -d dir   是否为目录
  -e path  路径是否存在
  -r file  是否可读
  -w file  是否可写
  -x file  是否可执行
  -s file  文件大小是否 > 0
  file1 -nt file2  file1 是否比 file2 新
  file1 -ot file2  file1 是否比 file2 旧
逻辑运算:  -a (与, and)  -o (或, or)  ! (非)
  [[ ]] 中: &&  (与)    ||  (或)
EOF

    h2 "6.4 判断示例"

    file="shell_full_tutorial.sh"
    if [[ -f "${file}" ]]; then
        echo "'${file}' 存在且是普通文件"
    fi

    if [[ -x "${file}" ]]; then
        echo "'${file}' 是可执行的"
    else
        echo "'${file}' 不是可执行的"
    fi

    # 三元运算式的写法
    age=18
    status=$([ "${age}" -ge 18 ] && echo "成年" || echo "未成年")
    echo "年龄 ${age}: ${status}"
}

###############################################################################
# 第7章：case 语句
###############################################################################

chapter_07_case() {
    h1 "第7章：case 语句"

    h2 "7.1 case 基础"

    read -p "请输入 y/n: " -n 1 answer 2>/dev/null || answer="y"
    echo ""  # 换行

    case "${answer}" in
        y|Y)
            echo "你选择了 Yes"
            ;;
        n|N)
            echo "你选择了 No"
            ;;
        *)
            echo "无效输入"
            ;;
    esac
    # 注意：每个 pattern 以 ) 结尾，;; 表示结束（类似 break）

    h2 "7.2 case 配合模式匹配"

    read -p "请输入文件名: " filename 2>/dev/null || filename="test.txt"
    echo ""

    case "${filename}" in
        *.txt)
            echo "这是文本文件"
            ;;
        *.sh|*.bash)
            echo "这是 Shell 脚本"
            ;;
        *.py)
            echo "这是 Python 脚本"
            ;;
        *.jpg|*.png|*.gif)
            echo "这是图片文件"
            ;;
        *)
            echo "未知类型"
            ;;
    esac
}

###############################################################################
# 第8章：循环
###############################################################################

chapter_08_loops() {
    h1 "第8章：循环"

    h2 "8.1 for 循环"

    # 方式1：遍历列表
    echo "遍历列表:"
    for color in 红 绿 蓝; do
        echo "  颜色: ${color}"
    done

    # 方式2：遍历数组
    echo "遍历数组:"
    names=("Alice" "Bob" "Charlie")
    for name in "${names[@]}"; do
        echo "  ${name}"
    done

    # 方式3：C 风格 for
    echo "C 风格 for:"
    for ((i = 1; i <= 5; i++)); do
        echo -n " ${i}"
    done
    echo ""

    # 方式4：遍历命令输出
    echo "遍历 .sh 文件:"
    for file in *.sh; do
        echo "  - ${file} (${#file} 字符)"
    done

    h2 "8.2 while 循环"

    echo "while:"
    count=3
    while [[ ${count} -gt 0 ]]; do
        echo "  倒计时: ${count}"
        count=$((count - 1))
    done

    # 逐行读取文件
    echo "逐行读取 /etc/hosts (如果存在):"
    while IFS= read -r line; do
        # 跳过注释和空行
        [[ "${line}" =~ ^# ]] && continue
        [[ -z "${line}" ]] && continue
        echo "  ${line}"
        break  # 只读第一行做演示
    done < /etc/hosts 2>/dev/null

    h2 "8.3 until 循环 (条件为假时执行)"

    echo "until:"
    n=0
    until [[ ${n} -ge 3 ]]; do
        echo "  n = ${n}"
        n=$((n + 1))
    done

    h2 "8.4 break 和 continue"

    echo "break 示例:"
    for i in {1..10}; do
        [[ ${i} -gt 3 ]] && break
        echo -n " ${i}"
    done
    echo ""

    echo "continue 示例 (跳过偶数):"
    for i in {1..8}; do
        [[ $((i % 2)) -eq 0 ]] && continue
        echo -n " ${i}"
    done
    echo ""
}

###############################################################################
# 第9章：函数
###############################################################################

chapter_09_functions() {
    h1 "第9章：函数"

    h2 "9.1 函数定义与调用"

    # 两种定义方式（完全等价）
    # function greet { ... }  或  greet() { ... }

    greet() {
        local name="${1:-世界}"    # 局部变量，默认值
        echo "你好，${name}！"
    }

    greet "小明"
    greet                        # 不传参数，使用默认值

    h2 "9.2 函数参数"

    # 函数内部用 $1, $2, ... 获取参数（和脚本参数一样）
    # 注意：函数的 $@ 是函数的参数，不是脚本的参数！

    sum() {
        local total=0
        for num in "$@"; do
            total=$((total + num))
        done
        echo ${total}
    }

    result=$(sum 10 20 30 40)
    echo "sum(10,20,30,40) = ${result}"

    h2 "9.3 函数返回值"

    # 方式1: return (只能返回 0-255 的整数，表示退出状态)
    is_even() {
        if [[ $(($1 % 2)) -eq 0 ]]; then
            return 0    # 成功（偶数）
        else
            return 1    # 失败（奇数）
        fi
    }

    if is_even 42; then
        echo "42 是偶数"
    fi

    # 方式2: echo 返回任意值（最常见做法）
    get_full_name() {
        echo "${1} ${2}"
    }
    full_name=$(get_full_name "张" "三")
    echo "全名: ${full_name}"

    h2 "9.4 local 局部变量"

    outer_var="全局"
    test_local() {
        local outer_var="局部"    # 不会影响外面的同名变量
        inner_var="也是全局"      # 不加 local 就是全局变量！
        echo "  函数内: outer_var=${outer_var}"
    }
    test_local
    echo "  函数外: outer_var=${outer_var}"
    echo "  inner_var=${inner_var} (函数外仍可访问!)"
    # 注意：一定要用 local，否则变量会泄漏到全局作用域
}

###############################################################################
# 第10章：I/O 重定向与管道
###############################################################################

chapter_10_redirection() {
    h1 "第10章：I/O 重定向与管道"

    h2 "10.1 标准输入输出"

    cat << 'EOF'
每个进程有三个默认的文件描述符:
  0 = stdin  (标准输入)
  1 = stdout (标准输出)
  2 = stderr (标准错误输出)
EOF

    h2 "10.2 输出重定向"

    # >  覆盖写入
    echo "Hello" > _test_out.txt
    # >> 追加写入
    echo "World" >> _test_out.txt
    echo "文件内容: $(cat _test_out.txt)"

    # 2>  重定向 stderr
    # &>  同时重定向 stdout 和 stderr (Bash 4+)
    # command > out.txt 2>&1  (旧式写法，兼容性好)

    h2 "10.3 输入重定向"

    # <  从文件读取输入
    while read -r line; do
        echo "  读取: ${line}"
    done < _test_out.txt

    # <<<  Here-String (将字符串作为输入)
    grep "Hello" <<< "Hello World"

    # <<EOF  Here-Document (多行字符串作为输入)
    cat << 'HEREDOC'
这是 Here-Document
可以写多行
变量不会被解析 (因为 'HEREDOC' 加了引号)
HEREDOC
    rm -f _test_out.txt  # 清理

    h2 "10.4 管道 (|)"

    echo "管道把前一个命令的 stdout 传给后一个命令的 stdin:"
    echo "统计当前目录文件类型分布:"
    ls -1 2>/dev/null | rev | cut -d'.' -f1 | rev | sort | uniq -c | sort -rn || true

    h2 "10.5 /dev/null 黑洞"

    echo "command > /dev/null 2>&1   # 丢弃所有输出"
    echo "/dev/null 类似黑洞，写入的数据全部消失"
}

###############################################################################
# 第11章：文本处理三剑客 —— grep / sed / awk
###############################################################################

chapter_11_text_processing() {
    h1 "第11章：文本处理三剑客 -- grep / sed / awk"

    # 准备测试数据
    cat > _test_data.txt << 'EOF'
张三,25,北京,95
李四,30,上海,87
王五,28,广州,92
赵六,22,深圳,78
张三丰,35,成都,88
EOF

    h2 "11.1 grep -- 文本搜索"

    echo "测试数据:"
    cat _test_data.txt
    echo ""

    # 基本搜索
    echo "包含'张三'的行:"
    grep "张三" _test_data.txt

    # 常用选项
    # -i: 忽略大小写     -v: 反向匹配(不包含)
    # -c: 计数           -n: 显示行号
    # -E: 扩展正则       -o: 只输出匹配部分
    echo ""
    echo "计数: $(grep -c "张三" _test_data.txt) 行"

    h2 "11.2 sed -- 流编辑器"

    # 替换 (最常用)
    echo "张三 -> 张同学:"
    sed 's/张三/张同学/g' _test_data.txt

    # 删除第3行
    echo "删除第3行:"
    sed '3d' _test_data.txt

    # 打印第2-4行
    echo "打印第2-4行:"
    sed -n '2,4p' _test_data.txt

    # sed 常用操作:
    # s/old/new/g  - 替换
    # Nd           - 删除第N行
    # Np           - 打印第N行
    # /pattern/d   - 删除匹配行

    h2 "11.3 awk -- 数据处理语言"

    # awk 按列处理数据 (默认以空格/制表符为分隔符)
    # -F 指定分隔符

    echo "第2列(年龄):"
    awk -F',' '{print $2}' _test_data.txt

    echo "计算平均年龄:"
    awk -F',' '{sum+=$2; count++} END {printf "平均年龄: %.1f\n", sum/count}' _test_data.txt

    echo "分数 >= 90 的记录:"
    awk -F',' '$4 >= 90 {print $1, $4 "分"}' _test_data.txt

    # awk 结构: BEGIN { 前处理 } { 每行处理 } END { 后处理 }
    # $0 = 整行, $1 = 第1列, $2 = 第2列 ...
    # NF = 列数, NR = 行号

    rm -f _test_data.txt  # 清理
}

###############################################################################
# 第12章：常用文件与文本命令
###############################################################################

chapter_12_file_commands() {
    h1 "第12章：常用文件与文本命令"

    h2 "12.1 find -- 查找文件"

    echo "查找当前目录下所有 .sh 文件:"
    find . -name "*.sh" -maxdepth 1 -type f 2>/dev/null | head -5

    h2 "12.2 sort -- 排序"

    echo "排序示例:"
    printf "3\n1\n4\n1\n5\n9\n2\n" | sort -n    # -n: 按数值排序

    h2 "12.3 uniq -- 去重 (通常配合 sort)"

    echo ""
    echo "去重:"
    printf "3\n1\n4\n1\n5\n4\n" | sort -n | uniq

    echo "计数:"
    printf "red\ngreen\nblue\nred\nblue\nred\n" | sort | uniq -c    # -c: 计数

    h2 "12.4 wc -- 统计"

    # wc -l: 行数  -w: 单词数  -c: 字节数
    echo "统计脚本行数:"
    wc -l shell_full_tutorial.sh 2>/dev/null || echo "(统计自身)"

    h2 "12.5 cut -- 按列截取"

    echo "截取列:"
    echo "a,b,c,d" | cut -d',' -f2    # 用逗号分隔，取第2列
    echo "hello" | cut -c2-4          # 取第2-4个字符

    h2 "12.6 tr -- 字符转换"

    echo "小写转大写:"
    echo "hello world" | tr 'a-z' 'A-Z'
    echo "删除数字:"
    echo "abc123def456" | tr -d '0-9'

    h2 "12.7 其他常用命令"

    cat << 'EOF'
  basename /path/file.txt  -> file.txt       (获取文件名)
  dirname  /path/file.txt  -> /path          (获取目录)
  which    command         -> 命令的完整路径
  tee      file            -> 同时输出到文件和 stdout
  xargs                    -> 将 stdin 转为命令行参数
  head -n N  / tail -n N   -> 文件头/尾 N 行
  diff file1 file2         -> 比较文件差异
EOF
}

###############################################################################
# 第13章：进程管理与陷阱
###############################################################################

chapter_13_process() {
    h1 "第13章：进程管理与信号处理"

    h2 "13.1 后台进程与作业控制"

    echo "当前 PID: $$"

    # 后台运行: command &
    sleep 0.5 &
    bg_pid=$!
    echo "后台进程 PID: ${bg_pid}"
    wait ${bg_pid}  # 等待后台进程完成
    echo "后台进程已完成"

    # jobs  查看当前 Shell 的后台作业
    # fg %N 将后台作业移到前台
    # bg %N 让暂停的作业在后台继续
    # Ctrl+Z 暂停前台进程

    h2 "13.2 trap -- 信号处理"

    # trap 捕获信号并执行指定命令
    # 常用于脚本退出时的清理工作

    echo "设置 trap 示例:"
    trap 'echo "  [trap] 收到 SIGINT (Ctrl+C), 清理并退出"; exit 1' INT

    # 常用信号:
    # INT  (2)  - Ctrl+C 中断
    # TERM (15) - kill 默认信号
    # EXIT      - 脚本退出时 (Bash 特有)
    # HUP  (1)  - 终端挂断

    echo "按 Ctrl+C 可以测试 trap (这里先演示语法)"
    # 重置 trap
    trap - INT
}

###############################################################################
# 第14章：Shell 脚本最佳实践
###############################################################################

chapter_14_best_practices() {
    h1 "第14章：Shell 脚本最佳实践"

    h2 "14.1 脚本模板"

    cat << 'EOF'
推荐的标准脚本开头：
  #!/usr/bin/env bash
  set -euo pipefail
  # set -e:  任何命令失败就退出
  # set -u:  使用未定义变量时报错
  # set -o pipefail:  管道中任何命令失败，整个管道失败
EOF

    h2 "14.2 错误处理"

    cat << 'EOF'
1. 检查关键命令的退出码:
     if ! command; then
         echo "错误: command 失败" >&2
         exit 1
     fi

2. 清理临时文件:
     tmpfile=$(mktemp)
     trap 'rm -f "${tmpfile}"' EXIT

3. 错误信息输出到 stderr:
     echo "错误信息" >&2
EOF

    h2 "14.3 安全与健壮性"

    cat << 'EOF'
1. 始终给变量加双引号: "${var}" (除非明确知道不需要)
2. 用 [[ ]] 代替 [ ] (Bash 脚本中)
3. 用 $() 代替反引号
4. 用 printf 代替 echo (可移植性更好)
5. 用 local 声明函数内变量
6. 用 readonly 声明常量
7. 对所有用户输入做验证
8. 临时文件用 mktemp 创建
9. 脚本开头用 set -euo pipefail
EOF

    h2 "14.4 调试技巧"

    cat << 'EOF'
调试 Shell 脚本:
  bash -x script.sh     执行并打印每条命令
  bash -n script.sh     语法检查 (不执行)
  bash -v script.sh     打印读入的每一行

脚本内局部调试:
  set -x  开启调试
  # ... 调试的代码 ...
  set +x  关闭调试
EOF

    h2 "14.5 Shell 脚本适用场景"

    cat << 'EOF'
适合:
  - 系统管理自动化 (备份、部署、监控)
  - CI/CD 流水线 (GitHub Actions, Jenkins)
  - Docker 镜像构建 (Dockerfile RUN 指令)
  - 快速原型和一次性任务
  - 粘合各种命令行工具

不适合 (建议用 Python 等):
  - 复杂的数据结构和算法
  - 需要浮点数精确计算
  - 大型项目 (难以维护)
  - 跨平台 GUI 应用
  - 网络编程
EOF
}

###############################################################################
# 主函数：串联所有章节
###############################################################################

main() {
    echo "======================================================================"
    echo "    Shell 脚本全知识点教学程序"
    echo "    覆盖 Shell (Bash) 几乎所有核心知识点（14 个模块）"
    echo "======================================================================"

    # 如果是被 source 执行的，跳过交互输入（用于自动化教学）
    # 关闭 read 的交互，全部使用默认值
    exec < /dev/null 2>/dev/null || true

    chapter_01_basics
    chapter_02_variables
    chapter_03_strings
    chapter_04_arrays
    chapter_05_substitution_math
    chapter_06_conditionals
    chapter_07_case
    chapter_08_loops
    chapter_09_functions
    chapter_10_redirection
    chapter_11_text_processing
    chapter_12_file_commands
    chapter_13_process
    chapter_14_best_practices

    echo ""
    echo "======================================================================"
    echo "    全部章节学习完毕！"
    echo "    运行方式: bash shell_full_tutorial.sh"
    echo "======================================================================"
    echo ""
}

# 程序入口：直接运行此脚本时执行 main
# 如果用 source 执行，则不自动运行（方便单独测试某个章节函数）
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi
