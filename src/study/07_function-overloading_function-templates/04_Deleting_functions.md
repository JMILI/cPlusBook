### 函数的 `delete` 说明符概述
在 C++ 中，有时我们编写的函数不希望被某些类型的参数调用。为了实现这个目的，C++ 提供了 `=delete` 说明符，它可以将函数定义为删除的，当编译器将函数调用与已删除的函数相匹配时，会导致编译错误。

### 示例分析

#### 未使用 `=delete` 说明符的情况
```cpp
#include <iostream>

void printInt(int x)
{
    std::cout << x << '\n';
}

int main()
{
    printInt(5);    // okay: 打印 5
    printInt('a');  // 打印 97 -- 这样的输出是否有意义?
    printInt(true); // 打印 1 -- 这样的输出是否有意义?
    
    return 0;
}
```
在这个例子中，`printInt` 函数原本是用于打印整数的。但是当传入 `char` 类型的 `'a'` 和 `bool` 类型的 `true` 时，编译器会将它们分别提升为 `int` 类型（`'a'` 提升为 97，`true` 提升为 1）并调用 `printInt` 函数，这可能不是我们想要的结果。

#### 使用 `=delete` 说明符的情况
```cpp
#include <iostream>

void printInt(int x)
{
    std::cout << x << '\n';
}

void printInt(char) = delete; // 编译时匹配到这个函数将报错
void printInt(bool) = delete; // 编译时匹配到这个函数将报错

int main()
{
    printInt(97);   // okay

    printInt('a');  // 编译失败: 匹配到的函数被删除
    printInt(true); // 编译失败: 匹配到的函数被删除

    printInt(5.0);  // 编译失败: 有多个可匹配的函数
    
    return 0;
}
```
- **`printInt('a')`**：`'a'` 与 `printInt(char)` 精确匹配，而 `printInt(char)` 已被删除，所以编译器产生编译错误。
- **`printInt(true)`**：`true` 与 `printInt(bool)` 精确匹配，该函数也被删除，同样会产生编译错误。
- **`printInt(5.0)`**：编译器先检查是否有 `printInt(double)`，未找到。然后尝试查找最佳匹配，虽然 `printInt(int)` 是唯一未删除的函数，但已删除的函数仍然参与函数重载决议。由于没有明确的最佳匹配，编译器发出不明确匹配的编译错误。

### 关键点总结
- **`=delete` 的含义**：`=delete` 表示“我禁止这样做”，而不是“这不存在”。即使函数被删除，它在函数重载决议过程中仍然会被考虑。
- **删除函数参与重载决议**：删除的函数参与函数重载决议的所有阶段（不仅仅是精确匹配阶段）。如果匹配到已删除的函数，则会导致编译错误。

### 删除所有不匹配的重载
如果要禁止除特定类型外的其他类型调用函数，逐个删除重载函数可能会很冗长。可以使用函数模板来实现这一点。

```cpp
#include <iostream>

// 这个函数优先使用int参数来匹配
void printInt(int x)
{
    std::cout << x << '\n';
}

// 这个函数模版将会匹配其它所有类型
// 因为这个函数模版被删除，因此匹配到它，将会导致编译失败
template <typename T>
void printInt(T x) = delete;

int main()
{
    printInt(97);   // okay
    printInt('a');  // 编译报错
    printInt(true); // 编译报错
    
    return 0;
}
```
在这个例子中，`printInt(int)` 是普通函数，用于处理 `int` 类型的参数。而函数模板 `template <typename T> void printInt(T x) = delete;` 会匹配除 `int` 以外的所有类型，并且该模板函数被删除。所以当传入 `char` 或 `bool` 类型的参数时，会匹配到被删除的函数模板，从而导致编译错误。

### 总结
`=delete` 说明符为我们提供了一种有效的方式来控制函数的调用，避免不期望的参数类型调用函数。通过合理使用它和函数模板，可以更灵活地管理函数的重载和调用规则。 