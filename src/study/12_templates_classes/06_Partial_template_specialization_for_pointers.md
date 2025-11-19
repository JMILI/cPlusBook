

### 指针的部分模板特化详解

---

#### **1. 问题背景**
当模板类 `Storage<T>` 处理指针类型时（如 `T = int*`），直接存储指针会导致打印时输出地址而非实际值。需要定制指针类型的处理方式，同时确保资源管理安全。

---

#### **2. 完全模板特化的局限性**
为特定指针类型（如 `double*`）完全特化虽可行，但需为所有可能的指针类型重复代码，不可扩展。

```cpp
template<>
void Storage<double*>::print() {
    if (m_value) std::cout << *m_value << '\n';
}
// 无法处理 int*, char* 等其他指针类型
```

---

#### **3. 部分模板特化的实现**
通过部分特化类模板，处理所有指针类型 `T*`，无论 `T` 是什么。

##### **步骤**：
1. **定义基础模板**：处理非指针类型。
2. **定义部分特化模板**：处理指针类型 `T*`。

```cpp
// 基础模板（处理非指针类型）
template <typename T>
class Storage {
private:
    T m_value;
public:
    Storage(T value) : m_value(value) {}
    void print() { std::cout << m_value << '\n'; }
};

// 部分特化模板（处理指针类型 T*）
template <typename T>
class Storage<T*> {
private:
    std::unique_ptr<T> m_value; // 使用智能指针管理资源
public:
    Storage(T* value) : m_value(value ? new T(*value) : nullptr) {}
    void print() {
        if (m_value) std::cout << *m_value << '\n';
    }
};
```

---

#### **4. 解决资源管理问题**
• **问题**：若直接存储原始指针，可能导致悬空引用。
• **解决方案**：在部分特化类中使用 `std::unique_ptr`，拷贝指针指向的值，确保资源安全。

**示例**：
```cpp
int main() {
    int x = 42;
    Storage<int> s1(x);      // 存储 int，输出 42
    Storage<int*> s2(&x);    // 存储 int*，拷贝 x 的值
    s2.print();              // 输出 42
    x = 100;                 // 修改原变量
    s2.print();              // 仍输出 42（独立拷贝）
}
```

---

#### **5. 禁止指针类型的备用方案**
若需禁止用户传入指针，可用 `static_assert` 在编译期检查。

```cpp
template <typename T>
class Storage {
    static_assert(!std::is_pointer_v<T>, "Pointers are not allowed!");
    // 其余代码...
};
```

---

#### **6. 代码示例与解释**

##### **基础模板（非指针）**
```cpp
template <typename T>
class Storage {
private:
    T m_value;
public:
    Storage(T value) : m_value(value) {}
    void print() { std::cout << m_value << '\n'; }
};
```

##### **部分特化模板（指针类型）**
```cpp
template <typename T>
class Storage<T*> {
private:
    std::unique_ptr<T> m_value; // 智能指针管理拷贝
public:
    Storage(T* value) : m_value(value ? new T(*value) : nullptr) {}
    void print() {
        if (m_value) std::cout << *m_value << '\n';
    }
};
```

##### **使用示例**
```cpp
int main() {
    double d = 3.14;
    Storage<double> s1(d);    // 调用基础模板
    Storage<double*> s2(&d); // 调用部分特化模板
    s1.print(); // 输出 3.14
    s2.print(); // 输出 3.14
}
```

---

#### **7. 关键点总结**
| **要点**               | **说明**                                                                 |
|-------------------------|--------------------------------------------------------------------------|
| **部分特化语法**        | `template <typename T> class Storage<T*> { ... };`                     |
| **资源管理**            | 使用 `std::unique_ptr` 拷贝指针值，避免悬空引用。                        |
| **行为一致性**          | 无论 `T` 是否为指针，`Storage` 类都存储值的拷贝，语义一致。               |
| **编译期检查**          | 可用 `static_assert` 禁止指针类型，增强安全性。                          |

---

#### **8. 总结**
通过部分模板特化和智能指针，可以优雅地处理指针类型，确保资源安全且代码可扩展。此方法适用于需要为特定类型（如所有指针）定制行为的场景，同时保持代码的泛用性和安全性。