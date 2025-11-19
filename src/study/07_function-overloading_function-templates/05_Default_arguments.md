### 默认参数概述
默认参数是 C++ 中一个实用的特性，它允许为函数的参数提供默认值。当进行函数调用时，如果调用方提供了对应参数的值，那么函数将使用传递的值；如果调用方没有提供该参数的值，则函数会使用默认参数的值。

### 基本语法和使用示例
```cpp
#include <iostream>

// 定义带有默认参数的函数
void print(int x, int y = 4) {
    std::cout << "x: " << x << '\n';
    std::cout << "y: " << y << '\n';
}

int main() {
    print(1, 2); // 参数 y 使用传递的值 2
    print(3);    // 参数 y 使用默认值 4
    return 0;
}
```
上述代码中，`print` 函数的第二个参数 `y` 有默认值 4。在 `main` 函数里，第一次调用 `print(1, 2)` 时，`y` 使用传递的值 2；第二次调用 `print(3)` 时，`y` 使用默认值 4。

### 指定默认参数的语法要求
必须使用等号来指定默认参数，使用括号或大括号不符合语法：
```cpp
void foo(int x = 5);   // ok
void goo(int x ( 5 )); // 编译失败
void boo(int x { 5 }); // 编译失败
```

### 默认参数的处理机制
默认参数由编译器在函数调用点处理。当编译器看到 `print(3)` 这样的调用时，它会把这个函数调用重写为 `print(3, 4)`，使得传递值的数量与参数的数量匹配，然后按常规进行函数调用。

### 何时使用默认参数
当函数需要具有合理默认值，但又希望调用方可以根据需要覆盖该值时，默认参数是一个很好的选择。例如：
```cpp
int rollDie(int sides = 6);
void openLogFile(std::string filename = "default.log");
```
`rollDie` 函数用于模拟掷骰子，默认骰子有 6 个面；`openLogFile` 函数用于打开日志文件，默认文件名是 `"default.log"`。

### 多个默认参数
函数可以有多个具有默认值的参数，但要遵循一定规则：
```cpp
#include <iostream>

// 定义带有多个默认参数的函数
void print(int x = 10, int y = 20, int z = 30) {
    std::cout << "Values: " << x << " " << y << " " << z << '\n';
}

int main() {
    print(1, 2, 3); // 全部显式传值
    print(1, 2);    // z 使用默认参数
    print(1);       // y 和 z 使用默认参数
    print();        // x、y 和 z 使用默认参数
    return 0;
}
```
C++ 不支持 `print(, , 3)` 这样的调用语法，即如果一个参数使用了默认值，那么所有后续直到结束的参数都必须使用默认值。以下情况是不允许的：
```cpp
void print(int x = 10, int y); // 不允许
```

### 无法重新声明默认参数
一旦声明了默认参数，就不能在同一文件中重新声明。对于有前向声明和函数定义的函数，默认参数可以在前向声明或函数定义中单独声明，但不能同时在两者中声明。最佳实践是在前向声明中声明默认参数，特别是在头文件中：

错误做法：

```c++
#include <iostream>
void print(int x, int y=4); // 前向声明

void print(int x, int y=4) // error: 重新定义默认参数
{
    std::cout << "x: " << x << '\n';
    std::cout << "y: " << y << '\n';
}
```

正确：

- **`foo.h` 文件**
```cpp
#ifndef FOO_H
#define FOO_H
void print(int x, int y = 4);
#endif
```
- **`main.cpp` 文件**
```cpp
#include "foo.h"
#include <iostream>

void print(int x, int y) {
    std::cout << "x: " << x << '\n';
    std::cout << "y: " << y << '\n';
}

int main() {
    print(5);
    return 0;
}
```

### 默认参数和函数重载
可以重载具有默认参数的函数，但可能会导致潜在的不明确匹配：
```cpp
#include <string>

// 函数重载
void print(std::string) {
}

void print(char = ' ') {
}

int main() {
    print("Hello, world"); // 调用的是 print(std::string)
    print('a');            // 调用的是 print(char)
    print();               // 调用的是 print(char)
    return 0;
}
```
再看下面的例子：
```cpp
void print(int x);
void print(int x, int y = 10);
void print(int x, double y = 20.5);

print(1, 2);    // 解析到 print(int, int)
print(1, 2.5);  // 解析到 print(int, double) 
print(1);       // 不明确匹配的函数调用
```
在 `print(1)` 这种情况下，编译器无法判断它应该解析为 `print(int)`，还是第二个参数具有默认值的两个函数之一，从而导致不明确的函数调用。

### 总结
默认参数为函数调用提供了灵活性，用户可以选择覆盖或不覆盖参数的默认值。在使用时，要注意指定默认参数的语法、参数顺序规则、避免重新声明默认参数，以及处理好与函数重载的关系。 