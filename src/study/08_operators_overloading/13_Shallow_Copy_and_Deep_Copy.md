### C++ 浅拷贝与深拷贝详解

---

#### 一、浅拷贝与深拷贝的核心区别
- **浅拷贝**：仅复制成员变量的值（包括指针地址），多个对象共享同一块内存。
- **深拷贝**：为指针成员分配新内存并复制数据，每个对象拥有独立副本。

---

#### 二、浅拷贝的风险：动态内存管理
##### 示例：`MyString` 类的浅拷贝问题
```cpp
class MyString {
private:
    char* m_data; // 动态分配内存的指针
public:
    MyString(const char* source) {
        m_data = new char[strlen(source) + 1];
        strcpy(m_data, source);
    }
    ~MyString() { delete[] m_data; }
};

int main() {
    MyString hello{"Hello"};
    {
        MyString copy = hello; // 浅拷贝：copy.m_data 和 hello.m_data 指向同一内存
    } // copy 析构时释放内存，导致 hello.m_data 成为悬空指针
    std::cout << hello.m_data; // 未定义行为！
}
```
**问题**：
- `copy` 析构后，`hello` 的 `m_data` 指向已释放内存，访问导致崩溃或错误数据。

---

#### 三、深拷贝的解决方案
##### 1. 自定义拷贝构造函数
```cpp
MyString::MyString(const MyString& other) {
    m_data = new char[strlen(other.m_data) + 1];
    strcpy(m_data, other.m_data); // 深拷贝数据
}
```

##### 2. 自定义赋值运算符
```cpp
MyString& MyString::operator=(const MyString& other) {
    if (this != &other) { // 自我赋值检查
        delete[] m_data; // 释放旧内存
        m_data = new char[strlen(other.m_data) + 1];
        strcpy(m_data, other.m_data);
    }
    return *this;
}
```

##### 3. 辅助函数 `deepCopy`
```cpp
void MyString::deepCopy(const MyString& other) {
    delete[] m_data; // 释放旧资源
    m_data = new char[strlen(other.m_data) + 1];
    strcpy(m_data, other.m_data);
}

// 拷贝构造函数和赋值运算符复用 deepCopy
MyString::MyString(const MyString& other) { deepCopy(other); }
MyString& MyString::operator=(const MyString& other) {
    if (this != &other) deepCopy(other);
    return *this;
}
```

---

#### 四、Rule of Three（三法则）
**原则**：若类需要自定义以下任一函数，通常需定义全部三个：
1. **析构函数**：释放动态分配的资源。
2. **拷贝构造函数**：实现深拷贝。
3. **赋值运算符**：安全赋值，处理自我赋值和资源释放。

**示例**：
```cpp
class MyString {
public:
    ~MyString();                  // 析构函数
    MyString(const MyString&);    // 拷贝构造函数
    MyString& operator=(const MyString&); // 赋值运算符
};
```

---

#### 五、优先使用标准库工具
- **避免手动内存管理**：使用 `std::string`、`std::vector` 等容器，自动处理深拷贝。
- **示例**：
  ```cpp
  #include <string>
  int main() {
      std::string s1 = "Hello";
      std::string s2 = s1; // 深拷贝自动完成，无需手动管理
      return 0;
  }
  ```

---

#### 六、总结
- **浅拷贝**：适用于无动态资源的简单类（如 `Fraction`）。
- **深拷贝**：必需于含指针成员的类，防止悬空指针和内存泄漏。
- **Rule of Three**：动态资源管理类需定义析构函数、拷贝构造函数和赋值运算符。
- **标准库优先**：利用现有容器减少错误，提升开发效率。