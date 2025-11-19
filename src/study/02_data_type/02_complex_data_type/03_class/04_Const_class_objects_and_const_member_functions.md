### Const 类对象和 Const 成员函数

在 C++ 中，`const` 关键字可以用于修饰基本数据类型和类类型的对象，以确保这些对象在创建后不能被修改。`const` 的概念不仅适用于简单的变量，也同样适用于类的对象以及类的成员函数。下面我们将详细介绍如何在类对象和成员函数中使用 `const`，以及如何确保类对象的状态不会被不小心修改。

#### 1. **Const 类对象**

`const` 可以用于修饰类类型的对象，确保对象在创建后无法修改。与普通的基本数据类型类似，`const` 修饰的类对象必须在创建时初始化。

**例子：**
```cpp
struct Date {
    int year {};
    int month {};
    int day {};
};

int main() {
    const Date today { 2020, 10, 14 }; // const 类类型的对象
    // today.year = 2021;  // 编译失败，无法修改 const 对象的成员
    return 0;
}
```

在上述代码中，`today` 是一个 `const` 对象。因为它是 `const` 类型的对象，所以它的成员变量（如 `today.year`）不能在之后被修改。

#### 2. **不允许修改常量对象的数据成员**

一旦类对象被标记为 `const`，我们不能修改它的数据成员，无论是直接修改还是通过成员函数。试图修改 `const` 对象的成员变量会导致编译错误。

**示例：**
```cpp
struct Date {
    int year {};
    int month {};
    int day {};

    void incrementDay() {
        ++day;
    }
};

int main() {
    const Date today { 2020, 10, 14 }; // const 对象

    today.day += 1;        // 编译失败: 不能修改 const 对象的成员变量
    today.incrementDay();  // 编译失败: 不能调用修改 const 对象成员变量的成员函数

    return 0;
}
```

这里，`today.day += 1` 和 `today.incrementDay()` 都试图修改 `today` 这个 `const` 对象的成员，因此会导致编译错误。

#### 3. **Const 对象不能调用非 Const 成员函数**

即使成员函数本身没有修改成员变量，C++ 仍然要求 `const` 对象只能调用 `const` 成员函数。否则，编译器会报错。

**示例：**
```cpp
#include <iostream>

struct Date {
    int year {};
    int month {};
    int day {};

    void print() {
        std::cout << year << '/' << month << '/' << day;
    }
};

int main() {
    const Date today { 2020, 10, 14 }; // const 对象
    today.print();  // 编译失败: 不能调用非 const 成员函数
    return 0;
}
```

虽然 `print()` 函数本身没有修改任何成员变量，但由于它不是一个 `const` 成员函数，编译器不允许在 `const` 对象上调用它。

#### 4. **Const 成员函数**

为了允许在 `const` 对象上调用成员函数，必须将该成员函数声明为 `const`。这意味着该成员函数承诺不会修改对象的任何成员。

**示例：**
```cpp
#include <iostream>

struct Date {
    int year {};
    int month {};
    int day {};

    void print() const {  // 现在是 const 成员函数
        std::cout << year << '/' << month << '/' << day;
    }
};

int main() {
    const Date today { 2020, 10, 14 }; // const 对象
    today.print();  // ok: const 对象可以调用 const 成员函数
    return 0;
}
```

通过将 `print()` 函数声明为 `const`，它现在可以在 `const` 对象上正常调用。

#### 5. **Const 成员函数的限制**

如果一个成员函数被标记为 `const`，它不能修改任何数据成员，也不能调用其他非 `const` 成员函数。这是因为 `const` 成员函数被承诺不修改对象的状态。

**示例：**
```cpp
struct Date {
    int year {};
    int month {};
    int day {};

    void incrementDay() const {  // 编译错误: const 成员函数不能修改成员变量
        ++day;  // 不能修改成员变量
    }
};

int main() {
    const Date today { 2020, 10, 14 }; // const 对象
    today.incrementDay();  // 编译失败
    return 0;
}
```

`incrementDay()` 被声明为 `const`，但它试图修改 `day`，这违反了 `const` 成员函数的规则。

#### 6. **可以在非常量对象上调用 Const 成员函数**

`const` 成员函数不仅可以在 `const` 对象上调用，也可以在非常量对象上调用。对于不修改对象状态的成员函数，建议将其标记为 `const`，这使得它们可以在常量和非常量对象上共享。

**示例：**
```cpp
#include <iostream>

struct Date {
    int year {};
    int month {};
    int day {};

    void print() const {  // const 成员函数
        std::cout << year << '/' << month << '/' << day;
    }
};

int main() {
    Date today { 2020, 10, 14 }; // 非 const 对象
    today.print();  // 可以在非常量对象上调用 const 成员函数
    return 0;
}
```

#### 7. **通过常量引用传递常量对象**

将常量对象通过常量引用传递给函数是另一种常见的做法。这样做的好处是避免了不必要的对象复制，同时保证了对象在函数内部不会被修改。

**示例：**
```cpp
#include <iostream>

struct Date {
    int year {};
    int month {};
    int day {};

    void print() const {
        std::cout << year << '/' << month << '/' << day;
    }
};

void doSomething(const Date &date) {
    date.print();  // ok: 在常量引用上调用 const 成员函数
}

int main() {
    Date today { 2020, 10, 14 }; 
    today.print();

    doSomething(today);

    return 0;
}
```

在这个例子中，`doSomething` 函数通过常量引用接收一个 `const` 对象，确保对象不会被修改。由于 `print()` 是 `const` 成员函数，因此它可以在常量引用上正常调用。

#### 8. **成员函数常量和非常量重载**

C++ 允许我们为同一个成员函数提供 `const` 和非 `const` 两个重载版本，这样就能根据对象是否是常量来选择不同的函数。

**示例：**
```cpp
#include <iostream>

struct Something {
    void print() {
        std::cout << "non-const\n";
    }

    void print() const {
        std::cout << "const\n";
    }
};

int main() {
    Something s1{};
    s1.print(); // 调用非 const 版本

    const Something s2{};
    s2.print(); // 调用 const 版本
    
    return 0;
}
```

在这个例子中，`Something` 类有两个 `print()` 成员函数，一个是常量版本，另一个是非常量版本。通过 `const` 和非 `const` 对象的不同调用，C++ 自动选择合适的版本。

---

### 总结

| **关键点**                         | **描述**                                                       |
|-------------------------------------|---------------------------------------------------------------|
| **Const 类对象**                    | `const` 修饰的类对象在初始化后不能修改其数据成员。                       |
| **Const 成员函数**                  | 被标记为 `const` 的成员函数保证不修改对象的任何成员，可以在 `const` 对象上调用。 |
| **非 Const 成员函数**                | `const` 对象不能调用非 `const` 成员函数，因为它们可能修改对象状态。      |
| **通过常量引用传递对象**            | 通过常量引用传递对象可以避免对象副本，并确保对象在函数内部不会被修改。   |
| **成员函数重载**                    | 可以为同一个函数提供 `const` 和非 `const` 的重载版本，根据对象类型来调用。|

