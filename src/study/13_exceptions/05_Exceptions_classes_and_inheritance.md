
### 异常、类和继承详解

---

#### **1. 异常在成员函数中的应用**
##### **运算符重载中的异常**
在重载运算符时，由于函数签名固定，无法通过返回值传递错误。异常机制成为理想选择：
```cpp
class IntArray {
private:
    int* m_data;
    int m_size;
public:
    int& operator[](int index) {
        if (index < 0 || index >= m_size)
            throw ArrayException("Invalid index");
        return m_data[index];
    }
};
```
- **优点**：不改变函数签名，错误信息可通过自定义异常类传递。

##### **构造函数中的异常**
构造函数失败时，抛出异常阻止对象完全构造，但已初始化的成员会被正确销毁：
```cpp
class Member {
public:
    Member() { /* 分配资源 */ }
    ~Member() { /* 释放资源 */ }
};

class A {
    Member m_member;
public:
    A(int x) {
        if (x <= 0) throw 1;
    }
    // 析构函数不会执行，但 m_member 的析构函数会被调用
};
```
- **关键点**：利用 RAII，资源管理类的析构函数自动释放资源，即使构造函数失败。

---

#### **2. 自定义异常类**
##### **设计异常类**
通过继承标准异常类，集成到现有框架：
```cpp
#include <exception>
#include <string>

class ArrayException : public std::runtime_error {
public:
    ArrayException(const std::string& msg) 
        : std::runtime_error(msg) {}
};
```
- **why继承std::exception**：统一异常接口，允许通过基类捕获所有派生异常。

##### **使用示例**
```cpp
try {
    IntArray arr;
    int val = arr[5]; // 越界访问
} catch (const ArrayException& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```
- **优势**：提供清晰的错误上下文，支持多态捕获。

---

#### **3. 异常与继承**
##### **异常捕获顺序**
派生类异常应优先处理，避免基类截获：
```cpp
try {
    throw Derived();
} catch (const Derived& d) {
    // 处理派生类异常
} catch (const Base& b) {
    // 处理基类异常
}
```
- **规则**：先捕获具体异常（派生类），再捕获通用异常（基类）。

##### **标准库异常**
标准异常均派生自 `std::exception`，可通过基类统一处理：
```cpp
#include <stdexcept>
#include <iostream>

try {
    std::string s;
    s.resize(MAX_SIZE); // 可能抛出 std::bad_alloc 或 std::length_error
} catch (const std::exception& e) {
    std::cerr << "Standard exception: " << e.what() << std::endl;
}
```
- **实用场景**：捕获所有标准库异常，简化错误处理逻辑。

---

#### **4. 异常对象的生命周期**
##### **拷贝与生存期**
异常对象会被复制到安全区域，确保栈展开后仍有效：
```cpp
class MyException {
public:
    MyException(const MyException&) = default; // 必须可拷贝
};

try {
    MyException e;
    throw e; // 触发拷贝构造函数
} catch (const MyException& e) {
    // 处理异常
}
```
- **限制**：异常类需支持拷贝或移动构造，否则编译失败。

##### **避免悬空引用**
异常对象不应持有局部对象的指针/引用：
```cpp
try {
    int x = 5;
    throw &x; // 错误：x 在栈展开后被销毁
} catch (int* e) {
    // e 成为悬空指针
}
```
- **正确做法**：传递值或拷贝数据，而非指针/引用。

---

#### **5. 最佳实践总结**
1. **RAII 优先**：资源管理类自动释放资源，避免构造函数失败导致泄漏。
2. **继承标准异常**：自定义异常类继承 `std::exception` 或 `std::runtime_error`，重写 `what()`。
3. **异常层次结构**：通过多态捕获，先处理具体异常，再处理通用异常。
4. **异常安全**：确保异常类可拷贝，避免持有局部对象引用。
5. **标准库集成**：利用 `std::exception` 捕获所有标准异常，简化代码。

---

#### **示例：综合应用**
```cpp
#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>

class DataLoader {
public:
    DataLoader(const std::string& path) {
        if (path.empty()) 
            throw std::invalid_argument("Empty path provided");
        // 模拟资源加载
        if (!loadResource(path)) 
            throw std::runtime_error("Failed to load resource");
    }

private:
    bool loadResource(const std::string& path) { return false; }
};

int main() {
    try {
        DataLoader loader(""); // 触发无效参数异常
    } catch (const std::invalid_argument& e) {
        std::cerr << "参数错误: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "标准异常: " << e.what() << std::endl;
    }
    return 0;
}
```
**输出**：
```
参数错误: Empty path provided
```

**说明**：
- `DataLoader` 在构造函数中验证参数，抛出 `std::invalid_argument`。
- 主函数优先捕获具体异常，再捕获通用异常，确保错误处理精准。

---

通过结合异常机制、类设计与继承，可构建健壮且易维护的 C++ 程序，有效应对各类运行时错误。