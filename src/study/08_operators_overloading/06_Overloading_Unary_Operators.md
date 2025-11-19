### C++ 一元运算符重载详解

---

#### 一、一元运算符重载的核心特性
1. **操作数数量**  
   一元运算符仅作用于单个操作数，如 `+`（正号）、`-`（负号）、`!`（逻辑非）。
2. **成员函数优先**  
   由于一元运算符仅处理当前对象的状态，通常以成员函数形式重载，直接访问私有成员。

---

#### 二、运算符重载实现方法
##### 1. 负号运算符 `operator-`
**功能**：返回对象的相反值或逆状态。

```cpp
class Cents {
private:
    int m_cents;
public:
    Cents(int cents) : m_cents(cents) {}
    
    // 成员函数重载负号运算符
    Cents operator-() const {
        return Cents(-m_cents); // 隐式转换 int → Cents
    }
};

// 使用示例
Cents a(5);
Cents b = -a; // b.m_cents = -5
```

**关键点**：
- **`const` 修饰符**：不修改原对象，支持对 `const` 对象操作。
- **隐式转换**：返回 `Cents` 类型时，构造函数允许隐式转换。

##### 2. 逻辑非运算符 `operator!`
**功能**：判断对象是否处于“假”状态（如默认值或无效状态）。

```cpp
class Point {
private:
    double m_x, m_y, m_z;
public:
    Point(double x=0.0, double y=0.0, double z=0.0) 
        : m_x(x), m_y(y), m_z(z) {}
    
    // 成员函数重载逻辑非运算符
    bool operator!() const {
        return (m_x == 0.0 && m_y == 0.0 && m_z == 0.0);
    }
};

// 使用示例
Point p1;
if (!p1) {
    std::cout << "p1 是原点\n"; // 输出
}
```

**关键点**：
- **语义清晰性**：`operator!` 应反映对象的逻辑“假”状态（如空、零值）。

---

#### 三、隐式转换与返回值处理
##### 隐式构造函数触发场景
```cpp
Cents operator-() const {
    return -m_cents; // int → Cents 隐式转换
}
```
- **原理**：`Cents` 构造函数接受 `int` 参数，允许隐式构造。
- **潜在风险**：若构造函数为 `explicit`，需显式返回 `Cents(-m_cents)`。

##### 显式返回类型控制
```cpp
// 显式返回类型，避免隐式转换歧义
Cents operator-() const {
    return Cents(-m_cents); // 明确构造 Cents 对象
}
```

---

#### 四、成员函数 vs 友元函数
| **场景**               | **成员函数**           | **友元函数**           |
|------------------------|-----------------------|-----------------------|
| 需要访问私有成员        | ✔️ 直接访问           | ❌ 需友元声明          |
| 操作数类型不可修改       | ❌ 无法实现（如 `int`）| ✔️ 支持               |
| 保持代码封装性          | ✔️ 优先选择           | ❌ 破坏封装           |

**示例：无法用成员函数处理非类类型**
```cpp
// 错误：成员函数只能用于类类型左操作数
int operator-() const; // 无效
```

---

#### 五、设计原则与最佳实践
1. **语义一致性**
    - `operator-` 应返回数学意义上的相反值。
    - `operator!` 应反映对象的逻辑否定状态。

2. **常量正确性**  
   使用 `const` 修饰符确保不修改原对象：

```cpp
Cents operator-() const; // 正确：不修改对象
```

3. **返回值优化**  
   返回新对象而非引用，避免悬空引用：

```cpp
Cents operator-() const {
    return Cents(-m_cents); // 正确：返回临时对象
}
```

4. **避免滥用运算符**
    - 仅在语义明确时重载（如 `operator!` 用于检查空状态）。
    - 避免赋予运算符非直观行为（如用 `operator+` 实现减法）。

---

#### 六、综合示例：`Vector3D` 类
```cpp
#include <iostream>

class Vector3D {
private:
    double x, y, z;
public:
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}
    
    // 负号运算符（成员函数）
    Vector3D operator-() const {
        return Vector3D(-x, -y, -z);
    }
    
    // 逻辑非运算符（检查零向量）
    bool operator!() const {
        return (x == 0.0 && y == 0.0 && z == 0.0);
    }
    
    // 正号运算符（返回自身副本）
    Vector3D operator+() const {
        return *this;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Vector3D& v);
};

std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

int main() {
    Vector3D v1(1.0, -2.0, 3.0);
    Vector3D v2 = -v1;
    std::cout << "v2: " << v2 << "\n"; // 输出 (-1, 2, -3)
    
    if (!Vector3D(0, 0, 0)) {
        std::cout << "零向量\n"; // 输出
    }
    
    Vector3D v3 = +v1; // 显式调用正号运算符
    return 0;
}
```

---

#### 七、总结
- **成员函数优先**：一元运算符通常作为成员函数实现，直接访问私有数据。
- **语义明确性**：确保运算符行为符合直觉（如 `operator!` 检测无效状态）。
- **常量修饰**：对不修改对象的运算符使用 `const` 修饰符。
- **返回值优化**：返回新对象而非引用，避免生命周期问题。

通过合理设计，一元运算符重载可以显著提升代码的可读性和易用性，使自定义类型的行为更接近内置类型。