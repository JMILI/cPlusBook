### C++ 比较运算符重载详解

---

#### 一、比较运算符重载基础
C++ 中的六种比较运算符（`==`、`!=`、`<`、`>`、`<=`、`>=`）可以通过重载为类提供直观的比较逻辑。由于这些运算符**不修改左操作数且为二元操作符**，通常以**友元函数**形式重载。

---

#### 二、示例：`Car` 类的 `operator==` 和 `operator!=`
```cpp
#include <iostream>
#include <string>
#include <string_view>

class Car {
private:
    std::string m_make;
    std::string m_model;

public:
    Car(std::string_view make, std::string_view model)
        : m_make{make}, m_model{model} {}

    friend bool operator==(const Car& c1, const Car& c2);
    friend bool operator!=(const Car& c1, const Car& c2);
};

bool operator==(const Car& c1, const Car& c2) {
    return (c1.m_make == c2.m_make && c1.m_model == c2.m_model);
}

bool operator!=(const Car& c1, const Car& c2) {
    return !(c1 == c2); // 复用 operator==
}

int main() {
    Car corolla{"Toyota", "Corolla"};
    Car camry{"Toyota", "Camry"};

    if (corolla == camry) 
        std::cout << "Same model.\n"; // 不会执行
    if (corolla != camry)
        std::cout << "Different models.\n"; // 输出
    return 0;
}
```
**关键点**：
- `operator!=` 直接复用 `operator==`，减少代码冗余。
- 友元函数声明在类内，定义在类外。

---

#### 三、示例：`Cents` 类全比较运算符重载
```cpp
#include <iostream>

class Cents {
private:
    int m_cents;
public:
    Cents(int cents) : m_cents(cents) {}

    friend bool operator==(const Cents& c1, const Cents& c2);
    friend bool operator!=(const Cents& c1, const Cents& c2);
    friend bool operator<(const Cents& c1, const Cents& c2);
    friend bool operator>(const Cents& c1, const Cents& c2);
    friend bool operator<=(const Cents& c1, const Cents& c2);
    friend bool operator>=(const Cents& c1, const Cents& c2);
};

// 仅实现核心逻辑
bool operator==(const Cents& c1, const Cents& c2) {
    return c1.m_cents == c2.m_cents;
}

bool operator!=(const Cents& c1, const Cents& c2) {
    return !(c1 == c2);
}

bool operator<(const Cents& c1, const Cents& c2) {
    return c1.m_cents < c2.m_cents;
}

bool operator>(const Cents& c1, const Cents& c2) {
    return c2 < c1; // 复用 operator<
}

bool operator<=(const Cents& c1, const Cents& c2) {
    return !(c1 > c2);
}

bool operator>=(const Cents& c1, const Cents& c2) {
    return !(c1 < c2);
}

int main() {
    Cents dime{10}, nickel{5};
    if (nickel < dime) 
        std::cout << "Nickel < Dime\n"; // 输出
    return 0;
}
```

---

#### 四、最小化冗余的优化策略
通过复用核心运算符（`operator==` 和 `operator<`）实现其他运算符：

| **运算符** | **实现方式**               |
|------------|---------------------------|
| `operator!=` | `return !(a == b);`      |
| `operator>`  | `return b < a;`         |
| `operator<=` | `return !(b < a);`      |
| `operator>=` | `return !(a < b);`      |

**优化后的 `Cents` 类**：
```cpp
class Cents {
    // ... 其他代码同前 ...

    friend bool operator==(const Cents& c1, const Cents& c2) { 
        return c1.m_cents == c2.m_cents; 
    }
    friend bool operator!=(const Cents& c1, const Cents& c2) { 
        return !(c1 == c2); 
    }

    friend bool operator<(const Cents& c1, const Cents& c2) { 
        return c1.m_cents < c2.m_cents; 
    }
    friend bool operator>(const Cents& c1, const Cents& c2) { 
        return operator<(c2, c1); 
    }
    friend bool operator<=(const Cents& c1, const Cents& c2) { 
        return !(operator>(c1, c2)); 
    }
    friend bool operator>=(const Cents& c1, const Cents& c2) { 
        return !(operator<(c1, c2)); 
    }
};
```

---

#### 五、最佳实践与注意事项
1. **语义明确性**  
   仅重载对类有实际意义的运算符。例如，`Car` 类无需重载 `<` 或 `>`，但 `Cents` 类需要数值比较。

2. **避免冗余**  
   通过复用 `operator==` 和 `operator<` 实现其他运算符，减少维护成本。

3. **容器兼容性**  
   若需在标准库容器（如 `std::sort`）中使用自定义类，需至少定义 `operator<`。

4. **C++20 简化**  
   C++20 引入**太空船运算符** `operator<=>`，可自动生成所有比较运算符，进一步简化代码（需在后续课程中详细讨论）。

---

#### 六、总结
通过合理重载比较运算符，可以为自定义类型提供直观的比较逻辑：
- **友元函数形式**：适用于不修改左操作数的二元运算符。
- **复用核心逻辑**：通过 `operator==` 和 `operator<` 减少重复代码。
- **语义优先**：仅重载对类有实际意义的运算符，避免滥用。

完整代码示例可通过 [GitHub Gist](https://gist.github.com/example/comparison_operator_overloading) 获取。