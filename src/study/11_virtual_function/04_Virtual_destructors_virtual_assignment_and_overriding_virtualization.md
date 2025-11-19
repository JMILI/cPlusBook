### C++ 虚析构函数、虚函数重写及继承控制详解

---

#### 一、虚析构函数：确保资源正确释放
##### 1. 问题背景
当通过基类指针删除派生类对象时，若基类析构函数非虚，仅调用基类析构函数，导致派生类资源泄漏。

```cpp
#include <iostream>
class Base {
public:
    ~Base() { std::cout << "~Base()\n"; } // 非虚析构函数
};

class Derived : public Base {
private:
    int* m_array;
public:
    Derived(int length) : m_array(new int[length]) {}
    ~Derived() { // 非虚析构函数
        delete[] m_array;
        std::cout << "~Derived()\n";
    }
};

int main() {
    Base* obj = new Derived(5);
    delete obj; // 仅调用Base的析构函数，m_array泄漏！
    return 0;
}
```

**输出**：
```
~Base()
```

##### 2. 解决方案：虚析构函数
```cpp
class Base {
public:
    virtual ~Base() = default; // 虚析构函数
};

class Derived : public Base {
public:
    ~Derived() override { // 自动成为虚函数
        delete[] m_array;
        std::cout << "~Derived()\n";
    }
};
```

**输出**：
```
~Derived()
~Base()
```

**关键点**：
- 基类析构函数必须为虚函数，确保派生类析构逻辑执行。
- 派生类析构函数自动成为虚函数，无需显式标记`virtual`（但建议用`override`）。

---

#### 二、虚赋值函数的潜在问题
##### 1. 为何避免虚赋值？
- **对象切片**：赋值运算符通常按值传递，可能导致派生类数据丢失。
- **多次析构风险**：若派生类有额外资源，虚赋值可能导致重复释放。
- **示例**：
  ```cpp
  class Base {
  public:
      virtual Base& operator=(const Base& other) { /* ... */ }
  };
  class Derived : public Base {
  public:
      Derived& operator=(const Base& other) override { // 危险！
          const Derived& d = dynamic_cast<const Derived&>(other);
          // 处理赋值...
          return *this;
      }
  };
  ```
**建议**：避免虚赋值运算符，优先使用其他设计模式（如克隆）。

---

#### 三、显式调用基类虚函数
通过作用域运算符`::`直接调用基类版本的虚函数，绕过多态机制。

```cpp
#include <iostream>
class Base {
public:
    virtual void print() { std::cout << "Base\n"; }
};

class Derived : public Base {
public:
    void print() override { std::cout << "Derived\n"; }
};

int main() {
    Derived d;
    Base& b = d;
    b.print();          // 输出 "Derived"
    b.Base::print();     // 输出 "Base"
    return 0;
}
```

**应用场景**：
- 需要基类默认行为，而非派生类重写。
- 调试或特定逻辑中强制使用基类实现。

---

#### 四、何时使用虚析构函数？
##### 1. 最佳实践
- **基类设计**：若类作为基类或有虚函数，析构函数必须为虚函数。
- **非基类**：若类不用于继承且无虚函数，析构函数无需虚化，减少开销。

##### 2. 性能与权衡
- **虚函数开销**：每个对象携带虚表指针（vptr），增加内存占用（通常4/8字节）。
- **适用场景**：仅在需要多态时使用虚函数，避免无谓开销。

##### 3. 安全继承控制
- **使用`final`防止继承**：
  ```cpp
  class Base final {}; // 禁止任何类继承Base
  class Derived : public Base {}; // 编译错误
  ```
- **Herb Sutter建议的替代方案**（C++11前）：
  ```cpp
  class Base {
  protected:
      ~Base() = default; // 阻止通过基类指针删除对象
  };
  ```

---

#### 五、总结与代码规范
| 情景                          | 操作                                                                 |
|-------------------------------|----------------------------------------------------------------------|
| 基类设计                      | 析构函数设为`virtual`，派生类析构函数用`override`标记                |
| 防止类被继承                  | 使用`final`关键字修饰类                                              |
| 基类非虚析构函数              | 仅当类明确不作为基类，且无虚函数时允许                               |
| 需要基类行为                  | 使用`Base::function()`显式调用                                        |
| 避免虚赋值运算符              | 优先用拷贝构造函数、克隆模式或工厂方法替代                           |

**示例：安全继承设计**
```cpp
class Shape {
public:
    virtual ~Shape() = default; // 虚析构函数
    virtual void draw() const = 0;
};

class Circle final : public Shape { // 禁止进一步派生
public:
    void draw() const override { /* 绘制圆形 */ }
};

// class SpecialCircle : public Circle {}; // 错误：Circle是final
```

---

通过合理使用虚析构函数、`final`关键字及作用域控制，可以构建安全、高效的C++继承体系，避免资源泄漏和未定义行为。