### `constexpr` 和 `consteval` 函数

#### 1. 引入与问题提出
在前面章节引入了 `constexpr` 关键字用于创建编译时常量，也介绍了常量表达式可在编译时计算。如使用两个 `constexpr` 变量的程序，编译器可在编译时计算常量表达式：
```cpp
#include <iostream>

int main()
{
    constexpr int x{ 5 };
    constexpr int y{ 6 };
    std::cout << (x > y ? x : y) << " is greater!\n";
    return 0;
}
```
但将表达式封装到函数中，调用会在运行时执行，失去了编译时计算的能力：
```cpp
#include <iostream>

int greater(int x, int y)
{
    return (x > y ? x : y);
}

int main()
{
    constexpr int x{ 5 };
    constexpr int y{ 6 };
    std::cout << greater(x, y) << " is greater!\n";
    return 0;
}
```

#### 2. `constexpr` 函数
- **可在编译时计算**：在返回类型前使用 `constexpr` 关键字使函数成为 `constexpr` 函数，其返回值可在编译时计算。
```cpp
#include <iostream>

constexpr int greater(int x, int y)
{
    return (x > y ? x : y);
}

int main()
{
    constexpr int x{ 5 };
    constexpr int y{ 6 };
    constexpr int g { greater(x, y) };
    std::cout << g << " is greater!\n";
    return 0;
}
```
为有资格进行编译时计算，函数须有 `constexpr` 返回类型，编译时计算不能调用非 `constexpr` 函数，且调用时传递 `constexpr` 参数。
- **也可在运行时求值**：若参数不是 `constexpr`，则函数在运行时调用并返回非 `constexpr` 结果。
```cpp
#include <iostream>

constexpr int greater(int x, int y)
{
    return (x > y ? x : y);
}

int main()
{
    int x{ 5 };
    int y{ 6 };
    std::cout << greater(x, y) << " is greater!\n";
    return 0;
}
```
关键点：允许 `constexpr` 函数在编译时或运行时计算，避免代码重复和命名问题，这也是 C++ 不允许 `constexpr` 函数参数的原因。
- **何时在编译时求值**：根据 C++ 标准，在需要常量表达式的地方使用返回值，有资格编译时计算的 `constexpr` 函数必须在编译时计算；否则，编译器可自由选择。
```cpp
#include <iostream>

constexpr int greater(int x, int y)
{
    return (x > y ? x : y);
}

int main()
{
    constexpr int g { greater(5, 6) };            // 编译时求值
    std::cout << g << " is greater!\n";

    int x{ 5 };
    std::cout << greater(x, 6) << " is greater!\n"; // 运行时求值

    std::cout << greater(5, 6) << " is greater!\n"; // 可能编译时，可能运行时求值
    return 0;
}
```
关键点：仅在需要常量表达式处使用返回值，`constexpr` 函数才保证编译时求值，最好认为是“可在常量表达式中使用”。
最佳实践：除非有特定理由，应将可成为 `constexpr` 的函数设为 `constexpr`。
- **确定求值时机**：C++20 中，`std::is_constant_evaluated()` 可指示当前函数调用是否在常量上下文中执行。
```cpp
#include <type_traits> 

constexpr int someFunction()
{
    if (std::is_constant_evaluated()) 
        // do something
    else 
        // do something else  
}
```
- **强制编译时计算**：C++20 前无标准方法，C++20 前常用返回值初始化 `constexpr` 变量，C++20 有更好方法。

