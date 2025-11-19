### **类与结构体简介**

在 C++ 中，`结构体` 和 `类` 是用于定义用户自定义数据类型的两种关键工具。它们在技术上非常相似，都可以包含数据成员（属性）和成员函数（行为）。然而，它们的设计哲学和使用场景有所不同。为了更好地理解它们，我们首先需要回顾一下 `结构体` 的概念，再逐步引入 `类` 的特性与优势。

### **1. 结构体（struct）**

结构体是 C++ 中的一种数据封装方式，它允许将多个不同类型的成员变量组合成一个单独的对象。结构体适合用于将相关数据捆绑在一起，便于在程序中传递和操作。

#### **结构体示例**

```cpp
#include <iostream>

struct Date {
    int day {};
    int month {};
    int year {};
};

void printDate(const Date& date) {
    std::cout << date.day << '/' << date.month << '/' << date.year; // 设置 DMY 的格式
}

int main() {
    Date date{ 4, 10, 21 }; // 使用聚合初始化
    printDate(date);        // 传递结构体给函数

    return 0;
}
```

在这个简单的例子中，我们定义了一个 `Date` 结构体，包含三个整数成员（`day`, `month`, `year`）。该结构体用于存储日期信息，并通过 `printDate` 函数打印该日期。

#### **结构体的局限性**

尽管结构体可以方便地组织数据，但它存在一些缺陷，尤其是在处理复杂程序时。例如：

- **没有数据有效性检查**：结构体本身没有内置机制来确保其数据始终处于有效状态（如，`Fraction` 分数类中的分母不能为零）。
- **类不变量的维护**：对于复杂对象，结构体并没有强制约束其成员之间的关系，容易产生无效状态。

### **2. 数据状态有效性问题**

在 C++ 中，我们希望对象始终保持有效的状态。类不变量（Class Invariant）指的是在对象生命周期内，必须始终为真的条件。例如，在 `Fraction` 类中，分母不能为零。这就要求程序在每次使用时确保不违反这个条件，否则会导致未定义的行为。

#### **无效状态的示例**

```cpp
#include <iostream>

struct Fraction {
    int numerator { 0 };
    int denominator { 1 }; // 默认初始化为有效值
};

void printFractionValue(const Fraction& f) {
    std::cout << f.numerator / f.denominator << '\n';  // 除零错误
}

int main() {
    Fraction f { 5, 0 };  // 创建一个分母为0的分数
    printFractionValue(f); // 会导致除零错误
    return 0;
}
```

在上面的例子中，`Fraction` 结构体的设计没有强制要求分母必须为非零。当我们显式地初始化分母为 0 时，程序会在运行时遇到除零错误。

#### **如何避免无效状态？**

- 使用断言：`assert(f.denominator != 0)`，可以在开发时捕捉错误。
- 但这种方法不能完全避免对象处于无效状态，尤其在复杂代码库中更难以维护。

### **3. 类（class）**

为了克服结构体的局限性，C++ 引入了 `类` 的概念。类不仅允许我们将数据和函数组合在一起，还提供了强大的机制来保证数据的一致性和有效性。类通常通过构造函数、成员函数以及访问控制等功能，来保证类的不变量。

#### **类的定义与使用**

类的定义与结构体相似，区别在于我们使用 `class` 关键字，而不是 `struct`。在类中，成员变量通常是私有的，只有通过成员函数来访问和修改。

```cpp
#include <iostream>

class Date {
public:
    int m_day{};
    int m_month{};
    int m_year{};

    void printDate() const {
        std::cout << m_day << '/' << m_month << '/' << m_year;
    }
};

int main() {
    Date date{ 4, 10, 21 };
    date.printDate();  // 输出：4/10/21

    return 0;
}
```

在这个例子中，`Date` 类与之前的结构体非常相似，但它提供了更多的功能性，比如方法 `printDate` 来打印日期。

#### **类与结构体的差异**

| 特性            | 结构体（struct）                         | 类（class）                   |
|-----------------|----------------------------------------|------------------------------|
| **默认访问权限** | 公有（public）                          | 私有（private）               |
| **成员函数**    | 支持，但通常不使用                       | 支持，通常用于操作类的成员    |
| **数据封装**    | 不强制封装，所有成员通常是公有的        | 强制封装，成员默认是私有的    |
| **面向对象支持** | 支持，但一般用于存储数据                | 完全支持面向对象特性，包括封装、继承和多态 |

结构体适用于简单的数据封装，而类则提供了更多的控制和封装能力，适用于更复杂的应用。

### **4. 类的不变量**

类通过构造函数、成员函数以及访问控制来确保类的有效性，防止类对象进入无效状态。一个常见的做法是使用构造函数进行初始化，并通过成员函数确保不变量。

#### **示例：Fraction 类**

```cpp
#include <iostream>

class Fraction {
private:
    int numerator;
    int denominator;

public:
    Fraction(int num, int denom) {
        if (denom == 0) {
            throw std::invalid_argument("Denominator cannot be zero");
        }
        numerator = num;
        denominator = denom;
    }

    void printFractionValue() const {
        std::cout << static_cast<double>(numerator) / denominator << '\n';
    }
};

int main() {
    try {
        Fraction f(5, 0);  // 会抛出异常
        f.printFractionValue();
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';  // 输出错误信息
    }

    return 0;
}
```

在这个例子中，`Fraction` 类的构造函数确保分母不为零。如果传递无效的值，构造函数会抛出异常，确保类对象始终有效。

### **5. 类的成员函数与封装**

类允许定义成员函数来操作其私有数据成员，从而确保数据的封装性和一致性。封装是面向对象编程的一个重要特性，它保证了对象的内部状态只能通过公开的接口进行访问和修改。

#### **封装示例**

```cpp
class Employee {
private:
    std::string name;
    int age;

public:
    Employee(const std::string& n, int a) : name(n), age(a) {}

    void setName(const std::string& n) { name = n; }
    std::string getName() const { return name; }

    void setAge(int a) { age = a; }
    int getAge() const { return age; }
};

int main() {
    Employee e("John", 30);
    e.setName("Alice");  // 修改姓名
    std::cout << e.getName() << " is " << e.getAge() << " years old\n";  // 输出：Alice is 30 years old
    return 0;
}
```

在这个例子中，`Employee` 类将数据成员 `name` 和 `age` 设为私有，外部无法直接访问。通过公有成员函数 `setName`、`getName` 和 `setAge`、`getAge` 来操作这些数据，确保数据的有效性。

### **6. 类的成员命名约定**

在 C++ 中，通常会遵循一定的命名约定。例如，类的成员变量通常以 `m_` 前缀开头，表示它们是成员变量。而成员函数的命名通常采用驼峰式命名法。

### **总结**

- **结构体（struct）** 适用于简单的数据组合，成员通常是公有的，易于使用，但不具备强大的数据封装和一致性检查能力。
- **类（class）** 提供了强大的封装、继承和多态能力，可以通过构造函数、成员函数和访问控制来确保类的不变量和对象的有效性。

通过使用类，开发者可以更精确地控制对象的状态，确保数据的一致性和有效性，避免无效对象的出现。类是面向对象编程的核心构建块，是 C++ 中最重要的编程工具之一。