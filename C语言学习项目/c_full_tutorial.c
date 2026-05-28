/**
 * ============================================================================
 * 脚本功能：C 语言全知识点教学程序
 * - 覆盖 C 语言几乎所有核心知识点（14 个模块）
 * - 每个模块以独立函数组织，附带详细中文注释
 * - 每个知识点包含：概念说明 + 示例代码 + 运行结果（注释中标明）
 * - 编译运行：gcc c_full_tutorial.c -o tutorial && ./tutorial
 * - 适合 C 语言零基础到进阶的系统学习
 * ============================================================================
 */

#include <stdio.h>      /* 标准输入输出：printf, scanf, fopen 等 */
#include <stdlib.h>     /* 标准库：malloc, free, atoi, exit 等 */
#include <string.h>     /* 字符串操作：strlen, strcpy, strcmp 等 */
#include <math.h>       /* 数学函数：sqrt, pow, sin 等 */
#include <time.h>       /* 时间函数：time, clock, localtime 等 */
#include <stdarg.h>     /* 可变参数：va_list, va_start, va_arg */
#include <stddef.h>     /* 标准定义：size_t, NULL, offsetof */
#include <setjmp.h>     /* 非本地跳转：setjmp, longjmp */
#include <ctype.h>      /* 字符分类：isdigit, isalpha 等 */
#include <assert.h>     /* 断言：assert 宏 */
#include <errno.h>      /* 错误码：errno, perror */

/* ============================================================================
 * 全局辅助函数：用于打印章节标题
 * ============================================================================ */

/* 打印一级标题（等号边框） */
void print_h1(const char* title) {
    printf("\n\n======================================================================\n");
    printf("  %s\n", title);
    printf("======================================================================\n\n");
}

/* 打印二级标题（星号线） */
void print_h2(const char* title) {
    printf("\n**************************************************\n");
    printf("  %s\n", title);
    printf("**************************************************\n\n");
}

/* 打印三级标题 */
void print_h3(const char* title) {
    printf("\n--- %s ---\n\n", title);
}

/* ============================================================================
 * 第1章：基础语法 —— 变量、数据类型、运算符、输入输出
 * ============================================================================
 * C 语言是静态类型语言，每个变量必须先声明类型才能使用。
 * 基本数据类型：int, float, double, char, void
 * C 没有 bool 类型（C99 引入 _Bool，stdbool.h 提供 bool）
 */

void chapter_01_basics(void) {
    print_h1("第1章：基础语法 -- 变量、数据类型、运算符");

    /* ---- 变量声明与初始化 ---- */
    print_h2("1.1 变量声明与初始化");

    int age = 25;                   /* 整数：通常 4 字节，范围约 -21亿 到 21亿 */
    float price = 19.99f;           /* 单精度浮点：4 字节，6-7 位有效数字 */
    double pi = 3.14159265358979;   /* 双精度浮点：8 字节，15-16 位有效数字 */
    char grade = 'A';               /* 字符：1 字节，用单引号 */
    /* C 中 0 表示 false，非 0 表示 true（C99 之前没有 bool） */
    int is_student = 1;

    /* sizeof 运算符：获取类型或变量的字节大小 */
    printf("int    age  = %d,  占用 %2zu 字节\n", age, sizeof(age));
    printf("float  price = %.2f, 占用 %2zu 字节\n", price, sizeof(price));
    printf("double pi    = %.10f, 占用 %2zu 字节\n", pi, sizeof(pi));
    printf("char   grade = '%c', 占用 %2zu 字节\n", grade, sizeof(grade));
    printf("int    is_student = %d\n", is_student);

    /* ---- 常用 printf 格式化符号 ---- */
    print_h2("1.2 printf 格式化符号");

    printf("%%d / %%i  : 有符号十进制整数  -> %d\n", 42);
    printf("%%u       : 无符号十进制整数  -> %u\n", 42U);
    printf("%%f       : 浮点数(默认6位)   -> %f\n", pi);
    printf("%%.2f     : 浮点数(2位小数)   -> %.2f\n", pi);
    printf("%%e       : 科学计数法        -> %e\n", pi);
    printf("%%c       : 单个字符          -> %c\n", 'X');
    printf("%%s       : 字符串            -> %s\n", "Hello C");
    printf("%%p       : 指针地址          -> %p\n", (void*)&age);
    printf("%%x / %%X : 十六进制          -> 0x%X\n", 255);
    printf("%%o       : 八进制            -> 0%o\n", 255);
    printf("%%%%      : 百分号本身        -> %%\n");

    /* ---- 运算符 ---- */
    print_h2("1.3 运算符");

    int a = 10, b = 3;
    printf("a=%d, b=%d\n", a, b);
    printf("算术: + %d, - %d, * %d, / %d, %% %d\n",
           a + b, a - b, a * b, a / b, a % b);
    /* 注意：两个整数相除结果仍是整数（截断，不是四舍五入）！ */
    printf("注意: 10/3 = %d (整数除法截断!), 10.0/3 = %.2f\n", a / b, 10.0 / 3);
    /* 比较运算符 */
    printf("比较: == %d, != %d, > %d, < %d, >= %d, <= %d\n",
           a == b, a != b, a > b, a < b, a >= b, a <= b);
    /* 逻辑运算符 */
    printf("逻辑: && %d, || %d, ! %d\n", 1 && 0, 1 || 0, !1);
    /* 位运算符（C 的独特优势：直接操作硬件位） */
    printf("位运算: &(与) %d, |(或) %d, ^(异或) %d, ~(取反) %d, <<(左移) %d, >>(右移) %d\n",
           5 & 3, 5 | 3, 5 ^ 3, ~5, 1 << 3, 8 >> 2);
    /* 复合赋值运算符 */
    int x = 10;
    x += 3;  /* x = x + 3 */  x *= 2;  /* x = x * 2 */
    printf("复合赋值: x=10; x+=3; x*=2 -> %d\n", x);
    /* 自增自减 */
    int i = 5;
    printf("自增自减: i++ = %d (后置,先用后加), ++i = %d (前置,先加后用)\n",
           i++, ++i);

    /* ---- 类型转换 ---- */
    print_h2("1.4 类型转换");

    /* 隐式转换（自动） */
    int num = 10;
    double d = num;    /* int -> double 自动 */
    printf("隐式转换 int->double: %f\n", d);

    /* 显式转换（强制类型转换） */
    double val = 3.14;
    int truncated = (int)val;  /* 截断小数，不是四舍五入 */
    printf("强制转换 (int)3.14 = %d\n", truncated);

    /* char 本质上就是小整数 */
    char ch = 'A';
    printf("char 本质是整数: '%c' = %d, '%c' + 1 = '%c'\n", ch, ch, ch, ch + 1);
}

