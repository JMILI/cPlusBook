### 引用或指针作为函数返回值概述
在 C++ 中，函数按值返回时，会将返回值的副本传递回调用方，对于类类型而言，这种复制操作成本可能很高。为避免复制成本，可使用引用或指针作为函数返回值。下面将详细介绍引用和指针作为函数返回值的相关内容。

### 引用作为返回值
#### 基本用法
通过将函数的返回值定义为引用类型，可以返回一个已经存在的对象的引用，避免复制返回值。例如：
```cpp
#include <iostream>
#include <string>

const std::string& getProgramName() {
    static const std::string s_programName{ "Calculator" };
    return s_programName;
}

int main() {
    std::cout << "This program is named " << getProgramName();
    return 0;
}
```
在上述代码中，`getProgramName` 函数返回一个 `const std::string` 类型的引用，避免了对 `s_programName` 的复制。

#### 注意事项
1. **返回对象的生命周期**：通过引用返回的对象必须在函数返回后仍然存在，否则会导致悬空引用，产生未定义行为。例如：
```cpp
#include <iostream>
#include <string>

const std::string& getProgramName() {
    const std::string programName{ "Calculator" };
    return programName;
}

int main() {
    std::cout << "This program is named " << getProgramName(); 
    return 0;
}
```
此代码中，`programName` 是具有自动存储期的局部变量，函数结束后会被销毁，返回的引用将悬空。

2. **生命周期扩展不能跨函数边界**：生命周期延长机制不能跨函数边界工作。例如：
```cpp
#include <iostream>

const int& returnByConstReference() {
    return 5; 
}

int main() {
    const int& ref{ returnByConstReference() };
    std::cout << ref; 
    return 0;
}
```
这里 `returnByConstReference` 函数返回临时对象的 `const` 引用，当返回值绑定到 `ref` 时，临时对象已被销毁，`ref` 成为悬空引用。

3. **避免返回非常量静态局部变量的引用**：返回非常量静态局部变量的引用可能会导致多个引用指向同一个对象，修改该对象会影响所有引用。并且没有标准化的方法将静态变量重置回默认状态。例如：
```cpp
#include <iostream>

const int& getNextId() {
    static int s_x{ 0 };
    ++s_x;
    return s_x;
}

int main() {
    const int& id1{ getNextId() };
    const int& id2{ getNextId() };
    std::cout << id1 << id2 << '\n';
    return 0;
}
```
此程序打印 `22`，因为 `id1` 和 `id2` 引用的是同一个对象 `s_x`。

4. **返回引用初始化普通变量会生成拷贝副本**：如果函数返回引用，且该引用用于初始化或赋值给非引用变量，则会复制返回值。例如：
```cpp
#include <iostream>

const int& getNextId() {
    static int s_x{ 0 };
    ++s_x;
    return s_x;
}

int main() {
    const int id1{ getNextId() };
    const int id2{ getNextId() };
    std::cout << id1 << id2 << '\n';
    return 0;
}
```
此程序打印 `12`，`id1` 和 `id2` 是普通变量，接收了返回引用值的拷贝。

#### 合理使用场景
1. **返回函数的引用参数**：如果通过引用将参数传递到函数中，通过引用返回该参数是安全的。例如：
```cpp
#include <iostream>
#include <string>

const std::string& firstAlphabetical(const std::string& a, const std::string& b) {
    return (a < b) ? a : b;
}

int main() {
    std::string hello{ "Hello" };
    std::string world{ "World" };
    std::cout << firstAlphabetical(hello, world) << '\n';
    return 0;
}
```
该函数通过引用返回按字母排序较小的 `std::string` 对象，避免了复制操作。

2. **右值传递给常量引用参数时返回**：当实际传递给常量引用参数的值是右值时，仍然可以通过常量引用返回该参数。例如：
```cpp
#include <iostream>
#include <string>

const std::string& foo(const std::string& s) {
    return s;
}

std::string getHello() {
    return std::string{"Hello"};
}

int main() {
    const std::string s{ foo(getHello()) };
    std::cout << s;
    return 0;
}
```

3. **调用者修改返回对象**：当从函数返回非常量引用时，调用方可以使用该引用来修改返回的值。例如：
```cpp
#include <iostream>

int& max(int& x, int& y) {
    return (x > y) ? x : y;
}

int main() {
    int a{ 5 };
    int b{ 6 };
    max(a, b) = 7;
    std::cout << a << b << '\n';
    return 0;
}
```

### 按地址返回
#### 基本用法
按地址返回与按引用返回工作原理类似，只是返回的是对象的指针。例如：
```cpp
#include <iostream>

int* getPtr(int& num) {
    return &num;
}

int main() {
    int a = 10;
    int* ptr = getPtr(a);
    std::cout << *ptr << std::endl;
    return 0;
}
```

#### 注意事项
按地址返回的对象必须在函数返回之后仍然存在，否则调用方将收到一个悬空指针。例如：
```cpp
#include <iostream>

int* getPtr() {
    int num = 10;
    return &num; 
}

int main() {
    int* ptr = getPtr();
    std::cout << *ptr << std::endl; 
    return 0;
}
```
此代码中，`num` 是局部变量，函数结束后被销毁，返回的指针成为悬空指针。

#### 优缺点
- **优点**：如果没有要返回的有效对象，可以使函数返回 `nullptr`。例如在搜索学生列表时，如果找不到匹配的学生，可以返回 `nullptr`。
- **缺点**：调用方必须记住在解引用返回值之前执行 `nullptr` 检查，否则可能会发生空指针解引用，导致未定义的行为。因此，应首选通过引用返回，除非需要返回“空对象”的能力。

综上所述，使用引用或指针作为函数返回值可以避免复制成本，但需要注意返回对象的生命周期和使用安全。 