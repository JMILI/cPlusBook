### C++ 继承与访问说明符详解

---

#### 一、访问说明符回顾
在继承体系中，访问说明符控制成员的可访问性：
- **`public`**：任何代码均可访问。
- **`protected`**：仅类自身、友元及派生类可访问。
- **`private`**：仅类自身及友元可访问。

```cpp
class Base {
public:
    int m_public;    // 公开访问
protected:
    int m_protected; // 派生类和友元可访问
private:
    int m_private;   // 仅基类和友元访问
};
```

---

#### 二、继承类型对成员访问权限的影响
派生类的继承方式（`public`、`protected`、`private`）决定基类成员在派生类中的访问权限变化。

##### 1. **`public` 继承**（最常用）
- **规则**：
    - `public` → `public`
    - `protected` → `protected`
    - `private` → 不可访问。
- **用途**：明确表达 "is-a" 关系，保留基类接口。

```cpp
class DerivedPublic : public Base {
public:
    void accessDemo() {
        m_public = 1;    // ✅ 允许（public）
        m_protected = 2; // ✅ 允许（protected）
        // m_private = 3; // ❌ 不可访问
    }
};
```

##### 2. **`protected` 继承**（极少使用）
- **规则**：
    - `public` → `protected`
    - `protected` → `protected`
    - `private` → 不可访问。
- **用途**：隐藏基类接口，仅允许派生类进一步继承。

```cpp
class DerivedProtected : protected Base {
public:
    void accessDemo() {
        m_public = 1;    // ✅ 允许（protected）
        m_protected = 2; // ✅ 允许（protected）
        // m_private = 3; // ❌ 不可访问
    }
};
```

##### 3. **`private` 继承**（较少使用）
- **规则**：
    - `public` → `private`
    - `protected` → `private`
    - `private` → 不可访问。
- **用途**：实现细节复用，不暴露基类接口。

```cpp
class DerivedPrivate : private Base {
public:
    void accessDemo() {
        m_public = 1;    // ✅ 允许（private）
        m_protected = 2; // ✅ 允许（private）
        // m_private = 3; // ❌ 不可访问
    }
};
```

---

#### 三、访问权限变化总结表
| 基类成员访问说明符 | `public` 继承后 | `protected` 继承后 | `private` 继承后 |
|---------------------|------------------|--------------------|-------------------|
| `public`            | `public`         | `protected`        | `private`         |
| `protected`         | `protected`      | `protected`        | `private`         |
| `private`           | 不可访问         | 不可访问           | 不可访问          |

---

#### 四、多层继承的访问权限示例
```cpp
class Base {
public:
    int m_public;
protected:
    int m_protected;
private:
    int m_private;
};

class D2 : private Base { // 私有继承
public:
    int m_public2;
protected:
    int m_protected2;
private:
    int m_private2;
};

class D3 : public D2 { // 公有继承D2
public:
    void accessDemo() {
        m_public2 = 1;    // ✅ D2的public成员
        m_protected2 = 2; // ✅ D2的protected成员
        // m_private2 = 3; // ❌ 不可访问

        // 来自Base的成员：
        // m_public = 1;    // ❌ 在D2中变为private
        // m_protected = 2; // ❌ 在D2中变为private
    }
};
```
- **关键点**：
    - `D2` 私有继承 `Base`，导致 `Base` 的成员在 `D2` 中变为 `private`。
    - `D3` 继承 `D2` 时，无法访问 `Base` 的原始成员。

---

#### 五、何时使用 `protected` 成员
- **优点**：允许派生类直接访问，简化代码。
- **缺点**：破坏封装性，基类修改可能影响所有派生类。
- **建议**：
    - 优先使用 `private`，通过公有接口暴露必要功能。
    - 仅在派生类需要频繁访问基类内部时使用 `protected`。

---

#### 六、关键注意事项
1. **默认继承类型**：`class Derived : Base {}` 默认为 `private` 继承。
2. **构造函数与析构函数**：
    - 派生类需显式调用基类构造函数。
    - 基类析构函数应声明为 `virtual`（多态场景下）。
3. **访问权限传递**：
    - 派生类无法提升基类成员的访问权限（如将 `private` 变为 `public`）。
    - 继承方式仅影响派生类及其后续继承链。

---

#### 七、综合示例
```cpp
#include <iostream>

class Animal {
public:
    void eat() { std::cout << "Eating...\n"; }
protected:
    void sleep() { std::cout << "Sleeping...\n"; }
private:
    void breathe() { std::cout << "Breathing...\n"; }
};

class Dog : public Animal {
public:
    void bark() { 
        eat();    // ✅ 允许（public→public）
        sleep();  // ✅ 允许（protected→protected）
        // breathe(); // ❌ 不可访问
    }
};

class GuardDog : private Dog { // 私有继承Dog
public:
    void guard() {
        bark();   // ✅ 允许（Dog的public→private）
        // eat(); // ❌ 在GuardDog中变为private
    }
};

int main() {
    Dog dog;
    dog.eat();     // ✅ 允许
    // dog.sleep(); // ❌ 不可访问（protected）

    GuardDog guard;
    // guard.bark(); // ❌ 在GuardDog中变为private
    return 0;
}
```

---

#### 八、总结
- **`public` 继承**：用于 "is-a" 关系，保留基类接口。
- **`private` 继承**：实现细节复用，隐藏基类接口。
- **`protected` 继承**：罕见，用于特定多层继承场景。
- **访问权限管理**：优先使用 `private` 和公有接口，谨慎使用 `protected`。

通过合理选择继承类型和访问说明符，可以构建灵活且维护性高的类层次结构。