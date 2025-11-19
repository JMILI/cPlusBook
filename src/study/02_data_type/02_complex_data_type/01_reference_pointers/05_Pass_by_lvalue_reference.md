### 通过左值引用传递函数参数

在 C++ 中，左值引用不仅是理解引用的基础，它在函数参数传递中也非常有用。通过左值引用传递参数时，函数可以避免复制对象，从而提高性能，特别是在处理大对象时。左值引用传递的真正价值体现在避免不必要的复制，并允许直接修改函数参数。

#### 1. **按值传递**

首先，回顾一下函数参数传递的常见方式——按值传递：

```cpp
#include <iostream>

void printValue(int y)
{
    std::cout << y << '\n';
} // y 在这里被销毁

int main()
{
    int x { 2 };

    printValue(x); // x 按值拷贝到 y
    return 0;
}
```

在这个例子中，`x` 被传递给 `printValue()` 函数时，发生了值拷贝。`x` 的值（2）被复制到 `y` 中，而在 `printValue()` 函数结束时，`y` 被销毁。

对于基础类型（如 `int`），这种传递方式开销相对较小。然而，对于类类型（例如 `std::string`），复制可能非常昂贵，特别是当对象较大时。

#### 2. **按引用传递**

为了避免复制高开销的对象，可以使用 **按引用传递** 参数：

```cpp
#include <iostream>
#include <string>

void printValue(std::string& y) // 传递引用
{
    std::cout << y << '\n';
} // y 在这里被销毁

int main()
{
    std::string x { "Hello, world!" };

    printValue(x); // 传递 x 的引用，而不是复制它
    return 0;
}
```

在这个例子中，`x` 是一个 `std::string` 类型的对象。传递 `x` 的引用（`std::string&`）意味着没有进行复制，函数 `printValue()` 直接访问 `x`，并且只对引用进行操作。这样可以显著提高性能，尤其是当对象较大时。

#### 3. **通过引用修改参数**

通过按引用传递参数，不仅可以避免复制，还能让函数直接修改传入的原始对象：

```cpp
#include <iostream>

void addOne(int& y) // y 是 x 的引用
{
    ++y; // 修改的是 x，而不是副本
}

int main()
{
    int x { 5 };

    std::cout << "value = " << x << '\n'; // 输出 5

    addOne(x); // 增加 x 的值

    std::cout << "value = " << x << '\n'; // 输出 6
    return 0;
}
```

在这个例子中，`addOne(x)` 通过引用传递了 `x`，函数修改了 `x` 的值，因此在函数执行完后，`x` 被直接修改为 6，而不是只修改其副本。

#### 4. **按引用传递不可修改的对象**

通过 **常量引用**，我们可以在传递参数时保持对象的不可修改性，同时避免复制：

```cpp
#include <iostream>

void printValue(const int& y) // 常量引用
{
    std::cout << y << '\n';
}

int main()
{
    int x { 5 };
    printValue(x); // ok: x 是可修改的左值

    const int z { 5 };
    printValue(z); // ok: z 是常量左值

    printValue(5); // ok: 5 是右值, 临时对象
    return 0;
}
```

在这个例子中，`printValue()` 函数接受一个常量引用参数，确保不会修改传入的对象。常量引用既可以绑定到常量左值（如 `z`），也可以绑定到临时对象（如 `5`）。使用常量引用不仅避免了不必要的复制，还提供了对传入对象的保护，避免函数修改原始数据。

#### 5. **按引用传递的限制**

按引用传递有一些限制，特别是对于 **非常量引用**：

- **非常量引用只能绑定到可修改的左值**，这意味着我们不能将常量对象或右值传递给非常量引用：

```cpp
#include <iostream>

void printValue(int& y) // 非常量引用
{
    std::cout << y << '\n';
}

int main()
{
    int x { 5 };
    printValue(x); // ok: x 是可修改的左值

    const int z { 5 };
    printValue(z); // 错误: z 是不可修改的左值

    printValue(5); // 错误: 5 是右值
    return 0;
}
```

在上面的代码中，我们试图将常量 `z` 或右值 `5` 传递给非常量引用，都会导致编译错误。**非常量引用只能绑定到可修改的左值**。

#### 6. **解决方案：常量引用**

为了能够传递常量左值或右值，可以使用 **常量引用**：

```cpp
#include <iostream>

void printValue(const int& y) // 常量引用
{
    std::cout << y << '\n';
}

int main()
{
    int x { 5 };
    printValue(x); // ok: x 是可修改的左值

    const int z { 5 };
    printValue(z); // ok: z 是不可修改的左值

    printValue(5); // ok: 5 是右值
    return 0;
}
```

通过常量引用，我们可以将常量对象或右值传递给函数，同时避免不必要的复制。

### 总结

- **按引用传递参数**：避免了高成本的对象复制，并允许直接修改传入的对象。
- **按引用修改参数**：通过传递非常量引用，函数能够直接修改传入的参数值。
- **常量引用**：允许传递常量对象或右值，同时避免复制并防止修改。
- **限制**：非常量引用只能绑定到可修改的左值，不能绑定常量或右值。

理解如何有效地使用引用，尤其是常量引用，可以显著提高代码的效率和可读性，特别是在处理大对象时。