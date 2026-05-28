/**
 * ============================================================================
 * 脚本功能：C++ 全知识点教学程序
 * - 覆盖 C++ 几乎所有核心知识点（14 个模块）
 * - 每个模块以独立函数组织，附带详细中文注释
 * - 每个知识点包含：概念说明 + 示例代码 + 运行结果（注释中标明）
 * - 编译运行：g++ -std=c++17 cpp_full_tutorial.cpp -o tutorial && ./tutorial
 * - 适合 C++ 零基础到进阶的系统学习（建议有一定 C 语言基础）
 * ============================================================================
 */

#include <iostream>       /* cin/cout 标准输入输出流 */
#include <string>         /* std::string 真正的字符串类 */
#include <vector>         /* std::vector 动态数组 */
#include <list>           /* std::list 双向链表 */
#include <map>            /* std::map 红黑树有序字典 */
#include <unordered_map>  /* std::unordered_map 哈希表字典 */
#include <set>            /* std::set 有序集合 */
#include <algorithm>      /* sort/find/for_each 等算法 */
#include <memory>         /* unique_ptr/shared_ptr/weak_ptr 智能指针 */
#include <functional>     /* std::function/bind/lambda 支持 */
#include <thread>         /* C++11 多线程 */
#include <mutex>          /* 互斥锁 */
#include <fstream>        /* 文件输入输出流 */
#include <sstream>        /* 字符串流 */
#include <stdexcept>      /* 标准异常类 */
#include <initializer_list> /* 初始化列表 */
#include <tuple>          /* std::tuple 元组 */
#include <array>          /* std::array 固定大小数组 */
#include <deque>          /* std::deque 双端队列 */
#include <iomanip>        /* setprecision/setw 等格式化 */

using namespace std;

/* ============================================================================
 * 辅助函数：打印章节标题
 * ============================================================================ */

void print_h1(const string& title) {
    cout << "\n\n======================================================================\n";
    cout << "  " << title << "\n";
    cout << "======================================================================\n\n";
}

void print_h2(const string& title) {
    cout << "\n**************************************************\n";
    cout << "  " << title << "\n";
    cout << "**************************************************\n\n";
}

void print_h3(const string& title) {
    cout << "\n--- " << title << " ---\n\n";
}

/* ============================================================================
 * 第1章：基础语法 -- 输入输出、命名空间、auto、范围for
 * ============================================================================ */

void chapter_01_basics() {
    print_h1("第1章：C++ 基础语法 -- 与 C 不同的地方");

    /* ---- cout 输出流 ---- */
    print_h2("1.1 cout 输出流（替代 printf）");

    // cout 是输出流对象，<< 是流插入运算符
    // endl 是换行 + 刷新缓冲区（\\n 只换行不刷新）
    int age = 25;
    double pi = 3.14159;
    string name = "张三";

    cout << "姓名: " << name << ", 年龄: " << age << ", pi = " << pi << endl;
    // 格式化输出（需要 <iomanip>）
    cout << fixed << setprecision(2);  // 固定小数点，保留2位
    cout << "格式化 pi = " << pi << endl;

    /* ---- cin 输入流 ---- */
    print_h2("1.2 cin 输入流（替代 scanf）");
    // cin >> 变量（不需要 &，因为 C++ 有引用）
    cout << "(此处跳过交互输入，展示语法)" << endl;
    cout << "int x; cin >> x;  // 从标准输入读取整数" << endl;

    /* ---- namespace 命名空间 ---- */
    print_h2("1.3 namespace 命名空间");
    // 命名空间用于组织代码，避免名称冲突
    namespace MyLib {
        const int VERSION = 1;
        void hello() { cout << "Hello from MyLib!" << endl; }
    }
    MyLib::hello();
    cout << "MyLib::VERSION = " << MyLib::VERSION << endl;

    /* ---- auto 自动类型推导 ---- */
    print_h2("1.4 auto 自动类型推导（C++11）");

    auto i = 42;           // int
    auto d = 3.14;         // double
    auto s = "hello";      // const char*
    auto str = string("hi"); // string
    auto vec = vector<int>{1, 2, 3};  // vector<int>

    cout << "auto 推导: i=" << i << " (int), d=" << d << " (double)" << endl;
    // auto 不能用于函数参数（C++20 之前），主要用于简化复杂类型

    /* ---- decltype 获取表达式类型 ---- */
    int x = 10;
    decltype(x) y = 20;    // y 的类型与 x 相同(int)
    cout << "decltype(x) y = " << y << endl;
    // auto 与 decltype 的区别：auto 推导值类型，decltype 推导声明类型

    /* ---- 范围 for 循环（C++11） ---- */
    print_h2("1.5 范围 for 循环（Range-based for）");

    int arr[] = {10, 20, 30, 40, 50};
    cout << "遍历数组: ";
    for (int n : arr) {       // 值拷贝（不修改原数组）
        cout << n << " ";
    }
    cout << endl;

    cout << "翻倍(引用): ";
    for (int& n : arr) {      // 引用方式，可修改原数组
        n *= 2;
    }
    for (int n : arr) cout << n << " ";
    cout << endl;

    vector<string> names = {"Alice", "Bob", "Charlie"};
    cout << "遍历 vector: ";
    for (const auto& name : names) {  // const auto& 避免拷贝
        cout << name << " ";
    }
    cout << endl;
}