/* ============================================================================
 * 第2章：控制流 —— if/switch/while/for
 * ============================================================================
 */

void chapter_02_control_flow(void) {
    print_h1("第2章：控制流");

    /* ---- if / else if / else ---- */
    print_h2("2.1 if / else if / else");

    int score = 85;
    char* grade;
    if (score >= 90) {
        grade = "A";
    } else if (score >= 80) {   /* 注意：C 写 "else if"（两个词），不是 "elif" */
        grade = "B";
    } else if (score >= 70) {
        grade = "C";
    } else if (score >= 60) {
        grade = "D";
    } else {
        grade = "F";
    }
    printf("成绩 %d: %s\n", score, grade);

    /* ---- 三元运算符 ---- */
    print_h2("2.2 三元运算符 ? :  ");

    int age = 18;
    const char* status = (age >= 18) ? "成年" : "未成年";
    printf("年龄 %d: %s\n", age, status);

    /* ---- switch / case ---- */
    print_h2("2.3 switch / case");

    int day = 3;
    printf("星期%d: ", day);
    switch (day) {
        case 1:  printf("星期一\n"); break;
        case 2:  printf("星期二\n"); break;
        case 3:  printf("星期三\n"); break;
        case 4:  printf("星期四\n"); break;
        case 5:  printf("星期五\n"); break;
        case 6:
        case 7:  printf("周末!\n");   break;  /* 多个 case 共享一个执行体 */
        default: printf("无效\n");    break;  /* 可以用 default（但非必须） */
    }
    /* 注意：没有 break 会"穿透"到下一个 case！这是常见 bug 来源 */

    /* ---- while 循环 ---- */
    print_h2("2.4 while 循环");

    int count = 3;
    while (count > 0) {
        printf("  倒计时: %d\n", count);
        count--;
    }

    /* ---- do-while 循环 ---- */
    print_h2("2.5 do-while 循环（至少执行一次）");

    int j = 0;
    do {
        printf("  do-while 第%d次\n", j + 1);
        j++;
    } while (j < 3);

    /* ---- for 循环 ---- */
    print_h2("2.6 for 循环");

    /* 标准 for 循环：for(初始化; 条件; 更新) */
    printf("标准 for (i=0; i<5; i++): ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", i);
    }
    printf("\n");

    /* for 的三种表达式都可以省略 */
    printf("死循环 for(;;) 需要 break 跳出\n");

    /* ---- break / continue ---- */
    print_h2("2.7 break 和 continue");

    printf("break: 遇到 5 就跳出 -> ");
    for (int i = 0; i < 10; i++) {
        if (i == 5) break;
        printf("%d ", i);
    }
    printf("\n");

    printf("continue: 跳过偶数 -> ");
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) continue;
        printf("%d ", i);
    }
    printf("\n");
}

/* ============================================================================
 * 第3章：数组与字符串
 * ============================================================================
 * C 的字符串本质是字符数组，以 '\\0' (空字符) 结尾。
 * 数组名是指向第一个元素的指针（大部分情况下）。
 */

