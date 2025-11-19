### 通过常量左值引用传递函数参数

在 C++ 中，常量左值引用是一个非常有用的工具，它允许我们避免昂贵的对象复制，同时保护对象不被修改。常量引用可以绑定到可修改的左值、不可修改的左值以及右值，因此它在传递函数参数时具有很大的灵活性。

#### 1. **常量引用与非常量引用的区别**

与非常量引用（它只能绑定到可修改的左值）不同，常量引用（`const T&`）可以绑定到任何类型的对象，包括：

- **可修改的左值**
- **不可修改的左值**
- **右值**

例如：

```cpp
#include <iostream>

void printValue(const int& y) // y 是常量引用
{
    std::cout << y << '\n';
}

int main()
{
    int x { 5 };
    printValue(x); // ok: x 是可修改的左值

    const int z { 5 };
    printValue(z); // ok: z 是不可修改的左值

    printValue(5); // ok: 5 是右值

    return 0;
}
```

在上述代码中，`printValue()` 函数可以接受三种类型的参数：一个普通左值 `x`，一个常量左值 `z`，以及一个右值 `5`。常量引用的灵活性使得它比非常量引用更适用。

#### 2. **常量引用的优势**

常量引用不仅避免了对象的复制，还能保证在函数内部不会修改原始对象。例如，以下代码会编译失败，因为我们不能修改常量引用绑定的对象：

```cpp
void addOne(const int& ref)
{
    ++ref; // 不允许：ref 是 const
}
```

在很多情况下，我们希望函数能够安全地访问传入的对象，而不更改它的值。常量引用正是提供这种安全性的工具。

#### 3. **常量引用与右值**

常量左值引用的一个显著优势是它能够绑定到右值。没有这个功能，我们就无法通过引用传递字面值（或其他右值）给函数。例如：

```cpp
printValue(5); // 5 是右值
```

这样，常量引用让我们能够避免不必要的复制，并且可以在函数中访问临时对象。

#### 4. **混合使用按值传递与按引用传递**

C++ 允许我们为函数的不同参数指定不同的传递方式。可以根据每个参数的需要，选择按值传递或按引用传递。以下是一个示例：

```cpp
#include <string>

void foo(int a, int& b, const std::string& c)
{
}

int main()
{
    int x { 5 };
    const std::string s { "Hello, world!" };

    foo(5, x, s); // a按值传递，b按引用传递，c按常量引用传递

    return 0;
}
```

在这个示例中，第一个参数 `a` 通过值传递，第二个参数 `b` 通过引用传递，第三个参数 `c` 通过常量引用传递。这样，我们就能够灵活地控制每个参数的传递方式。

#### 5. **最佳实践：何时使用常量引用**

- **基本类型**：基本类型（如 `int`、`char` 等）通常通过值传递，因为它们的复制成本非常低。
- **类类型**：对于类类型（如 `std::string`、`std::vector` 等），常量引用通常是最佳选择，因为复制成本较高，通过常量引用可以避免不必要的复制。

最佳实践：
- **值传递**：适用于基本类型和小型对象。
- **常量引用**：适用于类类型、结构体、`std::string` 等具有较高复制成本的对象。

#### 6. **传递值与传递引用的成本**

当我们考虑传递值与传递引用时，有两个主要因素影响选择：

- **对象的大小**：对于小对象，复制的成本通常较低，而引用的成本与复制相差不大。因此，对于小型对象，我们更倾向于传值。
- **对象的设置成本**：一些类类型的对象在复制时需要进行额外的设置（如动态内存分配）。这些额外的成本意味着我们应该避免复制这些对象，而应该使用引用传递。

对于较大的类类型（如 `std::string` 或 `std::vector`），复制的成本可能很高，因此通过常量引用传递参数通常是更高效的选择。

```cpp
// 判断一个对象是否“廉价复制”
#define isSmall(T) (sizeof(T) <= 2 * sizeof(void*))

struct S
{
    double a;
    double b;
    double c;
};

int main()
{
    std::cout << std::boolalpha; // 打印 true/false
    std::cout << isSmall(int) << '\n'; // true
    std::cout << isSmall(double) << '\n'; // true
    std::cout << isSmall(S) << '\n'; // false

    return 0;
}
```

在这个例子中，`isSmall()` 宏帮助我们判断对象是否足够小，以便在复制时没有显著的性能成本。如果对象很小且没有复杂的构造成本，值传递通常是较好的选择。

#### 7. **使用 `std::string_view` 代替 `const std::string&`**

在现代 C++ 中，`std::string_view` 是一种常用于替代 `const std::string&` 的工具，尤其是在处理字符串参数时。与 `const std::string&` 相比，`std::string_view` 的优点是它的转换成本低，因为它不会复制字符串数据。

```cpp
#include <iostream>
#include <string>
#include <string_view>

void printSV(std::string_view sv)
{
    std::cout << sv << '\n';
}

void printS(const std::string& s)
{
    std::cout << s << '\n';
}

int main()
{
    std::string s{ "Hello, world" };
    std::string_view sv { s };

    // 传递 `std::string_view` 参数
    printSV(s);              // ok: 从 std::string 转换到 std::string_view
    printSV(sv);             // ok: 拷贝 std::string_view
    printSV("Hello, world"); // ok: 从 C 样式字符串转换到 std::string_view

    // 传递 `const std::string&` 参数
    printS(s);              // ok: 代价很小的引用绑定
    printS(sv);             // 编译失败: 不能隐式转换 std::string_view
    printS(static_cast<std::string>(sv)); // bad: 昂贵的创建 std::string 临时对象
    printS("Hello, world"); // bad: 昂贵的创建 std::string 临时对象

    return 0;
}
```

在这个示例中，`std::string_view` 能够有效地处理三种类型的字符串输入，转换成本相对较低。而 `const std::string&` 只能绑定到 `std::string` 类型，并且如果传递其他类型的数据，可能会产生昂贵的转换操作。

### 总结

- **常量引用**（`const T&`）提供了避免复制的优势，同时可以防止修改传入的对象。
- 常量引用可以绑定到可修改的左值、不可修改的左值和右值。
- 对于具有较高复制成本的类类型，通常通过常量引用传递更为高效。
- 对于基本类型，复制成本很低，通常选择按值传递。
- 在处理字符串时，`std::string_view` 通常是一个更高效的选择，比 `const std::string&` 更灵活且开销更小。