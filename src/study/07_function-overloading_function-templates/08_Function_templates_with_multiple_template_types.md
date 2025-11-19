### 具有多个模板类型的函数模板概述
在之前我们学习了简单的函数模板，例如计算两个相同类型值最大值的 `max` 函数模板。但当传递不同类型的参数调用函数模板时，会出现一些问题，下面我们详细探讨这些问题及解决方案。

### 问题引入：传递不同类型参数调用单模板类型函数模板
```cpp
#include <iostream>

template <typename T>
T max(T x, T y) {
    return (x < y) ? y : x;
}

int main() {
    std::cout << max(2, 3.5) << '\n';  // 编译失败
    return 0;
}
```
上述代码无法编译，原因是在函数调用 `max(2, 3.5)` 中传递了两种不同类型的参数（`int` 和 `double`）。编译器在进行模板参数推导时，由于函数模板 `max<T>(T, T)` 中的 `T` 只能表示单个类型，无法将其实例化为具有两种不同参数类型的函数，且模板参数推导时不会进行类型转换，所以函数调用无法解析，导致编译错误。

### 解决方案

#### 1. 使用 `static_cast` 将参数转换为匹配类型
```cpp
#include <iostream>

template <typename T>
T max(T x, T y) {
    return (x < y) ? y : x;
}

int main() {
    std::cout << max(static_cast<double>(2), 3.5) << '\n'; 
    return 0;
}
```
这种方法让调用者将参数转换为匹配类型，使两个参数都为 `double` 类型，编译器就能实例化 `max(double, double)` 来满足函数调用。但该方法不一定总能满足需求。

#### 2. 提供显式类型模板参数
```cpp
#include <iostream>

template <typename T>
T max(T x, T y) {
    return (x < y) ? y : x;
}

int main() {
    std::cout << max<double>(2, 3.5) << '\n';
    return 0;
}
```
指定显式类型模板参数后，编译器不会使用模板参数推导，而是直接实例化 `max<double>(double, double)`，并将不匹配的 `int` 参数隐式转换为 `double`。这种方法比使用 `static_cast` 更具可读性，但我们还是希望调用时不必考虑类型。

#### 3. 具有多个模板类型参数的函数模板
```cpp
#include <iostream>

template <typename T, typename U> 
T max(T x, U y) {
    return (x < y) ? y : x; 
}

int main() {
    std::cout << max(2, 3.5) << '\n';
    return 0;
}
```
通过使用两个模板类型参数 `T` 和 `U`，`x` 和 `y` 可以独立解析类型。调用 `max(2, 3.5)` 时，`T` 是 `int`，`U` 是 `double`，编译器将实例化 `max<int, double>(int, double)`。但该代码存在问题，由于返回类型为 `T`，可能会出现将 `double` 返回值窄化转换为 `int` 的情况，导致编译告警和数据丢失。

为解决返回类型问题，可使用自动返回类型：
```cpp
#include <iostream>

template <typename T, typename U>
auto max(T x, U y) {
    return (x < y) ? y : x;
}

int main() {
    std::cout << max(2, 3.5) << '\n';
    return 0;
}
```
使用 `auto` 让编译器从 `return` 语句中推断返回类型，这样函数就能很好地处理不同类型的操作数。

### 关键点
每个模板类型参数都将独立解析其类型，具有两个类型参数 `T` 和 `U` 的模板可以将 `T` 和 `U` 解析为不同的类型，也可以解析为相同的类型。

### 函数模板简化（C++20）
C++20 引入了 `auto` 关键字的新用法，当 `auto` 用作普通函数中的参数类型时，编译器会自动将函数转换为函数模板，每个 `auto` 参数都成为独立的模板类型参数，这种方法称为简写函数模板。

```cpp
auto max(auto x, auto y) {
    return (x < y) ? y : x;
}
```
上述代码是以下代码的简写：
```cpp
template <typename T, typename U>
auto max(T x, U y) {
    return (x < y) ? y : x;
}
```
在希望每个模板类型参数都是独立类型的情况下，使用简写函数模板可使代码更简洁和可读。但当希望多个自动参数为同一类型时，没有简单的缩写函数模板。

### 最佳实践
如果语言标准设置为 C++20 或更高版本，且每个 `auto` 参数应该是独立的类型，可以使用简写的函数模板。 