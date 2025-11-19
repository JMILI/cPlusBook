### 通过指针传递函数参数

在C++中，传递参数给函数的方式有三种：**值传递**、**引用传递**和**指针传递**。这些方式分别决定了函数参数如何处理、是否会影响原始数据以及性能开销。在这篇文章中，我们将深入了解**通过指针传递参数**的机制和实践。

#### 1. 通过值传递和引用传递

首先，我们来看一下通过**值传递**和**引用传递**的例子：

```cpp
#include <iostream>
#include <string>

void printByValue(std::string val) {
    std::cout << val << '\n'; // 打印副本
}

void printByReference(const std::string& ref) {
    std::cout << ref << '\n'; // 打印引用
}

int main() {
    std::string str{ "Hello, world!" };
    
    printByValue(str); // 通过值传递，传递的是 str 的副本
    printByReference(str); // 通过引用传递，直接传递 str 本身

    return 0;
}
```

- **值传递**：传递的是 `str` 的副本。函数内部的任何更改不会影响原始的 `str`。
- **引用传递**：传递的是 `str` 的引用，函数对 `ref` 的任何更改都会影响原始的 `str`。

#### 2. 通过指针传递参数

接下来，我们将探讨**通过指针传递参数**的方式。通过指针传递时，函数接收对象的地址，函数可以解引用该指针以访问对象的值。以下是修改后的程序：

```cpp
#include <iostream>
#include <string>

void printByValue(std::string val) {
    std::cout << val << '\n'; // 通过拷贝打印
}

void printByReference(const std::string& ref) {
    std::cout << ref << '\n'; // 通过引用打印
}

void printByAddress(const std::string* ptr) {
    std::cout << *ptr << '\n'; // 通过解引用指针打印
}

int main() {
    std::string str{ "Hello, world!" };
    
    printByValue(str); // 通过值传递 str 的拷贝
    printByReference(str); // 通过引用传递 str，不会制作副本
    printByAddress(&str); // 通过地址传递 str，不会制作副本

    return 0;
}
```

在 `printByAddress` 中，我们传递了 `str` 的地址（`&str`）。这样，函数内部通过解引用指针 (`*ptr`) 访问 `str` 的值。与引用传递类似，传递指针不会进行对象拷贝。

#### 3. 通过地址传递的优点

通过地址传递具有以下优点：

- **避免对象拷贝**：如果对象较大（如 `std::string`），通过值传递会复制整个对象，这可能会影响性能。通过指针传递只会复制指针本身，避免了复制整个对象。

- **可以修改对象**：与通过引用传递类似，通过指针传递可以让函数修改传入的对象。例如，传递一个指向非常量的指针，函数可以通过该指针修改对象内容。

```cpp
#include <iostream>

void changeValue(int* ptr) {
    *ptr = 6; // 修改指针所指向的值
}

int main() {
    int x{ 5 };

    std::cout << "x = " << x << '\n';  // 输出 x 的初始值

    changeValue(&x);  // 传递 x 的地址

    std::cout << "x = " << x << '\n';  // 输出修改后的 x 的值

    return 0;
}
```

输出：

```
x = 5
x = 6
```

#### 4. 通过指针传递和常量指针

如果我们不希望函数修改传入的对象，可以使用**指向常量的指针**：

```cpp
void changeValue(const int* ptr) {
    *ptr = 6;  // 编译错误：不能修改常量对象
}
```

这样，指针 `ptr` 指向一个常量对象，编译器会禁止通过指针修改对象的值。

#### 5. 空指针检查

通过指针传递时，传入的指针可能是空指针。如果在函数中解引用空指针，将导致未定义行为。因此，必须在解引用指针之前进行空指针检查：

```cpp
#include <iostream>

void print(int* ptr) {
    if (ptr) { // 检查指针是否为空
        std::cout << *ptr << '\n';
    }
}

int main() {
    int x{ 5 };
    
    print(&x);  // 正常工作
    print(nullptr);  // 空指针，不会解引用

    return 0;
}
```

在上面的例子中，如果传入空指针，程序不会崩溃，而是直接跳过解引用操作。

#### 6. 使用断言来检查空指针

