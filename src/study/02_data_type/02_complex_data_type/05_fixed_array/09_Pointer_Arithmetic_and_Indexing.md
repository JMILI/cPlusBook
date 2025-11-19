### 指针运算与下标

在学习指针与数组时，指针运算与数组下标运算是密切相关的。通过理解指针运算和数组下标的关系，我们能够深入理解内存中的数据结构和访问方式。指针运算允许直接访问内存，并通过指针偏移来遍历数组或其它数据结构。以下内容将详细介绍指针运算和下标如何工作，并展示其在实际代码中的应用。

---

### 1. **什么是指针运算？**

指针运算是指对指针变量执行加法、减法等整数算术操作，从而得到新的内存地址。指针加法或减法不是直接的整数运算，而是基于指针所指向的类型大小来进行的偏移。

#### 示例：

假设我们有一个 `int` 类型的指针 `ptr`，指向一个整数变量。每执行一次 `ptr + 1` 操作，指针将增加 `sizeof(int)` 字节。在大多数平台上，`int` 占 4 字节，因此 `ptr + 1` 会使指针增加 4 个字节。

```cpp
#include <iostream>

int main()
{
    int x{};
    const int* ptr{ &x }; // 假设 int 是 4 字节
    std::cout << ptr << ' ' << (ptr + 1) << ' ' << (ptr + 2) << '\n';

    return 0;
}
```

**输出示例：**

```
00AFFD80 00AFFD84 00AFFD88
```

解释：每个内存地址比前一个地址大 4 个字节。

### 2. **指针运算中的减法**

与加法类似，指针运算也支持减法。当我们对指针执行 `ptr - 1` 操作时，它返回的是指向前一个对象的地址。

```cpp
#include <iostream>

int main()
{
    int x{};
    const int* ptr{ &x }; // 假设 int 是 4 字节

    std::cout << ptr << ' ' << (ptr - 1) << ' ' << (ptr - 2) << '\n';

    return 0;
}
```

**输出示例：**

```
00AFFD80 00AFFD7C 00AFFD78
```

解释：每个内存地址比前一个地址少 4 个字节。

### 3. **自增与自减操作符的作用**

自增（`++`）与自减（`--`）操作符不仅适用于普通整数，也可以应用于指针。当我们对指针执行自增操作时，它会使指针偏移到下一个对象的位置。

```cpp
#include <iostream>

int main()
{
    int x{};
    const int* ptr{ &x }; // 假设 int 是 4 字节

    std::cout << ptr << '\n';

    ++ptr; // ptr = ptr + 1
    std::cout << ptr << '\n';

    --ptr; // ptr = ptr - 1
    std::cout << ptr << '\n';

    return 0;
}
```

**输出示例：**

```
00AFFD80 00AFFD84 00AFFD80
```

### 4. **指针运算与数组访问**

C++中的数组退化为指向数组首元素的指针，因此，我们可以通过指针运算来访问数组的各个元素。数组下标访问（`arr[n]`）实际上是对指针运算的封装，等价于 `*(arr + n)`。

```cpp
#include <iostream>

int main()
{
    const int arr[] { 9, 7, 5, 3, 1 };
    
    const int* ptr{ arr }; // 保存数组初始元素的地址
    std::cout << ptr[2];   // 访问第 2 个元素, 打印 5

    return 0;
}
```

这里，`ptr[2]` 等价于 `*(ptr + 2)`，即通过指针偏移访问数组的第 2 个元素。

### 5. **指针与下标是相对地址**

指针运算与数组下标的关系可以通过相对地址来理解。数组下标并不代表数组中的固定元素，而是相对于数组首地址的偏移量。

例如：

```cpp
#include <array>
#include <iostream>

int main()
{
    const int arr[] { 9, 8, 7, 6, 5 };
    const int *ptr { arr }; // 退化为指向第 0 个元素的指针

    std::cout << *ptr << ptr[0] << '\n'; // 打印 99
    std::cout << *(ptr+1) << ptr[1] << '\n'; // 打印 88

    ptr = &arr[3]; // 指向第 3 个元素

    std::cout << *ptr << ptr[0] << '\n'; // 打印 66
    std::cout << *(ptr+1) << ptr[1] << '\n'; // 打印 55
}
```

**输出：**

```
9 9
8 8
6 6
5 5
```

在这个例子中，指针 `ptr` 被重新指向数组中的不同位置，表明指针和数组下标是相对的。

### 6. **负指针**

在C++中，我们可以通过负下标来访问数组的前一个元素。例如：

```cpp
#include <array>
#include <iostream>

int main()
{
    const int arr[] { 9, 8, 7, 6, 5 };
    const int* ptr { &arr[3] };

    std::cout << *ptr << ptr[0] << '\n'; // 打印 66
    std::cout << *(ptr-1) << ptr[-1] << '\n'; // 打印 77
}
```

**输出：**

```
66 66
77 77
```

这里，`ptr[-1]` 是合法的，等同于 `*(ptr - 1)`，指向数组中的前一个元素。

### 7. **使用指针算法遍历数组**

指针运算最常见的用途之一是遍历数组。例如，我们可以不使用显式的数组下标，通过指针算法来遍历数组。

```cpp
#include <iostream>

int main()
{
    constexpr int arr[]{ 9, 7, 5, 3, 1 };

    const int* begin{ arr };                // 从开头
    const int* end{ arr + std::size(arr) }; // 到结束

    for (; begin != end; ++begin)           // 进行遍历（排除end）
    {
        std::cout << *begin << ' ';     // 打印每个元素
    }

    return 0;
}
```

**输出：**

```
9 7 5 3 1
```

这里，我们通过 `begin` 指针遍历数组，直到指针等于 `end`。每次循环，`begin` 都会自增，指向下一个数组元素。

### 8. **基于范围的for循环与指针运算**

基于范围的 `for` 循环背后其实也使用了指针运算。例如，以下代码：

```cpp
#include <iostream>

int main()
{
    constexpr int arr[]{ 9, 7, 5, 3, 1 };

    for (auto e : arr)         // 从 `begin` 遍历到 `end`
    {
        std::cout << e << ' '; // 打印每个元素
    }

    return 0;
}
```

实际上，编译器将其转换为类似于指针算法的实现，如下所示：

```cpp
#include <iostream>

int main()
{
    constexpr int arr[]{ 9, 7, 5, 3, 1 };

    auto __begin = arr;                // arr 是 begin-expr
    auto __end = arr + std::size(arr); // arr + std::size(arr) 是 end-expr

    for ( ; __begin != __end; ++__begin)
    {
        auto e = *__begin;         // e 是 range-declaration
        std::cout << e << ' ';     // 这里是 loop-statement
    }

    return 0;
}
```

---

### 总结

指针运算和下标是基于内存相对地址的操作。通过指针运算，我们可以高效地遍历数组，甚至实现一些高效的算法。而数组下标则是一种简化的访问方式，实际上是指针运算的封装。理解这两者的关系，有助于我们深入掌握内存管理和数据访问的效率。