void chapter_03_arrays_strings(void) {
    print_h1("第3章：数组与字符串");

    /* ---- 数组基础 ---- */
    print_h2("3.1 一维数组");

    /* 数组声明：类型 名称[大小]; 索引从 0 开始！ */
    int nums[5] = {10, 20, 30, 40, 50};  /* 初始化列表 */

    printf("数组 nums = ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");
    printf("首元素: nums[0]=%d, 末元素: nums[4]=%d\n", nums[0], nums[4]);

    /* 不指定大小的初始化 */
    int auto_sized[] = {1, 2, 3, 4};  /* 编译器自动计算大小 */
    printf("自动大小数组: 元素数=%zu\n", sizeof(auto_sized) / sizeof(auto_sized[0]));

    /* 部分初始化：未指定的元素自动置 0 */
    int partial[5] = {1, 2};  /* 等价于 {1, 2, 0, 0, 0} */
    printf("部分初始化: [0]=%d, [2]=%d, [4]=%d\n",
           partial[0], partial[2], partial[4]);

    /* C 不会检查数组越界！这是最常见的安全漏洞来源（缓冲区溢出） */
    printf("C 不检查越界: 访问 nums[100] 可能导致崩溃或安全漏洞!\n");

    /* ---- 多维数组 ---- */
    print_h2("3.2 多维数组");

    int matrix[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };
    printf("3x3 矩阵:\n");
    for (int r = 0; r < 3; r++) {
        printf("  ");
        for (int c = 0; c < 3; c++) {
            printf("%d ", matrix[r][c]);
        }
        printf("\n");
    }

    /* ---- 字符串 ---- */
    print_h2("3.3 字符串（以 '\\0' 结尾的字符数组）");

    /* C 没有 string 类型！字符串是 char 数组 */
    char str1[] = "Hello";      /* 编译器自动加 '\0'，共 6 字节 */
    char str2[20] = "World";    /* 预分配 20 字节 */
    const char* str3 = "C语言";  /* 字符串字面量存储在只读区，不可修改 */

    printf("str1=\"%s\", 长度=%zu (字节), sizeof=%zu\n",
           str1, strlen(str1), sizeof(str1));
    /* strlen: "Hello" = 5, 但 sizeof = 6 (含 '\0') */

    /* 字符串操作（需 #include <string.h>） */
    char dest[20];
    strcpy(dest, str1);         /* 复制: dest = str1 */
    strcat(dest, " ");          /* 拼接: dest += " " */
    strcat(dest, str2);         /* 拼接: dest += "World" */
    printf("拼接后: \"%s\"\n", dest);

    printf("strcmp(\"abc\",\"abc\"): %d (相等返回0)\n", strcmp("abc", "abc"));
    printf("strcmp(\"abc\",\"abd\"): %d (不等返回差值)\n", strcmp("abc", "abd"));

    /* 注意：strcpy/strcat 不检查缓冲区大小，容易溢出！
       推荐用 strncpy/strncat（指定最大长度）或 snprintf */
}

/* ============================================================================
 * 第4章：指针 —— C 语言最核心也最难的概念
 * ============================================================================
 * 指针是存储内存地址的变量。
 * & 取地址，* 解引用，指针类型决定了解引用时读取多少字节。
 */

void chapter_04_pointers(void) {
    print_h1("第4章：指针 -- C 语言最核心的概念");

    /* ---- 指针基础 ---- */
    print_h2("4.1 指针基础：& 取地址，* 解引用");

    int x = 42;
    int* ptr = &x;          /* ptr 指向 x 的地址（int* 是指向 int 的指针类型） */

    printf("x 的值: %d\n", x);
    printf("x 的地址: %p\n", (void*)ptr);
    printf("通过指针访问: *ptr = %d\n", *ptr);

    /* 通过指针修改原变量 */
    *ptr = 100;             /* 解引用 ptr，修改它指向的值 */
    printf("通过指针修改后 x = %d\n", x);

    /* ---- 指针与数组 ---- */
    print_h2("4.2 指针与数组的关系");

    int arr[] = {10, 20, 30, 40, 50};
    int* parr = arr;        /* 数组名退化为指向首元素的指针 */
    /* arr[i] 等价于 *(arr + i) ! */
    printf("arr[2] = %d, *(arr+2) = %d (等价!)\n", arr[2], *(arr + 2));
    printf("指针算术: parr=%p, parr+2=%p (跳了 %d 字节)\n",
           (void*)parr, (void*)(parr + 2), (int)((char*)(parr + 2) - (char*)parr));

    /* 用指针遍历数组 */
    printf("指针遍历: ");
    for (int* p = arr; p < arr + 5; p++) {
        printf("%d ", *p);
    }
    printf("\n");

    /* ---- 多级指针 ---- */
    print_h2("4.3 二级指针（指针的指针）");

    int val = 10;
    int* p1 = &val;     /* 一级指针 */
    int** p2 = &p1;     /* 二级指针：指向指针的指针 */
    printf("val=%d, *p1=%d, **p2=%d\n", val, *p1, **p2);

    /* ---- 指针与 const ---- */
    print_h2("4.4 指针与 const（常量性）");

    int a = 10, b = 20;

    const int* cp1 = &a;      /* 指向常量的指针：不能通过它修改值 */
    /* *cp1 = 30; */          /* 错误！ */
    cp1 = &b;                 /* 但可以改变指向 */

    int* const cp2 = &a;      /* 常量指针：不能改变指向 */
    *cp2 = 30;                /* 但可以修改值 */
    /* cp2 = &b; */           /* 错误！ */

    const int* const cp3 = &a; /* 常量指针指向常量：都不能改 */
    printf("cp1 -> %d, *cp2 -> %d, *cp3 -> %d\n", *cp1, *cp2, *cp3);

    /* ---- 函数指针 ---- */
    print_h2("4.5 函数指针");

    /* 函数名本身就是函数指针 */
    int (*func_ptr)(const char*, ...) = &printf;
    func_ptr("通过函数指针调用 printf: hello!\n");

    /* 函数指针常用于回调（callback） */
    /* 比如 qsort 需要传入比较函数 */
}

/* ---- 指针练习：swap 函数 ---- */
/* 值传递 vs 指针传递：C 的默认参数传递是"传值"，函数内修改不影响调用者 */
/* 要修改外部变量，必须传指针！ */
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* ============================================================================
 * 第5章：函数
 * ============================================================================
 */

/* 函数声明（原型）：告诉编译器函数的签名，定义可以放在后面 */
int add(int a, int b);                  /* 两个整数相加 */
void modify_array(int arr[], int n);    /* 数组参数（退化为指针） */
int factorial_recursive(int n);         /* 递归示例 */

