### 转换构造函数和 `explicit` 关键字

在 C++ 中，**类型转换**允许编译器在不同类型之间自动进行转换。当两种类型不匹配时，编译器会尝试进行隐式类型转换。比如，以下示例中的 `int` 会被自动转换为 `double`：

```cpp
#include <iostream>

void printDouble(double d) {
    std::cout << d;
}

int main() {
    printDouble(5); // 使用 int 来调用
    return 0;
}
```

在此，`int` 类型的 5 会被隐式转换为 `double` 类型的 5.0。

#### 用户定义的转换构造函数

当涉及到用户定义的类型时，编译器没有内建规则来将基本类型转换为自定义类型。为此，我们可以定义转换构造函数。构造函数不仅可以用来显式地创建对象，还可以被用作隐式转换。例如：

```cpp
#include <iostream>

class Foo {
private:
    int m_x;
public:
    Foo(int x) : m_x(x) {}
    int getX() const { return m_x; }
};

void printFoo(Foo f) {
    std::cout << f.getX();
}

int main() {
    printFoo(5); // 输入为 int
    return 0;
}
```

在这里，`Foo(int)` 构造函数将 `int` 类型的 `5` 转换为 `Foo` 类型的对象，并将其传递给 `printFoo` 函数。

#### 转换构造函数的限制

有时，多个隐式转换可能需要同时应用，这在某些情况下会导致编译错误。例如：

```cpp
#include <iostream>
#include <string_view>

class Employee {
private:
    std::string m_name;

public:
    Employee(std::string_view name) : m_name(name) {}
    const std::string& getName() const { return m_name; }
};

void printEmployee(Employee e) {
    std::cout << e.getName();
}

int main() {
    printEmployee("Joe"); // 错误
    return 0;
}
```

在上面的代码中，`"Joe"` 是一个 C 样式字符串，它首先需要被转换为 `std::string_view`，然后再通过 `Employee(std::string_view)` 转换为 `Employee` 对象。由于编译器只会执行一次隐式转换，代码无法编译。

为了让程序正确编译，有两种解决方法：

1. 显式使用 `std::string_view` 字面值：
   ```cpp
   printEmployee("Joe"sv);
   ```

2. 显式构造 `Employee` 对象：
   ```cpp
   printEmployee(Employee{"Joe"});
   ```

### `explicit` 关键字

`explicit` 关键字的主要作用是阻止构造函数作为隐式转换的候选者。换句话说，`explicit` 会使得构造函数无法用于隐式类型转换，从而避免了某些不必要的隐式转换。

#### 示例：使用 `explicit` 防止隐式转换

考虑以下代码：

```cpp
#include <iostream>

class Dollars {
private:
    int m_dollars;

public:
    explicit Dollars(int d) : m_dollars(d) {}
    int getDollars() const { return m_dollars; }
};

void print(Dollars d) {
    std::cout << "$" << d.getDollars();
}

int main() {
    print(5); // 编译失败，因为 Dollars(int) 是 explicit
    return 0;
}
```

在这个示例中，`Dollars(int)` 被标记为 `explicit`，因此编译器不能将 `5` 隐式转换为 `Dollars` 对象，导致编译错误。

#### 显式构造和列表初始化

尽管 `explicit` 阻止了隐式转换，它仍然可以用于显式初始化对象。例如：

```cpp
Dollars d1(5);  // ok
Dollars d2{5};  // ok
```

如果需要使用 `explicit` 构造函数，但又希望避免编译错误，可以显式地传递构造函数：

```cpp
print(Dollars{5}); // ok
```

### 转换构造函数导致的意外行为

隐式转换可能会导致一些难以发现的错误。例如，在以下程序中：

```cpp
#include <iostream>

class Dollars {
private:
    int m_dollars;

public:
    Dollars(int d) : m_dollars(d) {}
    int getDollars() const { return m_dollars; }
};

void print(Dollars d) {
    std::cout << "$" << d.getDollars();
}

int main() {
    print(5); // 编译失败，因为 Dollars(int) 是 explicit
    return 0;
}
```

虽然预期的行为是打印 `5`，但编译器实际上将 `5` 隐式转换为 `Dollars` 对象。这种行为可能会让代码的作者产生困惑。

### 使用 `explicit` 防止转换

为了避免不必要的隐式转换，最好的做法是使用 `explicit` 来标记单参数构造函数。这样可以确保构造函数不会在隐式类型转换中被调用。比如：

```cpp
class Dollars {
public:
    explicit Dollars(int d) : m_dollars(d) {}
};
```

### 按值返回和 `explicit` 构造函数

当函数返回值时，C++ 允许隐式类型转换。但如果返回类型与函数返回的值不匹配，且构造函数是 `explicit`，则编译器会报错。例如：

```cpp
#include <iostream>

class Foo {
public:
    explicit Foo() {}
    explicit Foo(int x) {}
};

Foo getFoo() {
    return 5;  // 错误，不能隐式转换
}
```

此时，由于 `Foo(int)` 被标记为 `explicit`，编译器无法隐式地将 `int` 转换为 `Foo` 对象。

### `explicit` 的最佳实践

1. **默认情况下，标记单参数构造函数为 `explicit`**，以避免隐式转换。
2. **不推荐对拷贝构造函数和移动构造函数使用 `explicit`**，因为它们并不执行类型转换。
3. **如果某个构造函数的转换是语义上合理的且性能较好，可以考虑不使用 `explicit`**，例如将 C 样式字符串转换为 `std::string_view` 的构造函数。

#### 关键点总结

| 关键概念            | 说明                                                       |
|-------------------|----------------------------------------------------------|
| 转换构造函数         | 构造函数用于将一种类型的值转换为另一种类型的对象。                          |
| `explicit` 关键字   | 防止构造函数参与隐式类型转换，确保转换只发生在显式初始化时。                   |
| 隐式转换的副作用     | 隐式转换可能会导致意外行为，使用 `explicit` 可以减少这种问题的发生。           |
| 显式初始化和列表初始化 | 即使构造函数是 `explicit`，仍然可以在显式初始化和列表初始化中使用它。          |

这些做法和规则能帮助我们避免类型转换带来的不期望行为，并使代码更加明确和易于维护。