/* ============================================================================
 * 第2章：引用 —— C++ 区别于 C 的核心特性
 * ============================================================================ */

// 左值引用：变量的别名
void increment_ref(int& x) {
    x++;                     // 修改引用就是修改原变量
}

// 常量引用：可接受左值和右值，但不允许修改
void print_ref(const int& x) {
    cout << "  const int& x = " << x << endl;
    // x++ 错误！不能通过 const 引用修改
}

// 右值引用（C++11）：绑定到临时对象，用于移动语义
void print_rvalue(int&& x) {
    cout << "  int&& x = " << x << " (右值引用)" << endl;
}

void chapter_02_references() {
    print_h1("第2章：引用（Reference）-- C++ 区别于 C 的核心");

    /* ---- 左值引用 ---- */
    print_h2("2.1 左值引用（lvalue reference）");

    int a = 10;
    int& ref = a;    // ref 是 a 的别名（引用），不是指针！
    ref = 20;        // 修改 ref 就是修改 a
    cout << "a = " << a << ", ref = " << ref << " (同一个变量!)" << endl;
    cout << "&a = " << &a << ", &ref = " << &ref << " (地址相同!)" << endl;

    // 引用作为函数参数（避免拷贝，允许修改）
    int val = 5;
    increment_ref(val);
    cout << "increment_ref 后 val = " << val << endl;

    /* ---- 常量引用 ---- */
    print_h2("2.2 const 引用");

    print_ref(42);           // 可以绑定到临时对象(字面量)
    int num = 100;
    print_ref(num);          // 也可以绑定到变量

    // 为什么要用 const&？
    // 1. 避免拷贝大对象（比如 vector<string>）
    // 2. 可以接受临时对象
    // 3. 明确表示"我不会修改这个参数"

    /* ---- 引用 vs 指针 ---- */
    print_h2("2.3 引用 vs 指针");

    cout << "引用: 必须初始化, 不能为null, 不能改变绑定, 语法更简洁" << endl;
    cout << "指针: 可以为null, 可以改变指向, 需要 * 解引用, 更灵活" << endl;
    cout << "优先使用引用(更安全), 需要'空'语义时用指针" << endl;

    /* ---- 右值引用基础 ---- */
    print_h2("2.4 右值引用预览（&&）");

    print_rvalue(100);       // 100 是右值（临时值）
    // int&& r = a;          // 错误！不能将右值引用绑定到左值
    int&& r = 42;            // 正确：绑定到字面量
    cout << "  int&& r = " << r << endl;
    // 右值引用是移动语义的基础，第11章详细讲解
}

/* ============================================================================
 * 第3章：类 —— C++ 面向对象基础
 * ============================================================================ */

// 类定义（类似 C 的 struct，但多了访问控制和成员函数）
class Person {
private:                     // 私有成员（默认 class 是 private）
    string name_;
    int age_;

public:                      // 公有成员
    // ---- 构造函数 ----
    // 默认构造函数
    Person() : name_("未知"), age_(0) {
        cout << "  [构造] 默认: " << name_ << endl;
    }

    // 带参构造函数（使用初始化列表）
    Person(const string& name, int age) : name_(name), age_(age) {
        cout << "  [构造] 带参: " << name_ << endl;
    }

    // 委托构造函数（C++11）：调用另一个构造函数
    Person(const string& name) : Person(name, 0) {
        cout << "    (委托构造完成)" << endl;
    }

    // ---- 析构函数 ----
    // 对象销毁时自动调用，用于释放资源
    ~Person() {
        cout << "  [析构] " << name_ << endl;
    }

    // ---- 拷贝构造 ----
    Person(const Person& other) : name_(other.name_), age_(other.age_) {
        cout << "  [拷贝构造] " << name_ << endl;
    }

    // ---- 拷贝赋值 ----
    Person& operator=(const Person& other) {
        cout << "  [拷贝赋值] " << other.name_ << endl;
        if (this != &other) {   // 防止自赋值
            name_ = other.name_;
            age_ = other.age_;
        }
        return *this;
    }

    // ---- Getter/Setter ----
    string getName() const { return name_; }   // const 方法：不修改成员
    int getAge() const { return age_; }
    void setAge(int age) { age_ = age; }
};

