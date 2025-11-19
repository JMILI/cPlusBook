### 拷贝构造函数简介

拷贝构造函数是C++中用于通过现有对象初始化新对象的一种特殊构造函数。当你创建一个新的对象并使用一个已存在对象来初始化它时，拷贝构造函数会被自动调用。拷贝构造函数的作用是创建一个新对象，并使其成为已存在对象的副本。

#### 示例：拷贝构造函数的使用

考虑以下示例：

```cpp
#include <iostream>

class Fraction {
private:
    int m_numerator{ 0 };
    int m_denominator{ 1 };

public:
    // 默认构造函数
    Fraction(int numerator = 0, int denominator = 1)
        : m_numerator{ numerator }, m_denominator{ denominator } {}

    void print() const {
        std::cout << "Fraction(" << m_numerator << ", " << m_denominator << ")\n";
    }
};

int main() {
    Fraction f { 5, 3 };  // 调用 Fraction(int, int) 构造函数
    Fraction fCopy { f }; // 调用拷贝构造函数

    f.print();
    fCopy.print();

    return 0;
}
```

输出：
```
Fraction(5, 3)
Fraction(5, 3)
```

在这个例子中，`fCopy` 是通过调用拷贝构造函数来用 `f` 初始化的。拷贝构造函数将 `f` 的成员变量复制到 `fCopy` 中，从而创建了一个副本。

### 隐式拷贝构造函数

如果你没有显式定义拷贝构造函数，C++ 编译器会自动为你生成一个隐式的拷贝构造函数。默认情况下，隐式拷贝构造函数会对每个成员执行成员级初始化，将传入对象的每个成员复制到新对象的对应成员中。

在上述程序中，`Fraction fCopy { f };` 调用了编译器生成的隐式拷贝构造函数。

#### 自定义拷贝构造函数

你还可以为类显式定义自己的拷贝构造函数。例如：

```cpp
#include <iostream>

class Fraction {
private:
    int m_numerator{ 0 };
    int m_denominator{ 1 };

public:
    // 默认构造函数
    Fraction(int numerator = 0, int denominator = 1)
        : m_numerator{ numerator }, m_denominator{ denominator } {}

    // 自定义拷贝构造函数
    Fraction(const Fraction& fraction)
        : m_numerator{ fraction.m_numerator }, m_denominator{ fraction.m_denominator } {
        std::cout << "Copy constructor called\n";
    }

    void print() const {
        std::cout << "Fraction(" << m_numerator << ", " << m_denominator << ")\n";
    }
};

int main() {
    Fraction f { 5, 3 };  // 调用 Fraction(int, int) 构造函数
    Fraction fCopy { f }; // 调用 Fraction(const Fraction&) 拷贝构造函数

    f.print();
    fCopy.print();

    return 0;
}
```

输出：
```
Copy constructor called
Fraction(5, 3)
Fraction(5, 3)
```

在这个例子中，定义了一个拷贝构造函数，并在每次调用时打印出一条消息，证明拷贝构造函数确实被调用了。

### 拷贝构造函数的最佳实践

1. **只做复制操作：** 拷贝构造函数应该仅仅用于复制数据，不应该做任何其他操作。
2. **使用常量左值引用：** 拷贝构造函数的参数应该是常量左值引用 (`const T&`)，这样可以避免不必要的拷贝并提高效率。

### 按值传递与拷贝构造函数

当对象按值传递给函数时，会触发拷贝构造函数。类似地，当对象按值返回时，拷贝构造函数也会被调用。

```cpp
#include <iostream>

class Fraction {
private:
    int m_numerator{ 0 };
    int m_denominator{ 1 };

public:
    Fraction(int numerator = 0, int denominator = 1)
        : m_numerator{ numerator }, m_denominator{ denominator } {}

    Fraction(const Fraction& fraction)
        : m_numerator{ fraction.m_numerator }, m_denominator{ fraction.m_denominator } {
        std::cout << "Copy constructor called\n";
    }

    void print() const {
        std::cout << "Fraction(" << m_numerator << ", " << m_denominator << ")\n";
    }
};

void printFraction(Fraction f) {  // 按值传递
    f.print();
}

Fraction generateFraction(int n, int d) {
    Fraction f{ n, d };
    return f; // 按值返回
}

int main() {
    Fraction f { 5, 3 };
    printFraction(f); // 按值传递，调用拷贝构造函数

    Fraction f2 { generateFraction(1, 2) }; // 按值返回，调用拷贝构造函数

    printFraction(f2); // 按值传递，调用拷贝构造函数

    return 0;
}
```

输出：
```
Copy constructor called
Fraction(5, 3)
Copy constructor called
Copy constructor called
Fraction(1, 2)
```

### 显式请求默认拷贝构造函数

可以使用 `= default` 显式请求编译器为类生成默认的拷贝构造函数：

```cpp
#include <iostream>

class Fraction {
private:
    int m_numerator{ 0 };
    int m_denominator{ 1 };

public:
    Fraction(int numerator = 0, int denominator = 1)
        : m_numerator{ numerator }, m_denominator{ denominator } {}

    // 显式请求默认的拷贝构造函数
    Fraction(const Fraction& fraction) = default;

    void print() const {
        std::cout << "Fraction(" << m_numerator << ", " << m_denominator << ")\n";
    }
};

int main() {
    Fraction f { 5, 3 };
    Fraction fCopy { f };

    f.print();
    fCopy.print();

    return 0;
}
```

### 删除拷贝构造函数

有时你可能不希望某个类的对象是可复制的，可以使用 `= delete` 来删除拷贝构造函数：

```cpp
#include <iostream>

class Fraction {
private:
    int m_numerator{ 0 };
    int m_denominator{ 1 };

public:
    Fraction(int numerator = 0, int denominator = 1)
        : m_numerator{ numerator }, m_denominator{ denominator } {}

    // 删除拷贝构造函数
    Fraction(const Fraction& fraction) = delete;

    void print() const {
        std::cout << "Fraction(" << m_numerator << ", " << m_denominator << ")\n";
    }
};

int main() {
    Fraction f { 5, 3 };
    // 以下会导致编译错误，因为拷贝构造函数已删除
    Fraction fCopy { f };

    return 0;
}
```

### 高级讨论

在C++中，如果一个类需要用户定义的拷贝构造函数、析构函数或拷贝赋值运算符，那么它可能需要所有这三个函数。此外，在C++11及以后的版本中，还引入了移动构造函数和移动赋值运算符。

对于涉及动态内存分配的类，拷贝构造函数的定义尤为重要，因为它可能需要执行深拷贝，而不仅仅是简单的成员级复制。