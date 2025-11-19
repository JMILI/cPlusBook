### 函数模板实例化概述
函数模板本身不是函数，其代码不会直接编译或执行，它的主要作用是生成函数。使用函数模板时，需要进行函数模板实例化，即从函数模板（具有模板类型）创建函数（具有特定类型）的过程。

### 使用函数模板
#### 显式指定模板参数
要使用函数模板，可以使用 `max<实际类型>(参数一, 参数二)` 的语法进行函数调用，其中尖括号中的类型为模板参数，指定了将用于代替模板类型 `T` 的实际类型。例如：
```cpp
#include <iostream>

template <typename T>
T max(T x, T y) {
    return (x < y) ? y : x;
}

int main() {
    std::cout << max<int>(1, 2) << '\n'; // 实例化并调用函数 max<int>(int, int)
    return 0;
}
```
当编译器遇到 `max<int>(1, 2)` 时，若 `max<int>(int, int)` 的函数定义不存在，编译器将使用 `max<T>` 函数模板创建该函数。实例化过程中，编译器会克隆函数模板，并用指定的实际类型（如 `int`）替换模板类型（`T`）。

#### 函数模板实例化规则
- 函数模板仅在每个转换单元中首次进行函数调用时实例化，对函数的进一步调用会路由到已经实例化的函数。
- 如果没有对函数模板进行函数调用，则函数模板不会在该转换单元中实例化。

### 模板参数推导
在大多数情况下，如果函数调用中的参数类型与要实例化的实际类型匹配，可以使用模板参数推导，让编译器从函数调用中的参数类型推断出应该使用的实际类型。有两种常见的写法：
```cpp
std::cout << max<>(1, 2) << '\n';
std::cout << max(1, 2) << '\n';
```
两者的区别在于编译器解析函数调用的方式：
- `max<>(1, 2)`：编译器在确定要调用哪个重载函数时，仅考虑 `max<int>` 模板函数重载。
- `max(1, 2)`：编译器将同时考虑 `max<int>` 模板函数重载和 `max` 非模板函数重载。

示例代码：
```cpp
#include <iostream>

template <typename T>
T max(T x, T y) {
    std::cout << "called max<int>(int, int)\n";
    return (x < y) ? y : x;
}

int max(int x, int y) {
    std::cout << "called max(int, int)\n";
    return (x < y) ? y : x;
}

int main() {
    std::cout << max<int>(1, 2) << '\n'; // 调用max<int>(int, int)
    std::cout << max<>(1, 2) << '\n';    // 推导 max<int>(int, int) (不考虑非模版函数)
    std::cout << max(1, 2) << '\n';      // 调用 max(int, int)
    return 0;
}
```
通常推荐使用 `max(1, 2)` 这种正常的函数调用语法，原因如下：
- 语法更简洁。
- 很少同时具有匹配的非模板函数和函数模板。
- 如果同时存在匹配的非模板函数和匹配的函数模板，通常更喜欢调用非模板函数，因为非模板函数可以针对特定类型进行更优化的实现。

### 具有非模板参数的函数模板
可以创建同时具有模板参数和非模板参数的函数模板，模板类型参数可以与任何类型匹配，而非模板参数的工作方式类似于普通函数的参数。例如：
```cpp
template <typename T>
int someFcn(T, double) {
    return 5;
}

int main() {
    someFcn(1, 3.4); // 匹配 someFcn(int, double)
    someFcn(1, 3.4f); // 匹配 someFcn(int, double) -- float 被提升为 double
    someFcn(1.2, 3.4); // 匹配 someFcn(double, double)
    someFcn(1.2f, 3.4); // 匹配 someFcn(float, double)
    someFcn(1.2f, 3.4f); // 匹配 someFcn(float, double) -- float 被提升为 double
    return 0;
}
```

### 实例化函数的问题
#### 实例化函数不一定能通过编译
编译器会根据函数模板实例化函数，但实例化后的函数不一定能通过编译。例如：
```cpp
#include <iostream>
#include <string>

template <typename T>
T addOne(T x) {
    return x + 1;
}

int main() {
    std::string hello{ "Hello, world!" };
    std::cout << addOne(hello) << '\n'; // 编译错误，因为 std::string 不能直接加 1
    return 0;
}
```

#### 实例化函数在语义上可能无意义
编译器会实例化和编译语法上有效的函数模板，但无法检查其语义是否有意义。例如：
```cpp
#include <iostream>

template <typename T>
T addOne(T x) {
    return x + 1;
}

int main() {
    std::cout << addOne("Hello, world!") << '\n'; // 语义无意义，但可能编译通过
    return 0;
}
```
因此，开发者有责任确保使用有意义的参数调用函数模板。对于高级读者，可以使用函数模板特化和 `=delete` 来禁止实例化具有某些参数的函数模板。

### 在多个文件中使用函数模板
如果将函数模板的声明和定义分别放在不同的源文件中，可能会出现链接器错误。例如：
```cpp
// main.cpp
#include <iostream>

template <typename T>
T addOne(T x); // 函数模版前向声明

int main() {
    std::cout << addOne(1) << '\n';
    std::cout << addOne(2.3) << '\n';
    return 0;
}

// add.cpp
template <typename T>
T addOne(T x) {
    return x + 1;
}
```
上述代码会出现链接器错误，因为在 `main.cpp` 中编译器无法看到函数模板 `addOne` 的定义，无法实例化这些函数；而在 `add.cpp` 中没有使用该模板，编译器也不会实例化任何内容。

解决此问题的传统方法是将所有模板代码放在头（`.h`）文件中，而不是源（`.cpp`）文件：
```cpp
// add.h
#ifndef ADD_H
#define ADD_H

template <typename T>
T addOne(T x) {
    return x + 1;
}

#endif

// main.cpp
#include "add.h"
#include <iostream>

int main() {
    std::cout << addOne(1) << '\n';
    std::cout << addOne(2.3) << '\n';
    return 0;
}
```
这样，任何需要访问模板的文件都可以 `include` 相关的头文件，编译器就能实例化所需的任何函数。模板定义不受单定义规则中每个程序只需要对象一个定义的限制，从函数模板隐式实例化的函数是隐式内联的，因此可以在多个文件中定义它们，只要每个定义都相同。

### 泛型编程
由于模板类型可以替换为任何实际类型，模板类型有时称为泛型类型。使用模板编程有时被称为泛型编程，它让我们能够专注于算法的逻辑和数据结构的设计，而不必太担心类型信息。

### 函数模板的优缺点
#### 优点
- 减少代码维护工作和错误：通过最小化需要编写和维护的代码量，函数模板可以显著减少代码维护工作和错误。
#### 缺点
- 代码膨胀和编译时间变慢：编译器会为每个具有唯一参数类型的函数调用创建（和编译）一个函数，可能导致代码膨胀和编译时间变慢。
- 难读的错误消息：函数模板往往会产生疯狂、不可读的错误消息，比常规函数的编译错误消息更难破译。

总体而言，与模板为编程工具包带来的功能和安全性相比，这些缺点相对较小，可以在任何需要类型灵活性的地方自由使用模板。一个好的经验法则是首先创建普通函数，然后如果发现需要不同参数类型的重载，则将它们转换为函数模板。 