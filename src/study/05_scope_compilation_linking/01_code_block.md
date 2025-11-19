# 代码块
代码块语句（block statement）是由零个或多个语句组成的复合语句，编译器会将其视作单个语句。代码块以「 { 」开始，以「 } 」结束，要执行的语句置于两者之间，可在允许单个语句出现的任何位置使用，且末尾无需分号。例如，函数体就是典型的代码块：
```cpp
int add(int x, int y)
{ // 代码块开始
    return x + y;
} // 代码块结束 (没有分号)

int main()
{ // 代码块开始
    // 多个语句
    int value {}; // 这是变量初始化，不是代码块
    add(3, 4);
    return 0;
} // 代码块结束 (没有分号)
```

### 代码块嵌套特性
在 C++ 里，函数不能嵌套在其他函数中，但代码块可以嵌套在其他代码块中。示例如下：
```cpp
int add(int x, int y)
{ // 代码块开始
    return x + y;
} // 代码块结束 

int main()
{ // 外围代码块
    // 多个语句
    int value {};
    { // 内部嵌套的代码块
        add(3, 4);
    } // 内部嵌套的代码块结束
    return 0;
} // 外围代码块结束
```

### 代码块在条件语句中的应用
代码块的一个常见用途是与 `if` 语句结合。默认情况下，当 `if` 语句的条件表达式结果为 `true` 时，仅执行单个语句。若要执行多个语句，可使用代码块替换单语句。示例代码如下：
```cpp
#include <iostream>

int main()
{ // 外围代码块
    std::cout << "Enter an integer: ";
    int value {};
    std::cin >> value;
    
    if (value >= 0)
    { // 内部嵌套的代码块
        std::cout << value << " is a positive integer (or zero)\n";
        std::cout << "Double this number is " << value * 2 << '\n';
    } // 内部嵌套的代码块结束
    else
    { // 内部嵌套的代码块
        std::cout << value << " is a negative integer\n";
        std::cout << "The positive of this number is " << -value << '\n';
    } // 内部嵌套的代码块结束
    return 0;
} // 外围代码块结束
```
当用户输入 `3` 时，程序输出：
```
Enter an integer: 3
3 is a positive integer (or zero)
Double this number is 6
```
当用户输入 `-4` 时，程序输出：
```
Enter an integer: -4
-4 is a negative integer
The positive of this number is 4
```

### 代码块的多层嵌套
代码块能够进行多层嵌套。以下是多层嵌套的示例：
```cpp
#include <iostream>

int main()
{ // block 1, 层级 1
    std::cout << "Enter an integer: ";
    int value {};
    std::cin >> value;
    
    if (value >  0)
    { // block 2, 层级 2
        if ((value % 2) == 0)
        { // block 3, 层级 3
            std::cout << value << " is positive and even\n";
        }
        else
        { // block 4, 层级 3
            std::cout << value << " is positive and odd\n";
        }
    }
    return 0;
}
```
函数中代码块的最大嵌套深度指的是从最外围到最内层代码块的级数。在上述函数里，有 4 个代码块，但嵌套级别为 3，因为程序中最大嵌套到第三层。

C++ 标准规定，C++ 编译器应支持 256 级嵌套，但并非所有编译器都能满足这一要求（例如，在编写本文时，Visual Studio 支持的嵌套级别较少）。

### 最佳实践建议
为保证代码的可读性和可维护性，建议将函数的嵌套级别保持在 3 级或更少。如果函数确实需要更多的嵌套级别，可考虑将其重构为多个子函数。 