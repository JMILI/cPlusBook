### 类模板参数推导（CTAD）

类模板参数推导（Class Template Argument Deduction，简称CTAD）是C++17引入的一项特性。CTAD允许编译器在实例化类模板时，从传递给构造函数的初始化值推导模板的参数类型。CTAD大大简化了代码，避免了显式指定模板参数的需求。通过CTAD，编译器能够根据传入的值自动推导出模板的类型。

### 基本概念

CTAD的核心功能是允许编译器从对象初始化的类型推导出模板参数类型。例如：

```cpp
#include <utility> // for std::pair

int main()
{
    std::pair p1{ 1, 2 }; // 推导为 std::pair<int, int>
    return 0;
}
```

在这个例子中，`std::pair`类模板的类型参数会根据初始化列表`{1, 2}`的类型（即`int`）进行推导。CTAD会自动推导出`std::pair<int, int>`。

### CTAD不适用的情况

CTAD只有在没有显式指定模板参数的情况下才会发生。以下两种情况会导致编译错误：

```cpp
#include <utility> // for std::pair

int main()
{
    std::pair<> p1 { 1, 2 };    // 错误: 缺少模板参数
    std::pair<int> p2 { 3, 4 }; // 错误: 第二个模板参数缺失
    return 0;
}
```

在上面的代码中，`std::pair<>`和`std::pair<int>`没有足够的模板参数，编译器无法推导出正确的类型。

### 推导不同类型的例子

CTAD不仅支持`int`类型，还支持其他类型。例如，通过字面值后缀，推导出的类型可以为`float`或`unsigned int`等：

```cpp
#include <utility> // for std::pair

int main()
{
    std::pair p1 { 3.4f, 5.6f }; // 推导为 std::pair<float, float>
    std::pair p2 { 1u, 2u };     // 推导为 std::pair<unsigned int, unsigned int>
    return 0;
}
```

### 类模板参数推导指南（C++17）

尽管CTAD在许多情况下是自动生效的，但对于某些类模板（尤其是聚合类型）来说，编译器可能需要更多的帮助来推导类型。一个典型的例子是自定义类模板。

#### 例1：自定义`Pair`类模板

```cpp
template <typename T, typename U>
struct Pair
{
    T first{};
    U second{};
};

int main()
{
    Pair<int, int> p1{ 1, 2 }; // 明确指定模板类型
    Pair p2{ 1, 2 };           // 错误: C++17 中不支持 CTAD
    return 0;
}
```

在上面的代码中，`Pair`类模板没有CTAD指引，因此C++17编译器无法推导出`Pair<int, int>`。

为了使`Pair`类模板支持CTAD，我们需要为其提供**推导指引**：

```cpp
template <typename T, typename U>
Pair(T, U) -> Pair<T, U>;  // 推导指引

int main()
{
    Pair<int, int> p1{ 1, 2 }; // 显式声明
    Pair p2{ 1, 2 };           // C++17 中编译通过
    return 0;
}
```

推导指引（C++17特性）指定了如果构造函数的参数是`T`和`U`类型，则推导为`Pair<T, U>`。当编译器看到`Pair p2{ 1, 2 }`时，会根据推导指引推导出`Pair<int, int>`。

#### 例2：使用单一类型参数的`Pair`类模板

```cpp
template <typename T>
struct Pair
{
    T first{};
    T second{};
};

template <typename T>
Pair(T, T) -> Pair<T>;  // 推导指引

int main()
{
    Pair<int> p1{ 1, 2 }; // 显式声明
    Pair p2{ 1, 2 };      // C++17 中编译通过
    return 0;
}
```

在这个例子中，`Pair`类模板只需要一个类型参数`T`，通过推导指引`Pair(T, T) -> Pair<T>`来告知编译器如何推导类型。

### 默认模板参数

与函数参数一样，模板参数也可以有默认值。在CTAD的上下文中，当没有显式提供模板参数时，编译器会使用默认值。以下示例演示了如何为模板参数提供默认值：

```cpp
template <typename T=int, typename U=int>
struct Pair
{
    T first{};
    U second{};
};

template <typename T, typename U>
Pair(T, U) -> Pair<T, U>;

int main()
{
    Pair<int, int> p1{ 1, 2 }; // 显式指定模板参数
    Pair p2{ 1, 2 };           // 使用 CTAD 推导为 Pair<int, int>
    Pair p3;                   // 使用默认模板参数 Pair<int, int>
    return 0;
}
```

在此示例中，`p3`没有显式指定模板参数，因此编译器使用默认值`int`来推导类型为`Pair<int, int>`。

### CTAD不适用的场景

1. **非静态成员初始化时不支持CTAD**  
   如果类的成员变量在类中有默认初始化值，CTAD将不起作用，必须显式提供模板参数：

   ```cpp
   struct Foo
   {
       std::pair<int, int> p1{ 1, 2 }; // 正常，显式指定模板参数
       std::pair p2{ 1, 2 };           // 错误: CTAD 不在这里生效
   };
   ```

2. **CTAD不适用于函数参数类型**  
   CTAD仅在类模板实例化时生效，不适用于函数参数。以下代码会编译失败：

   ```cpp
   void print(std::pair p) // 错误: CTAD 不在函数参数中生效
   {
       std::cout << p.first << ' ' << p.second << '\n';
   }

   int main()
   {
       std::pair p { 1, 2 }; // CTAD 生效，推导为 std::pair<int, int>
       print(p);  // 错误：无法推导出函数参数类型
       return 0;
   }
   ```

   正确的做法是使用模板来替代CTAD：

   ```cpp
   template <typename T, typename U>
   void print(std::pair<T, U> p)
   {
       std::cout << p.first << ' ' << p.second << '\n';
   }

   int main()
   {
       std::pair p { 1, 2 }; // CTAD 生效，推导为 std::pair<int, int>
       print(p);
       return 0;
   }
   ```

### 小结

类模板参数推导（CTAD）是C++17中的一项强大功能，它能够通过初始化列表自动推导类模板的类型，减少了显式指定模板参数的必要。CTAD在大多数情况下是自动生效的，但对于自定义类模板或复杂类型，可能需要提供推导指引。C++20增强了这项功能，使得编译器能自动为聚合类型生成推导指引，从而进一步简化了代码。