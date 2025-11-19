### C++ 对象切片（Object Slicing）详解

---

#### 一、什么是对象切片？
**对象切片（Object Slicing）** 是指将派生类对象赋值给基类对象时，派生类的特有成员被“截断”，仅保留基类部分的现象。这种行为会导致派生类的数据和虚函数无法被正确访问。

###### 示例：基础切片
```cpp
#include <iostream>
#include <string_view>

class Base {
protected:
    int m_value;
public:
    Base(int value) : m_value(value) {}
    virtual ~Base() = default;
    virtual std::string_view getName() const { return "Base"; }
    int getValue() const { return m_value; }
};

class Derived : public Base {
public:
    Derived(int value) : Base(value) {}
    std::string_view getName() const override { return "Derived"; }
};

int main() {
    Derived d{5};
    Base b = d; // 对象切片：仅复制Base部分，Derived部分丢失
    std::cout << "b is a " << b.getName() << " with value " << b.getValue(); 
    // 输出：b is a Base with value 5
    return 0;
}
```
- **关键点**：`b` 是 `Base` 类型，只能保留 `d` 的 `Base` 部分，虚函数仍调用基类版本。

---

#### 二、函数传参中的切片问题
当派生类对象以**值传递**方式传递给基类参数时，会触发切片，导致派生类部分丢失。

###### 错误示例：按值传递基类参数
```cpp
void printName(Base base) { // 按值传递
    std::cout << "I am a " << base.getName();
}

int main() {
    Derived d{5};
    printName(d); // 触发切片，base.getName()调用Base版本
    // 输出：I am a Base
    return 0;
}
```
- **解决方法**：改为**引用传递**或**指针传递**，避免切片。
```cpp
void printName(const Base& base) { // 按引用传递
    std::cout << "I am a " << base.getName(); 
    // 输出：I am a Derived
}
```

---

#### 三、容器中的切片问题
使用容器存储基类对象时，派生类对象会被切片，导致多态失效。

###### 错误示例：`std::vector<Base>` 存储派生类
```cpp
#include <vector>

int main() {
    std::vector<Base> v;
    v.push_back(Base{5});     // 正确
    v.push_back(Derived{6});  // 切片！Derived部分丢失

    for (const auto& obj : v) {
        std::cout << obj.getName() << " "; // 输出：Base Base
    }
    return 0;
}
```
- **解决方法**：使用**指针**或 `std::reference_wrapper` 存储多态对象。

###### 正确方法：指针容器
```cpp
#include <vector>

int main() {
    std::vector<Base*> v;
    Derived d1{5}, d2{6};
    v.push_back(&d1);
    v.push_back(&d2);

    for (const auto* ptr : v) {
        std::cout << ptr->getName() << " "; // 输出：Derived Derived
    }
    return 0;
}
```

###### 正确方法：`std::reference_wrapper`
```cpp
#include <vector>
#include <functional> // 需包含头文件

int main() {
    Derived d1{5}, d2{6};
    std::vector<std::reference_wrapper<Base>> v;
    v.push_back(d1);
    v.push_back(d2);

    for (const auto& ref : v) {
        std::cout << ref.get().getName() << " "; // 输出：Derived Derived
    }
    return 0;
}
```

---

#### 四、赋值操作导致的对象破坏
通过基类引用或指针赋值可能破坏派生类对象的结构。

###### 示例：赋值导致数据不一致
```cpp
int main() {
    Derived d1{5}, d2{6};
    Base& b = d2; 
    b = d1; // 仅复制Base部分，d2的Derived部分未改变！

    std::cout << d2.getValue(); // 输出：5（Base部分被覆盖）
    // 但d2的Derived特有数据可能处于不一致状态
    return 0;
}
```
- **原因**：基类的赋值运算符（`operator=`）默认非虚，仅复制基类数据。

---

#### 五、如何避免对象切片？
1. **避免值传递**：
    - 使用**引用**或**指针**传递多态对象。
   ```cpp
   void process(const Base& obj); // 正确：引用传递
   ```

2. **容器存储指针或引用包装器**：
    - 使用 `std::vector<Base*>` 或 `std::vector<std::reference_wrapper<Base>>`。

3. **设计基类不可拷贝**：
    - 删除基类的拷贝构造函数和赋值运算符，强制使用指针或引用。
   ```cpp
   class Base {
   public:
       Base(const Base&) = delete;
       Base& operator=(const Base&) = delete;
   };
   ```

4. **使用抽象接口类**：
    - 基类定义纯虚函数，无法实例化，避免意外切片。
   ```cpp
   class IBase {
   public:
       virtual ~IBase() = default;
       virtual void action() = 0;
   };
   ```

---

### 总结
对象切片是C++中常见的陷阱，尤其在涉及多态和继承时。通过以下措施可有效避免：
- **引用或指针传递**：确保派生类对象的完整性。
- **容器存储指针**：维护多态行为。
- **基类设计**：限制拷贝操作或使用纯虚接口。

理解对象切片的机制，能帮助开发者编写更健壮的面向对象代码，避免因值传递或错误赋值导致的数据丢失和逻辑错误。