void chapter_05_functions(void) {
    print_h1("第5章：函数");

    /* ---- 函数调用 ---- */
    print_h2("5.1 函数调用基础");

    int result = add(10, 5);
    printf("add(10, 5) = %d\n", result);

    /* 演示 swap（必须传地址） */
    int x = 10, y = 20;
    printf("swap 前: x=%d, y=%d\n", x, y);
    swap(&x, &y);   /* & 取地址 */
    printf("swap 后: x=%d, y=%d\n", x, y);

    /* ---- 数组作为参数 ---- */
    print_h2("5.2 数组作为参数（退化为指针）");

    int arr[] = {1, 2, 3, 4, 5};
    modify_array(arr, 5);
    printf("modify_array 后: [0]=%d, [4]=%d\n", arr[0], arr[4]);

    /* ---- 递归 ---- */
    print_h2("5.3 递归函数");

    printf("factorial(5) = %d\n", factorial_recursive(5));
    printf("factorial(10) = %d\n", factorial_recursive(10));

    /* ---- static 局部变量 ---- */
    print_h2("5.4 static 局部变量（函数内持久化）");

    for (int i = 0; i < 3; i++) {
        /* static 局部变量只在第一次初始化，之后保持值 */
        static int counter = 0;  /* 只初始化一次 */
        counter++;
        printf("第%d次调用: counter=%d\n", i + 1, counter);
    }
}

/* 函数定义 */
int add(int a, int b) {
    return a + b;
}

void modify_array(int arr[], int n) {
    /* arr 在这里退化为指针，arr[0] 等价于 *arr */
    arr[0] = 999;          /* 修改会影响调用者的数组 */
    arr[n - 1] = 888;
}

int factorial_recursive(int n) {
    /* 递归终止条件 */
    if (n <= 1) return 1;
    return n * factorial_recursive(n - 1);
}

/* ---- 可变参数函数 ---- */
/* ... 表示可变参数，用 stdarg.h 的宏来访问 */
double average(int count, ...) {
    va_list args;
    va_start(args, count);     /* 初始化，count 是最后一个固定参数 */

    double sum = 0;
    for (int i = 0; i < count; i++) {
        sum += va_arg(args, double);  /* 每次调用获取下一个参数 */
    }
    va_end(args);              /* 清理 */
    return sum / count;
}

void demo_variadic(void) {
    print_h2("5.5 可变参数函数");
    printf("average(85, 92, 78, 90) = %.1f\n", average(4, 85.0, 92.0, 78.0, 90.0));
    /* printf 和 scanf 就是可变参数函数 */
}

/* ============================================================================
 * 第6章：内存管理 —— malloc / free
 * ============================================================================
 * C 不提供垃圾回收，程序员必须手动管理堆内存。
 * malloc 分配，free 释放。忘记 free = 内存泄漏；提前 free = 悬垂指针。
 */

void chapter_06_memory(void) {
    print_h1("第6章：动态内存管理 -- malloc / free / calloc / realloc");

    /* ---- malloc：分配指定字节数 ---- */
    print_h2("6.1 malloc -- 在堆上分配内存");

    /* 分配 5 个 int 的空间 */
    int* dyn_arr = (int*)malloc(5 * sizeof(int));
    if (dyn_arr == NULL) {
        printf("内存分配失败!\n");
        return;
    }

    /* 像普通数组一样使用 */
    for (int i = 0; i < 5; i++) {
        dyn_arr[i] = (i + 1) * 10;
    }
    printf("动态数组: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", dyn_arr[i]);
    }
    printf("\n");

    /* ---- calloc：分配并清零 ---- */
    print_h2("6.2 calloc -- 分配并初始化为0");

    int* zeroed = (int*)calloc(5, sizeof(int));
    printf("calloc 分配的数组(已清零): ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", zeroed[i]);
    }
    printf("\n");

    /* ---- realloc：调整已分配内存的大小 ---- */
    print_h2("6.3 realloc -- 重新调整内存大小");

    int* expanded = (int*)realloc(dyn_arr, 10 * sizeof(int));
    if (expanded != NULL) {
        dyn_arr = expanded;  /* 重要：realloc 可能返回新地址! */
        for (int i = 5; i < 10; i++) {
            dyn_arr[i] = (i + 1) * 10;
        }
        printf("realloc 扩充到 10 个: ");
        for (int i = 0; i < 10; i++) {
            printf("%d ", dyn_arr[i]);
        }
        printf("\n");
    }

    /* ---- free：释放内存 ---- */
    print_h2("6.4 free -- 释放内存（必须！）");

    free(dyn_arr);      /* 释放 malloc/calloc/realloc 分配的内存 */
    free(zeroed);
    dyn_arr = NULL;     /* 好习惯：置 NULL 防止悬垂指针 */
    zeroed = NULL;

    printf("free 后指针置 NULL: dyn_arr=%p, zeroed=%p\n",
           (void*)dyn_arr, (void*)zeroed);

    /* ---- 常见内存错误 ---- */
    print_h2("6.5 常见内存错误");

    printf("常见内存问题:\n");
    printf("  1. 内存泄漏: malloc 后忘了 free\n");
    printf("  2. 悬垂指针: free 后继续使用该指针\n");
    printf("  3. 重复释放: 对同一个指针 free 两次\n");
    printf("  4. 越界访问: 访问超出分配范围的地址\n");
    printf("  5. 使用未初始化的指针\n");
    printf("工具检测: valgrind (Linux), AddressSanitizer (GCC/Clang)\n");
}

/* ============================================================================
 * 第7章：结构体、联合体、枚举
 * ============================================================================
 */

