### C++ 继承中隐藏与修改基类功能的机制详解

---

#### 一、使用 `using` 声明修改访问权限
通过 `using` 声明，可以在派生类中调整基类成员的访问级别，但需满足派生类对基类成员的原访问权限。

##### 1. 提升基类成员的访问权限
**适用场景**：将基类的 `protected` 成员在派生类中设为 `public`。

```cpp
class Base {
protected:
    void printValue() const { std::cout << "Base"; }
};

class Derived : public Base {
public:
    using Base::printValue; // 提升为 public
};

int main() {
    Derived d;
    d.printValue(); // 直接访问，输出 "Base"
    return 0;
}
```

##### 2. 降低基类成员的访问权限
**适用场景**：将基类的 `public` 成员在派生类中设为 `private`。

```cpp
class Base {
public:
    int m_value;
};

class Derived : public Base {
private:
    using Base::m_value; // 降级为 private
};

int main() {
    Derived d;
    // d.m_value = 5;        // 错误：m_value 在 Derived 中是 private
    static_cast<Base&>(d).m_value = 5; // 合法：通过基类引用访问
    return 0;
}
```

##### 3. 注意事项
- **无法修改基类 `private` 成员**：派生类无权访问基类 `private` 成员，因此无法调整其访问级别。
- **类型转换可绕过限制**：通过将派生类对象转换为基类引用或指针，仍可访问基类原始权限的成员。

---

#### 二、隐藏基类功能的方法
##### 1. 隐藏所有重载函数
使用 `using` 声明可批量修改基类同名重载函数的访问权限。

```cpp
class Base {
public:
    void func(int) {}
    void func(double) {}
};

class Derived : public Base {
private:
    using Base::func; // 所有 func 重载变为 private
};

int main() {
    Derived d;
    // d.func(1);    // 错误：func 在 Derived 中是 private
    d.Base::func(1); // 合法：显式调用基类版本
    return 0;
}
```

##### 2. 删除派生类中的特定函数
通过 `= delete` 阻止调用派生类的某个函数，但基类版本仍可访问。

```cpp
class Base {
public:
    void doWork() {}
};

class Derived : public Base {
public:
    void doWork() = delete; // 禁止调用派生类版本
};

int main() {
    Derived d;
    // d.doWork();          // 错误：doWork 被删除
    d.Base::doWork();       // 合法：调用基类版本
    static_cast<Base&>(d).doWork(); // 合法：通过基类引用调用
    return 0;
}
```

---

#### 三、虚函数的访问控制陷阱
派生类可重写基类虚函数并修改其访问权限，但通过基类接口仍可能调用私有函数。

```cpp
class Base {
public:
    virtual void execute() { std::cout << "Base::execute\n"; }
};

class Derived : public Base {
private:
    void execute() override { std::cout << "Derived::execute\n"; }
};

int main() {
    Derived d;
    // d.execute();               // 错误：execute 在 Derived 中是 private
    static_cast<Base&>(d).execute(); // 输出 "Derived::execute"
    return 0;
}
```

**关键点**：
- **编译时检查**：访问权限在编译时基于静态类型检查。
- **运行时绑定**：虚函数调用在运行时根据对象实际类型解析。

---

#### 四、访问权限修改的限制与应对
| 操作                  | 限制条件                             | 绕过方法                     |
|-----------------------|--------------------------------------|------------------------------|
| 提升 `protected` 为 `public` | 派生类需有访问权（原为 `protected`） | 无                           |
| 降低 `public` 为 `private`   | 基类成员需在派生类可见              | 通过基类引用或指针访问       |
| 隐藏基类虚函数        | 无法阻止通过基类接口调用             | 设计时避免暴露不安全虚函数   |

---

#### 五、最佳实践与总结
1. **谨慎使用 `using` 声明**：明确调整访问权限的意图，避免混淆接口。
2. **优先组合而非继承**：若需隐藏大量基类功能，考虑使用组合模式替代继承。
3. **虚函数设计原则**：基类虚函数应保持 `public`，派生类重写时勿降低访问权限。
4. **单元测试验证**：确保隐藏或修改的功能在不同使用场景下行为符合预期。

**总结**：通过 `using` 声明和函数删除，可在派生类中灵活调整基类接口，但需注意类型转换和虚函数带来的潜在风险。合理设计继承层次，确保代码的封装性和可维护性。