void chapter_03_classes() {
    print_h1("第3章：类(Class) -- C++ 面向对象");

    /* ---- 构造与析构 ---- */
    print_h2("3.1 构造函数与析构函数");

    cout << "创建 p1:" << endl;
    Person p1;                          // 默认构造

    cout << "创建 p2:" << endl;
    Person p2("张三", 25);               // 带参构造

    cout << "创建 p3(委托构造):" << endl;
    Person p3("李四");                   // 委托构造

    // 注意：析构函数在离开作用域时自动调用（RAII 原则）

    /* ---- 拷贝构造 vs 拷贝赋值 ---- */
    print_h2("3.2 拷贝构造 vs 拷贝赋值");
    // 重要区别：拷贝构造创建新对象，拷贝赋值修改已有对象
    cout << "Person p4 = p2; // 这是拷贝构造（不是赋值！）" << endl;
    Person p4 = p2;                     // 拷贝构造(不是赋值)

    cout << "Person p5; p5 = p2; // 这才是拷贝赋值" << endl;
    Person p5;
    p5 = p2;                            // 拷贝赋值

    /* ---- const 成员函数 ---- */
    print_h2("3.3 const 成员函数");

    const Person cp("常量对象", 30);
    cout << "常量对象: " << cp.getName() << ", " << cp.getAge() << "岁" << endl;
    // cp.setAge(31);  // 错误！const 对象不能调用非 const 方法

    /* ---- 移动构造与移动赋值（C++11，预览） ---- */
    print_h2("3.4 移动构造/移动赋值（预览）");
    cout << "移动语义是 C++11 最重要的特性，通过 '窃取' 资源避免深拷贝" << endl;
    cout << "详见第11章" << endl;
}

/* ============================================================================
 * 第4章：继承与多态
 * ============================================================================ */

// 基类
class Animal {
protected:                     // protected：子类可访问，外部不可
    string name_;
    int energy_;

public:
    Animal(const string& name) : name_(name), energy_(100) {}

    // virtual 声明虚函数，实现多态
    virtual string speak() const {
        return name_ + "发出了声音";
    }

    // 纯虚函数：子类必须实现，使得 Animal 成为"抽象类"（不能直接实例化）
    virtual string type() const = 0;

    // 普通函数也可以被子类调用
    void eat(const string& food) {
        energy_ += 10;
        cout << name_ << " 吃了 " << food << ", 能量: " << energy_ << endl;
    }

    // 虚析构函数：通过基类指针删除子类对象时，正确调用子类析构函数
    virtual ~Animal() {
        cout << "  [~Animal] " << name_ << endl;
    }
};

class Cat : public Animal {
public:
    Cat(const string& name) : Animal(name) {}

    string speak() const override {    // override 关键字：编译器检查是否正确重写
        return name_ + "说: 喵喵!";
    }

    string type() const override {
        return "猫科";
    }
};

class Dog : public Animal {
private:
    string breed_;                     // 子类独有的成员
public:
    Dog(const string& name, const string& breed = "土狗")
        : Animal(name), breed_(breed) {}

    string speak() const override {
        return name_ + "说: 汪汪! (品种: " + breed_ + ")";
    }

    string type() const override {
        return "犬科";
    }
};

void chapter_04_inheritance() {
    print_h1("第4章：继承与多态");

    /* ---- 基础继承 ---- */
    print_h2("4.1 继承基础");

    Cat cat("小花");
    Dog dog("旺财", "金毛");

    cout << cat.speak() << endl;
    cout << dog.speak() << endl;
    cat.eat("鱼");
    dog.eat("骨头");

    /* ---- 多态 ---- */
    print_h2("4.2 多态（虚函数）");

    // 多态：通过基类指针/引用调用虚函数，运行时决定调用哪个版本
    Animal* zoo[] = {&cat, &dog};
    cout << "多态调用 speak():" << endl;
    for (Animal* a : zoo) {
        cout << "  " << a->speak() << endl;
    }

    // 不使用 virtual 的话，通过基类指针调用的永远是基类版本！
    // 这就是为什么基类需要 virtual 析构函数

    /* ---- final 和 override ---- */
    print_h2("4.3 final 和 override 关键字（C++11）");

    cout << "override: 显式声明重写，编译器检查签名是否匹配" << endl;
    cout << "final: 阻止进一步重写 或 阻止继承" << endl;

    // class FinalCat final : public Cat {};  // FinalCat 不能被继承
}

/* ============================================================================
 * 第5章：操作符重载
 * ============================================================================ */

class Complex {
private:
    double real_, imag_;

public:
    Complex(double r = 0, double i = 0) : real_(r), imag_(i) {}

    // 成员函数重载 +（a + b，a 必须是 Complex）
    Complex operator+(const Complex& other) const {
        return Complex(real_ + other.real_, imag_ + other.imag_);
    }

    // 重载 +=（成员函数）
    Complex& operator+=(const Complex& other) {
        real_ += other.real_;
        imag_ += other.imag_;
        return *this;               // 返回引用，支持链式调用
    }