/* 结构体定义：将多个相关数据组合成一个类型 */
struct Student {
    char name[50];
    int age;
    float score;
};  /* 注意：分号不能省略！ */

/* typedef：给类型起别名 */
typedef struct {
    double x;
    double y;
} Point;  /* Point 现在可以直接作为类型使用 */

/* 联合体(union)：所有成员共享同一块内存 */
union Data {
    int i;
    float f;
    char str[20];
};

/* 枚举(enum)：定义命名整数常量 */
typedef enum {
    MONDAY = 1,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY,
} Weekday;  /* 未指定的值自动递增 */

void chapter_07_structs(void) {
    print_h1("第7章：结构体、联合体、枚举");

    /* ---- 结构体 ---- */
    print_h2("7.1 结构体(struct)");

    struct Student s1 = {"张三", 20, 95.5};  /* 按顺序初始化 */
    /* 也可以指定字段初始化(C99) */
    struct Student s2 = {.name = "李四", .age = 22, .score = 88.0};

    printf("学生1: %s, %d岁, %.1f分\n", s1.name, s1.age, s1.score);
    printf("学生2: %s, %d岁, %.1f分\n", s2.name, s2.age, s2.score);

    /* 结构体指针：用 -> 访问成员 */
    struct Student* ps = &s1;
    printf("指针访问: ps->name = %s, ps->age = %d\n", ps->name, ps->age);
    /* ps->age 等价于 (*ps).age，-> 只是语法糖 */

    /* typedef 的类型直接使用 */
    Point p = {3.0, 4.0};
    printf("Point(%.1f, %.1f)\n", p.x, p.y);

    /* ---- 结构体内存对齐 ---- */
    print_h2("7.2 结构体内存对齐");

    struct Example {
        char c;      /* 1 字节 */
        int i;       /* 4 字节 */
        short s;     /* 2 字节 */
    } ex;
    printf("sizeof(char)=%zu, sizeof(int)=%zu, sizeof(short)=%zu\n",
           sizeof(char), sizeof(int), sizeof(short));
    printf("sizeof(struct Example)=%zu (不是 1+4+2=7!)\n", sizeof(ex));
    printf("原因：编译器会插入 padding 对齐内存地址，用空间换速度\n");

    /* ---- 联合体 ---- */
    print_h2("7.3 联合体(union)：所有成员共享内存");

    union Data data;
    data.i = 10;
    printf("union: data.i = %d\n", data.i);
    data.f = 220.5;
    /* data.i 现在被覆盖了！所有成员共享同一段内存 */
    printf("union: data.f = %.1f (data.i 被覆盖了)\n", data.f);
    printf("sizeof(union Data) = %zu (取最大成员的大小)\n", sizeof(union Data));

    /* ---- 枚举 ---- */
    print_h2("7.4 枚举(enum)");

    Weekday today = WEDNESDAY;
    printf("today = %d (WEDNESDAY)\n", today);

    /* 用 switch 配合 enum 写出可读代码 */
    switch (today) {
        case MONDAY:    printf("星期一，开始工作\n"); break;
        case FRIDAY:    printf("星期五，快周末了\n"); break;
        case SATURDAY:
        case SUNDAY:    printf("周末愉快!\n"); break;
        default:        printf("工作日\n"); break;
    }
}

/* ============================================================================
 * 第8章：预处理指令
 * ============================================================================
 * 预处理在编译之前执行，由预处理器处理以 # 开头的指令。
 * 常用：#include, #define, #ifdef, #ifndef, #pragma
 */

/* 宏定义：文本替换 */
#define PI 3.14159265
#define SQUARE(x) ((x) * (x))     /* 注意：宏参数必须加括号！ */
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* 条件编译 */
#define DEBUG 1   /* 注释掉这行就不会输出调试信息 */

void chapter_08_preprocessor(void) {
    print_h1("第8章：预处理指令");

    /* ---- 宏 ---- */
    print_h2("8.1 宏定义 #define");

    printf("PI = %.7f\n", PI);
    printf("SQUARE(5) = %d\n", SQUARE(5));
    printf("MAX(10, 20) = %d\n", MAX(10, 20));

    /* 宏 vs 函数的陷阱 */
    int i = 5;
    printf("SQUARE(++i) = %d (可能不是你想的 36!)\n", SQUARE(++i));
    printf("原因：宏是文本替换，++i 被替换了两次，行为未定义\n");

    /* ---- 条件编译 ---- */
    print_h2("8.2 条件编译 #ifdef / #ifndef / #if");

#if DEBUG
    printf("[DEBUG] 调试模式已开启\n");
#else
    printf("[RELEASE] 发布模式\n");
#endif

    /* 防止头文件重复包含的标准写法：
       #ifndef MY_HEADER_H
       #define MY_HEADER_H
       // ... 头文件内容 ...
       #endif
    */

    /* ---- 预定义宏 ---- */
    print_h2("8.3 预定义宏");

    printf("__FILE__: %s\n", __FILE__);   /* 当前文件名 */
    printf("__LINE__: %d\n", __LINE__);   /* 当前行号 */
    printf("__DATE__: %s\n", __DATE__);   /* 编译日期 */
    printf("__TIME__: %s\n", __TIME__);   /* 编译时间 */
    printf("__STDC_VERSION__: %ld\n", __STDC_VERSION__);  /* C 标准版本 */
}

/* ============================================================================
 * 第9章：文件操作
 * ============================================================================
 */

