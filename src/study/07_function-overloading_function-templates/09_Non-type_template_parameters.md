### 非类型模板参数概述
在之前的课程中，我们主要讨论了使用模板类型参数的函数模板，模板类型参数是实际类型的占位符。而这部分内容将介绍另一种模板参数——非类型模板参数。非类型模板参数是具有固定类型的模板参数，它作为模板参数传入的 `constexpr` 值的占位符。

### 非类型模板参数的类型
非类型模板参数可以是以下几种类型：
1. **整型**
2. **枚举类型**
3. **std::nullptr_t**
4. **浮点类型（自 C++20）**
5. **指向对象的指针或引用**
6. **指向函数的指针或引用**
7. **指向成员函数的指针或引用**
8. **字面值类类型（自 C++20）**

### 定义自己的非类型模板参数
下面是一个使用 `int` 非类型模板参数的简单示例：
```cpp
#include <iostream>

template <int N> // 声明一个非类型模板参数，类型是 int 名称是 N
void print()
{
    std::cout << N << '\n'; // 在这里使用N
}

int main()
{
    print<5>(); // 5 是传入的非类型模板参数

    return 0;
}
```
在这个示例中，`template <int N>` 定义了一个名为 `N` 的非类型模板参数，它是 `int` 类型值的占位符。在 `print` 函数内部使用了 `N` 的值。当调用 `print<5>()` 时，编译器会实例化如下函数：
```cpp
template <>
void print<5>()
{
    std::cout << 5 << '\n';
}
```
运行时，该函数会打印出 `5`。通常，`N` 常被用作 `int` 非类型模板参数的名称。

### 非类型模板参数的用途
从 C++20 开始，函数参数不能声明为 `constexpr`。考虑下面的 `getSqrt` 函数：
```cpp
#include <cassert>
#include <cmath> // for std::sqrt
#include <iostream>

double getSqrt(double d)
{
    assert(d >= 0.0 && "getSqrt(): d must be non-negative");

    // 上面的assert在非debug环境可能不会编译进来
    if (d >= 0)
        return std::sqrt(d);

    return 0.0;
}

int main()
{
    std::cout << getSqrt(5.0) << '\n';
    std::cout << getSqrt(-5.0) << '\n';

    return 0;
}
```
调用 `getSqrt(-5.0)` 会在运行时触发 `assert` 失败。虽然这比没有检查要好，但如果能在编译时捕获这种错误会更好。由于 `static_assert` 需要常量表达式，而函数参数不能是 `constexpr`，所以无法直接使用 `static_assert`。

不过，如果将函数参数改为非类型模板参数，就可以实现编译时检查：
```cpp
#include <cmath> // for std::sqrt
#include <iostream>

template <double D> // 需要 C++20 才能使用浮点 非类型模板参数
double getSqrt()
{
    static_assert(D >= 0.0, "getSqrt(): D must be non-negative");

    if constexpr (D >= 0) // 忽略这里的 constexpr
        return std::sqrt(D);

    return 0.0;
}

int main()
{
    std::cout << getSqrt<5.0>() << '\n';
    std::cout << getSqrt<-5.0>() << '\n';

    return 0;
}
```
当编译器遇到 `getSqrt<-5.0>()` 时，会实例化如下函数：
```cpp
template <>
double getSqrt<-5.0>()
{
    static_assert(-5.0 >= 0.0, "getSqrt(): D must be non-negative");

    if constexpr (-5.0 >= 0) // 忽略这里的 constexpr
        return std::sqrt(-5.0);

    return 0.0;
}
```
由于 `static_assert` 条件为 `false`，编译器会报错。非类型模板参数主要用于需要将 `constexpr` 值传递给函数（或类类型），以便在需要常量表达式的上下文中使用。例如，`std::bitset` 类使用非类型模板参数来定义要存储的位数，因为位数必须是 `constexpr` 值。

### 非类型模板参数的隐式转换
可以隐式转换某些非类型模板参数，以匹配不同类型的非类型模板形参。例如：
```cpp
#include <iostream>

template <int N> // int 非类型模板参数
void print()
{
    std::cout << N << '\n';
}

int main()
{
    print<5>();   // 无需转换
    print<'c'>(); // 'c' 被转为 int, 打印 99

    return 0;
}
```
在这个示例中，`'c'` 被转换为 `int` 以匹配函数模板 `print` 的非类型模板参数，然后函数将值打印为 `int`。在此上下文中，仅允许某些类型的 `constexpr` 转换，最常见的允许转换类型包括：
1. **整形提升（例如从 `char` 到 `int`）**
2. **整形转换（例如，`char` 到 `long` 或 `int` 到 `char`）**
3. **用户自定义的转换（例如，自定义的类到 `int`）**
4. **左值到右值的转换（例如，变量 `x` 转换为变量 `x` 的值）**

需要注意的是，与列表初始化所允许的隐式转换类型相比，该列表的权限更小。例如，可以使用 `constexpr int` 列表初始化类型为 `double` 的变量，但 `constexpr int` 非类型模板参数不会转换为 `double` 非类型模板参数。

如果为不同类型的非类型模板参数重载函数模板，很容易导致不明确的匹配：
```cpp
#include <iostream>

template <int N> // int 非类型模板参数
void print()
{
    std::cout << N << '\n';
}

template <char N> // char 非类型模板参数
void print()
{
    std::cout << N << '\n';
}

int main()
{
    print<5>();   // 不明确的匹配 int N = 5 与 char N = 5
    print<'c'>(); // 不明确的匹配 int N = 99 与 char N = 'c'

    return 0;
}
```
上述代码中，两个对 `print()` 的调用都会导致不明确的匹配。

### 使用 `auto` (C++17) 对非类型模板参数进行类型推导
从 C++17 开始，可以使用 `auto` 让编译器从实际调用推导非类型模板的参数类型：
```cpp
#include <iostream>

template <auto N> // 针对模板参数的类型，让编译器从调用自动推导
void print()
{
    std::cout << N << '\n';
}

int main()
{
    print<5>();   // N 被推导为 int `5`
    print<'c'>(); // N 被推到为 char `c`

    return 0;
}
```
这个示例不会产生不明确的匹配，因为编译器首先查找不明确的匹配，然后在不存在不明确匹配的情况下实例化函数模板。在这个例子中，只有一个函数模板，所以没有可能的歧义。实例化后的程序如下：
```cpp
#include <iostream>

template <auto N>
void print()
{
    std::cout << N << '\n';
}

template <>
void print<5>() // 注意这是 print<5> 而不是 print<int>
{
    std::cout << 5 << '\n';
}

template <>
void print<'c'>() // 注意这是 print<`c`> 而不是 print<char>
{
    std::cout << 'c' << '\n';
}

int main()
{
    print<5>();   // 调用 print<5>
    print<'c'>(); // 调用 print<'c'>

    return 0;
}
```

综上所述，非类型模板参数为我们在编译时处理常量表达式提供了一种有效的方式，但在使用时需要注意隐式转换和函数模板重载可能带来的不明确匹配问题。 