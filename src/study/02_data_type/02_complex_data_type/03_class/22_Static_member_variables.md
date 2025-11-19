### 静态成员变量

C++ 中的 `static` 关键字不仅用于静态局部变量，还可以用于类的成员变量，生成**静态成员变量**。静态成员变量的特性使其成为类中与所有实例共享数据的有力工具。理解静态成员变量的定义、使用及其生命周期是掌握类设计的关键。

### 1. 静态成员变量的基本概念

静态成员变量属于类，而不是类的实例。每个类的所有实例共享同一个静态成员变量。因此，修改静态成员变量的值会影响类的所有实例，而不是仅仅影响某个特定的对象。

例如，考虑一个简单的类 `Something`：

```cpp
#include <iostream>

struct Something
{
    static int s_value; // 声明静态成员变量
};

int Something::s_value{ 1 }; // 定义静态成员变量

int main()
{
    Something first{};
    Something second{};

    first.s_value = 2;

    std::cout << first.s_value << '\n';  // 输出 2
    std::cout << second.s_value << '\n'; // 输出 2

    return 0;
}
```

输出：

```
2
2
```

#### 关键点：
- `s_value` 是静态成员变量，它在类的所有实例之间共享。
- 修改 `first.s_value` 也会影响 `second.s_value`，因为它们指向同一个静态变量。

### 2. 静态成员与类对象无关

尽管静态成员变量可以通过类的实例（如 `first.s_value`）访问，但静态成员与类的对象无关。即使没有实例化类对象，静态成员也依然存在。

例如：

```cpp
#include <iostream>

class Something
{
public:
    static int s_value; // 声明静态成员变量
};

int Something::s_value{ 1 }; // 定义静态成员变量

int main()
{
    Something::s_value = 2;  // 通过类名访问静态成员变量
    std::cout << Something::s_value << '\n'; // 输出 2
    return 0;
}
```

输出：

```
2
```

在上述代码中，我们直接通过类名 `Something::s_value` 访问了静态成员变量，而没有创建任何类的实例。

#### 关键点：
- 静态成员变量可以通过类名和域解析符（`::`）直接访问，这是推荐的访问方式。
- 静态成员存在于类的作用域内，类似于全局变量，但它们的生命周期受类的作用域限制。

### 3. 定义和初始化静态成员变量

在类内部声明静态成员变量时，只是告诉编译器其存在，并没有为其分配内存或初始化。在类外部必须显式定义并初始化静态成员变量。

```cpp
#include <iostream>

class Something
{
public:
    static int s_value; // 声明静态成员变量
};

int Something::s_value{ 1 }; // 定义并初始化静态成员变量

int main()
{
    std::cout << Something::s_value << '\n'; // 输出 1
    return 0;
}
```

#### 关键点：
- 静态成员变量必须在类外部定义。
- 如果没有显式初始化，静态成员变量默认初始化为零。

### 4. 静态成员的最佳实践

#### 4.1 通过类名访问静态成员

使用类名和域解析符（`::`）访问静态成员是推荐的做法。例如：

```cpp
Something::s_value = 5;
std::cout << Something::s_value << '\n';
```

#### 4.2 将静态成员声明为 `inline` 或 `constexpr`

从 C++17 开始，静态成员可以直接在类内部定义和初始化，尤其是常量类型。使用 `inline` 或 `constexpr` 使得静态成员变量成为内联变量，从而允许在类内部初始化。

```cpp
class MyClass
{
public:
    static inline int s_value{ 42 }; // 静态内联变量可以在类内部定义和初始化
};
```

对于 `constexpr` 成员，C++17 允许在类定义内部直接初始化。

```cpp
class MyClass
{
public:
    static constexpr double s_value{ 3.14 }; // 静态 constexpr 成员在类内部初始化
};
```

### 5. 静态成员变量的应用

#### 5.1 为类的每个实例分配唯一的 ID

使用静态成员变量来分配唯一的 ID 是静态成员变量的一种常见用途。静态变量在所有对象之间共享，因此可以用来生成全局唯一的标识符。

```cpp
#include <iostream>

class Something
{
private:
    static inline int s_idGenerator{ 1 };
    int m_id {};

public:
    Something() : m_id{ s_idGenerator++ } {}

    int getID() const { return m_id; }
};

int main()
{
    Something first{};
    Something second{};
    Something third{};

    std::cout << first.getID() << '\n';  // 输出 1
    std::cout << second.getID() << '\n'; // 输出 2
    std::cout << third.getID() << '\n';  // 输出 3

    return 0;
}
```

输出：

```
1
2
3
```

#### 5.2 静态成员变量作为查找表

静态成员变量还可以用于存储查找表等全局共享数据，这样避免为每个对象创建多个副本，从而节省内存。

```cpp
class LookupTable
{
public:
    static inline std::vector<int> table{ 10, 20, 30 }; // 查找表
};
```

### 6. 访问静态成员变量时的注意事项

#### 6.1 非静态成员不能使用 `auto` 或 CTAD

静态成员变量可以使用 `auto` 或类模板参数推断（CTAD）来推导其类型，但非静态成员变量不能使用这些功能。

```cpp
class Foo
{
private:
    static inline auto s_x { 5 }; // 静态成员可以使用 auto
    static inline std::pair s_v { 1, 2.3 }; // 静态成员可以使用 CTAD

    auto m_x { 5 }; // 非静态成员不能使用 auto
    std::pair m_v { 1, 2.3 }; // 非静态成员不能使用 CTAD
};
```

#### 6.2 静态成员的生命周期

静态成员变量的生命周期与程序的执行周期一致——它们在程序开始时初始化，在程序结束时销毁。由于静态成员变量的存储是全局的，它们不会随着对象的创建或销毁而改变。

### 7. 静态成员变量的总结

| 特性                 | 描述                                                         |
|----------------------|--------------------------------------------------------------|
| **共享性**           | 静态成员变量由类的所有实例共享，所有对象对其进行访问时操作的是同一个变量。 |
| **生命周期**         | 静态成员变量在程序开始时创建，在程序结束时销毁，独立于对象的生命周期。 |
| **访问方式**         | 静态成员通常通过类名和域解析符（`::`）访问，也可以通过类的实例访问。 |
| **定义和初始化**     | 静态成员变量必须在类外定义并初始化，常量类型可以在类内初始化。 |
| **优化**             | 静态成员变量适用于跨多个对象共享数据、生成唯一标识符或存储查找表等场景。 |


### 9. 结论

静态成员变量是 C++ 类中非常重要的功能，它们通过类的所有对象共享数据。静态成员的定义和初始化方式有所不同，通常需要在类外显式定义和初始化。合理使用静态成员变量可以大大提高内存效率，减少冗余数据，适用于生成唯一标识符、存储共享数据等场景。在实际应用中，建议优先考虑使用 `inline` 或 `constexpr` 来在类内部初始化静态成员。