#### 3. `consteval` 函数（C++20 标准）
- **必须在编译时求值**：使用 `consteval` 关键字的函数为即时函数，必须在编译时求值，否则编译错误。
```cpp
#include <iostream>

consteval int greater(int x, int y) 
{
    return (x > y ? x : y);
}

int main()
{
    constexpr int g { greater(5, 6) };              // ok: 在编译时求值
    std::cout << g << '\n';

    std::cout << greater(5, 6) << " is greater!\n"; // ok: 在编译时求值

    int x{ 5 }; 
    std::cout << greater(x, 6) << " is greater!\n"; // error: consteval 函数必须在编译时求值
    return 0;
}
```
最佳实践：因性能等原因函数必须编译时运行，使用 `consteval`。
- **使 `constexpr` 函数编译时执行**：`consteval` 函数不灵活，可通过辅助函数让 `constexpr` 函数在编译时求值。
```cpp
#include <iostream>

consteval auto compileTime(auto value)
{
    return value;
}

constexpr int greater(int x, int y) 
{
    return (x > y ? x : y);
}

int main()
{
    std::cout << greater(5, 6) << '\n';              // 可能在编译时求值
    std::cout << compileTime(greater(5, 6)) << '\n'; // 在编译时求值

    int x { 5 };
    std::cout << greater(x, 6) << '\n';              // greater函数仍可在运行时求值
    return 0;
}
```

---

下面将结合具体例子详细解释你提供的这段内容中涉及的几个关键概念。

### 1. `constexpr`/`consteval` 函数隐式内联

#### 1.1 编译器需要看到完整定义
`constexpr` 和 `consteval` 函数因为可能在编译时求值，编译器必须能在调用这些函数的所有位置看到其完整定义，前向声明是不够的。

**错误示例**：
```cpp
// 前向声明
constexpr int add(int a, int b);

int main() {
    constexpr int result = add(3, 4); // 错误，编译器此时没有看到完整定义
    return 0;
}

// 函数定义
constexpr int add(int a, int b) {
    return a + b;
}
```
在这个例子中，虽然有前向声明，但编译器在 `main` 函数中调用 `add` 函数时，没有看到完整的函数定义，会导致编译错误。

#### 1.2 隐式内联避免单定义规则问题
由于 `constexpr` 函数可能会在多个文件中被调用，若直接将定义包含在多个文件中，会违反单定义规则。而 `constexpr` 函数是隐式内联的，不受单定义规则约束。

> 内联函数遵守单定义规则的方式
> 
> 内联函数可以在多个翻译单元中重复定义，这看似违反了单定义规则，但实际上是符合规则的特殊情况。C++ 标准允许内联函数在多个翻译单元中有相同的定义，只要这些定义在每个翻译单元中都是相同的。具体体现在以下几个方面：
> 1. 定义一致性:
> 内联函数在每个翻译单元中的定义必须完全相同，包括函数体的代码、参数列表、返回类型等。如果定义不一致，就会违反单定义规则。
> 2. 隐式内联特性:
> 编译器在处理内联函数调用时，会尝试将函数体代码直接插入到调用处，而不是通过常规的函数调用机制。这意味着每个翻译单元中的内联函数定义虽然存在，但实际上不会产生多个独立的函数实例，因此不会引发链接冲突。

**示例**：
- **头文件 `math_utils.h`**
```cpp
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// constexpr 函数定义在头文件中
constexpr int multiply(int a, int b) {
    return a * b;
}

#endif
```
- **源文件 `main.cpp`**
```cpp
#include <iostream>
#include "math_utils.h"

int main() {
    constexpr int result = multiply(2, 5);
    std::cout << "Result: " << result << std::endl;
    return 0;
}
```
- **源文件 `another.cpp`**
```cpp
#include <iostream>
#include "math_utils.h"

void anotherFunction() {
    constexpr int result = multiply(3, 6);
    std::cout << "Another result: " << result << std::endl;
}
```
在这个例子中，`multiply` 函数定义在头文件中，`main.cpp` 和 `another.cpp` 都包含了该头文件。由于 `constexpr` 函数隐式内联，不会违反单定义规则。

#### 1.3 最佳实践
- **单个源文件**：在单个 `.cpp` 文件中使用的 `constexpr`/`consteval` 函数，需要在使用它们之前的地方定义。
```cpp
#include <iostream>

// 定义在使用之前
constexpr int square(int x) {
    return x * x;
}

int main() {
    constexpr int result = square(4);
    std::cout << "Square: " << result << std::endl;
    return 0;
}
```
- **多个源文件**：在多个源文件中使用的 `constexpr`/`consteval` 函数，需要将它们定义在头文件，以便包含在每个源文件中，就像上面 `math_utils.h` 的例子。

