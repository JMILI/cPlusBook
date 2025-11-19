### C++ 虚函数与多态性详解

---

#### 一、虚函数的作用与定义
**虚函数（Virtual Function）** 允许派生类覆盖基类的方法，实现运行时多态。通过基类指针或引用调用时，实际执行的是对象类型的派生类方法。

##### 定义语法：
```cpp
class Base {
public:
    virtual void func() const { // 基类虚函数
        std::cout << "Base::func()\n";
    }
};

class Derived : public Base {
public:
    void func() const override { // 派生类覆盖
        std::cout << "Derived::func()\n";
    }
};
```

---

#### 二、多态性的实现机制
##### 1. 运行时多态（动态绑定）
- **虚函数表（vtable）**：每个含虚函数的类拥有一个虚函数表，存储虚函数地址。
- **虚指针（vptr）**：对象实例包含指向其类的vtable的指针。
- **动态绑定流程**：
  ```mermaid
  graph TD
      A[基类指针调用虚函数] --> B[通过vptr找到vtable]
      B --> C[查找函数地址]
      C --> D[执行派生类函数]
  ```

##### 2. 示例：动物叫声的多态
```cpp
class Animal {
public:
    virtual ~Animal() = default;
    virtual std::string speak() const { return "???"; }
};

class Cat : public Animal {
public:
    std::string speak() const override { return "Meow"; }
};

class Dog : public Animal {
public:
    std::string speak() const override { return "Woof"; }
};

void report(const Animal& animal) {
    std::cout << animal.speak() << std::endl;
}

int main() {
    Cat cat;
    Dog dog;
    report(cat); // 输出 "Meow"
    report(dog); // 输出 "Woof"
    return 0;
}
```

---

#### 三、关键注意事项
##### 1. 虚析构函数
- **必要性**：确保通过基类指针删除派生类对象时，正确调用派生类析构函数。
- **示例**：
  ```cpp
  class Base {
  public:
      virtual ~Base() = default;
  };

  class Derived : public Base {
  public:
      ~Derived() override { /* 清理派生类资源 */ }
  };
  ```

##### 2. 覆盖（Override）的严格性
- **函数签名一致**：派生类覆盖虚函数时，函数名、参数列表、返回类型（允许协变）必须严格匹配。
- **使用`override`关键字**（C++11起）：
  ```cpp
  class Derived : public Base {
  public:
      void func() const override { // 明确表示覆盖
          std::cout << "Derived::func()\n";
      }
  };
  ```

##### 3. 避免在构造/析构函数中调用虚函数
- **原因**：构造时派生类部分未初始化，析构时已销毁，虚函数无法正确绑定。
- **示例**：
  ```cpp
  class Base {
  public:
      Base() { callVirtual(); } // 危险：调用Base::callVirtual()
      virtual void callVirtual() { /* ... */ }
  };

  class Derived : public Base {
  public:
      void callVirtual() override { /* ... */ } // 不会被调用
  };
  ```

---

#### 四、虚函数的性能与开销
##### 1. 性能影响
- **内存开销**：每个对象携带一个vptr（通常4或8字节）。
- **时间开销**：虚函数调用需两次间接寻址（vptr → vtable → 函数地址）。

##### 2. 适用场景
- **需要运行时多态**：处理多种派生类对象。
- **接口抽象**：定义通用接口，由派生类具体实现。

---

#### 五、高级主题：协变返回类型
派生类覆盖虚函数时，返回类型可以是基类返回类型的派生类。
```cpp
class Base {
public:
    virtual Base* clone() const { return new Base(*this); }
};

class Derived : public Base {
public:
    Derived* clone() const override { // 协变返回类型
        return new Derived(*this);
    }
};
```

---

#### 六、总结与最佳实践
| 要点                  | 说明                                                                 |
|-----------------------|----------------------------------------------------------------------|
| 虚函数声明            | 基类中使用`virtual`，派生类使用`override`                           |
| 虚析构函数            | 基类析构函数必须为虚函数                                             |
| 避免构造/析构中调用    | 防止未定义行为                                                       |
| 性能权衡              | 在需要多态时使用，避免不必要的虚函数                                 |
| 使用`final`禁止覆盖   | 在类或函数后加`final`，禁止进一步派生或覆盖                          |

##### 示例代码：
```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0; // 纯虚函数，抽象类
};

class Circle : public Shape {
public:
    void draw() const override { std::cout << "Drawing Circle\n"; }
};

class Rectangle : public Shape {
public:
    void draw() const override { std::cout << "Drawing Rectangle\n"; }
};

int main() {
    std::vector<Shape*> shapes{new Circle(), new Rectangle()};
    for (const auto& shape : shapes) {
        shape->draw(); // 动态绑定调用具体draw()
    }
    // 清理内存
    for (auto& shape : shapes) delete shape;
    return 0;
}
```

**输出**：
```
Drawing Circle
Drawing Rectangle
```

---

通过合理使用虚函数和多态性，可以构建灵活、可扩展的代码结构，同时注意性能优化和资源管理，确保程序的健壮性。