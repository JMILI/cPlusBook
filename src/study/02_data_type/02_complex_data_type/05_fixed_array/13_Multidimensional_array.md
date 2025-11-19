### 多维 `std::array` 详解

在 C++ 中，处理多维数组的方式有很多。传统的 C 风格数组虽然简单，但由于没有类型安全和灵活性，通常不推荐使用。现代 C++ 提供了 `std::array`，它是一个固定大小的容器，可以在编译时确定大小，并且提供了许多额外的功能。下面我们详细探讨如何使用 `std::array` 来处理多维数组，并介绍一些与之相关的知识点。

### 1. C 风格的多维数组

首先，让我们回顾一下 C 风格的多维数组：

```cpp
int arr[3][4] {
    { 1, 2, 3, 4 },
    { 5, 6, 7, 8 },
    { 9, 10, 11, 12 }
};
```

这是一个 3x4 的二维数组，包含 3 行和 4 列。虽然 C 风格数组使用起来简单，但它们存在很多问题，例如数组的大小无法在运行时修改，且没有提供边界检查。

### 2. `std::array` 和多维数组

`std::array` 本质上是一个封装了固定大小数组的模板类，其语法相对简单且类型安全。然而，`std::array` 本身是一个一维数组，所以要创建一个多维数组，我们需要嵌套 `std::array`。

#### 2.1 创建二维 `std::array`

创建一个 3 行 4 列的二维数组，标准的写法如下：

```cpp
std::array<std::array<int, 4>, 3> arr {{
    { 1, 2, 3, 4 },
    { 5, 6, 7, 8 },
    { 9, 10, 11, 12 }
}};
```

- 这里，`std::array<std::array<int, 4>, 3>` 表示一个包含 3 行，每行 4 列的二维数组。
- 使用双大括号初始化数组。
- 如果我们需要访问元素，可以通过 `arr[row][col]` 语法，如 `arr[1][2]`。

#### 2.2 通过函数传递二维 `std::array`

我们可以像传递一维 `std::array` 一样，将二维 `std::array` 作为参数传递给函数：

```cpp
#include <array>
#include <iostream>

template <typename T, std::size_t Row, std::size_t Col>
void printArray(const std::array<std::array<T, Col>, Row> &arr) {
    for (const auto& row : arr) {
        for (const auto& e : row) {
            std::cout << e << ' ';
        }
        std::cout << '\n';
    }
}

int main() {
    std::array<std::array<int, 4>, 3> arr {{
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 },
        { 9, 10, 11, 12 }
    }};
    printArray(arr);
}
```

#### 2.3 使用别名模板简化代码

创建二维 `std::array` 时，模板参数比较冗长，为了使代码更简洁，可以使用类型别名：

```cpp
template <typename T, std::size_t Row, std::size_t Col>
using Array2d = std::array<std::array<T, Col>, Row>;
```

使用这个别名模板后，定义和使用二维数组就变得非常简洁：

```cpp
Array2d<int, 3, 4> arr {{
    { 1, 2, 3, 4 },
    { 5, 6, 7, 8 },
    { 9, 10, 11, 12 }
}};
```

#### 2.4 高维数组的别名模板

`std::array` 的嵌套可以扩展到更高维度，例如三维数组：

```cpp
template <typename T, std::size_t Row, std::size_t Col, std::size_t Depth>
using Array3d = std::array<std::array<std::array<T, Depth>, Col>, Row>;
```

### 3. 获取多维数组的维度长度

对于一维数组，我们可以使用 `size()` 来获取数组的大小，但对于二维数组，我们需要使用更细致的方法。可以使用模板函数来获取每个维度的长度：

```cpp
template <typename T, std::size_t Row, std::size_t Col>
constexpr int rowLength(const Array2d<T, Row, Col>&) {
    return Row;
}

template <typename T, std::size_t Row, std::size_t Col>
constexpr int colLength(const Array2d<T, Row, Col>&) {
    return Col;
}
```

这种方法避免了 `arr[0].size()` 的潜在问题，且更加安全。

### 4. 展平二维数组

为了简化多维数组的操作，可以将多维数组展平为一维数组，并提供一个接口来模拟多维数组。我们可以通过计算每个元素在一维数组中的位置来实现这一点。

```cpp
template <typename T, std::size_t Row, std::size_t Col>
using ArrayFlat2d = std::array<T, Row * Col>;

template <typename T, std::size_t Row, std::size_t Col>
class ArrayView2d {
    std::reference_wrapper<ArrayFlat2d<T, Row, Col>> m_arr;

public:
    ArrayView2d(ArrayFlat2d<T, Row, Col>& arr) : m_arr{arr} {}

    T& operator()(int row, int col) {
        return m_arr.get()[row * Col + col];
    }

    int rows() const { return Row; }
    int cols() const { return Col; }
};
```

这样，我们可以通过二维索引访问一维数组的数据。

### 5. `std::mdspan`（C++23）

在 C++23 中，引入了 `std::mdspan`，它提供了一个多维数组的视图。与我们自定义的 `ArrayView2d` 类似，`std::mdspan` 可以用来处理多维数组的数据，并提供高效的访问。

```cpp
#include <array>
#include <iostream>
#include <mdspan>

template <typename T, std::size_t Row, std::size_t Col>
using ArrayFlat2d = std::array<T, Row * Col>;

int main() {
    ArrayFlat2d<int, 3, 4> arr { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    std::mdspan mdView{ arr.data(), 3, 4 };

    std::cout << "Rows: " << mdView.extents().extent(0) << '\n';
    std::cout << "Cols: " << mdView.extents().extent(1) << '\n';

    for (std::size_t row = 0; row < 3; ++row) {
        for (std::size_t col = 0; col < 4; ++col) {
            std::cout << mdView[row, col] << ' ';
        }
        std::cout << '\n';
    }
}
```

### 6. 总结

- **多维 `std::array`**: 通过嵌套 `std::array` 来实现多维数组，但这种方式在维度较高时，代码冗长且不易理解。
- **类型别名模板**: 使用类型别名模板简化多维数组的定义和操作。
- **展平数组**: 将多维数组展平为一维数组，并使用接口模拟多维数组的访问。
- **C++23 `std::mdspan`**: 引入了 `std::mdspan`，提供了一个简洁且高效的方式来处理多维数组视图。

通过以上方法，我们可以更方便、更安全地使用 `std::array` 处理多维数组，避免了传统 C 风格数组的局限性。