void chapter_09_file_io(void) {
    print_h1("第9章：文件操作");

    /* ---- 写入文件 ---- */
    print_h2("9.1 写入文件");

    const char* filename = "_test_c.txt";
    /* fopen 模式: r 读, w 写(清空), a 追加, rb/wb 二进制 */
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("打开文件失败");  /* perror 打印错误原因 */
        return;
    }

    fprintf(fp, "第一行: Hello C!\n");          /* 写入格式化文本 */
    fprintf(fp, "第二行: 数字=%d, 浮点=%.2f\n", 42, 3.14);
    fputs("第三行: fputs 写入字符串\n", fp);
    fclose(fp);  /* 记得关闭！ */
    printf("已写入: %s\n", filename);

    /* ---- 读取文件 ---- */
    print_h2("9.2 读取文件");

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("打开文件失败");
        return;
    }

    printf("文件内容:\n");
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("  %s", line);  /* fgets 保留换行符 */
    }
    fclose(fp);

    /* ---- 二进制读写 ---- */
    print_h2("9.3 二进制读写");

    int data[] = {1, 2, 3, 4, 5};
    fp = fopen("_test_binary.bin", "wb");
    fwrite(data, sizeof(int), 5, fp);  /* 写入 5 个 int */
    fclose(fp);

    int readback[5] = {0};
    fp = fopen("_test_binary.bin", "rb");
    fread(readback, sizeof(int), 5, fp);
    fclose(fp);
    printf("二进制读回: ");
    for (int i = 0; i < 5; i++) printf("%d ", readback[i]);
    printf("\n");

    /* 清理文件 */
    remove(filename);
    remove("_test_binary.bin");
}

/* ============================================================================
 * 第10章：标准库常用函数
 * ============================================================================
 */

void chapter_10_stdlib(void) {
    print_h1("第10章：C 标准库常用函数速览");

    /* ---- stdlib.h ---- */
    print_h2("10.1 <stdlib.h> 常用函数");

    printf("atoi(\"42\"): %d\n", atoi("42"));
    printf("atof(\"3.14\"): %.2f\n", atof("3.14"));
    printf("abs(-5): %d\n", abs(-5));

    /* 随机数 */
    srand((unsigned)time(NULL));    /* 设置随机种子 */
    printf("rand() 随机数: %d, %d, %d\n", rand() % 100, rand() % 100, rand() % 100);
    /* 注意：rand() % N 不是均匀分布的！ */

    /* 二分搜索和快速排序 */
    int arr[] = {34, 12, 56, 78, 23, 9, 45};
    int n = sizeof(arr) / sizeof(arr[0]);

    /* qsort 排序 */
    /* 比较函数：返回负数表示 a<b，0 表示相等，正数表示 a>b */
    int cmp_int(const void* a, const void* b) {
        return (*(int*)a - *(int*)b);
    }
    qsort(arr, n, sizeof(int), cmp_int);
    printf("qsort 排序后: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    /* bsearch 二分查找（前提：数组已排序） */
    int key = 34;
    int* found = (int*)bsearch(&key, arr, n, sizeof(int), cmp_int);
    printf("bsearch(%d): %s\n", key, found ? "找到了" : "没找到");

    /* ---- string.h ---- */
    print_h2("10.2 <string.h> 常用函数");

    printf("strlen(\"hello\"): %zu\n", strlen("hello"));
    printf("strcmp(\"abc\",\"abd\"): %d\n", strcmp("abc", "abd"));
    printf("strchr(\"hello\",'l'): %s\n", strchr("hello", 'l'));  /* "llo" */
    printf("strstr(\"hello\",\"ll\"): %s\n", strstr("hello", "ll")); /* "llo" */

    /* memcpy vs strcpy：memcpy 复制固定字节，不依赖 '\0' */
    int src[3] = {1, 2, 3}, dst[3];
    memcpy(dst, src, sizeof(src));  /* 复制整块内存 */
    printf("memcpy: [0]=%d, [1]=%d, [2]=%d\n", dst[0], dst[1], dst[2]);

    /* ---- math.h ---- */
    print_h2("10.3 <math.h> 常用函数(编译时加 -lm)");

    printf("sqrt(16) = %.1f\n", sqrt(16));
    printf("pow(2, 10) = %.1f\n", pow(2, 10));
    printf("fabs(-3.14) = %.2f\n", fabs(-3.14));
    printf("ceil(3.2) = %.1f\n", ceil(3.2));
    printf("floor(3.8) = %.1f\n", floor(3.8));
    printf("round(3.5) = %.1f\n", round(3.5));
    /* 三角函数：参数是弧度，不是角度 */
    printf("sin(PI/2) = %.1f\n", sin(PI / 2));
    printf("cos(0) = %.1f\n", cos(0));

    /* ---- time.h ---- */
    print_h2("10.4 <time.h> 时间函数");

    time_t now = time(NULL);               /* 当前 Unix 时间戳 */
    printf("当前时间戳: %lld\n", (long long)now);

    struct tm* local = localtime(&now);
    printf("本地时间: %04d-%02d-%02d %02d:%02d:%02d\n",
           local->tm_year + 1900,   /* tm_year 从 1900 开始 */
           local->tm_mon + 1,       /* tm_mon 从 0 开始 */
           local->tm_mday,
           local->tm_hour,
           local->tm_min,
           local->tm_sec);

    clock_t start = clock();
    /* 做一些计算 */
    volatile long sum = 0;
    for (long i = 0; i < 10000000; i++) sum += i;
    clock_t end = clock();
    printf("计算耗时: %.3f 秒 (CPU时间)\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    /* ---- ctype.h ---- */
    print_h2("10.5 <ctype.h> 字符判断");

    printf("isdigit('5'): %d\n", isdigit('5'));
    printf("isalpha('A'): %d\n", isalpha('A'));
    printf("isalnum('x'): %d\n", isalnum('x'));
    printf("isspace(' '): %d\n", isspace(' '));
    printf("toupper('a'): '%c'\n", toupper('a'));
    printf("tolower('Z'): '%c'\n", tolower('Z'));
}

/* ============================================================================
 * 第11章：常见编程模式与惯用法
 * ============================================================================
 */

void chapter_11_patterns(void) {
    print_h1("第11章：C 语言常见模式与惯用法");

    /* ---- 字符串数组 ---- */
    print_h2("11.1 字符串数组");

    const char* names[] = {"Alice", "Bob", "Charlie", NULL};  /* NULL 结尾 */
    for (int i = 0; names[i] != NULL; i++) {
        printf("  %d: %s\n", i, names[i]);
    }

    /* ---- 函数指针实现回调 ---- */
    print_h2("11.2 回调函数(callback)模式");

    /* 遍历数组并对每个元素执行回调 */
    void for_each(int* arr, int n, void (*callback)(int*, int)) {
        for (int i = 0; i < n; i++) {
            callback(arr, i);
        }
    }
    void print_element(int* arr, int idx) {
        printf("  arr[%d] = %d\n", idx, arr[idx]);
    }
    int nums[] = {10, 20, 30};
    for_each(nums, 3, print_element);

    /* ---- 安全宏写法 ---- */
    print_h2("11.3 安全宏的写法");

    /* 用 do-while(0) 包裹多语句宏，确保在任何上下文中安全 */
    #define SAFE_SWAP(a, b, type) do { \
        type temp = a; \
        a = b; \
        b = temp; \
    } while(0)

    int x = 10, y = 20;
    SAFE_SWAP(x, y, int);
    printf("SAFE_SWAP 后: x=%d, y=%d\n", x, y);

    /* do-while(0) 确保宏在 if/else 等结构中安全使用
       if (cond) SAFE_SWAP(a,b,int); else ...    // 正确！
    */

    /* ---- error 处理模式 ---- */
    print_h2("11.4 C 语言错误处理模式");

    /* 模式1: 返回错误码 */
    /* int func(...) { return 0 成功; return -1 失败; } */

    /* 模式2: 通过指针返回结果 */
    /* int func(int* result) { ... *result = value; return 0; } */

    /* 模式3: setjmp/longjmp 非本地跳转（类似异常） */
    jmp_buf jump_buffer;
    if (setjmp(jump_buffer) == 0) {
        printf("  setjmp 设置了跳转点\n");
        /* 发生错误时跳转 */
        longjmp(jump_buffer, 1);  /* 跳回 setjmp，返回 1 */
    } else {
        printf("  longjmp 跳回，进行错误恢复\n");
    }
    printf("  注意: longjmp 不调用析构，可能泄漏资源!\n");

    /* ---- 泛型(C11 _Generic) ---- */
    print_h2("11.5 C11 _Generic 泛型");

    /* _Generic 实现编译时的类型选择（类似 C++ 的函数重载） */
    #define print_value(x) _Generic((x), \
        int:    printf("int: %d\n", (x)), \
        double: printf("double: %f\n", (x)), \
        char*:  printf("string: %s\n", (x)), \
        default: printf("unknown type\n") \
    )
    print_value(42);
    print_value(3.14);
    print_value("hello");
}

