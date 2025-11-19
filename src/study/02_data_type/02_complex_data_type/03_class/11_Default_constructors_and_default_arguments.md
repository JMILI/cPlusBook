### 默认构造函数和默认参数

在C++中，构造函数用于在对象创建时初始化数据成员。对于没有任何参数的构造函数，我们称之为**默认构造函数**。默认构造函数通常用于在没有外部提供初始化参数的情况下创建对象。

#### 默认构造函数

默认构造函数是一个没有参数的构造函数。它通常是类中没有参数的构造函数定义。

**示例：**
```cpp
#include <iostream>

class Foo {
public:
    Foo() {  // 默认构造函数
        std::cout << "Foo default constructed\n";
    }
};

int main() {
    Foo foo{};  // 调用默认构造函数
    return 0;
}
```

**输出：**
```
Foo default constructed
```

在这个例子中，`Foo`类有一个默认构造函数，它打印出一条信息，表示`Foo`对象已被构造。当创建`foo`对象时，由于没有提供任何参数，它调用了默认构造函数。

#### 类类型的值初始化与默认初始化

在C++中，类类型的对象如果没有显式初始化，将进行默认初始化。默认初始化会调用类的默认构造函数。

这两种初始化方式基本上是等效的：

```cpp
Foo foo{};  // 值初始化，调用默认构造函数
Foo foo2;   // 默认初始化，调用默认构造函数
```

这里需要注意的是，值初始化会将类成员变量初始化为默认值，特别是聚合类型，而默认初始化则由构造函数定义来决定。

#### 具有默认参数的构造函数

构造函数的参数可以有默认值，这样就可以在创建对象时不提供某些参数。如果未提供某个参数，构造函数会使用默认值。

**示例：**
```cpp
#include <iostream>

class Foo {
private:
    int m_x {};
    int m_y {};

public:
    Foo(int x = 0, int y = 0) // 默认参数
        : m_x{x}, m_y{y} {
        std::cout << "Foo(" << m_x << ", " << m_y << ") constructed\n";
    }
};

int main() {
    Foo foo1{};     // 调用 Foo(int, int) 使用默认参数
    Foo foo2{6, 7}; // 调用 Foo(int, int)
    return 0;
}
```

**输出：**
```
Foo(0, 0) constructed
Foo(6, 7) constructed
```

在这里，`Foo`的构造函数有默认参数值，当没有提供参数时，使用默认值（`0, 0`）。这使得可以更灵活地创建对象。

#### 构造函数重载

C++允许构造函数重载，即同一个类可以有多个构造函数。这样可以根据不同的需求以不同的方式初始化对象。

**示例：**
```cpp
#include <iostream>

class Foo {
private:
    int m_x {};
    int m_y {};

public:
    Foo() {  // 默认构造函数
        std::cout << "Foo constructed\n";
    }

    Foo(int x, int y) // 重载构造函数
        : m_x{x}, m_y{y} {
        std::cout << "Foo(" << m_x << ", " << m_y << ") constructed\n";
    }
};

int main() {
    Foo foo1{};     // 调用默认构造函数
    Foo foo2{6, 7}; // 调用带参数的构造函数
    return 0;
}
```

**输出：**
```
Foo constructed
Foo(6, 7) constructed
```

构造函数的重载使得可以根据传递的参数选择不同的构造函数。这个例子展示了默认构造函数和带参数构造函数的调用。

#### 构造函数重载中的歧义问题

当有多个构造函数并且它们的默认参数值不同，可能会导致歧义，编译器无法确定应该使用哪个构造函数。

**示例：**
```cpp
#include <iostream>

class Foo {
private:
    int m_x {};
    int m_y {};

public:
    Foo() {  // 默认构造函数
        std::cout << "Foo constructed\n";
    }

    Foo(int x = 1, int y = 2) // 默认参数
        : m_x{x}, m_y{y} {
        std::cout << "Foo(" << m_x << ", " << m_y << ") constructed\n";
    }
};

int main() {
    Foo foo{};  // 编译错误：歧义的默认构造函数
    return 0;
}
```

在上面的代码中，`Foo`类有两个默认构造函数，其中一个没有参数，另一个有带默认参数。编译器无法决定使用哪一个构造函数，因此会导致编译错误。

#### 隐式默认构造函数

当没有显式声明构造函数时，编译器会自动生成一个**隐式默认构造函数**。它通常没有参数，且没有成员初始化。

**示例：**
```cpp
#include <iostream>

class Foo {
private:
    int m_x{};
    int m_y{};
    // 注意：没有声明构造函数
};

int main() {
    Foo foo{};  // 隐式调用默认构造函数
    return 0;
}
```

编译器会为类生成一个隐式默认构造函数，它等价于：
```cpp
public:
    Foo() {}
```

#### 使用`=default`生成显式默认构造函数

如果希望显式地告诉编译器生成默认构造函数，可以使用`=default`语法。

**示例：**
```cpp
#include <iostream>

class Foo {
private:
    int m_x{};
    int m_y{};

public:
    Foo() = default;  // 显式生成默认构造函数
    Foo(int x, int y) : m_x{x}, m_y{y} {
        std::cout << "Foo(" << m_x << ", " << m_y << ") constructed\n";
    }
};

int main() {
    Foo foo{};  // 调用 Foo() 默认构造函数
    return 0;
}
```

**输出：**
```
Foo(0, 0) constructed
```

使用`=default`可以明确告诉编译器生成默认构造函数，并保证其行为符合预期。

#### 显式默认构造函数与空的用户定义构造函数

显式默认构造函数与用户定义的空构造函数略有不同。显式默认构造函数保证对象被零初始化，而空的用户定义构造函数则不会进行初始化。

**示例：**
```cpp
#include <iostream>

class User {
private:
    int m_a;  // 没有默认值
    int m_b{};

public:
    User() {}  // 空的用户定义构造函数
    int a() const { return m_a; }
    int b() const { return m_b; }
};

class Default {
private:
    int m_a;  // 没有默认值
    int m_b{};

public:
    Default() = default;  // 显式默认构造函数
    int a() const { return m_a; }
    int b() const { return m_b; }
};

int main() {
    User user{};
    std::cout << user.a() << ' ' << user.b() << '\n';

    Default def{};
    std::cout << def.a() << ' ' << def.b() << '\n';

    return 0;
}
```

**输出：**
```
782510864 0
0 0
```

- 在`User`类中，空的构造函数没有进行零初始化，因此`m_a`的值是未定义的。
- 在`Default`类中，显式默认构造函数保证了零初始化。

### 最佳实践

- 优先使用显式默认构造函数（`=default`）而不是定义一个空构造函数。
- 默认构造函数应该只有在必要时才使用，特别是当类类型的默认初始化具有意义时。
- 使用默认构造函数时，确保类成员变量有合理的默认值，避免未初始化的成员变量导致未定义行为。

通过这些实践，可以确保代码更加清晰、健壮，并减少错误。