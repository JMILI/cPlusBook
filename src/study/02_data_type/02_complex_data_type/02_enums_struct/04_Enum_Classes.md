### 1. **非限定作用域枚举（普通枚举）**

在C++中，**非限定作用域枚举**（也称为普通枚举）使用`enum`关键字定义。虽然它们能为程序提供一定的可读性和便捷性，但它们并不具备类型安全，这会导致一些潜在的错误。下面是一个例子：

```cpp
#include <iostream>

int main() {
    enum Color { red, blue };
    enum Fruit { banana, apple };

    Color color { red };
    Fruit fruit { banana };

    if (color == fruit) 
        std::cout << "color and fruit are equal\n"; 
    else
        std::cout << "color and fruit are not equal\n";

    return 0;
}
```

**输出**：
```
color and fruit are equal
```

**问题**：
在这段代码中，虽然`Color`和`Fruit`属于不同的枚举类型，但在比较时，编译器将其视为整数进行比较。由于`red`和`banana`的默认值为`0`，因此它们被认为相等。这种行为在语义上没有意义，因为我们本不应将这两种不同的枚举类型进行比较。

### 2. **限定作用域枚举（枚举类）**

为了解决上述问题，C++引入了**限定作用域枚举**（通常称为**枚举类**），它通过`enum class`关键字进行定义。限定作用域枚举的两个显著特点：

- **不会隐式转换为整数**：这避免了类型之间不应进行比较的情况。
- **枚举元素的作用域限定**：枚举元素仅能在其定义的枚举类型的作用域内使用，不会污染全局命名空间。

#### 语法：
```cpp
enum class EnumName {
    EnumValue1,
    EnumValue2,
    ...
};
```

#### 示例：
```cpp
#include <iostream>

int main() {
    enum class Color { red, blue };
    enum class Fruit { banana, apple };

    Color color { Color::red };
    Fruit fruit { Fruit::banana };

    if (color == fruit)  // 编译错误
        std::cout << "color and fruit are equal\n";
    else
        std::cout << "color and fruit are not equal\n";

    return 0;
}
```

**输出**：
```
编译错误
```

**解释**：
在限定作用域枚举中，`color`和`fruit`属于不同的枚举类型，因此它们不能直接进行比较。编译器会报错，阻止这种不合逻辑的操作。

### 3. **限定作用域枚举的作用域**

限定作用域枚举为其元素提供了一个独立的命名空间。举例如下：

```cpp
#include <iostream>

int main() {
    enum class Color { red, blue };

    std::cout << red << '\n';       // 编译错误：red 不在全局命名空间
    std::cout << Color::red << '\n'; // 正确，访问 Color 枚举的元素

    return 0;
}
```

**解释**：
由于`Color`是限定作用域枚举，`red`只能通过`Color::red`的形式访问。它不再污染全局命名空间。

### 4. **限定作用域枚举不会隐式转换为整数**

限定作用域枚举不会像普通枚举那样隐式地转换为整数，这有助于避免误用。例如：

```cpp
#include <iostream>

int main() {
    enum class Color { red, blue };
    Color shirt { Color::red };

    if (shirt == Color::red) // 正确
        std::cout << "The shirt is red!\n";
    else if (shirt == Color::blue)
        std::cout << "The shirt is blue!\n";

    return 0;
}
```

**输出**：
```
The shirt is red!
```

但如果你需要将枚举类型转换为整数值，可以显式使用`static_cast`，如：

```cpp
#include <iostream>

int main() {
    enum class Color { red, blue };
    Color color { Color::blue };

    std::cout << static_cast<int>(color) << '\n';  // 显式转换为整数
    return 0;
}
```

**输出**：
```
1
```

在C++23中，`std::to_underlying()`被引入作为更简洁的方式来获取枚举的底层整数值：

```cpp
#include <iostream>
#include <utility>

int main() {
    enum class Color { red, blue };
    Color color { Color::blue };

    std::cout << std::to_underlying(color) << '\n'; // 使用 std::to_underlying
    return 0;
}
```

**输出**：
```
1
```

### 5. **使用 `static_cast` 转换整数为枚举类型**

有时，特别是在获取用户输入时，可能需要将整数转换为枚举类型。这可以通过`static_cast`实现：

```cpp
#include <iostream>

int main() {
    enum class Pet { cat, dog, pig, whale };

    std::cout << "Enter a pet (0=cat, 1=dog, 2=pig, 3=whale): ";

    int input{};
    std::cin >> input;

    Pet pet{ static_cast<Pet>(input) }; // 将整数转换为 Pet 枚举类型

    return 0;
}
```

### 6. **`using enum`语句（C++20）**

在C++20中，`using enum`语句允许将枚举中的所有元素导入到当前作用域，从而避免冗余的前缀。例如：

```cpp
#include <iostream>
#include <string_view>

enum class Color { black, red, blue };

constexpr std::string_view getColor(Color color) {
    using enum Color; // 引入 Color 枚举中的所有元素到当前作用域

    switch (color) {
    case black: return "black";
    case red: return "red";
    case blue: return "blue";
    default: return "???";
    }
}

int main() {
    Color shirt{ Color::blue };
    std::cout << "Your shirt is " << getColor(shirt) << '\n';
    return 0;
}
```

**输出**：
```
Your shirt is blue
```

### 7. **最佳实践**

- **使用限定作用域枚举**：除非有特别的需求，否则建议优先使用限定作用域枚举。它们能提供类型安全，避免了名称污染，并且不会隐式转换为整数。

- **当需要频繁转换为整数时**：如果程序中经常需要将枚举值转换为整数，考虑重载运算符或使用`static_cast`和`std::to_underlying()`。

### 8. **总结表格**

| 特性 | 非限定作用域枚举 | 限定作用域枚举 |
|------|------------------|----------------|
| 类型安全 | 否 | 是 |
| 隐式转换为整数 | 是 | 否 |
| 枚举元素作用域 | 全局命名空间 | 枚举类型作用域内 |
| 访问方式 | 可以直接使用枚举值名 | 必须通过`EnumType::EnumValue`方式访问 |

### 9. **简化限定作用域枚举到整数的转换（高级）**

如果你需要频繁地将限定作用域枚举转换为整数值，可以通过重载运算符来简化这一过程：

```cpp
#include <iostream>
#include <type_traits>

enum class Animals { chicken, dog, cat, elephant, duck, snake };

constexpr auto operator+(Animals a) noexcept {
    return static_cast<std::underlying_type_t<Animals>>(a);
}

int main() {
    std::cout << +Animals::elephant << '\n';  // 使用重载的运算符+转换为整数
    return 0;
}
```

**输出**：
```
3
```

通过重载`operator+`，我们使得枚举值的转换变得简洁和直观，避免了频繁使用`static_cast`。