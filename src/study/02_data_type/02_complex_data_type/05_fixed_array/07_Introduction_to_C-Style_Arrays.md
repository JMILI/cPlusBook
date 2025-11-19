### C样式数组简介

C样式数组是C语言风格的数组，直接内置于C++中。与其他数组类型（如`std::array`和`std::vector`）不同，C样式数组是C++语言的核心组成部分，因此不需要引用任何头文件来使用它们。

#### 声明C样式数组
C样式数组的声明有独特的语法，使用方括号`[]`来表示数组。可以在方括号内指定数组的长度，这是一个`std::size_t`类型的整数，表示数组中元素的数量。

```cpp
int main() {
    int testScore[30] {};  // 定义名为 testScore 的C样式数组，包含 30 个 int 类型的元素
    return 0;
}
```

- 数组长度必须是常量表达式。
- 如果不指定长度，编译器会根据初始化列表推导出数组的长度。

#### 访问C样式数组
C样式数组可以通过下标访问元素，就像`std::array`一样。

```cpp
int main() {
    int arr[5];  // 定义一个包含 5 个元素的数组

    arr[1] = 7;  // 将数组索引为 1 的元素设置为 7
    std::cout << arr[1];  // 打印 7
    return 0;
}
```

C样式数组没有边界检查，越界访问会导致未定义行为。

#### 聚合初始化
C样式数组支持聚合初始化，可以通过初始化列表直接设置数组的值。

```cpp
int main() {
    int fibonacci[6] = { 0, 1, 1, 2, 3, 5 };  // 初始化数组
    int prime[5] { 2, 3, 5, 7, 11 };           // 推荐的初始化方式
    return 0;
}
```

- 如果初始化列表的元素少于数组的大小，剩余元素将被值初始化（例如，整数类型的默认值是 0）。

#### 使用`constexpr`和`const`定义常量数组
C样式数组可以使用`constexpr`或`const`定义，使其成为常量数组。

```cpp
namespace ProgramData {
    constexpr int squares[5] { 1, 4, 9, 16, 25 };  // constexpr 数组
}

int main() {
    const int prime[5] { 2, 3, 5, 7, 11 };  // const 数组
    // prime[0] = 17; // 编译失败: 不能修改 const 数组
    return 0;
}
```

#### 数组的大小
使用`sizeof`操作符可以获取整个C样式数组的大小（以字节为单位）。

```cpp
int main() {
    const int prime[] { 2, 3, 5, 7, 11 };  // 编译器推导数组长度为 5
    std::cout << sizeof(prime);  // 输出字节数，例如 20（假设 `int` 是 4 字节）
    return 0;
}
```

#### 获取数组长度
C++17引入了`std::size`，可以获取数组的长度（元素数量）。

```cpp
#include <iterator>

int main() {
    const int prime[] { 2, 3, 5, 7, 11 };  // 编译器推导数组长度为 5
    std::cout << std::size(prime) << '\n';  // 输出 5
    return 0;
}
```

在C++17之前，可以使用如下方法来计算数组长度：

```cpp
template <typename T, std::size_t N>
constexpr std::size_t length(const T(&)[N]) noexcept {
    return N;
}
```

#### C样式数组的赋值
C样式数组不支持直接赋值操作。如果需要赋值，可以逐个元素进行赋值，或者使用`std::copy`复制数组内容。

```cpp
#include <algorithm>

int main() {
    int arr[] { 1, 2, 3 };
    int src[] { 5, 6, 7 };

    std::copy(std::begin(src), std::end(src), std::begin(arr));  // 复制 src 到 arr
    return 0;
}
```

### 总结
C样式数组是C++语言的基本数组类型，不支持动态大小、元素赋值等功能。虽然它非常高效，但现代C++倾向于使用`std::array`和`std::vector`，这些容器提供更多的安全性和灵活性。