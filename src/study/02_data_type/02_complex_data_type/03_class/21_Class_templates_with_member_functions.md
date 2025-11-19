### 具有成员函数的类模板

类模板是 C++ 中非常强大的特性，允许我们定义一个类模板，可以在不同的类型上进行实例化。与普通的类不同，类模板的类型参数是模板参数，通常可以根据需求选择不同的数据类型。

#### 1. 函数模板回顾

我们先回顾一下简单的函数模板。函数模板允许我们为一个函数定义通用的类型参数，并根据实际传递的类型来实例化该函数：

```cpp
template <typename T> // 模板参数声明
T max(T x, T y) // 函数模板 max<T> 定义
{
    return (x < y) ? y : x;
}
```

上面的模板允许你定义一个可以比较任意类型的函数。只要类型 `T` 支持 `<` 运算符，它就能够接受不同的数据类型，返回其中的最大值。

#### 2. 类模板简介

类模板的概念和函数模板类似，它允许我们为类的成员定义类型参数。这意味着你可以在类中使用模板参数来定义成员变量的类型和成员函数的参数类型。下面是一个类模板的简单示例：

```cpp
template <typename T>
struct Pair
{
    T first{};
    T second{};
};

int main()
{
    Pair<int> p1{ 5, 6 };        // 实例化 Pair<int> 并创建对象 p1
    std::cout << p1.first << ' ' << p1.second << '\n';

    Pair<double> p2{ 1.2, 3.4 }; // 实例化 Pair<double> 并创建对象 p2
    std::cout << p2.first << ' ' << p2.second << '\n';

    return 0;
}
```

在这个示例中，`Pair` 是一个类模板，具有两个成员变量 `first` 和 `second`，它们的类型由模板参数 `T` 决定。我们可以使用不同的类型实例化 `Pair`，如 `Pair<int>` 或 `Pair<double>`。

#### 3. 具有成员函数的类模板

在类模板中，我们不仅可以使用模板参数来定义成员变量的类型，还可以使用它们来定义成员函数的参数类型。例如，我们可以定义一个成员函数 `isEqual`，用于比较两个 `Pair` 对象是否相等：

```cpp
template <typename T>
class Pair
{
private:
    T m_first{};
    T m_second{};

public:
    Pair(const T& first, const T& second)
        : m_first{ first }, m_second{ second }
    {
    }

    bool isEqual(const Pair<T>& pair);
};

template <typename T>
bool Pair<T>::isEqual(const Pair<T>& pair)
{
    return m_first == pair.m_first && m_second == pair.m_second;
}

int main()
{
    Pair p1{ 5, 6 }; // 使用 CTAD 来推导 Pair<int>
    std::cout << std::boolalpha << "isEqual(5, 6): " << p1.isEqual(Pair{ 5, 6 }) << '\n';
    std::cout << std::boolalpha << "isEqual(5, 7): " << p1.isEqual(Pair{ 5, 7 }) << '\n';

    return 0;
}
```

**说明**：
- `Pair` 类模板有两个成员变量：`m_first` 和 `m_second`，它们的类型都是模板类型 `T`。
- `Pair` 类的构造函数接受两个 `T` 类型的参数并初始化这两个成员。
- `isEqual` 成员函数比较当前对象与另一个 `Pair` 对象的成员 `first` 和 `second` 是否相等。

#### 4. 在类模板外部定义成员函数

如果我们希望在类模板外部定义成员函数，则需要在外部提供模板参数的声明，并使用完整的类模板名称来限定成员函数。举个例子：

```cpp
template <typename T>
bool Pair<T>::isEqual(const Pair<T>& pair)
{
    return m_first == pair.m_first && m_second == pair.m_second;
}
```

这里，`Pair<T>::isEqual` 明确告诉编译器我们是在定义 `Pair` 类模板的成员函数。

#### 5. 类模板外部成员函数的定义位置

通常情况下，我们希望将类模板的定义和成员函数定义放在同一个文件中。这样，编译器就能更好地理解类模板的实现。特别是当成员函数定义在类模板外部时，最好将成员函数的定义紧随类定义之后，以便编译器能很容易地看到类模板及其成员函数：

```cpp
template <typename T>
class Pair
{
private:
    T m_first{};
    T m_second{};

public:
    Pair(const T& first, const T& second)
        : m_first{ first }, m_second{ second }
    {
    }

    bool isEqual(const Pair<T>& pair);
};

// 在类定义后立即定义成员函数
template <typename T>
bool Pair<T>::isEqual(const Pair<T>& pair)
{
    return m_first == pair.m_first && m_second == pair.m_second;
}
```

#### 6. 类型推导与成员函数

在 C++17 引入了 **类模板自动类型推导（CTAD）**，即通过构造函数的初始值来推导模板参数。例如，在上面的代码中，`Pair p1{ 5, 6 };` 使用了CTAD机制来推导出 `Pair<int>` 类型。

### 7. 模板函数的最佳实践

1. **成员函数模板定义位置**：将成员函数模板定义放置在类模板定义下方是一个常见的最佳实践，尤其是在头文件中。这样可以确保编译器在看到类定义时，能够同时看到相关的成员函数模板定义。

2. **隐式内联**：成员函数模板隐式内联，因此即使我们在多个代码文件中包含头文件，编译器会根据模板实例化生成内联函数，而不需要担心重复定义的问题。

3. **常量引用传递**：在模板类的构造函数中，建议通过常量引用来传递参数，以避免不必要的拷贝操作，尤其是当类型 `T` 的拷贝成本较高时。

### 8. 类模板与成员函数模板的总结

| 特性                 | 描述                                                         |
|----------------------|--------------------------------------------------------------|
| **模板参数**         | 类模板和成员函数模板都可以接受类型参数 `T`，并在类或成员函数中使用这些参数定义类型。 |
| **成员函数定义位置** | 如果成员函数在类模板内部定义，则无需额外声明模板参数；如果在类模板外部定义，则需要使用模板参数并指定类模板名称。 |
| **模板类型推导（CTAD）** | 在 C++17 中引入的类型推导机制，允许编译器根据构造函数的参数自动推导出模板类型。 |
| **隐式内联**         | 类模板的成员函数模板会被隐式内联，避免了多次定义时的问题。 |



### 10. 完整代码示例

```cpp
#include <iostream>
#include <ios>

template <typename T>
class Pair
{
private:
    T m_first{};
    T m_second{};

public:
    Pair(const T& first, const T& second)
        : m_first{ first }, m_second{ second }
    {
    }

    bool isEqual(const Pair<T>& pair);
};

// 在类外定义成员函数
template <typename T>
bool Pair<T>::isEqual(const Pair<T>& pair)
{
    return m_first == pair.m_first && m_second == pair.m_second;
}

int main()
{
    Pair p1{ 5, 6 };
    std::cout << std::boolalpha << "isEqual(5, 6): " << p1.isEqual(Pair{5, 6}) << '\n';
    std::cout << std::boolalpha << "isEqual(5, 7): " << p1.isEqual(Pair{5, 7}) << '\n';

    return 0;
}
```

### 11. 结论

通过类模板和成员函数模板，C++ 提供了一个强大的机制来创建通用的类型安全数据结构。模板允许你根据不同的数据类型创建相同的类和函数，避免了重复代码并提高了代码的灵活性。了解如何在类模板中定义成员函数，以及如何在类模板外部定义成员函数，是掌握模板编程的一个重要步骤。