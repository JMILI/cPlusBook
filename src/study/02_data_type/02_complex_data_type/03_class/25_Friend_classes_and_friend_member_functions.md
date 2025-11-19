### 友元类和友元成员函数

在C++中，`friend` 关键字提供了对封装性的灵活控制，允许某些类或函数绕过类的访问控制机制（`private` 和 `protected`）。这为实现复杂的功能提供了更多的自由度。

### 1. 友元类（Friend Classes）

**定义：**
友元类是被授予访问另一个类的私有和受保护成员的类。通常，类的成员是不能直接访问其私有数据的，但如果它是另一个类的友元类，它就可以访问该类的所有成员。

**代码示例：**

```cpp
#include <iostream>

class Storage {
private:
    int m_nValue {};
    double m_dValue {};
public:
    Storage(int nValue, double dValue)
       : m_nValue { nValue }, m_dValue { dValue }
    { }

    // 设置 Display 是 Storage 的友元类
    friend class Display; // 友元类声明
};

class Display {
private:
    bool m_displayIntFirst {};

public:
    Display(bool displayIntFirst)
         : m_displayIntFirst { displayIntFirst }
    {
    }

    // Display 可以访问 Storage 的所有成员
    void displayStorage(const Storage& storage)
    {
        if (m_displayIntFirst)
            std::cout << storage.m_nValue << ' ' << storage.m_dValue << '\n';
        else // 首先显示 double 值
            std::cout << storage.m_dValue << ' ' << storage.m_nValue << '\n';
    }

    void setDisplayIntFirst(bool b)
    {
         m_displayIntFirst = b;
    }
};

int main() {
    Storage storage { 5, 6.7 };
    Display display { false };

    display.displayStorage(storage);

    display.setDisplayIntFirst(true);
    display.displayStorage(storage);

    return 0;
}
```

**输出：**

```
6.7 5
5 6.7
```

**注意事项：**

- **非对等性：** 友元关系不是对等的。如果`A`是`B`的友元，`B`并不是`A`的友元。也就是说，友元关系是单向的。
- **不可传递：** 如果`A`是`B`的友元，`B`是`C`的友元，并不意味着`A`是`C`的友元。
- **类前向声明：** 友元类可以同时作为前向声明，这意味着在将类绑定为友元之前不必显式声明该类。

### 2. 友元成员函数（Friend Member Functions）

友元成员函数是指定某个类的特定成员函数可以访问另一个类的私有成员。通过这种方式，可以允许某些函数访问类的内部数据，而不暴露给外部世界。

**代码示例：**

```cpp
#include <iostream>

class Display; // 前向声明 Display

class Storage {
private:
    int m_nValue {};
    double m_dValue {};
public:
    Storage(int nValue, double dValue)
        : m_nValue { nValue }, m_dValue { dValue }
    {
    }

    // 让 Display::displayStorage 成员函数成为 Storage 的友元函数
    friend void Display::displayStorage(const Storage& storage); // 编译失败: Storage 这里看不到 Display 类的定义
};

class Display {
private:
    bool m_displayIntFirst {};

public:
    Display(bool displayIntFirst)
        : m_displayIntFirst { displayIntFirst }
    {
    }

    void displayStorage(const Storage& storage)
    {
        if (m_displayIntFirst)
            std::cout << storage.m_nValue << ' ' << storage.m_dValue << '\n';
        else
            std::cout << storage.m_dValue << ' ' << storage.m_nValue << '\n';
    }
};

int main() {
    Storage storage { 5, 6.7 };
    Display display { false };
    display.displayStorage(storage);

    return 0;
}
```

**问题及解决：**

1. 由于友元成员函数需要完整的类定义，编译器必须在引用该成员函数时已经看到类的完整定义。如果类的定义顺序不对，编译器就无法识别成员函数的友元关系。
2. 解决方案：通过将类的完整定义提前，或者通过前向声明类来解决这一问题。

### 3. 解决友元成员函数的编译问题

为了使友元成员函数编译通过，我们需要确保类的完整定义在友元函数声明时已经可用。以下是经过修改的代码：

```cpp
#include <iostream>

class Storage; // 前向声明 Storage

class Display {
private:
    bool m_displayIntFirst {};

public:
    Display(bool displayIntFirst)
        : m_displayIntFirst { displayIntFirst }
    {
    }

    void displayStorage(const Storage& storage); // 这一行需要看到 Storage 的前向声明
};

class Storage {
private:
    int m_nValue {};
    double m_dValue {};
public:
    Storage(int nValue, double dValue)
        : m_nValue { nValue }, m_dValue { dValue }
    {
    }

    // 让 Display::displayStorage 成员函数成为 Storage 的友元函数
    friend void Display::displayStorage(const Storage& storage);
};

// 现在来定义 Display::displayStorage
// 需要看到 Storage 的完整定义 (因为要访问 Storage 的成员)
void Display::displayStorage(const Storage& storage)
{
    if (m_displayIntFirst)
        std::cout << storage.m_nValue << ' ' << storage.m_dValue << '\n';
    else
        std::cout << storage.m_dValue << ' ' << storage.m_nValue << '\n';
}

int main() {
    Storage storage { 5, 6.7 };
    Display display { false };
    display.displayStorage(storage);

    return 0;
}
```

### 4. 总结友元类与友元成员函数的区别

| **特性**                | **友元类（Friend Class）**                                  | **友元成员函数（Friend Member Function）**                       |
|---------------------|--------------------------------------------------|------------------------------------------------------|
| **作用**              | 允许整个类访问另一个类的私有成员。                        | 允许特定的成员函数访问另一个类的私有成员。                        |
| **应用场景**           | 当需要多个成员函数访问私有成员时，使用友元类更方便。            | 当只有一个成员函数需要访问私有成员时，使用友元成员函数。             |
| **声明位置**           | 在类的内部使用`friend class`声明。                          | 在类的内部使用`friend`关键字声明特定的成员函数。                   |
| **访问范围**           | 友元类可以访问目标类的所有私有成员。                        | 友元成员函数可以访问目标类的私有成员，但其他成员函数无法访问。         |
| **非对等性**            | 友元关系是单向的，需要显式声明对方为友元。                       | 友元关系是单向的。                                                  |

### 5. 友元的限制

- **友元不可继承：** 如果一个类`B`是类`A`的友元，类`B`的子类不会自动成为`A`的友元。
- **友元不可传递：** 即便类`B`是类`A`的友元，类`C`是类`B`的友元，类`A`也不会自动成为`C`的友元。
- **友元是由目标类授予的：** 友元关系是由被访问的类（即目标类）授予的，而不是由访问者类（即友元类）授予。

通过理解友元类和友元成员函数，开发者可以更精细地控制哪些类或函数能够访问私有数据，从而有效地管理封装和数据保护的平衡。