### `std::array` 简介

`std::array` 是 C++11 引入的一个容器，它提供了比 C 风格数组（例如 `int arr[5];`）更强大的功能，同时保持了与 C 风格数组相似的性能。它是固定大小的数组，因此其大小在编译时必须确定，并且在运行时不可改变。`std::array` 提供了很多类似 `std::vector` 的功能，但其大小是常量，且数组元素的存储是连续的。

#### 容器 vs 数组

在 C++ 中，数组和容器各有其特点：

- **数组**：内存连续，访问速度快，大小固定。
- **容器**：如 `std::vector` 提供灵活的大小，可以在运行时动态调整，虽然牺牲了一部分性能，但提供了更多的功能。

`std::array` 属于固定大小的数组，它与 C 风格数组不同之处在于，它是一个对象，可以使用标准库中的功能和算法，也有更多的类型安全性。

### 为什么选择 `std::array` 而不是动态数组？

虽然 `std::vector` 提供了更高的灵活性，但其性能通常稍差，特别是在内存重分配的情况下。而 `std::array` 则提供了更高效的性能和更严格的类型检查。特别是，`std::array` 能够在编译时提供 `constexpr` 支持，这使得它可以用于常量表达式，能够由编译器进行优化。

### `std::array` 的基本定义

`std::array` 是一个模板类，定义时需要提供两个模板参数：一个是元素类型，另一个是数组大小（必须是常量表达式）。

```cpp
#include <array>

int main() {
    std::array<int, 5> a {};  // 5个int的 std::array
    std::vector<int> b(5);    // 5个int的 std::vector

    return 0;
}
```

在上面的代码中，`std::array<int, 5>` 是一个包含 5 个整数的数组。它与 `std::vector<int>` 类似，但 `std::array` 的大小在编译时就确定，无法动态调整。

### `std::array` 长度必须是常量表达式

与 `std::vector` 不同，`std::array` 的大小必须在编译时就已知，不能在运行时动态改变。例如：

```cpp
constexpr int len = 8;
std::array<int, len> arr {};  // 使用 constexpr 变量
```

在运行时通过变量定义长度会导致编译错误：

```cpp
void foo(const int length) {
    std::array<int, length> e {};  // 错误：length 不是常量表达式
}
```

### 聚合初始化

`std::array` 支持聚合初始化，这意味着你可以通过大括号 `{}` 列表初始化它：

```cpp
std::array<int, 6> fibonacci = {0, 1, 1, 2, 3, 5};
std::array<int, 5> prime = {2, 3, 5, 7, 11};
```

如果没有提供初始化器，元素将被默认初始化：

```cpp
std::array<int, 5> a;   // 默认初始化，元素值未定义
std::array<int, 5> b{}; // 值初始化，元素被设置为0
```

### `std::array` 与 `constexpr`

`std::array` 支持 `constexpr`，这使得它可以在编译时进行计算，并且能够由编译器进行更多的优化。例如：

```cpp
constexpr std::array<int, 5> prime = {2, 3, 5, 7, 11};
```

使用 `constexpr` 时，`std::array` 的所有元素必须是常量表达式。

### C++17 的类模板参数推导（CTAD）

C++17 引入了类模板参数推导（CTAD），允许编译器根据初始化列表自动推导 `std::array` 的类型和大小：

```cpp
constexpr std::array a1 { 9, 7, 5, 3, 1 }; // 类型推导为 std::array<int, 5>
constexpr std::array a2 { 9.7, 7.31 };     // 类型推导为 std::array<double, 2>
```

这种写法非常简洁，优先使用此语法可以让代码更简洁易读。如果编译器不支持 C++17，则需要显式提供类型和大小模板参数。

### `std::to_array`（C++20）

C++20 引入了 `std::to_array`，它使得你可以省略 `std::array` 的数组长度，从而简化代码：

```cpp
constexpr auto myArray1 { std::to_array<int, 5>({ 9, 7, 5, 3, 1 }) };
constexpr auto myArray2 { std::to_array<int>({ 9, 7, 5, 3, 1 }) };    // 省略大小
constexpr auto myArray3 { std::to_array({ 9, 7, 5, 3, 1 }) };         // 省略类型和大小
```

`std::to_array` 使得代码更简洁，但需要注意它比直接创建 `std::array` 的代价更高，因为涉及到临时对象的创建。因此，`std::to_array` 应该在无法从初始化列表推断类型或大小时使用，并且避免在性能敏感的代码中频繁使用。

### 访问元素

访问 `std::array` 的元素最常用的方法是使用下标操作符（`[]`）：

```cpp
std::array<int, 5> prime { 2, 3, 5, 7, 11 };
std::cout << prime[3];  // 输出 7
```

需要注意的是，`[]` 运算符不进行边界检查，如果访问越界的元素，会导致未定义行为。因此，在使用时要确保索引有效。

### 最佳实践

- **使用 `constexpr` 时优先选择 `std::array`**，因为它可以被编译器优化。
- **对于非常量表达式数组，使用 `std::vector`**，因为它提供了动态大小。
- **尽可能使用 C++17 的类模板参数推导（CTAD）**，使代码更加简洁。
- **尽量避免使用 `std::to_array` 在性能敏感的场合**，因为它涉及到临时对象的创建。

### 总结

`std::array` 是一个固定大小的数组容器，适用于编译时已知大小且需要高效性能的场合。它结合了传统数组的优点和 `std::vector` 的一些便利功能，但限制了大小的动态调整。了解其优势和使用场景，可以帮助你在代码中更好地选择合适的容器。