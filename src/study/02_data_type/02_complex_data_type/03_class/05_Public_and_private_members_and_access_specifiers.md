### 公共和私有成员以及访问说明符

在C++中，类和结构体的成员可以具有不同的访问级别。理解如何设置这些访问权限对于编写健壮、可维护的代码至关重要。以下内容将详细探讨C++中的访问说明符，包括公共成员、私有成员以及它们如何影响代码的可访问性。

#### 类和结构体的成员访问权限

C++中的每个类或结构体成员都可以指定不同的访问级别，这些级别确定了外部代码是否能够访问该成员。C++提供了三种主要的访问说明符：**public**、**private** 和 **protected**。在实际编程中，最常用的是 `public` 和 `private`，而 `protected` 则在继承时使用较多。我们将主要讨论 `public` 和 `private`。

##### 公共成员（Public Members）

**公共成员**是指类或结构体的成员，可以从任何地方访问，包括类外部的代码。它们没有访问限制，因此，任何代码都可以修改它们的值或调用它们的函数。

在C++中，结构体默认成员是 `public`，而类的成员默认是 `private`。

**示例：结构体的公共成员**

```cpp
#include <iostream>

struct Date {
    int year {};       // public 默认
    int month {};      // public 默认
    int day {};        // public 默认

    void print() const { // public 默认
        std::cout << year << '/' << month << '/' << day << '\n';
    }
};

int main() {
    Date today{2020, 10, 14}; // 聚合初始化

    // 直接访问 public 成员
    today.day = 16; // OK: day 是 public
    today.print();  // OK: print() 是 public

    return 0;
}
```

在上述代码中，`Date` 结构体的成员（如 `year`, `month`, `day` 和 `print()`）都属于 `public`，因此它们可以被 `main()` 函数（甚至是其他任何函数）直接访问。

**关键点**：
- 结构体成员默认是 `public`。
- 公共成员不受任何限制，可以被类外部的代码访问。

##### 私有成员（Private Members）

**私有成员**是只能在类内部访问的成员。它们不能被类外部的代码直接访问。私有成员通常用于实现类的封装，确保类的内部状态不会被不受控制的外部代码修改。

在类中，成员默认为 `private`，因此当使用 `class` 定义类时，成员默认是私有的。

**示例：类的私有成员**

```cpp
#include <iostream>

class Date {
    int m_year {};     // private 默认
    int m_month {};    // private 默认
    int m_day {};      // private 默认

    void print() const { // private 默认
        std::cout << m_year << '/' << m_month << '/' << m_day << '\n';
    }
};

int main() {
    Date today{2020, 10, 14}; // 错误：无法使用聚合初始化

    // 私有成员不能直接访问
    today.m_day = 16; // 错误：m_day 是 private
    today.print();    // 错误：print() 是 private

    return 0;
}
```

在这个例子中，由于 `Date` 类的成员是私有的，所以在 `main()` 函数中尝试访问 `m_day` 或调用 `print()` 会导致编译错误。私有成员只能由 `Date` 类的成员函数访问。

**关键点**：
- 类的成员默认是 `private`。
- 私有成员只能由类内的成员函数访问，外部代码无法直接访问。

#### 通过访问说明符设置访问级别

C++ 允许使用访问说明符来显式地设置成员的访问级别。访问说明符分为三种：
- `public`: 公共成员，任何代码都可以访问。
- `private`: 私有成员，仅限类内部的成员函数访问。
- `protected`: 受保护成员，允许类内部成员和派生类中的成员访问，但不允许外部代码直接访问。

访问说明符后的成员都是该级别的。例如：

```cpp
class Date {
public:
    void print() const {
        std::cout << m_year << '/' << m_month << '/' << m_day << '\n';
    }

private:
    int m_year {2020};
    int m_month {10};
    int m_day {14};
};
```

在这个示例中：
- `print()` 是 `public`，因此可以被类外部的代码访问。
- `m_year`、`m_month` 和 `m_day` 是 `private`，只能在类内部的成员函数中访问。

#### 命名私有成员变量的惯例

在C++中，常见的做法是给私有成员变量添加 `m_` 前缀，表示这是一个成员变量。这种命名习惯有助于避免与局部变量或函数参数发生命名冲突，并且能帮助开发者更清晰地理解哪些变量属于对象的状态。

例如：

```cpp
class Person {
private:
    std::string m_name;
public:
    void setName(const std::string& name) {
        m_name = name;
    }

    void printName() const {
        std::cout << m_name << std::endl;
    }
};
```

#### 访问级别总结

| 访问级别 | 访问说明符 | 类外部可访问 | 类内部可访问 | 子类可访问 |
|-----------|------------|--------------|--------------|------------|
| 公共      | `public`   | 是           | 是           | 是         |
| 受保护    | `protected`| 否           | 是           | 是         |
| 私有      | `private`  | 否           | 是           | 否         |

#### 结构体与类的差异

- **结构体**：
    - 默认成员是 `public`。
    - 通常用于简单的数据结构，不需要封装。
    - 适用于聚合初始化（所有成员公开）。

- **类**：
    - 默认成员是 `private`。
    - 用于具有封装需求的复杂数据结构，成员通常应为私有。
    - 不支持聚合初始化（除非显式声明公共成员）。

**最佳实践**：
- 结构体应当只用于简单数据集合，不涉及封装和复杂行为。
- 类应当将数据成员设为私有，提供公共方法来访问和修改数据。

#### 访问级别按类工作

访问级别是按类类型工作，而不是按对象工作。这意味着类的成员函数可以访问同一类的任何对象的私有成员。

**示例：类成员函数访问其他对象的私有成员**

```cpp
#include <iostream>
#include <string>

class Person {
private:
    std::string m_name;

public:
    void kisses(const Person& p) const {
        std::cout << m_name << " kisses " << p.m_name << std::endl;
    }

    void setName(const std::string& name) {
        m_name = name;
    }
};

int main() {
    Person joe;
    joe.setName("Joe");

    Person kate;
    kate.setName("Kate");

    joe.kisses(kate);  // Joe kisses Kate

    return 0;
}
```

在这个示例中，`kisses` 方法能够访问 `p.m_name`，因为 `p` 是 `Person` 类型的对象，而 `kisses` 是 `Person` 类的成员函数，因此可以访问类中任何对象的私有成员。

#### 结构体和类的选择

- **使用结构体**：当你需要一个简单的数据结构，并且没有成员需要封装时，结构体是一个不错的选择。
    - 例如：表示一个坐标（`struct Point { int x, y; };`）。
    - 不需要额外的封装，成员可以直接访问。

- **使用类**：当你需要封装数据，限制外部访问时，应该使用类。
    - 例如：表示一个银行账户，必须确保账户余额不能被任意修改。

### 结论

C++的访问说明符是控制类成员访问权限的核心工具。通过合理使用 `public`、`private` 和 `protected`，可以更好地封装类，确保数据的安全性与一致性。结构体和类之间的差异主要在于默认的访问权限，因此在选择使用结构体或类时，应根据需求作出适当选择。