### 2. `constexpr`/`consteval` 函数的参数不是 `constexpr`，但可作其他 `constexpr` 函数的参数

#### 2.1 参数不是 `constexpr`
`constexpr` 函数的参数本身不是 `constexpr`，不能直接用于常量表达式。
```cpp
#include <iostream>

// constexpr 函数
constexpr int doubleValue(int x) {
    return x * 2;
}

int main() {
    int num = 5;
    // 错误，num 不是 constexpr，不能用于初始化 constexpr 变量
    // constexpr int result = doubleValue(num); 
    return 0;
}
```

#### 2.2 可作其他 `constexpr` 函数的参数
`constexpr` 函数的参数可以作为其他 `constexpr` 函数的参数进行编译时计算。
```cpp
#include <iostream>

// 第一个 constexpr 函数：计算平方
constexpr int square(int x) {
    return x * x;
}

// 第二个 constexpr 函数：计算平方的两倍
constexpr int doubleSquare(int y) {//constexpr函数参数y不是constexpr,但可作为其他constexpr函数的参数，如下面的square(y)
    return 2 * square(y); // y 作为参数传递给 square 函数
}

int main() {
    constexpr int num = 3;
    constexpr int result = doubleSquare(num); // 编译时求值
    std::cout << "Double square of " << num << " is " << result << std::endl;
    return 0;
}
```

### 3. `constexpr` 函数调用非常量表达式函数

#### 3.1 `constexpr` 函数在非常量上下文中调用非常量表达式函数
```cpp
#include <iostream>
#include <type_traits>

// 非常量表达式函数
int getRandomNumber() {
    return 42;
}

// 常量表达式函数
constexpr int fixedNumber() {
    return 10;
}

// constexpr 函数根据求值上下文调用不同函数
constexpr int chooseNumber() {
    if (std::is_constant_evaluated()) { // 如果在编译时求值
        return fixedNumber();
    } else { // 如果在运行时求值
        return getRandomNumber();
    }
}

int main() {
    int runtimeResult = chooseNumber(); // 运行时求值，调用 getRandomNumber
    constexpr int compileTimeResult = chooseNumber(); // 编译时求值，调用 fixedNumber

    std::cout << "Runtime result: " << runtimeResult << std::endl;
    std::cout << "Compile-time result: " << compileTimeResult << std::endl;
    return 0;
}
```

#### 3.2 `constexpr` 函数在常量上下文中不能调用非常量表达式函数
```cpp
#include <iostream>

// 非常量表达式函数
int randomValue() {
    return 5;
}

// 错误示例，无条件调用非常量表达式函数
// constexpr int badFunction() {
//     return randomValue(); // 错误：在常量上下文中不能调用
// }

// 正确示例，根据条件调用
constexpr int goodFunction(bool flag) {
    if (flag) {
        return 2; // 常量表达式结果
    } else {
        return randomValue();
    }
}

int main() {
    // 常量上下文调用，只能传入 true
    constexpr int result = goodFunction(true); 
    std::cout << "Result: " << result << std::endl;
    return 0;
}
```
在这个例子中，`badFunction` 无条件调用非常量表达式函数，在常量上下文中会出错；`goodFunction` 根据条件调用，只要在常量表达式中调用时传入合适的参数，就可以避免错误。




### 要点总结
- `constexpr` 函数返回值可在编译时计算，也可在运行时求值，仅在需要常量表达式处使用返回值才保证编译时求值。
- `consteval` 函数（C++20）必须在编译时求值，否则编译错误，可辅助 `constexpr` 函数编译时执行。
- `constexpr` 和 `consteval` 函数隐式内联，参数不是 `constexpr`，但可作为其他 `constexpr` 函数参数。
- `constexpr` 函数在非常量上下文可调用非常量表达式函数，建议避免并使用 `std::is_constant_evaluated()` 区分行为，测试用常量上下文。 