    // 重载 << 输出（必须是非成员函数，声明为友元）
    friend ostream& operator<<(ostream& os, const Complex& c) {
        os << "(" << c.real_ << " + " << c.imag_ << "i)";
        return os;
    }

    // 重载 ==
    bool operator==(const Complex& other) const {
        return real_ == other.real_ && imag_ == other.imag_;
    }

    // 重载前置 ++（成员函数，无参数）
    Complex& operator++() {
        ++real_;
        return *this;
    }

    // 重载后置 ++（int 参数是假的，仅用于区分前置和后置）
    Complex operator++(int) {
        Complex temp = *this;
        real_++;
        return temp;    // 返回旧值
    }
};

void chapter_05_operators() {
    print_h1("第5章：操作符重载");

    Complex c1(3, 4), c2(1, 2);
    cout << "c1 = " << c1 << ", c2 = " << c2 << endl;

    Complex c3 = c1 + c2;
    cout << "c1 + c2 = " << c3 << endl;

    c1 += c2;
    cout << "c1 += c2 后: " << c1 << endl;

    cout << "c1 == c2 : " << (c1 == c2 ? "true" : "false") << endl;

    cout << "++c2 = " << ++c2 << endl;
    cout << "c2++ = " << c2++ << ", 之后 c2 = " << c2 << endl;

    // 可重载的运算符: + - * / % ^ & | ~ ! = < > += -= *= /=
    //   == != <= >= ++ -- << >> [] () -> new delete
    // 不可重载: :: .* . ?: sizeof typeid
}

/* ============================================================================
 * 第6章：模板 —— 泛型编程
 * ============================================================================ */

// 函数模板：一个函数适配多种类型
template<typename T>
T max_val(T a, T b) {
    return (a > b) ? a : b;
}

// 多类型参数模板
template<typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype(a + b) {  // 尾置返回类型
    return a + b;
}

// 类模板
template<typename T, int Size>
class Stack {
private:
    T data_[Size];          // 编译期固定大小
    int top_;               // 注意：不是指针！

public:
    Stack() : top_(-1) {}

    bool push(const T& val) {
        if (top_ >= Size - 1) return false;
        data_[++top_] = val;
        return true;
    }

    bool pop(T& val) {
        if (top_ < 0) return false;
        val = data_[top_--];
        return true;
    }

    int size() const { return top_ + 1; }
    static constexpr int capacity = Size;  // 编译期常量
};

void chapter_06_templates() {
    print_h1("第6章：模板(Template) -- 泛型编程");

    /* ---- 函数模板 ---- */
    print_h2("6.1 函数模板");

    cout << "max_val(3, 7) = " << max_val(3, 7) << endl;
    cout << "max_val(3.14, 2.71) = " << max_val(3.14, 2.71) << endl;
    cout << "max_val(string(\"abc\"), string(\"abd\")) = "
         << max_val(string("abc"), string("abd")) << endl;
    // 编译器为每种类型组合生成一份代码（编译期多态）

    cout << "add(10, 3.14) = " << add(10, 3.14) << endl;

    /* ---- 类模板 ---- */
    print_h2("6.2 类模板");

    Stack<int, 100> intStack;
    intStack.push(10);
    intStack.push(20);
    intStack.push(30);
    cout << "栈容量: " << intStack.capacity << ", 当前大小: " << intStack.size() << endl;

    int val;
    while (intStack.pop(val)) {
        cout << "  弹出: " << val << endl;
    }

    // 模板特化：为特定类型提供专门实现
    /* template<>
    class Stack<bool, 100> { ... };  // bool 的专门版本 */
}

/* ============================================================================
 * 第7章：STL 容器 —— vector/list/map/set
 * ============================================================================ */

