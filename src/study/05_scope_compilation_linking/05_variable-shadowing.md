
# 变量遮蔽

## 一、变量名称遮挡概述
在 C++ 编程中，每个代码块都拥有独立的变量命名空间。当在嵌套的内部代码块中定义了一个与外部代码块变量同名的变量时，就会出现变量名称遮挡现象，此时嵌套块内的变量会“遮挡”外部变量，使得在嵌套块内部对该名称的引用指向内部变量而非外部变量。

## 二、局部变量名称遮挡
1. **示例代码及分析**
```cpp
#include <iostream>

int main()
{ // 外围代码块
    int apples { 5 }; // 在外围代码块中声明 apples 变量并初始化为 5

    { // 内层代码块
        // 此时 apples 指的是外围代码块中的变量
        std::cout << apples << '\n'; // 打印外围 apples 的值 5

        int apples{ 0 }; // 在嵌套代码块中重新声明 apples 变量并初始化为 0

        // 从这里开始，apples 指代内层代码块里的变量，外围的被遮挡
        apples = 10; // 对内层的 apples 赋值为 10
        std::cout << apples << '\n'; // 打印内层 apples 的值 10
    } // 内层代码块结束，其中的 apples 变量被销毁

    std::cout << apples << '\n'; // 打印外围代码块中的 apples 变量，值仍为 5

    return 0;
} // 外围代码块结束，其中的 apples 变量被销毁
```
运行该程序，输出结果为：
```
5
10
5
```
在这个程序中，首先在外部块声明了 `apples` 变量，它在内层块初始时是可见的。当在内层块重新声明同名变量后，后续对 `apples` 的操作就针对内层变量。内层块结束后，内层变量销毁，不影响外围变量的值。

2. **未重新定义的情况**
```cpp
#include <iostream>

int main()
{ // 外围代码块
    int apples{5}; // 在外围代码块中声明 apples 变量并初始化为 5

    { // 内层代码块
        // 这里的 apples 指代外部块的 apples
        std::cout << apples << '\n'; // 打印外围的 apples 的值 5

        // 此例未在内层定义 apples 变量
        apples = 10; // 对外部块的 apples 赋值为 10
        std::cout << apples << '\n'; // 打印外围的 apples 的值 10
    } // 内层代码块结束，外围代码块中的 apples 仍然存在

    std::cout << apples << '\n'; // 再次打印外围代码块中的 apples 变量，值为 10

    return 0;
} // 外围代码块结束，其中的 apples 变量被销毁
```
上述程序的输出为：
```
5
10
10
```
如果未在嵌套块内重新定义 `apples` 变量，那么嵌套块中的 `apples` 就仍然指向外部块的 `apples`，对其赋值操作会影响外部块变量的值。

## 三、遮挡全局变量
1. **局部变量遮挡全局变量示例**
```cpp
#include <iostream>
int value { 5 }; // 声明全局变量 value 并初始化为 5

void foo()
{
    // 这里使用的是全局变量 value
    std::cout << "global variable value: " << value << '\n'; 
}

int main()
{
    int value { 7 }; // 声明局部变量 value 并初始化为 7，会遮挡全局变量 value 直到本代码块结束

    ++value; // 对局部变量 value 加 1
    std::cout << "local variable value: " << value << '\n';

    foo(); // 调用 foo 函数，打印全局变量 value 的值

    return 0;
} // 局部变量 value 销毁
```
此代码的输出为：
```
local variable value: 8
global variable value: 5
```
与嵌套块中变量遮挡外部块变量类似，局部变量若与全局变量同名，会遮挡全局变量。在 `main` 函数中，对 `value` 的操作针对局部变量，而在 `foo` 函数中使用的是全局变量。

2. **访问被遮挡的全局变量**
   由于全局变量属于全局命名空间，我们可以使用无前缀的域操作符 `::` 来明确指示编译器我们要使用的是全局变量，而非局部变量。
```cpp
#include <iostream>
int value { 5 }; // 声明全局变量 value 并初始化为 5

int main()
{
    int value { 7 }; // 声明局部变量 value 并初始化为 7，会遮挡全局变量 value 直到本代码块结束
    ++value; // 对局部变量 value 加 1

    --(::value); // 对全局变量 value 减 1，括号是为了提高可读性

    std::cout << "local variable value: " << value << '\n';
    std::cout << "global variable value: " << ::value << '\n';

    return 0;
} // 局部变量 value 销毁
```
此代码的输出为：
```
local variable value: 8
global variable value: 4
```

## 四、避免变量遮挡
1. **原因**
   通常应避免局部变量的命名遮挡，因为在使用或修改变量时，容易误操作被遮挡的变量，从而导致意外错误。当出现变量遮挡情况时，部分编译器会发出警告。同理，也应避免遮挡全局变量，可通过为所有全局变量名称添加“g_”前缀来方便地避免这种情况。
2. **最佳实践**
   避免变量遮挡是良好的编程习惯。对于 GCC/G++ 用户，GCC 和 Clang 支持 `-Wshadow` 标记，若变量被遮挡会生成警告，该标记还有 `-Wshadow=global`、`-Wshadow=local` 和 `-Wshadow=compatible-local` 等子标记，具体差异可查阅 GCC 文档。而 Visual Studio 默认已启用此类警告。 