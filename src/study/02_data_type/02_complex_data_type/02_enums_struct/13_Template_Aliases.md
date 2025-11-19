### 模板别名

模板别名是C++中一个非常有用的特性，允许为模板类型或模板本身创建别名，使代码更加简洁且易于理解。模板别名（Alias Templates）与普通类型别名（如`typedef`或`using`）的概念相似，但它们作用于模板，而不是普通类型。

#### 1. **为类模板创建别名**

与使用`typedef`或`using`创建现有类型别名类似，我们也可以使用`using`为类模板创建别名。模板别名可以简化代码，尤其是在频繁使用某些类型或模板时。下面是一个常见的例子：

```cpp
#include <iostream>

template <typename T>
struct Pair {
    T first{};
    T second{};
};

template <typename T>
void print(const Pair<T>& p) {
    std::cout << p.first << ' ' << p.second << '\n';
}

int main() {
    using Point = Pair<int>; // 创建类型别名
    Point p { 1, 2 };        // 编译器会把 Point 替换为 Pair<int>
    
    print(p);
    return 0;
}
```

在这个例子中，我们使用`using`为`Pair<int>`创建了一个别名`Point`。在`main`函数中，`Point`被用作`Pair<int>`的简写。

- **注意**：这个别名是局部的，它仅在`main`函数内有效。
- **模板别名的作用**：通过模板别名，可以简化模板的使用，尤其是在复杂模板类型名的情况下。

#### 2. **模板别名（Alias Template）**

模板别名不仅可以为具体类型创建别名，也可以为模板本身创建别名。在这种情况下，模板别名提供了一个替代名称，用来实例化相同的模板类型。

```cpp
#include <iostream>

template <typename T>
struct Pair {
    T first{};
    T second{};
};

// 模板别名，定义 Coord<T> 为 Pair<T> 的别名
template <typename T>
using Coord = Pair<T>; // Coord 是 Pair<T> 的别名

template <typename T>
void print(const Coord<T>& c) {
    std::cout << c.first << ' ' << c.second << '\n';
}

int main() {
    Coord<int> p1 { 1, 2 }; // Coord<int> 代替了 Pair<int>
    Coord p2 { 1, 2 };      // C++20之后: 可以使用CTAD推导模板类型

    std::cout << p1.first << ' ' << p1.second << '\n';
    print(p2);

    return 0;
}
```

在这个例子中，我们使用`using Coord = Pair<T>`为`Pair<T>`创建了一个别名`Coord<T>`。`Coord<T>`是`Pair<T>`的简化名称。这样，所有使用`Pair<T>`的地方都可以用`Coord<T>`代替。

- **全局作用域限制**：与普通类型别名不同，模板别名必须在**全局作用域**中定义。
- **C++20及以后的改进**：从C++20开始，模板别名支持与类模板参数推导（CTAD）一起使用，允许编译器从初始化列表推导模板类型，避免显式指定模板类型。

#### 3. **关于模板别名的几点注意事项**

##### a) **模板别名的作用范围**

模板别名与类型别名（`using`或`typedef`）不同，后者可以在局部作用域内定义，而模板别名必须在**全局作用域**中定义。

```cpp
template <typename T>
using Coord = Pair<T>; // 必须在全局作用域中定义
```

##### b) **CTAD 与模板别名**

在C++20之前，当使用模板别名实例化对象时，必须显式指定模板类型参数。然而，从C++20开始，可以使用类模板参数推导（CTAD）来自动推导模板类型参数。这使得使用模板别名时更加方便。例如：

```cpp
Coord p2 { 1, 2 }; // C++20及以后，可以通过CTAD推导为 Coord<int>
```

##### c) **模板别名与函数参数**

CTAD并不适用于函数参数的类型推导。换句话说，当模板别名作为函数参数时，必须显式指定模板类型参数。例如：

```cpp
template <typename T>
void print(const Coord<T>& c) {
    std::cout << c.first << ' ' << c.second << '\n';
}

// 无法编译
void print(const Coord& c) { // 错误: 缺少模板类型参数
    std::cout << c.first << ' ' << c.second << '\n';
}
```

这是因为CTAD只在类模板实例化时生效，而在函数参数中使用时，模板类型参数无法推导。

#### 4. **总结与表格对比**

| **特性**                    | **类型别名**               | **模板别名**                |
|-----------------------------|----------------------------|-----------------------------|
| **定义位置**                 | 局部或全局作用域           | 只能在全局作用域定义        |
| **是否支持CTAD**             | 不支持                     | 支持（C++20及以后）         |
| **使用场景**                 | 简化类型名                 | 简化模板名                  |
| **是否要求显式指定模板类型** | 必须显式指定模板类型        | C++20及以后可以自动推导类型 |
| **作用范围**                 | 局部作用域有效             | 全局作用域有效              |

### 代码示例：结合CTAD和模板别名

```cpp
#include <iostream>

template <typename T>
struct Pair {
    T first{};
    T second{};
};

// 模板别名
template <typename T>
using Coord = Pair<T>; // Coord<T> 为 Pair<T> 的别名

template <typename T>
void print(const Coord<T>& c) {
    std::cout << c.first << ' ' << c.second << '\n';
}

int main() {
    Coord<int> p1 { 1, 2 }; // 显式指定模板参数
    Coord p2 { 1, 2 };      // C++20 以后，使用CTAD自动推导类型为 Coord<int>

    print(p1);
    print(p2);

    return 0;
}
```

### 结论

模板别名是一个强大的功能，允许我们为模板创建简洁的替代名称，尤其适用于在复杂模板类型上进行抽象。模板别名不仅能提高代码的可读性，还能与C++20的CTAD相结合，进一步减少了显式指定模板类型的需求。通过合理使用模板别名，可以使代码更加清晰、简洁且易于维护。