void chapter_07_containers() {
    print_h1("第7章：STL 容器（标准模板库）");

    /* ---- vector：动态数组 ---- */
    print_h2("7.1 vector -- 动态数组（最常用）");

    vector<int> vec = {1, 2, 3, 4, 5};  // 初始化列表
    vec.push_back(6);                    // 末尾追加
    vec.push_back(7);

    cout << "vector: ";
    for (int v : vec) cout << v << " ";
    cout << endl;

    cout << "大小: " << vec.size() << ", 容量: " << vec.capacity() << endl;
    cout << "首元素: " << vec.front() << ", 尾元素: " << vec.back() << endl;
    cout << "vec[2] = " << vec[2] << " (不检查越界)" << endl;
    cout << "vec.at(2) = " << vec.at(2) << " (检查越界，越界抛异常)" << endl;

    vec.pop_back();                      // 弹出末尾
    vec.insert(vec.begin() + 2, 99);     // 在索引2处插入
    vec.erase(vec.begin() + 3);          // 删除索引3

    cout << "修改后: ";
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
    cout << endl;
    // vector vs C 数组: 自动扩容、知道自己的大小、支持拷贝

    /* ---- list：双向链表 ---- */
    print_h2("7.2 list -- 双向链表");

    list<string> lst = {"C++", "Python", "Lua"};
    lst.push_front("C");                 // 头部插入（vector 没有这个）
    lst.push_back("Rust");               // 尾部插入

    cout << "list: ";
    for (const auto& s : lst) cout << s << " ";
    cout << endl;

    // list 的优势：任意位置插入/删除 O(1)
    // list 的劣势：不支持随机访问(没有 [] 运算符)

    /* ---- map：有序字典（红黑树） ---- */
    print_h2("7.3 map -- 有序字典（红黑树实现）");

    map<string, int> scores;
    scores["张三"] = 95;                 // 直接赋值
    scores["李四"] = 87;
    scores["王五"] = 92;
    scores.insert({"赵六", 78});         // insert 方式

    cout << "map 遍历(按键排序):" << endl;
    for (const auto& [key, value] : scores) {  // C++17 结构化绑定
        cout << "  " << key << ": " << value << endl;
    }

    // 查找
    auto it = scores.find("李四");
    if (it != scores.end()) {
        cout << "找到李四: " << it->second << "分" << endl;
    }

    /* ---- unordered_map：哈希表字典 ---- */
    print_h2("7.4 unordered_map -- 哈希表（O(1)查找）");

    unordered_map<string, string> dict;
    dict["hello"] = "你好";
    dict["world"] = "世界";
    dict["C++"] = "C++语言";

    cout << "dict[\"hello\"] = " << dict["hello"] << endl;
    cout << "dict[\"unknown\"] = " << dict["unknown"]
         << " (不存在的 key 返回默认值)" << endl;

    /* ---- 其他常用容器 ---- */
    print_h2("7.5 其他 STL 容器速览");

    cout << "array<int, 5>:  固定大小数组(比 C 数组安全)" << endl;
    cout << "deque<T>:       双端队列(两端插入O(1))" << endl;
    cout << "set<T>:         有序集合(元素唯一)" << endl;
    cout << "multiset<T>:    有序集合(允许重复)" << endl;
    cout << "stack<T>:       栈(后进先出, 适配器)" << endl;
    cout << "queue<T>:       队列(先进先出, 适配器)" << endl;
    cout << "priority_queue<T>: 优先队列(最大堆, 适配器)" << endl;
}

/* ============================================================================
 * 第8章：STL 算法与 Lambda 表达式
 * ============================================================================ */

void chapter_08_algorithms() {
    print_h1("第8章：STL 算法与 Lambda 表达式");

    vector<int> nums = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    /* ---- Lambda 表达式（C++11） ---- */
    print_h2("8.1 Lambda 表达式");

    // Lambda 语法: [捕获列表](参数列表) -> 返回类型 { 函数体 }
    // 返回类型可以省略，编译器自动推导

    auto square = [](int x) { return x * x; };
    cout << "square(5) = " << square(5) << endl;

    int factor = 10;
    // [=] 按值捕获外部变量，[&] 按引用捕获
    auto multiply = [factor](int x) { return x * factor; };
    cout << "multiply(5) = " << multiply(5) << endl;

    /* ---- 常用算法 ---- */
    print_h2("8.2 常用 STL 算法");

    // sort 排序
    vector<int> sorted = nums;
    sort(sorted.begin(), sorted.end());
    cout << "sort 升序: ";
    for (int n : sorted) cout << n << " ";
    cout << endl;

    // sort 自定义排序（Lambda）
    sort(sorted.begin(), sorted.end(), [](int a, int b) { return a > b; });
    cout << "sort 降序: ";
    for (int n : sorted) cout << n << " ";
    cout << endl;

    // find 查找
    auto it = find(nums.begin(), nums.end(), 7);
    if (it != nums.end()) {
        cout << "找到 7, 索引: " << distance(nums.begin(), it) << endl;
    }

    // count_if 条件计数
    int even_count = count_if(nums.begin(), nums.end(),
                              [](int n) { return n % 2 == 0; });
    cout << "偶数个数: " << even_count << endl;

    // transform 转换（类似 Python 的 map）
    vector<int> doubled(nums.size());
    transform(nums.begin(), nums.end(), doubled.begin(),
              [](int n) { return n * 2; });
    cout << "transform 翻倍: ";
    for (int n : doubled) cout << n << " ";
    cout << endl;

    // remove_if + erase（删除符合条件的元素）
    nums.erase(remove_if(nums.begin(), nums.end(),
                         [](int n) { return n % 2 == 0; }),
               nums.end());
    cout << "删除偶数后: ";
    for (int n : nums) cout << n << " ";

    // for_each
    cout << "\nfor_each: ";
    for_each(nums.begin(), nums.end(), [](int n) { cout << n << " "; });
    cout << endl;
}

/* ============================================================================
 * 第9章：智能指针 —— unique_ptr / shared_ptr / weak_ptr
 * ============================================================================ */

