## 非限定作用域枚举的输入与输出

### 1. 枚举基本概念

枚举类型是由一组命名的整数常量组成的类型，它们通常用于表示一组相关的值。枚举类型使得代码更加可读，并且避免了在代码中使用数字常量。枚举元素是符号常量，通常是整型常量。因此，枚举类型实际持有的是整数值。

**例如：**

```cpp
enum Color {
    black, // 默认值 0
    red,   // 默认值 1
    blue,  // 默认值 2
    green, // 默认值 3
    white, // 默认值 4
};
```

### 2. 枚举的默认值与显式值

枚举中的每个元素都有一个整数值，通常从 `0` 开始，后续的枚举元素值依次递增。你也可以显式指定枚举元素的值：

```cpp
enum Animal {
    cat = -3,  // 指定值为 -3
    dog,       // 值为 -2
    pig,       // 值为 -1
    horse = 5, // 指定值为 5
    giraffe = 5, // 值为 5，与 horse 相同
    chicken,   // 值为 6
};
```

**注意：** 当多个枚举元素被赋予相同的值时，这些元素变得不可区分，可能会带来一些潜在的问题，通常应避免这种做法。

### 3. 非限定作用域枚举的隐式转换

在非限定作用域枚举中，枚举值会自动转换为对应的整数值。当你尝试打印枚举元素时，编译器会隐式将枚举类型转换为整数。例如：

```cpp
#include <iostream>

enum Color {
    black, // 0
    red,   // 1
    blue,  // 2
};

int main() {
    Color shirt = blue;
    std::cout << "Your shirt is " << shirt << '\n'; // 输出：2
    return 0;
}
```

由于枚举类型是整型的常量，因此枚举类型会被转换为其对应的整数值（在此例中是 `2`）。

### 4. 打印枚举元素名称

默认情况下，枚举输出的是整数值。如果你想打印枚举元素的名称，可以编写一个辅助函数：

```cpp
#include <iostream>
#include <string>

enum Color {
    black,
    red,
    blue,
};

std::string getColor(Color color) {
    switch (color) {
        case black: return "black";
        case red:   return "red";
        case blue:  return "blue";
        default:    return "???";
    }
}

int main() {
    Color shirt = blue;
    std::cout << "Your shirt is " << getColor(shirt) << '\n'; // 输出：Your shirt is blue
    return 0;
}
```

使用 `switch` 语句是处理枚举类型的一个高效方法，比 `if-else` 链更优。

### 5. 重载 `operator<<` 输出枚举

为了避免每次都调用辅助函数，你可以通过重载 `operator<<` 来直接打印枚举类型：

```cpp
#include <iostream>

enum Color {
    black,
    red,
    blue,
};

// 重载 operator<<
std::ostream& operator<<(std::ostream& out, Color color) {
    switch (color) {
        case black: return out << "black";
        case red:   return out << "red";
        case blue:  return out << "blue";
        default:    return out << "???";
    }
}

int main() {
    Color shirt = blue;
    std::cout << "Your shirt is " << shirt << '\n'; // 输出：Your shirt is blue
    return 0;
}
```

通过重载 `operator<<`，你可以轻松地打印任何枚举类型，而不需要每次都写额外的函数。

### 6. 枚举的底层类型

C++标准没有指定非限定作用域枚举使用什么类型作为底层类型。大多数编译器会使用 `int` 作为底层类型，但你可以指定其他类型，特别是当需要节省内存时。

**示例：**

```cpp
#include <cstdint>  // for std::int8_t

enum Color : std::int8_t { // 指定底层类型为 8 位整数
    black,
    red,
    blue
};

int main() {
    Color c = black;
    std::cout << sizeof(c) << '\n'; // 输出：1 (字节)
    return 0;
}
```

### 7. 整数到枚举类型的转换

虽然编译器会隐式将枚举类型转换为整数，但不会自动将整数转换为枚举类型。你需要使用 `static_cast` 来执行这种转换：

```cpp
enum Pet {
    cat,  // 0
    dog,  // 1
    pig,  // 2
    whale // 3
};

int main() {
    Pet pet = static_cast<Pet>(2); // 将整数 2 转换为 Pet
    std::cout << pet << '\n'; // 输出：2
    return 0;
}
```

### 8. 非限定作用域枚举的输入

默认情况下，C++ 不知道如何从输入流读取非限定作用域枚举。你可以手动读取整数并将其转换为枚举类型：

```cpp
#include <iostream>

enum Pet {
    cat,   // 0
    dog,   // 1
    pig,   // 2
    whale  // 3
};

int main() {
    std::cout << "Enter a pet (0=cat, 1=dog, 2=pig, 3=whale): ";
    int input;
    std::cin >> input; // 输入整数
    Pet pet = static_cast<Pet>(input); // 转换为 Pet 枚举类型
    std::cout << pet << '\n'; // 输出 Pet 枚举
    return 0;
}
```

如果你想让输入更灵活，你也可以通过重载 `operator>>` 来处理输入：

```cpp
#include <iostream>

enum Pet {
    cat,   // 0
    dog,   // 1
    pig,   // 2
    whale  // 3
};

std::istream& operator>>(std::istream& in, Pet& pet) {
    int input;
    in >> input; // 读取整数
    pet = static_cast<Pet>(input);
    return in;
}

int main() {
    std::cout << "Enter a pet (0=cat, 1=dog, 2=pig, 3=whale): ";
    Pet pet;
    std::cin >> pet; // 读取输入
    std::cout << pet << '\n'; // 输出对应的枚举
    return 0;
}
```

### 9. 总结

- **枚举元素是整型常量**，可以通过隐式转换为整数。
- **显式赋值**给枚举元素可能导致枚举元素值相同，从而变得不可区分。
- 使用 `switch` 或者 `operator<<` 来高效打印枚举类型。
- 可以通过 `static_cast` 将整数转换为枚举类型，但无法直接通过整数初始化非限定作用域枚举，除非指定底层类型。
- 可以通过重载 `operator>>` 来允许输入枚举类型。

### 推荐最佳实践

1. 尽量避免为枚举元素显式赋值，除非有特殊需求。
2. 为了更好的性能，尽量使用 `switch` 语句而不是 `if-else` 链来处理枚举。
3. 只有在需要节省内存时才指定枚举的底层类型。

