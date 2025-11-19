### C++ 纯虚函数、抽象基类与接口类详解

---

#### 一、纯虚函数与抽象基类
**纯虚函数（Pure Virtual Function）** 是一种没有具体实现的虚函数，用于强制派生类提供实现。通过将虚函数赋值为 `0` 来声明：

```cpp
class Animal {
public:
    virtual void speak() const = 0; // 纯虚函数
};
```

**抽象基类（Abstract Base Class）**：
- **定义**：包含至少一个纯虚函数的类。
- **特性**：
    - **不可实例化**：无法创建抽象基类的对象。
    - **派生类必须实现所有纯虚函数**，否则派生类也会成为抽象类。

###### 示例：抽象基类与派生类
```cpp
class Animal {
public:
    virtual void speak() const = 0; // 纯虚函数
    virtual ~Animal() = default;     // 虚析构函数
};

class Dog : public Animal {
public:
    void speak() const override {    // 必须实现纯虚函数
        std::cout << "Woof!" << std::endl;
    }
};

int main() {
    // Animal animal; // 错误：Animal是抽象基类
    Dog dog;
    dog.speak();     // 输出 "Woof!"
    return 0;
}
```

---

#### 二、纯虚函数的默认实现
纯虚函数可以在类外提供默认实现，但派生类仍需显式覆盖：

```cpp
class Animal {
public:
    virtual void speak() const = 0;
};

// 类外定义纯虚函数的默认实现
void Animal::speak() const {
    std::cout << "Default sound" << std::endl;
}

class Cat : public Animal {
public:
    void speak() const override {
        Animal::speak(); // 显式调用基类默认实现
    }
};
```

**用途**：为派生类提供可选默认逻辑，但仍强制派生类显式声明覆盖。

---

#### 三、接口类（Interface Class）
**接口类** 是一种特殊的抽象基类，仅包含纯虚函数，用于定义统一的操作规范：

###### 定义接口类：
```cpp
class ILogger {
public:
    virtual void log(const std::string& message) = 0;
    virtual ~ILogger() = default;
};
```

###### 派生类实现接口：
```cpp
class FileLogger : public ILogger {
public:
    void log(const std::string& message) override {
        // 将日志写入文件
    }
};

class ConsoleLogger : public ILogger {
public:
    void log(const std::string& message) override {
        std::cout << message << std::endl;
    }
};
```

###### 使用接口提高灵活性：
```cpp
void processData(ILogger& logger) {
    logger.log("Processing data...");
}

int main() {
    FileLogger fileLogger;
    ConsoleLogger consoleLogger;
    
    processData(fileLogger);    // 写入文件
    processData(consoleLogger); // 输出到控制台
    return 0;
}
```

**优势**：
- **解耦**：代码依赖接口而非具体实现，易于扩展。
- **多态**：同一接口支持多种实现，运行时动态绑定。

---

#### 四、关键注意事项
1. **虚析构函数**：
    - 抽象基类必须定义虚析构函数，确保派生类对象正确释放资源。
   ```cpp
   class ILogger {
   public:
       virtual ~ILogger() = default;
       virtual void log(const std::string&) = 0;
   };
   ```

2. **纯虚函数与虚函数表**：
    - 抽象基类仍有虚函数表（vtable），纯虚函数在表中通常指向占位符或错误处理函数。
    - 构造函数/析构函数可调用虚函数，但需注意对象生命周期（派生类部分可能未构造或已销毁）。

3. **接口类的命名规范**：
    - 通常以 `I` 开头（如 `ILogger`），明确标识其为接口。

---

#### 五、应用场景总结
| 场景                | 说明                                                                 |
|---------------------|----------------------------------------------------------------------|
| **强制派生类实现**  | 纯虚函数确保派生类必须提供特定功能的实现。                           |
| **定义通用协议**    | 接口类标准化模块间的交互方式，如日志、网络通信等。                   |
| **运行时多态**      | 通过基类指针或引用调用接口方法，支持动态绑定不同实现。               |
| **代码解耦**        | 依赖接口而非具体实现，提升代码可维护性和可测试性。                   |

---

#### 六、总结
- **纯虚函数**：强制派生类实现特定功能，定义抽象基类。
- **抽象基类**：不可实例化，用于定义通用接口。
- **接口类**：纯虚函数的集合，实现多态和解耦。
- **虚析构函数**：确保资源正确释放，避免内存泄漏。

通过合理使用纯虚函数和接口类，可以构建高度灵活、可扩展的C++程序，尤其在需要多态和模块化设计的场景中表现卓越。