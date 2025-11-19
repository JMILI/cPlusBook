## **复合数据类型简介**

在 C++ 中，复合数据类型（Composite Data Types）是由一个或多个基本数据类型（或其他复合类型）组合而成的类型。它们为程序提供了比基本数据类型（如 `int`、`char`、`double` 等）更多的功能和灵活性。当你需要表示更加复杂的数据结构时，复合数据类型尤为重要。

### **基本数据类型与复合数据类型的关系**

基本数据类型非常适合处理简单的数据，如数字、字符等。然而，当问题变得更加复杂时，基本类型就显得力不从心。复合数据类型则提供了解决方案。

例如，在你编写一个数学程序时，如何表示一个分数？使用两个整数（一个表示分子，一个表示分母）来表示分数是一个简单而直接的做法。但是，这种做法有一些缺陷，例如数据结构不够清晰，分子和分母没有明确的关联，代码的可维护性较差。

### **实例：分数乘法程序**

假设你正在编写一个程序来表示和乘法分数，下面是一个简单的程序：

```cpp
#include <iostream>

int main() {
    int num1 {}, den1 {};  // 第一个分数的分子和分母
    int num2 {}, den2 {};  // 第二个分数的分子和分母
    char ignore {};  // 用于读取分数中的 '/'
    
    // 输入第一个分数
    std::cout << "Enter a fraction: ";
    std::cin >> num1 >> ignore >> den1;
    
    // 输入第二个分数
    std::cout << "Enter a fraction: ";
    std::cin >> num2 >> ignore >> den2;

    // 输出乘积
    std::cout << "The two fractions multiplied: "
              << num1 * num2 << '/' << den1 * den2 << '\n';

    return 0;
}
```

**运行示例：**

```
Enter a fraction: 1/2
Enter a fraction: 3/4
The two fractions multiplied: 3/8
```

### **问题：**

- **数据松散**：分子和分母仅通过两个整数表示，缺少明确的关系。
- **可维护性差**：假设我们需要处理更多分数，或者需要进行错误处理，这段代码会变得冗长且难以维护。
- **函数返回值限制**：函数只能返回单个值，但我们需要返回两个值（分子和分母）。

### **解决方案：复合数据类型**

C++ 提供了复合数据类型来解决这些问题。复合数据类型能够将相关的数据组合在一起，从而让代码更加结构化、易于维护。

### **C++支持的复合数据类型：**

1. **函数**  
   函数本身就是一种复合类型。函数类型由返回类型和参数类型组成，参数类型可以是基本类型，也可以是复合类型。

2. **数组 (Arrays)**  
   数组是由相同类型的元素构成的集合。可以通过下标访问数组中的元素。

3. **指针类型 (Pointers)**  
   指针类型是存储内存地址的类型。指针可以指向基本类型、数组、函数，甚至是类成员。

    - **对象指针**：指向对象的指针。
    - **函数指针**：指向函数的指针。
    - **指向成员类型的指针**：指向类的成员函数或成员数据的指针。

4. **引用类型 (References)**  
   引用是对变量的别名，分为左值引用和右值引用。

    - **左值引用 (Lvalue Reference)**：引用一个可修改的对象。
    - **右值引用 (Rvalue Reference)**：引用一个临时对象（右值）。

5. **枚举类型 (Enumerations)**  
   枚举类型允许为一组常量定义一个名字，分为强作用域枚举和弱作用域枚举。

6. **类类型 (Class Types)**
    - **结构体 (struct)**：类类型的一种特殊形式。
    - **类 (class)**：更复杂的数据类型，包含数据成员和成员函数。
    - **联合体 (union)**：多个数据成员共用同一块内存。
7. **模板类型**
   - 类模板和函数模板实例化后的类型

### **复合数据类型应用实例**

1. **分数类 (Fraction Class)**

我们可以创建一个 `Fraction` 类来表示分数，从而避免多个独立整数带来的混乱。

```cpp
#include <iostream>
#include <stdexcept>

class Fraction {
public:
    int numerator;  // 分子
    int denominator;  // 分母

    // 构造函数
    Fraction(int num, int den) : numerator(num), denominator(den) {
        if (denominator == 0) {
            throw std::invalid_argument("Denominator cannot be zero.");
        }
    }

    // 分数乘法
    Fraction multiply(const Fraction& other) {
        return Fraction(numerator * other.numerator, denominator * other.denominator);
    }
};

int main() {
    Fraction frac1(1, 2);
    Fraction frac2(3, 4);

    Fraction result = frac1.multiply(frac2);
    std::cout << "The product of the fractions is: " 
              << result.numerator << '/' << result.denominator << std::endl;

    return 0;
}
```

2. **员工ID列表**

当你需要存储大量员工ID时，可以使用数组或 `std::vector` 来存储这些ID，而不需要为每个ID声明一个单独的变量。

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> employee_ids = {42, 57, 162, 203, 315};  // 存储员工ID

    // 打印所有员工ID
    for (int id : employee_ids) {
        std::cout << "Employee ID: " << id << std::endl;
    }

    return 0;
}
```

---

## **复合数据类型总结**

| 类型           | 描述                         | 示例                                   |
|----------------|------------------------------|--------------------------------------|
| **函数类型**    | 由返回类型和参数类型组成       | `void doSomething(int, double)`      |
| **数组类型**    | 同一类型元素的集合             | `int arr[10];`                       |
| **指针类型**    | 存储对象的内存地址             | `int* ptr;`                          |
| **引用类型**    | 对变量的别名                   | `int& ref = x;`                      |
| **枚举类型**    | 为常量定义名字                 | `enum Day {Sun, Mon};`               |
| **类类型**      | 更复杂的数据类型，包含成员数据 | `class Person { int age; };`         |
|**模板类型**|类模板和函数模板实例化后的类型| 实例化后：`void doSomething(int, double)` |

---



通过以上内容，我们已经涵盖了复合数据类型的基础以及如何通过它们解决复杂的问题。在后续的章节中，我们将进一步探索其他复合数据类型的细节，例如结构体、类以及更复杂的容器类型（如 `std::vector` 和 `std::array`）。