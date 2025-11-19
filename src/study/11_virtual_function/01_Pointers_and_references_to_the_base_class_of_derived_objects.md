### 使用虚函数实现多态性

在C++中，当通过基类指针或引用调用成员函数时，默认情况下会根据指针/引用的静态类型（即声明类型）调用对应版本的函数。要启用**运行时多态性**，需要将基类函数声明为`virtual`，并在派生类中进行重写。

#### 修改后的动物类示例
```cpp
#include <iostream>
#include <string_view>

class Animal {
protected:
    std::string m_name;

    Animal(std::string_view name) : m_name(name) {}
public:
    virtual ~Animal() = default;  // 虚析构函数（重要！）
    virtual std::string_view speak() const { return "???"; }  // 声明为虚函数
    std::string_view getName() const { return m_name; }
};

class Cat : public Animal {
public:
    Cat(std::string_view name) : Animal(name) {}
    std::string_view speak() const override { return "Meow"; }  // 使用override确保正确重写
};

class Dog : public Animal {
public:
    Dog(std::string_view name) : Animal(name) {}
    std::string_view speak() const override { return "Woof"; }  // 运行时将调用此版本
};
```

#### 验证多态行为
```cpp
int main() {
    const Cat cat{ "Fred" };
    const Dog dog{ "Garbo" };

    const Animal* animals[] = { &cat, &dog };  // 基类指针数组

    for (const auto animal : animals) {
        std::cout << animal->getName() << " says " 
                  << animal->speak() << '\n';  // 正确调用派生类版本
    }
}
```

#### 输出结果
```
Fred says Meow
Garbo says Woof
```

---

### 关键机制说明
1. **虚函数表（vtable）**：
    - 编译器为每个包含虚函数的类生成一个虚函数表。
    - 表中存储该类所有虚函数的实际地址。
    - 对象在内存中携带一个指向该表的指针（vptr）。

2. **动态绑定流程**：
   ```mermaid
   graph TD
       A[基类指针调用speak()] --> B[查询对象的vptr]
       B --> C[访问虚函数表]
       C --> D[找到实际函数地址]
       D --> E[调用Cat::speak或Dog::speak]
   ```

3. **必须遵循的规则**：
    - 基类函数必须声明为`virtual`
    - 派生类函数签名必须完全一致
    - 使用`override`关键字（C++11起）可帮助检测签名错误

---

### 常见问题解答
#### 1. 为什么要虚析构函数？
```cpp
virtual ~Animal() = default;  // 必须声明为虚函数
```
- **作用**：确保通过基类指针删除派生类对象时，能正确调用派生类析构函数。
- **后果**：若基类析构函数非虚，删除基类指针时只会调用基类析构函数，导致派生类部分内存泄漏。

#### 2. 性能影响有多大？
- **虚函数调用**：比普通函数多一次指针解引用（访问vtable）和一次间接跳转。
- **典型开销**：约5-10%额外性能损耗，在大多数场景可忽略不计。
- **优化建议**：对性能关键路径避免过度使用虚函数。

#### 3. 如何防止被继承？
```cpp
class FinalClass final {  // C++11起支持
    // 此类不能被继承
};
```

---

### 多态性应用场景总结
| 场景                | 解决方案                     | 优势                          |
|---------------------|------------------------------|-------------------------------|
| 统一接口处理不同对象 | 基类定义虚函数，派生类重写   | 代码复用，扩展性强            |
| 运行时类型决策       | 通过基类指针/引用动态调用     | 灵活适应不同子类行为          |
| 插件式架构          | 通过接口类（纯虚类）定义规范 | 模块解耦，支持动态加载        |

通过合理使用虚函数，您可以在保持类型安全的同时，构建高度灵活的可扩展系统。