/* ============================================================================
 * 第12章：C 语言项目构建
 * ============================================================================
 */

void chapter_12_build(void) {
    print_h1("第12章：C 语言编译与构建");

    print_h2("12.1 编译流程（4个步骤）");

    printf("C 语言编译分为 4 个阶段:\n\n");
    printf("1. 预处理(Preprocessing)\n");
    printf("   #include/#define 展开 -> .i 文件\n");
    printf("   gcc -E source.c -o source.i\n\n");
    printf("2. 编译(Compilation)\n");
    printf("   源代码 -> 汇编代码 -> .s 文件\n");
    printf("   gcc -S source.c -o source.s\n\n");
    printf("3. 汇编(Assembly)\n");
    printf("   汇编代码 -> 目标文件 -> .o/.obj 文件\n");
    printf("   gcc -c source.c -o source.o\n\n");
    printf("4. 链接(Linking)\n");
    printf("   目标文件 + 库 -> 可执行文件\n");
    printf("   gcc source.o -o program\n");

    print_h2("12.2 GCC 常用编译选项");

    printf("gcc -Wall -Wextra    开启几乎所有警告\n");
    printf("gcc -O2              优化级别 2（平衡速度和体积）\n");
    printf("gcc -g               包含调试信息（配合 GDB）\n");
    printf("gcc -std=c11         指定 C11 标准\n");
    printf("gcc -pedantic        严格按标准检查\n");
    printf("gcc -fsanitize=address 启用地址检查（检测内存错误）\n");
    printf("gcc -lm              链接数学库（使用 math.h 时必须加）\n");
    printf("gcc -o output        指定输出文件名\n");

    print_h2("12.3 Makefile 基础");

    printf("简单的 Makefile 示例:\n");
    printf("```makefile\n");
    printf("# 变量定义\n");
    printf("CC = gcc\n");
    printf("CFLAGS = -Wall -Wextra -O2\n");
    printf("\n");
    printf("# 目标: 依赖\n");
    printf("#   \\t命令 (注意必须是 Tab 缩进!) \n");
    printf("program: main.o utils.o\n");
    printf("\\t$(CC) $(CFLAGS) -o program main.o utils.o\n");
    printf("\n");
    printf("main.o: main.c utils.h\n");
    printf("\\t$(CC) $(CFLAGS) -c main.c\n");
    printf("\n");
    printf("clean:\n");
    printf("\\trm -f *.o program\n");
    printf("```\n");
}

