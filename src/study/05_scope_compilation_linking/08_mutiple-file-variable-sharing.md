### 在多个文件中共享全局常量（使用内联变量）

在C++中，全局常量是程序中不会改变的值，例如数学常数（如π）或应用特定的系数（如重力系数）。为了在多个文件中共享这些常量，C++提供了多种方法。以下是详细介绍：

---

#### **1. 使用 `constexpr` 和头文件（C++17 之前）**
在C++17之前，最常见的做法是将常量定义在头文件中，并使用 `constexpr` 声明。

##### **实现步骤**
1. 创建一个头文件（如 `constants.h`），定义命名空间和常量。
2. 在需要使用常量的文件中包含该头文件。

##### **示例代码**
```cpp
// constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
    constexpr double pi{ 3.14159 };          // π
    constexpr double avogadro{ 6.0221413e23 }; // 阿伏伽德罗常数
    constexpr double myGravity{ 9.2 };       // 重力系数
}

#endif
```

```cpp
// main.cpp
#include "constants.h"
#include <iostream>

int main() {
    std::cout << "Enter a radius: ";
    double radius{};
    std::cin >> radius;

    std::cout << "Circumference: " << 2 * radius * constants::pi << '\n';
    return 0;
}
```

##### **优点**
- 简单易用。
- 常量是编译时常量，编译器可以优化。

##### **缺点**
- 每个包含头文件的源文件都会复制一份常量，可能导致重复定义。
- 修改常量值需要重新编译所有包含该头文件的源文件。

---

#### **2. 使用外部链接属性（C++17 之前）**
为了避免重复定义，可以将常量的定义放在源文件中，并在头文件中声明。

##### **实现步骤**
1. 在头文件中声明常量（使用 `extern`）。
2. 在源文件中定义常量。
3. 在需要使用常量的文件中包含头文件。

##### **示例代码**
```cpp
// constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
    extern const double pi;          // 声明
    extern const double avogadro;    // 声明
    extern const double myGravity;   // 声明
}

#endif
```

```cpp
// constants.cpp
#include "constants.h"

namespace constants {
    const double pi{ 3.14159 };          // 定义
    const double avogadro{ 6.0221413e23 }; // 定义
    const double myGravity{ 9.2 };       // 定义
}
```

```cpp
// main.cpp
#include "constants.h"
#include <iostream>

int main() {
    std::cout << "Enter a radius: ";
    double radius{};
    std::cin >> radius;

    std::cout << "Circumference: " << 2 * radius * constants::pi << '\n';
    return 0;
}
```

##### **优点**
- 常量只定义一次，避免重复。
- 修改常量值只需重新编译定义文件。

##### **缺点**
- 常量在定义文件之外是运行时常量，无法用于编译时上下文（如数组大小）。
- 编译器优化受限。

---

#### **3. 使用内联变量（C++17 及以后）**
C++17 引入了内联变量（`inline`），允许在头文件中定义全局常量，同时避免重复定义问题。

##### **实现步骤**
1. 在头文件中定义内联常量（使用 `inline constexpr`）。
2. 在需要使用常量的文件中包含头文件。

##### **示例代码**
```cpp
// constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
    inline constexpr double pi{ 3.14159 };          // 内联常量
    inline constexpr double avogadro{ 6.0221413e23 }; // 内联常量
    inline constexpr double myGravity{ 9.2 };       // 内联常量
}

#endif
```

```cpp
// main.cpp
#include "constants.h"
#include <iostream>

int main() {
    std::cout << "Enter a radius: ";
    double radius{};
    std::cin >> radius;

    std::cout << "Circumference: " << 2 * radius * constants::pi << '\n';
    return 0;
}
```

##### **优点**
- 常量是编译时常量，编译器可以优化。
- 常量只定义一次，避免重复。
- 修改常量值只需重新编译包含头文件的文件。

##### **缺点**
- 修改常量值需要重新编译所有包含头文件的文件。

---

#### **4. 最佳实践**
1. **优先使用内联变量**：如果编译器支持C++17，优先使用 `inline constexpr` 定义全局常量。
2. **避免扩展 `std` 命名空间**：不要将自定义常量放入 `std` 命名空间。
3. **合理组织常量**：将相关常量分组到命名空间中，例如 `Math::`、`Physics::`。
4. **使用 `std::string_view` 定义字符串常量**：对于字符串常量，使用 `std::string_view` 代替 `const char*`。

##### **示例：使用 `std::string_view`**
```cpp
#include <string_view>

namespace constants {
    inline constexpr std::string_view appName{ "MyApp" };
    inline constexpr std::string_view version{ "1.0.0" };
}
```

---

#### **5. 总结**
| 方法                     | 适用场景                     | 优点                                   | 缺点                                   |
|--------------------------|------------------------------|----------------------------------------|----------------------------------------|
| `constexpr` + 头文件      | 小型项目，C++17 之前          | 简单易用，编译时常量                   | 重复定义，修改常量需重新编译所有文件   |
| 外部链接属性              | 大型项目，C++17 之前          | 避免重复定义，修改常量只需重新编译定义文件 | 运行时常量，编译器优化受限             |
| 内联变量（C++17 及以后）  | 所有项目，C++17 及以后        | 编译时常量，避免重复定义               | 修改常量需重新编译所有包含头文件的文件 |

---

#### **6. 代码示例对比**
##### **传统方法（C++17 之前）**
```cpp
// constants.h
namespace constants {
    constexpr double pi{ 3.14159 };
}

// main.cpp
#include "constants.h"
double circumference(double r) { return 2 * r * constants::pi; }
```

##### **内联变量方法（C++17 及以后）**
```cpp
// constants.h
namespace constants {
    inline constexpr double pi{ 3.14159 };
}

// main.cpp
#include "constants.h"
double circumference(double r) { return 2 * r * constants::pi; }
```

---

通过合理使用命名空间和内联变量，可以高效地在多个文件中共享全局常量，同时避免命名冲突和重复定义问题。