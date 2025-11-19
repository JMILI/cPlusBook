### 向派生类添加新功能的步骤与示例

在C++中，派生类可以通过继承基类并添加新功能来扩展其行为。以下是详细的步骤和示例：

#### 步骤说明：
1. **定义基类**：基类包含通用属性和方法，部分成员设为`protected`以便派生类访问。
2. **创建派生类**：使用公有继承（`public`）确保基类接口的可用性。
3. **初始化基类成员**：在派生类构造函数中调用基类构造函数。
4. **添加新功能**：在派生类中声明和定义新方法，访问基类`protected`成员或添加新成员变量。
5. **使用派生类**：实例化派生类对象并调用新方法。

#### 示例代码：
```cpp
#include <iostream>

// 基类定义
class Base {
protected:
    int m_value;

public:
    Base(int value) : m_value(value) {}
    void identify() const {
        std::cout << "I am a Base with value " << m_value << "\n";
    }
};

// 派生类定义
class Derived : public Base {
public:
    // 调用基类构造函数初始化m_value
    Derived(int value) : Base(value) {}

    // 新增方法：获取m_value
    int getValue() const {
        return m_value;
    }

    // 新增方法：修改m_value
    void setValue(int value) {
        m_value = value;
    }

    // 新增方法：扩展功能
    void display() const {
        std::cout << "Derived value: " << m_value << "\n";
    }
};

int main() {
    Derived derived(5);
    derived.identify();  // 继承自基类的方法
    std::cout << "Value via getValue(): " << derived.getValue() << "\n";

    derived.setValue(10);
    derived.display();    // 输出修改后的值

    return 0;
}
```

#### 代码解析：
- **基类`Base`**：
    - 包含`protected`成员`m_value`，允许派生类直接访问。
    - 公有方法`identify()`输出基本信息。

- **派生类`Derived`**：
    - 公有继承`Base`，获得其`protected`和`public`成员。
    - 构造函数通过初始化列表调用`Base`的构造函数。
    - 新增三个方法：
        - `getValue()`：返回`m_value`。
        - `setValue(int)`：修改`m_value`。
        - `display()`：输出当前值。

- **使用场景**：
    - 创建`Derived`对象，调用继承的`identify()`方法。
    - 使用`getValue()`和`setValue()`访问和修改基类成员。
    - 调用派生类特有的`display()`方法。

#### 关键点：
- **访问权限**：基类的`protected`成员在派生类中可直接访问，但对外部不可见。
- **功能扩展**：派生类在保持基类功能的同时，添加新方法增强功能。
- **构造函数链式调用**：确保基类部分正确初始化。

#### 常见问题解答：
1. **如果基类成员是`private`而非`protected`，如何访问？**
    - 派生类无法直接访问基类`private`成员。需通过基类提供的公有或`protected`接口（如getter/setter）间接访问。

2. **派生类方法名与基类相同会发生什么？**
    - 派生类方法会隐藏基类同名方法（非虚函数）。需使用`基类名::方法名`调用基类版本，或使用`virtual`和`override`实现多态。

3. **能否通过基类指针调用派生类新增方法？**
    - 不能直接调用。需将基类指针向下转型为派生类指针（如`dynamic_cast`），或通过虚函数接口实现多态调用。

#### 总结：
通过向派生类添加新功能，可以在不修改基类的情况下扩展其行为，适用于第三方库扩展或代码复用场景。合理使用`protected`访问权限和公有继承，确保派生类既能访问基类成员，又能灵活扩展新特性。