/* ============================================================================
 * 第13章：C 语言常见陷阱
 * ============================================================================
 */

void chapter_13_pitfalls(void) {
    print_h1("第13章：C 语言常见陷阱与最佳实践");

    printf("1. 数组越界\n");
    printf("   int arr[5]; arr[5] = 10; // 越界！索引是 0-4\n\n");

    printf("2. 字符串末尾没有 \\0\n");
    printf("   char str[5] = {'H','e','l','l','o'}; // 不是合法字符串!\n\n");

    printf("3. = 和 == 混淆\n");
    printf("   if (x = 5) ... // 赋值！永远为真！应该是 if (x == 5)\n\n");

    printf("4. 野指针和悬垂指针\n");
    printf("   free(ptr) 后继续使用 ptr -> 未定义行为\n\n");

    printf("5. 缓冲区溢出\n");
    printf("   gets() 是最危险的函数，用 fgets() 替代\n");
    printf("   strcpy 不检查大小，用 strncpy 或 snprintf\n\n");

    printf("6. 宏的副作用\n");
    printf("   #define SQUARE(x) x*x  // SQUARE(1+2) = 1+2*1+2 = 5!\n");
    printf("   应该: #define SQUARE(x) ((x)*(x))\n\n");

    printf("7. 整数溢出\n");
    printf("   int 最大值约 21亿，安全运算需检查边界\n\n");

    printf("8. 忘记关闭文件/释放内存\n");
    printf("   每次 fopen 都要 fclose，每次 malloc 都要 free\n\n");

    printf("9. 指针类型不匹配\n");
    printf("   不要随意在不同类型指针之间转换（除了 void*）\n\n");

    printf("10. 多线程问题\n");
    printf("    C 标准库本身不保证线程安全，需要用 pthread 或 C11 threads\n");
}

/* ============================================================================
 * 第14章：C 语言应用场景
 * ============================================================================
 */

void chapter_14_applications(void) {
    print_h1("第14章：C 语言应用场景概述");

    printf("C 语言诞生于 1972 年，至今仍是世界上最重要的编程语言之一。\n\n");

    const char* apps[][3] = {
        {"操作系统", "Linux 内核, Windows 内核, macOS 内核",
         "C 是唯一适合写操作系统的语言（直接操控硬件）"},
        {"嵌入式系统", "单片机, IoT 设备, 汽车 ECU",
         "资源受限设备上 C 是首选"},
        {"数据库", "MySQL, PostgreSQL, SQLite, Redis",
         "高性能数据处理需要 C 级别的控制"},
        {"编程语言", "Python(CPython), Lua, Ruby, PHP",
         "几乎所有脚本语言的解释器都是 C 写的"},
        {"网络服务", "Nginx, Apache, HAProxy",
         "高并发网络服务器追求极致性能"},
        {"图形与游戏", "OpenGL, Vulkan, 游戏引擎底层",
         "GPU 交互和实时渲染对性能要求极高"},
        {"安全工具", "Wireshark, Metasploit, OpenSSL",
         "底层网络和数据操作需要 C 语言"},
        {"编译器", "GCC, LLVM/Clang",
         "编译器本身几乎都是 C/C++ 实现"},
    };

    for (int i = 0; i < 8; i++) {
        printf("[%s]\n", apps[i][0]);
        printf("  实例: %s\n", apps[i][1]);
        printf("  原因: %s\n\n", apps[i][2]);
    }

    print_h2("14.2 为什么学 C 语言");

    printf("1. 理解计算机底层: 指针、内存、硬件交互\n");
    printf("2. 所有高级语言的基石: 学了 C 再学其他语言事半功倍\n");
    printf("3. 性能极致: C 程序通常是最快的之一\n");
    printf("4. 嵌入式/IoT: C 在这个领域无可替代\n");
    printf("5. 代码阅读: 大量基础软件(Linux/Git/Python)是 C 写的\n");
    printf("6. 培养严谨思维: 手动管理内存让你思考程序的每个细节\n");
}

/* ============================================================================
 * 主函数
 * ============================================================================
 */

int main(void) {
    printf("======================================================================\n");
    printf("    C 语言全知识点教学程序\n");
    printf("    覆盖 C 语言几乎所有核心知识点（14 个模块）\n");
    printf("======================================================================\n");
    printf("\n提示：每个章节都可以独立学习，直接跳到感兴趣的内容即可。\n");

    /* 运行所有章节 */
    chapter_01_basics();
    chapter_02_control_flow();
    chapter_03_arrays_strings();
    chapter_04_pointers();
    chapter_05_functions();
    demo_variadic();         /* 可变参数（独立小节） */
    chapter_06_memory();
    chapter_07_structs();
    chapter_08_preprocessor();
    chapter_09_file_io();
    chapter_10_stdlib();
    chapter_11_patterns();
    chapter_12_build();
    chapter_13_pitfalls();
    chapter_14_applications();

    printf("\n======================================================================\n");
    printf("    全部章节学习完毕！\n");
    printf("    编译命令: gcc c_full_tutorial.c -o tutorial -lm && ./tutorial\n");
    printf("======================================================================\n\n");

    return 0;
}