// 自定义类，演示自动释放
struct Resource {
    int id;
    Resource(int i) : id(i) {
        cout << "  [构造 Resource #" << id << "]" << endl;
    }
    ~Resource() {
        cout << "  [析构 Resource #" << id << "]" << endl;
    }
    void use() {
        cout << "  使用 Resource #" << id << endl;
    }
};

void chapter_09_smart_pointers() {
    print_h1("第9章：智能指针（告别 new/delete）");

    /* ---- unique_ptr：独占所有权 ---- */
    print_h2("9.1 unique_ptr -- 独占所有权（不能复制）");

    {
        // make_unique 创建（C++14，推荐）
        unique_ptr<Resource> up = make_unique<Resource>(1);
        up->use();
        // unique_ptr<Resource> up2 = up;  // 错误！不能拷贝
        unique_ptr<Resource> up2 = move(up);  // 但可以移动（转移所有权）
        // up 现在为空
        if (!up) cout << "up 已为空(所有权已转移)" << endl;
        up2->use();
    }  // 离开作用域，Resource 自动被 delete！

    /* ---- shared_ptr：共享所有权 ---- */
    print_h2("9.2 shared_ptr -- 共享所有权（引用计数）");

    {
        shared_ptr<Resource> sp1 = make_shared<Resource>(2);
        cout << "引用计数: " << sp1.use_count() << endl;
        {
            shared_ptr<Resource> sp2 = sp1;  // 拷贝，引用计数+1
            cout << "引用计数(sp1==sp2): " << sp1.use_count() << endl;
            sp2->use();
        }  // sp2 析构，引用计数-1
        cout << "sp2 离开作用域后 引用计数: " << sp1.use_count() << endl;
    }  // 引用计数归零，Resource 被 delete

    /* ---- weak_ptr：弱引用（不增加计数） ---- */
    print_h2("9.3 weak_ptr -- 弱引用（打破循环引用）");

    {
        shared_ptr<Resource> sp = make_shared<Resource>(3);
        weak_ptr<Resource> wp = sp;        // 不增加引用计数
        cout << "shared_ptr 计数: " << sp.use_count() << endl;

        // 使用 weak_ptr 前必须 lock() 检查是否过期
        if (auto locked = wp.lock()) {
            locked->use();
        }

        sp.reset();  // 手动释放
        cout << "sp reset 后: wp.expired() = "
             << (wp.expired() ? "true (已过期)" : "false") << endl;
    }

    /* ---- 总结 ---- */
    print_h2("9.4 智能指针选择指南");
    cout << "unique_ptr: 默认首选，独占所有权，零开销" << endl;
    cout << "shared_ptr: 需要共享所有权时使用（有引用计数开销）" << endl;
    cout << "weak_ptr:   配合 shared_ptr 打破循环引用" << endl;
    cout << "不要手动 new/delete！现代 C++ 几乎不需要" << endl;
}

/* ============================================================================
 * 第10章：异常处理
 * ============================================================================ */

// 自定义异常类
class DivideByZeroError : public runtime_error {
public:
    DivideByZeroError() : runtime_error("除数不能为零!") {}
};

double safe_divide(double a, double b) {
    if (b == 0) {
        throw DivideByZeroError();     // 抛出异常
    }
    return a / b;
}

void chapter_10_exceptions() {
    print_h1("第10章：异常处理");

    /* ---- try / catch / throw ---- */
    print_h2("10.1 try / catch / throw");

    // 基本的异常处理
    int tests[][2] = {{10, 2}, {10, 0}, {8, 3}};
    for (auto& test : tests) {
        try {
            double result = safe_divide(test[0], test[1]);
            cout << test[0] << " / " << test[1] << " = " << result << endl;
        } catch (const DivideByZeroError& e) {
            cout << test[0] << " / " << test[1] << " 异常: " << e.what() << endl;
        } catch (const exception& e) {       // 捕获所有标准异常
            cout << "其他异常: " << e.what() << endl;
        }
    }

    /* ---- RAII 与异常安全 ---- */
    print_h2("10.2 RAII 与异常安全");
    // RAII: Resource Acquisition Is Initialization
    // C++ 的核心设计哲学：资源在构造时获取，在析构时释放
    // 异常发生时，栈展开会自动调用所有局部对象的析构函数
    // 所以用智能指针/容器/fstream 等 RAII 类，不需要手动清理

    cout << "RAII 原则: 资源 = 构造获取, 析构释放" << endl;
    cout << "异常安全: 栈展开自动析构局部对象, 不会泄漏" << endl;
    cout << "推荐: 只用 RAII 类, 不写 try-finally 清理代码" << endl;

    /* ---- noexcept ---- */
    print_h2("10.3 noexcept 声明（C++11）");

    // noexcept 声明函数不会抛出异常（编译器可以优化）
    auto no_throw_func = []() noexcept { return 42; };
    cout << "noexcept 函数: " << no_throw_func() << endl;
    cout << "noexcept 好处: 编译器可以生成更优化的代码" << endl;
}

