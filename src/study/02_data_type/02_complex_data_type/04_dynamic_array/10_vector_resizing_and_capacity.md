# 详细介绍 `std::vector` 的大小调整与容量

在现代C++编程中，`std::vector` 是一个非常重要的容器。它不仅支持动态大小调整，还能通过合理的内存管理来优化性能。本章节将详细讲解 `std::vector` 的大小调整、容量管理及相关概念，帮助你更好地理解并使用 `std::vector`。

## 1. 固定大小数组与动态大小数组
### 固定大小数组
固定大小数组的长度在数组创建时就已经确定，并且一旦确定，就无法改变。例如：

```cpp
int arr[5];  // 固定大小数组，大小为5
```

在这种情况下，数组的大小是编译时就确定的，无法在运行时修改。

### 动态大小数组（`std::vector`）
与固定大小数组不同，`std::vector` 是一个动态数组，它的大小可以在运行时改变。使用 `std::vector` 时，你可以在程序运行过程中根据需求添加或删除元素。

```cpp
std::vector<int> vec;  // 空的动态数组
```

## 2. 调整 `std::vector` 的大小

### 2.1 `resize()` 函数
`std::vector` 允许我们通过 `resize()` 函数调整其大小。这个函数可以改变 `std::vector` 的长度（即正在使用的元素数量），并且会根据调整的大小进行相应的处理。

#### 示例 1: 增加 `std::vector` 的大小

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{ 0, 1, 2 };  // 初始长度为3
    std::cout << "Length: " << v.size() << '\n';  // 输出 3

    v.resize(5);  // 调整为5个元素
    std::cout << "Length: " << v.size() << '\n';  // 输出 5

    for (auto i : v)
        std::cout << i << ' ';  // 输出 0 1 2 0 0
    std::cout << '\n';

    return 0;
}
```

输出：
```
Length: 3
Length: 5
0 1 2 0 0
```

**注意：**
1. 调整大小时，现有元素的值会被保留。
2. 新增元素会被初始化为默认值（对于 `int` 类型是 0）。

#### 示例 2: 减小 `std::vector` 的大小

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{ 0, 1, 2, 3, 4 };  // 初始长度为5
    std::cout << "Length: " << v.size() << '\n';  // 输出 5

    v.resize(3);  // 调整为3个元素
    std::cout << "Length: " << v.size() << '\n';  // 输出 3

    for (int i : v)
        std::cout << i << ' ';  // 输出 0 1 2
    std::cout << '\n';

    return 0;
}
```

输出：
```
Length: 5
Length: 3
0 1 2
```

**注意：**
1. 调整大小时，`std::vector` 会丢弃多余的元素。

## 3. 长度与容量的区别

### 3.1 容量
`std::vector` 的容量表示为存储多少个元素分配了内存，而长度表示当前 `std::vector` 中实际使用的元素数量。容量可以大于或等于长度，表示 `std::vector` 可能会预留一些空间，以便未来添加更多元素而不需要重新分配。

#### 示例：查看容量与长度

```cpp
#include <iostream>
#include <vector>

void printCapLen(const std::vector<int>& v) {
    std::cout << "Capacity: " << v.capacity() << " Length: " << v.size() << '\n';
}

int main() {
    std::vector<int> v{ 0, 1, 2 };  // 长度为 3
    printCapLen(v);  // 输出 Capacity: 3 Length: 3

    v.resize(5);  // 调整为 5 个元素
    printCapLen(v);  // 输出 Capacity: 5 Length: 5

    return 0;
}
```

输出：
```
Capacity: 3 Length: 3
Capacity: 5 Length: 5
```

### 3.2 为什么有容量和长度的区别？
- **长度**（`size()`）：当前 `std::vector` 中的有效元素数量。
- **容量**（`capacity()`）：`std::vector` 为存储元素分配的内存空间。

`std::vector` 可以在不改变长度的情况下，通过扩展其容量来预留更多空间，从而避免频繁的内存重新分配。这种设计能够提高性能，尤其是在频繁添加元素时。

### 3.3 通过 `capacity()` 获取容量
你可以通过 `capacity()` 成员函数查看 `std::vector` 的容量：

```cpp
std::cout << "Capacity: " << v.capacity() << std::endl;
```

## 4. 内存重新分配与性能

### 4.1 重新分配过程
当 `std::vector` 调整其容量时，内存会重新分配。具体步骤如下：
1. 分配新的内存，大小为所需容量。
2. 将原有元素复制（或移动）到新的内存位置。
3. 释放旧内存。
4. 更新 `std::vector` 的容量和长度。

**注意：** 内存重新分配是一个昂贵的过程，尤其是当 `std::vector` 很大时。频繁的重新分配会影响程序的性能。

### 4.2 避免不必要的重新分配
为了避免重新分配带来的性能开销，可以在知道 `std::vector` 需要多少元素时，使用 `reserve()` 函数预分配内存。例如：

```cpp
std::vector<int> v;
v.reserve(1000);  // 提前分配1000个元素的内存
```

这样，`std::vector` 就不会在添加元素时进行多次内存分配。

## 5. `std::vector` 的索引
`std::vector` 的索引是基于其 **长度** 而不是 **容量** 的。这意味着，只有在索引范围在 `0` 到 `size() - 1` 之间时，访问元素才是合法的。

```cpp
std::vector<int> v{ 0, 1, 2 };
std::cout << v[0] << std::endl;  // 正常访问
std::cout << v[3] << std::endl;  // 越界访问，未定义行为
```

**警告：** 使用 `at()` 成员函数可以避免越界访问，它会在访问越界时抛出异常。

## 6. 收缩 `std::vector` 的容量
如果一个 `std::vector` 的大小大幅减少，你可能希望释放多余的内存。为此，`std::vector` 提供了 `shrink_to_fit()` 成员函数来请求收缩其容量，使其匹配当前的长度。

```cpp
std::vector<int> v(1000);
v.resize(0);  // 清空所有元素
v.shrink_to_fit();  // 请求收缩容量
```

**注意：** `shrink_to_fit()` 是一个建议，编译器可以选择是否实施此请求。

## 总结
1. **`resize()`** 可以调整 `std::vector` 的大小。
2. **长度** 和 **容量** 是不同的概念，长度表示当前使用的元素数量，容量表示为存储元素分配的内存空间。
3. 避免频繁的内存 **重新分配**，可以使用 `reserve()` 提前分配内存。
4. **索引** 基于 `std::vector` 的长度，而非容量。
5. 使用 `shrink_to_fit()` 来请求回收不再使用的内存。

通过理解这些概念，可以更高效地使用 `std::vector` 并提高程序性能。