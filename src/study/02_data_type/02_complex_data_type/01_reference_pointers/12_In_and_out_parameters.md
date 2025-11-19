### 输入和输出参数的详解

在函数调用中，参数可以分为输入参数、输出参数和输入输出参数。它们的处理方式有所不同，具体如下：

#### 输入参数
大多数情况下，函数的参数仅用于接收调用者传入的数据。输入参数通常通过 **值传递** 或 **常量引用** 传递。例如：

```cpp
#include <iostream>

void print(int x) // x 是输入参数
{
    std::cout << x << '\n';
}

void print(const std::string& s) // s 是输入参数
{
    std::cout << s << '\n';
}

int main()
{
    print(5);
    std::string s { "Hello, world!" };
    print(s);
    return 0;
}
```

- **按值传递**：会创建参数的副本，因此对参数的修改不会影响原始数据。
- **按常量引用传递**：避免了复制过程，但确保不会修改原始数据，通常用于较大对象（如`std::string`、`std::vector`等）或不希望被修改的数据。

#### 输出参数
输出参数用于函数修改并返回结果。当函数参数需要返回多个值时，可以使用输出参数，这些参数通常通过 **非常量引用** 或 **指针** 传递。例如：

```cpp
#include <cmath>    // for std::sin() 和 std::cos()
#include <iostream>

void getSinCos(double degrees, double& sinOut, double& cosOut)
{
    constexpr double pi { 3.14159265358979323846 }; 
    double radians = degrees * pi / 180.0;
    sinOut = std::sin(radians);
    cosOut = std::cos(radians);
}
 
int main()
{
    double sin { 0.0 };
    double cos { 0.0 };
    double degrees{};
    
    std::cout << "Enter the number of degrees: ";
    std::cin >> degrees;

    getSinCos(degrees, sin, cos);
 
    std::cout << "The sin is " << sin << '\n';
    std::cout << "The cos is " << cos << '\n';

    return 0;
}
```

在这个例子中，`sin` 和 `cos` 作为输出参数通过引用传递给 `getSinCos()`，在函数内被修改，并且这些修改反映到主函数中。

**缺点**：
- 需要传入一个已初始化的变量，因此不能传递临时对象。
- 不如返回值直接，因此代码可读性差，容易出错。

#### 输入/输出参数
在某些情况下，参数既作为输入又作为输出，这种参数通常被称为 **输入/输出参数**。这种参数的使用方式与输出参数相同，但它不仅能接收输入，还会修改其值并返回结果。

```cpp
void someFcn(Foo& inout)
{
    // 修改 inout
}
```

#### 使用非常量引用的场景
虽然常量引用通常是传递参数的首选方法，但在以下两种情况下，使用 **非常量引用** 可能更合适：
1. **输入输出参数**：当你需要修改传入的参数并且该修改对调用方有影响时，使用非常量引用可以更高效。
2. **对象的复制成本很高**：当你需要返回的对象很大或者复制开销较高时，传递非常量引用可以避免重复的复制。

```cpp
void generateExpensiveFoo(Foo& out)
{
    // 修改 out
}

int main()
{
    Foo foo{};
    generateExpensiveFoo(foo); // foo 在调用后会被修改
    return 0;
}
```

#### 输出参数的挑战
输出参数有时会导致一些不自然的代码，例如：

- **初始化对象**：调用方必须先初始化一个变量，再将其传递给函数。
- **不明确修改**：函数调用方可能不知道哪些参数会被修改，特别是当多个参数同时作为输入/输出参数时。此时，应通过命名约定（如添加 `Out` 后缀）来提示函数的行为。

例如，函数调用：

```cpp
getSinCos(degrees, sin, cos);
```

在这行代码中，`sin` 和 `cos` 是输出参数，但调用方不容易知道它们会被修改。如果使用按指针传递，调用方更容易看到可能会修改的数据。

#### 总结：
- **输入参数**：通常使用值或常量引用传递。
- **输出参数**：通常使用非常量引用或指针传递，允许函数修改参数值。
- **输入/输出参数**：在某些特殊情况下使用，允许函数修改并返回输入的参数。
- 使用输出参数时要注意语义清晰，可以通过传递指针或引用来避免不必要的副本。