/* ============================================================================
 * 第11章：移动语义与右值引用
 * ============================================================================ */

// 模拟一个大资源类
class BigBuffer {
private:
    int* data_;
    size_t size_;

public:
    explicit BigBuffer(size_t size) : size_(size) {
        data_ = new int[size]();
        cout << "  [BigBuffer 构造] 分配 " << size << " 个 int" << endl;
    }

    // 拷贝构造（深拷贝，开销大）
    BigBuffer(const BigBuffer& other) : size_(other.size_) {
        data_ = new int[size_];
        copy(other.data_, other.data_ + size_, data_);  // 深拷贝数据
        cout << "  [BigBuffer 拷贝] 深拷贝 " << size_ << " 个元素" << endl;
    }

    // 移动构造（浅拷贝 + 窃取所有权，开销极小）
    BigBuffer(BigBuffer&& other) noexcept
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;       // 原对象放弃所有权
        other.size_ = 0;
        cout << "  [BigBuffer 移动] 窃取所有权 (零拷贝!)" << endl;
    }

    ~BigBuffer() {
        if (data_) {
            cout << "  [BigBuffer 析构] 释放 " << size_ << " 个 int" << endl;
            delete[] data_;
        }
    }

    size_t size() const { return size_; }
};

// 工厂函数，返回临时对象（右值）
BigBuffer createBuffer(size_t size) {
    return BigBuffer(size);          // 返回局部对象 → 移动语义自动生效
}

void chapter_11_move_semantics() {
    print_h1("第11章：移动语义（Move Semantics）-- C++11 最重要的特性");

    /* ---- 问题：不必要的深拷贝 ---- */
    print_h2("11.1 为什么需要移动语义");

    {
        cout << "--- 拷贝（开销大）---" << endl;
        BigBuffer buf1(1000000);      // 分配 100 万个 int
        BigBuffer buf2 = buf1;        // 深拷贝（分配新内存 + 复制数据）
        // buf1 仍然有效
        cout << "buf1.size() = " << buf1.size() << endl;
    }

    {
        cout << "\n--- 移动（零开销）---" << endl;
        BigBuffer buf3(1000000);
        BigBuffer buf4 = move(buf3);  // 移动！buf3 的内容被"偷走"
        // buf3 现在为空（处于"有效但未指定"状态）
        cout << "buf3.size() = " << buf3.size()
             << " (所有权已转移)" << endl;
        cout << "buf4.size() = " << buf4.size() << endl;
    }

    /* ---- 编译器自动移动 ---- */
    print_h2("11.2 编译器自动使用移动语义");

    {
        BigBuffer buf = createBuffer(500000);  // 移动！不是拷贝
        cout << "buf.size() = " << buf.size() << endl;
    }
    // 编译器优化：NRVO（具名返回值优化）甚至可以完全消除拷贝/移动！

    /* ---- 移动语义的设计原则 ---- */
    print_h2("11.3 移动语义的使用原则");

    cout << "1. 大部分情况下，编译器自动处理移动（函数返回、临时对象）" << endl;
    cout << "2. 显式用 std::move()：明确放弃所有权（如传入容器）" << endl;
    cout << "3. 移动后的对象处于'有效但未指定'状态（可析构、可重新赋值）" << endl;
    cout << "4. 移动构造/赋值应标记为 noexcept" << endl;
}

/* ============================================================================
 * 第12章：文件流与字符串流
 * ============================================================================ */

void chapter_12_file_streams() {
    print_h1("第12章：文件流与字符串流");

    /* ---- 写入文件 ---- */
    print_h2("12.1 文件写入(ofstream)");

    const string filename = "_test_cpp.txt";
    ofstream fout(filename);          // 打开文件（写模式）
    if (!fout) {
        cerr << "打开文件失败!" << endl;
        return;
    }
    fout << "第一行: Hello C++!" << endl;
    fout << "第二行: 数字=" << 42 << ", 浮点=" << fixed << 3.14159 << endl;
    fout.close();
    cout << "已写入: " << filename << endl;

    /* ---- 读取文件 ---- */
    print_h2("12.2 文件读取(ifstream)");

    ifstream fin(filename);
    if (!fin) {
        cerr << "打开文件失败!" << endl;
        return;
    }

    cout << "文件内容:" << endl;
    string line;
    while (getline(fin, line)) {     // 逐行读取
        cout << "  " << line << endl;
    }
    fin.close();

    /* ---- 字符串流 ---- */
    print_h2("12.3 字符串流(stringstream)");

    // ostringstream：写入到字符串
    ostringstream oss;
    oss << "姓名: " << "张三" << ", 年龄: " << 25;
    string result = oss.str();
    cout << "ostringstream: " << result << endl;

    // istringstream：从字符串读取
    istringstream iss("42 3.14 hello");
    int i;
    double d;
    string s;
    iss >> i >> d >> s;  // 按空格分割读取
    cout << "istringstream: i=" << i << ", d=" << d << ", s=" << s << endl;

    remove(filename.c_str());  // 清理测试文件
}

