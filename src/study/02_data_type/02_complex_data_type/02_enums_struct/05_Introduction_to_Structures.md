### 结构体简介

在编程中，结构体（`struct`）是一种复合数据类型，用于将多个变量组织在一起，形成一个有意义的整体。结构体通常用来表示一个具体的事物或概念，例如员工信息、日期、坐标等。结构体的主要优势在于它能够将多个相关的数据片段组织在一起，提高代码的可读性和可维护性。

例如，我们可以用一个结构体来存储一个员工的详细信息，如姓名、职务、年龄、工资等。这些信息通常是相关联的，使用结构体可以避免分散的多个变量，使得数据更具可管理性。

### 定义结构体

在C++中，结构体的定义使用`struct`关键字来表示。下面是一个简化的示例，定义了一个`Employee`结构体，用于存储员工的基本信息：

```cpp
struct Employee
{
    int id {};        // 员工ID
    int age {};       // 年龄
    double wage {};   // 工资
};
```

在定义结构体时：
- 使用`struct`关键字标识结构体类型。
- 结构体的成员变量（成员）紧跟在花括号中定义。成员变量可以是任意类型，如`int`、`double`等。
- 在定义结构体时，成员变量通常会提供初始值，确保实例化时会进行值初始化。

### 创建结构体对象

一旦结构体类型定义完成，我们可以创建结构体的对象。创建结构体对象时，可以直接定义变量并初始化成员变量：

```cpp
Employee joe {}; // 创建一个类型为 Employee 的结构体对象 joe
```

也可以定义多个结构体对象：

```cpp
Employee joe {};    // 创建 joe
Employee frank {};  // 创建 frank
```

### 访问结构体成员

要访问结构体对象的成员变量，我们使用成员选择操作符（`.`）。例如，要访问`joe`的`age`成员，可以如下操作：

```cpp
std::cout << joe.age << '\n';  // 打印 joe 的年龄
```

下面的代码展示了如何操作结构体成员变量：

```cpp
#include <iostream>

struct Employee
{
    int id {};
    int age {};
    double wage {};
};

int main()
{
    Employee joe {};
    joe.age = 32;  // 设置 joe 的年龄为 32
    std::cout << joe.age << '\n'; // 输出 joe 的年龄

    return 0;
}
```

**输出**：
```
32
```

结构体的成员变量就像普通变量一样，可以进行赋值、算术运算、比较等操作。

### 结构体的优势

结构体的最大优势之一在于，它能让我们将多个相关的变量组合成一个单独的对象，从而使代码更加简洁和有组织。举个例子，假设我们有两个员工对象`joe`和`frank`：

```cpp
#include <iostream>

struct Employee
{
    int id {};
    int age {};
    double wage {};
};

int main()
{
    Employee joe {};
    joe.id = 14;
    joe.age = 32;
    joe.wage = 60000.0;

    Employee frank {};
    frank.id = 15;
    frank.age = 28;
    frank.wage = 45000.0;

    int totalAge { joe.age + frank.age };  // 计算 Joe 和 Frank 的总年龄

    if (joe.wage > frank.wage)
        std::cout << "Joe makes more than Frank\n";
    else if (joe.wage < frank.wage)
        std::cout << "Joe makes less than Frank\n";
    else
        std::cout << "Joe and Frank make the same amount\n";

    frank.wage += 5000.0;  // Frank 升职加薪
    ++joe.age;             // Joe 的年龄增加 1

    return 0;
}
```

在这个示例中，我们创建了两个`Employee`类型的对象`joe`和`frank`，并给他们分别赋予了不同的值。通过结构体，我们能够方便地管理员工的属性并进行操作。比如，我们计算了`joe`和`frank`的总年龄，并根据他们的工资进行比较。

### 结构体的初始化

结构体的初始化方式非常灵活，支持多种方式。最常见的初始化方法是使用大括号来进行初始化，如：

```cpp
Employee joe {14, 32, 60000.0};  // 初始化成员变量 id, age 和 wage
```

此外，还可以在定义时为成员变量指定初始值：

```cpp
struct Employee
{
    int id = 14;
    int age = 32;
    double wage = 60000.0;
};
```

这样做可以确保在没有显式初始化时，结构体的成员会有默认值。

### 结构体与类的区别

尽管结构体和类（`class`）在C++中都可以用于封装数据，但它们之间有一些差异。主要区别在于：

| 特性       | 结构体（`struct`）        | 类（`class`）           |
|------------|---------------------------|-------------------------|
| 默认访问控制 | 公有（`public`）           | 私有（`private`）       |
| 默认继承方式 | 公有继承（`public`）       | 私有继承（`private`）   |

在结构体中，成员默认是`public`的，而在类中，成员默认是`private`的。除此之外，结构体和类在功能上并没有太大区别。许多时候，结构体用于表示数据，而类通常用于表示有更复杂行为的对象。

### 结构体的扩展

结构体不仅限于存储数据，还可以包含成员函数。这样，我们可以为结构体添加一些功能，使其更加灵活。例如：

```cpp
struct Employee
{
    int id {};
    int age {};
    double wage {};

    void promote(double raise)
    {
        wage += raise;
    }
};
```

这样，我们就为`Employee`结构体添加了一个名为`promote`的成员函数，用于提升员工的工资。

### 总结

结构体（`struct`）是C++中用于封装一组相关变量的复合数据类型。它能帮助我们组织和管理数据，使代码更加简洁和易于理解。通过结构体，我们可以将多个变量绑定到一个类型中，并且可以方便地进行访问和修改。结构体是C++中一个非常基础且常用的概念，理解它的基本用法是非常重要的。

#### 示例：员工信息管理

```cpp
#include <iostream>

struct Employee
{
    int id {};
    int age {};
    double wage {};
    void promote(double raise) { wage += raise; }
};

int main()
{
    Employee joe {14, 32, 60000.0};  // 定义员工 Joe
    joe.promote(5000.0);  // Joe 升职加薪

    std::cout << "Joe's new wage: " << joe.wage << '\n';  // 输出 Joe 的新工资

    return 0;
}
```

这个简单的例子演示了如何使用结构体来封装员工信息并对其进行操作。