另一种检查空指针的方法是使用 `assert`，它在调试模式下会检查空指针并立即停止程序的执行：

```cpp
#include <iostream>
#include <cassert>

void print(const int* ptr) {
    assert(ptr);  // 如果 ptr 为 nullptr，程序会终止

    std::cout << *ptr << '\n';
}

int main() {
    int x{ 5 };
    
    print(&x); // 正常工作
    print(nullptr); // 程序会在此停止，报告错误

    return 0;
}
```

#### 7. 传递常量引用的优点

尽管指针传递有很多优点，**引用传递**在某些情况下更为方便和安全。引用传递可以避免指针带来的空指针问题，并且具有更直观的语法：

```cpp
void printByReference(const int& ref) {
    std::cout << ref << '\n'; // 通过引用打印
}

int main() {
    printByReference(5); // 直接传入右值
    printByReference(x); // 传入变量

    return 0;
}
```

引用传递不需要使用 `&` 获取地址，也不需要解引用，语法上更为简洁。

#### 8. 最佳实践

- **首选引用传递**：大多数情况下，使用引用传递比使用指针传递更加安全和清晰。引用传递避免了空指针问题，同时允许在不拷贝数据的情况下修改传入的对象。

- **必须时使用指针传递**：如果确实需要传递一个指针，应该确保指针有效（不为空），并且在函数中进行空指针检查。

#### 总结

通过指针传递参数可以提高程序性能，避免不必要的数据拷贝，同时也能让函数修改传入的对象。需要注意的是，指针传递会带来一些风险，如空指针解引用和指针的复杂性，因此在可能的情况下，优先选择引用传递。

- **引用传递**：避免拷贝开销，且没有空指针问题。
- **指针传递**：避免拷贝开销，可以在函数内修改对象，适用于动态内存和需要处理空指针的情况。

通过合理选择参数传递方式，可以编写高效且安全的代码。


---


### 通过指针传递函数参数（第2部分）

#### 传递地址的“可选”参数

在一些情况下，我们希望函数接受“可选”的参数，可以通过传递空指针（`nullptr`）来处理这些情况。例如，以下代码展示了如何使用指针来实现可选参数：

```cpp
#include <iostream>
#include <string>

void greet(std::string* name = nullptr)  // 默认参数为空指针
{
    std::cout << "Hello ";
    std::cout << (name ? *name : "guest") << '\n';
}

int main()
{
    greet();  // 不传递名字时，输出 "Hello guest"
    
    std::string joe{"Joe"};
    greet(&joe);  // 传递名字时，输出 "Hello Joe"
    
    return 0;
}
```

**输出：**
```
Hello guest
Hello Joe
```

解释：
- 在这个示例中，`greet()` 函数有一个默认参数`name`，它是一个指向 `std::string` 的指针，默认为 `nullptr`。
- 如果没有传递 `name`，它会打印 `"Hello guest"`。
- 如果传递了一个有效的字符串（如 `joe`），则打印 `Hello Joe`。

#### 通过指针传递修改指针参数

考虑以下示例，其中我们希望函数修改传入的指针指向 `nullptr`：

```cpp
#include <iostream>

void nullify([[maybe_unused]] int* ptr2)  // ptr2 是指针的副本
{
    ptr2 = nullptr;  // 只修改副本，原指针不受影响
}

int main()
{
    int x{5};
    int* ptr{&x};  // ptr 指向 x
    
    std::cout << "ptr is " << (ptr ? "non-null\n" : "null\n");
    
    nullify(ptr);  // 尝试将 ptr 设为 nullptr
    
    std::cout << "ptr is " << (ptr ? "non-null\n" : "null\n");
    return 0;
}
```

**输出：**
```
ptr is non-null
ptr is non-null
```

解释：
- 在此示例中，`nullify()`函数接收一个指向 `int` 的指针 `ptr2`，并将其设为 `nullptr`。
- 然而，由于 `ptr2` 是 `ptr` 的副本，因此对 `ptr2` 的修改不会影响原始的 `ptr`。即使在 `nullify()` 中将 `ptr2` 设置为 `nullptr`，`ptr` 依然指向 `x`。

#### 通过引用传递指针

