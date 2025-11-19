### 类模板的详细解析

类模板（Class Templates）与函数模板类似，允许你为类类型创建一个通用模板，使得你可以根据实际需求实例化不同的数据类型。这对于避免重复代码以及处理多种类型的场景非常有用。

#### 1. 类模板基础

类模板是通过 `template` 关键字定义的，允许为类的成员提供模板类型参数。在类模板的定义中，你可以指定一个或多个类型参数，编译器会根据所提供的类型实例化类。

例如，定义一个 `Pair` 类模板：

```cpp
#include <iostream>

template <typename T>
struct Pair
{
    T first{};  // 模板类型的成员
    T second{}; // 模板类型的成员
};

int main()
{
    Pair<int> p1{ 5, 6 };        // 实例化 Pair<int> 并创建对象 p1
    std::cout << p1.first << ' ' << p1.second << '\n';  // 输出: 5 6

    Pair<double> p2{ 1.2, 3.4 }; // 实例化 Pair<double> 并创建对象 p2
    std::cout << p2.first << ' ' << p2.second << '\n';  // 输出: 1.2 3.4

    return 0;
}
```

在这个示例中，`Pair` 是一个类模板，我们通过 `template <typename T>` 定义了一个模板，其中 `T` 是模板类型参数。在 `main` 函数中，我们实例化了 `Pair<int>` 和 `Pair<double>` 类型，并赋值给 `p1` 和 `p2`。

#### 2. 模板实例化

当你在代码中使用类模板时，编译器会根据提供的类型进行模板实例化，生成实际的类类型。例如，当你使用 `Pair<int>` 时，编译器将生成以下结构体：

```cpp
struct Pair<int>
{
    int first{};
    int second{};
};
```

同样，使用 `Pair<double>` 时，编译器会生成：

```cpp
struct Pair<double>
{
    double first{};
    double second{};
};
```

这种机制允许你为不同的类型复用同一个类模板定义。

#### 3. 类模板特化

类模板特化（Class Template Specialization）是对类模板进行个别化定义的技术。通过特化，你可以为某个特定类型定义一个特殊的类模板。

例如，对于 `Pair<int>` 类型，你可以进行特化：

```cpp
template <>
struct Pair<int>
{
    int first{};
    int second{};
    void print() { std::cout << "Int Pair: " << first << ", " << second << std::endl; }
};
```

在这里，我们为 `Pair<int>` 定义了一个特化版本，并添加了一个成员函数 `print()`。

#### 4. 类模板与函数模板结合

你可以将类模板与函数模板结合，创建一个接受类模板实例的函数模板。例如，创建一个 `max` 函数模板，计算 `Pair` 类型对象中 `first` 和 `second` 中较大的值：

```cpp
template <typename T>
struct Pair
{
    T first{};
    T second{};
};

template <typename T>
T max(const Pair<T>& p)
{
    return (p.first < p.second) ? p.second : p.first;
}

int main()
{
    Pair<int> p1{5, 6};
    std::cout << max(p1) << " is larger\n"; // 输出: 6

    Pair<double> p2{1.2, 3.4};
    std::cout << max(p2) << " is larger\n"; // 输出: 3.4

    return 0;
}
```

在这个例子中，`max` 函数是一个模板函数，它接受一个 `Pair<T>` 类型的对象作为参数，并返回 `first` 和 `second` 中较大的值。

#### 5. 多模板参数

类模板可以拥有多个模板参数，这样你可以为类的不同成员定义不同的类型。例如：

```cpp
template <typename T, typename U>
struct Pair
{
    T first{};
    U second{};
};

template <typename T, typename U>
void print(const Pair<T, U>& p)
{
    std::cout << "[" << p.first << ", " << p.second << "]\n";
}

int main()
{
    Pair<int, double> p1{ 1, 2.3 };
    Pair<double, int> p2{ 4.5, 6 };
    print(p1);
    print(p2);

    return 0;
}
```

在这个示例中，`Pair` 类模板使用了两个类型参数 `T` 和 `U`，分别表示 `first` 和 `second` 成员的类型。`print` 函数模板接受一个 `Pair<T, U>` 类型的对象，并输出其成员。

#### 6. 类模板的成员类型与普通类型结合

类模板允许在模板类型和普通类型之间进行组合。你可以定义一个类模板，其中某些成员使用模板类型，其他成员使用普通类型。例如：

```cpp
template <typename T>
struct Foo
{
    T first{};    // 模板类型
    int second{}; // 普通类型
};
```

在上面的例子中，`first` 是模板类型 `T`，而 `second` 是一个普通的 `int` 类型。

#### 7. std::pair 与自定义类模板

C++标准库提供了一个现成的 `std::pair` 类模板，用于存储两个元素。它的定义如下：

```cpp
#include <iostream>
#include <utility>  // std::pair

template <typename T, typename U>
void print(const std::pair<T, U>& p)
{
    std::cout << "[" << p.first << ", " << p.second << "]\n";
}

int main()
{
    std::pair<int, double> p1{ 1, 2.3 };
    print(p1); // 输出: [1, 2.3]

    return 0;
}
```

在这个示例中，我们使用了 C++ 标准库中的 `std::pair` 类模板，它与我们自定义的 `Pair` 类模板功能相同。`std::pair` 是一个非常常用的类模板，它可以存储两个不同类型的元素。

#### 8. 类模板的多文件使用

在多个文件中使用类模板时，通常将模板定义放在头文件中，这样其他文件可以引用它。由于模板定义不受单定义规则的约束，因此可以在多个源文件中使用相同的模板定义。

例如，`pair.h` 文件包含模板定义：

```cpp
#ifndef PAIR_H
#define PAIR_H

template <typename T>
struct Pair
{
    T first{};
    T second{};
};

template <typename T>
constexpr T max(const Pair<T>& p)
{
    return (p.first < p.second) ? p.second : p.first;
}

#endif
```

在 `main.cpp` 和其他源文件中包含该头文件：

```cpp
#include "pair.h"
#include <iostream>

int main()
{
    Pair<int> p1{ 1, 2 };
    std::cout << max(p1) << " is larger\n";

    return 0;
}
```

#### 9. 总结与最佳实践

- **类模板** 可以使你为多种数据类型定义通用的类。
- **模板特化** 提供了对特定类型的定制化处理。
- **多个模板类型参数** 允许你在类中使用不同类型的成员。
- 在多个源文件中使用类模板时，通常将其定义放在头文件中。
- 如果需要更复杂的功能，考虑使用 `std::pair` 或其他标准库类型。