/* ============================================================================
 * 第13章：多线程（C++11 threads）
 * ============================================================================ */

mutex cout_mutex;  // 保护 cout 的互斥锁

void worker(int id, int delay_ms) {
    // lock_guard：RAII 风格的锁，离开作用域自动释放
    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "  [线程 " << id << "] 开始工作" << endl;
    }
    this_thread::sleep_for(chrono::milliseconds(delay_ms));
    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "  [线程 " << id << "] 完成" << endl;
    }
}

void chapter_13_threads() {
    print_h1("第13章：多线程（C++11）");

    // 创建线程
    thread t1(worker, 1, 100);
    thread t2(worker, 2, 50);

    cout << "主线程: 等待子线程完成..." << endl;
    t1.join();  // 等待线程结束
    t2.join();
    cout << "主线程: 全部完成!" << endl;

    // C++11 线程常用工具:
    // thread, mutex, lock_guard, unique_lock
    // condition_variable (条件变量)
    // future, promise, async (异步任务)
    // atomic (原子操作)
}

/* ============================================================================
 * 第14章：C++ 开发工具与编译
 * ============================================================================ */

void chapter_14_ecosystem() {
    print_h1("第14章：C++ 开发工具与常用库");

    print_h2("14.1 编译与构建工具");

    cout << "编译器:" << endl;
    cout << "  GCC/g++:   开源, Linux 默认, 跨平台" << endl;
    cout << "  Clang/CL++: 开源, 错误信息更友好, macOS 默认" << endl;
    cout << "  MSVC:      Windows, Visual Studio 自带" << endl;
    cout << endl;
    cout << "构建系统:" << endl;
    cout << "  CMake:     最流行的跨平台构建系统（推荐）" << endl;
    cout << "  Make:      传统 Unix 构建工具" << endl;
    cout << "  Ninja:     速度最快的构建系统" << endl;
    cout << "  Meson:     新一代构建系统" << endl;

    print_h2("14.2 包管理器");

    cout << "vcpkg:    微软出品, 跨平台, 与 CMake 集成好" << endl;
    cout << "Conan:    最流行的 C/C++ 包管理器, 类似 pip" << endl;
    cout << "CPM.cmake: CMake 脚本, 轻量级依赖管理" << endl;

    print_h2("14.3 C++ 标准版本");

    cout << "C++98: 第一个标准, 基础模板/STL/异常" << endl;
    cout << "C++11: 革命性更新(auto/lambda/智能指针/移动语义/线程)" << endl;
    cout << "C++14: 小改进(make_unique/泛型lambda)" << endl;
    cout << "C++17: 结构化绑定/if constexpr/string_view/optional" << endl;
    cout << "C++20: 概念(concepts)/协程/范围(ranges)/模块(modules)" << endl;
    cout << "C++23: 进一步改进" << endl;
    cout << "推荐: 至少使用 C++17" << endl;

    print_h2("14.4 C++ vs C 对比");

    cout << "类型安全:   C++ 更强（string/vector/智能指针替代裸指针）" << endl;
    cout << "内存管理:   C++ RAII 自动管理 / C 手动 malloc/free" << endl;
    cout << "代码复用:   C++ 模板+继承 / C 宏+函数指针" << endl;
    cout << "标准库:     C++ STL 功能强大 / C 标准库精简" << endl;
    cout << "编译速度:   C 更快 / C++ 模板展开较慢" << endl;
    cout << "运行速度:   两者相当（零开销抽象）" << endl;
    cout << "学习曲线:   C 简单深入 / C++ 庞大复杂" << endl;
}

/* ============================================================================
 * 主函数
 * ============================================================================ */

int main() {
    cout << "======================================================================\n";
    cout << "    C++ 全知识点教学程序\n";
    cout << "    覆盖 C++ 几乎所有核心知识点（14 个模块）\n";
    cout << "======================================================================\n";
    cout << "\n提示：每个章节都可以独立学习，建议顺序学习。\n";

    chapter_01_basics();
    chapter_02_references();
    chapter_03_classes();
    chapter_04_inheritance();
    chapter_05_operators();
    chapter_06_templates();
    chapter_07_containers();
    chapter_08_algorithms();
    chapter_09_smart_pointers();
    chapter_10_exceptions();
    chapter_11_move_semantics();
    chapter_12_file_streams();
    chapter_13_threads();
    chapter_14_ecosystem();

    cout << "\n======================================================================\n";
    cout << "    全部章节学习完毕！\n";
    cout << "    编译: g++ -std=c++17 cpp_full_tutorial.cpp -o tutorial -pthread\n";
    cout << "======================================================================\n\n";

    return 0;
}