如果想要修改原始指针的值，可以通过引用传递指针，而不是通过值传递指针。下面是修改指针的示例：

```cpp
#include <iostream>

void nullify(int*& refptr)  // refptr 是指向指针的引用
{
    refptr = nullptr;  // 直接修改原指针
}

int main()
{
    int x{5};
    int* ptr{&x};  // ptr 指向 x
    
    std::cout << "ptr is " << (ptr ? "non-null\n" : "null\n");
    
    nullify(ptr);  // 修改 ptr 使其指向 nullptr
    
    std::cout << "ptr is " << (ptr ? "non-null\n" : "null\n");
    return 0;
}
```

**输出：**
```
ptr is non-null
ptr is null
```

解释：
- 由于 `refptr` 是 `ptr` 的引用，因此对 `refptr` 的修改会影响原始的 `ptr`。
- 在调用 `nullify(ptr)` 后，`ptr` 被修改为 `nullptr`，并且程序输出了 `ptr is null`。

#### 为什么不再推荐使用 `0` 或 `NULL` 作为空指针

使用 `0` 或 `NULL` 表示空指针时，可能会产生模棱两可的情况。例如，`0` 可以是一个整数，也可以是空指针。某些情况下，编译器可能无法正确判断。

下面是一个示例，展示了使用 `0` 或 `NULL` 的潜在问题：

```cpp
#include <iostream>
#include <cstddef> // for NULL

void print(int x)    // 这个函数接收 int 类型的参数
{
    std::cout << "print(int): " << x << '\n';
}

void print(int* ptr) // 这个函数接收 int* 类型的参数
{
    std::cout << "print(int*): " << (ptr ? "non-null\n" : "null\n");
}

int main()
{
    int x{5};
    int* ptr{&x};

    print(ptr);  // 调用 print(int*), 输出 "non-null"
    print(0);    // 调用 print(int), 输出 "0"
    print(NULL); // 调用 print(int) 或 print(int*)，具体行为依编译器而定
    print(nullptr); // 调用 print(int*), 输出 "null"
    
    return 0;
}
```

**输出（在某些编译器下）：**
```
print(int*): non-null
print(int): 0
print(int): 0
print(int*): null
```

解释：
- `print(0)` 可能会调用 `print(int)`，而 `0` 在某些情况下会被视为整数而不是空指针。
- `print(NULL)` 可能会导致不同的行为，取决于 `NULL` 如何定义。它可能被解释为整数 `0`，或者作为空指针。
- 使用 `nullptr` 可以消除这种不确定性，因为 `nullptr` 只能与指针类型匹配。

#### 使用 `std::nullptr_t`

`std::nullptr_t` 是一个类型，它表示空指针的值 `nullptr`。以下是如何使用它的示例：

```cpp
#include <iostream>
#include <cstddef> // for std::nullptr_t

void print(std::nullptr_t)
{
    std::cout << "in print(std::nullptr_t)\n";
}

void print(int* ptr)
{
    std::cout << "in print(int*)\n";
}

int main()
{
    print(nullptr);  // 调用 print(std::nullptr_t)
    
    int x {5};
    int* ptr {&x};
    
    print(ptr); // 调用 print(int*)
    ptr = nullptr;
    print(ptr); // 调用 print(int*)
    
    return 0;
}
```

**输出：**
```
in print(std::nullptr_t)
in print(int*)
in print(int*)
```

解释：
- `print(nullptr)` 调用了 `print(std::nullptr_t)`，因为 `nullptr` 与 `std::nullptr_t` 完全匹配。
- 当 `ptr` 是 `nullptr` 时，它仍然调用 `print(int*)`，因为 `ptr` 的类型是 `int*`。

#### 总结

- **传递指针参数**：指针传递通常是为了避免拷贝大对象或允许函数修改传入的对象。如果不需要修改对象，最好使用引用。
- **空指针**：尽量使用 `nullptr` 替代 `0` 或 `NULL`，因为 `nullptr` 可以明确标识为空指针，避免类型混淆。
- **引用指针**：通过引用传递指针，可以直接修改原始指针的值。

通过理解和灵活使用这些概念，能够让你在C++中更高效地操作内存、传递参数并避免潜在的错误。
