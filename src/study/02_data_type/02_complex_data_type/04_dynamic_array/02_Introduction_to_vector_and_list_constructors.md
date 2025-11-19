### **std::vector 和 列表构造函数简介**

在C++中，`std::vector` 是一个标准库容器类，用于实现动态数组，属于STL（标准模板库）的重要组成部分。与固定大小的数组不同，`std::vector` 可以根据需要动态调整其大小，是解决数组大小不可预见性问题的一个有效方案。本节将详细介绍`std::vector`的构造、初始化方式、访问元素的方法，以及相关的知识点。

---

### **1. std::vector简介**

`std::vector` 是一个动态数组容器，它的大小可以根据需要进行扩展。它在 `<vector>` 头文件中定义，且是一个类模板。`std::vector` 采用模板类型参数来指定容器中元素的类型。例如，`std::vector<int>` 表示一个存储整数的动态数组。

#### **基本实例化：**

```cpp
#include <vector>

int main()
{
    // 值初始化 (使用默认构造函数)
    std::vector<int> empty{}; // 含有0个int的vector
    return 0;
}
```

在上述示例中，`empty` 是一个空的 `std::vector<int>` 对象，且使用了默认构造函数进行初始化。这样创建的 `std::vector` 是空的，长度为0。

---

### **2. 使用列表初始化std::vector**

为了简化初始化过程，`std::vector` 提供了列表初始化的方式。使用 `{}` 语法，可以直接初始化容器中的元素。

```cpp
#include <vector>

int main()
{
    // 列表初始化
    std::vector<int> primes{2, 3, 5, 7};          // vector 包含 4 个 int：2, 3, 5, 7
    std::vector vowels {'a', 'e', 'i', 'o', 'u'}; // vector 包含 5 个 char：'a', 'e', 'i', 'o', 'u'
    
    return 0;
}
```

- `primes` 使用显式的类型 `std::vector<int>` 初始化，包含4个整数元素。
- `vowels` 使用C++17中的**类模板参数推导（CTAD）**自动推导类型为 `char`，包含5个字符元素。

#### **列表构造函数工作原理：**
列表构造函数做了以下几件事：
1. 确保容器有足够的空间存储所有初始化值。
2. 设置容器的长度为初始化列表中的元素数量。
3. 将元素按顺序初始化为列表中的值。

在实际使用中，列表构造函数提供了一个简洁的方式来初始化容器，避免手动指定元素数量和类型。

---

### **3. 使用下标运算符（operator[]）访问元素**

`std::vector` 支持通过下标运算符 `[]` 来访问元素，就像普通数组一样。下标从 `0` 开始，即第一个元素的索引为 `0`，第二个为 `1`，依此类推。

```cpp
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> primes { 2, 3, 5, 7, 11 }; // 有5个素数

    std::cout << "The first prime number is: " << primes[0] << '\n';
    std::cout << "The second prime number is: " << primes[1] << '\n';
    std::cout << "The sum of the first 5 primes is: " << primes[0] + primes[1] + primes[2] + primes[3] + primes[4] << '\n';

    return 0;
}
```

输出：

```
The first prime number is: 2
The second prime number is: 3
The sum of the first 5 primes is: 28
```

通过使用下标运算符，能够轻松访问 `std::vector` 中的元素，避免了使用多个单独的变量。

#### **下标越界：**
访问元素时，索引必须在有效范围内，即对于长度为 `N` 的数组，下标范围是 `0` 到 `N-1`。若使用无效索引（例如：`primes[N]`），会导致未定义行为。

```cpp
std::vector<int> primes{ 2, 3, 5, 7 };
std::cout << primes[4]; // 越界访问，未定义行为
```

C++标准库中的 `operator[]` 不进行越界检查，因此开发者需要特别注意索引范围。

---

### **4. std::vector在内存中的连续性**

`std::vector` 的元素在内存中是连续分配的，这意味着它们在内存地址上是相邻的。因此，元素之间没有空隙，这使得 `std::vector` 对于随机访问操作非常高效。

```cpp
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> primes{ 2, 3, 5, 7, 11 };
    
    std::cout << "An int is " << sizeof(int) << " bytes\n";
    std::cout << &(primes[0]) << '\n';
    std::cout << &(primes[1]) << '\n';
    std::cout << &(primes[2]) << '\n';

    return 0;
}
```

输出结果可能如下：

```
An int is 4 bytes
00DBF720
00DBF724
00DBF728
```

在此示例中，我们观察到元素在内存中的地址是连续的。每个整数元素占用4个字节，因此它们的内存地址相差4个字节。

---

### **5. 构造特定长度的std::vector**

在实际应用中，可能希望创建一个具有固定大小且所有元素初始化为某个默认值的 `std::vector`。`std::vector` 提供了通过单个整数参数初始化的构造函数，用于指定容器的大小。

```cpp
std::vector<int> data(10); // vector含有10个int，默认值为0
```

这种初始化方法为 `std::vector` 创建了一个包含10个元素的容器，每个元素都初始化为默认值 `0`。这种方式比使用手动列出每个元素的初始化列表更加简洁。

---

### **6. 列表初始化和构造函数优先级**

当使用列表初始化时，`std::vector` 会优先选择匹配的列表构造函数。例如，下面的代码：

```cpp
std::vector<int> v{ 10 }; // 列表初始化
```

这里的 `{ 10 }` 会被解析为**初始值设定项列表**，并匹配到 `std::vector<T>` 的列表构造函数，构造一个包含一个元素且该元素值为 `10` 的 `std::vector<int>`。

如果你使用直接初始化，如：

```cpp
std::vector<int> v(10);  // 10是长度，初始化为0
```

则会调用具有单一参数的构造函数，即构造一个长度为10、所有元素初始化为0的 `std::vector<int>`。

---

### **7. const和constexpr std::vector**

`std::vector` 对象可以声明为 `const`，使得容器及其元素不可修改：

```cpp
const std::vector<int> primes{ 2, 3, 5, 7, 11 };
```

但是需要注意的是，`std::vector` 本身不能是 `constexpr`，因为它的大小是动态变化的。对于需要编译时常量数组，可以使用 `std::array`。

---

### **8. 为什么它被称为 vector？**

尽管“vector”在数学中通常指的是有大小和方向的几何对象，但C++中的 `std::vector` 实际上只是一个动态数组。其名称的由来与早期编程语言（如Scheme和Common Lisp）中的命名有关。原本应被称为“动态数组”，但由于历史原因，名称延续至今。

---

### **总结与最佳实践**

- **列表构造函数**：用于通过初始值设定项列表初始化 `std::vector`。
- **随机访问**：`std::vector` 提供随机访问特性，元素按顺序存储在内存中，因此支持高效的随机访问。
- **初始化方式**：推荐使用列表初始化，尤其在元素已知的情况下。避免使用不明确的构造函数匹配。
- **边界检查**：`std::vector` 不会进行下标越界检查，使用时要注意索引范围。

`std::vector` 是C++中非常重要的容器之一，理解其初始化和构造函数的使用对于高效编程至关重要。