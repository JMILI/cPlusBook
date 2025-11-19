### C++ `std::unique_ptr` 详解

---

#### 一、核心概念
`std::unique_ptr` 是 C++11 引入的智能指针，用于独占管理动态分配的对象。其核心特性包括：
- **独占所有权**：同一时间只能有一个 `std::unique_ptr` 指向特定对象。
- **自动释放**：当 `std::unique_ptr` 超出作用域时，自动释放其管理的资源。
- **禁止拷贝**：禁用拷贝构造函数和拷贝赋值运算符，确保所有权唯一。
- **支持移动语义**：通过移动构造函数和移动赋值运算符转移所有权。

---

#### 二、基本用法
##### 1. 创建与销毁
```cpp
#include <memory>

class Resource {
public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
};

int main() {
    std::unique_ptr<Resource> ptr(new Resource());
    return 0;
} // ptr 超出作用域，Resource 被自动销毁
```
**输出**：
```
Resource acquired
Resource destroyed
```

##### 2. 移动语义转移所有权
```cpp
std::unique_ptr<Resource> ptr1(new Resource());
std::unique_ptr<Resource> ptr2 = std::move(ptr1); // 转移所有权
// ptr1 变为 nullptr，ptr2 接管资源
```

---

#### 三、访问托管对象
##### 1. 解引用与成员访问
```cpp
std::unique_ptr<Resource> ptr(new Resource());
if (ptr) { // 检查是否有效
    (*ptr).doSomething();  // 解引用
    ptr->doSomething();    // 箭头操作符
}
```

##### 2. 获取原始指针
```cpp
Resource* rawPtr = ptr.get(); // 获取原始指针（不释放所有权）
```

---

#### 四、数组管理
`std::unique_ptr` 支持动态数组，自动调用 `delete[]` 释放内存。
```cpp
auto arr = std::make_unique<int[]>(5); // 动态数组
arr[0] = 10;
```

---

#### 五、`std::make_unique` 的优势
##### 1. 简化创建
```cpp
auto ptr = std::make_unique<Resource>(); // 自动推导类型，避免显式 new
```

##### 2. 异常安全
```cpp
void func(std::unique_ptr<Resource> a, std::unique_ptr<Resource> b);

// 使用 make_unique 避免参数求值顺序问题
func(std::make_unique<Resource>(), std::make_unique<Resource>());
```

---

#### 六、函数传递与返回
##### 1. 按值传递（转移所有权）
```cpp
void takeOwnership(std::unique_ptr<Resource> res) {
    // res 接管资源
}

auto ptr = std::make_unique<Resource>();
takeOwnership(std::move(ptr)); // 显式转移所有权
```

##### 2. 按引用传递（不转移所有权）
```cpp
void useResource(const Resource& res) {
    // 使用资源，不获取所有权
}

auto ptr = std::make_unique<Resource>();
useResource(*ptr); // 传递引用
```

##### 3. 返回 `std::unique_ptr`
```cpp
std::unique_ptr<Resource> createResource() {
    return std::make_unique<Resource>(); // 返回值优化（RVO）
}

auto ptr = createResource(); // 正确接管资源
```

---

#### 七、作为类成员
类中使用 `std::unique_ptr` 管理资源，无需手动释放。
```cpp
class MyClass {
private:
    std::unique_ptr<Resource> res_;
public:
    MyClass() : res_(std::make_unique<Resource>()) {}
};

// MyClass 对象销毁时，res_ 自动释放资源
```

---

#### 八、常见误用与陷阱
##### 1. 多个 `unique_ptr` 管理同一资源
```cpp
Resource* raw = new Resource();
std::unique_ptr<Resource> ptr1(raw);
std::unique_ptr<Resource> ptr2(raw); // 错误！双重释放
```

##### 2. 手动释放资源
```cpp
std::unique_ptr<Resource> ptr(new Resource());
delete ptr.get(); // 错误！unique_ptr 会重复释放
```

##### 3. 未使用 `std::move` 转移所有权
```cpp
std::unique_ptr<Resource> ptr1 = ptr2; // 错误！拷贝被禁用
```

---

#### 九、总结
- **所有权管理**：`std::unique_ptr` 独占资源所有权，确保资源安全释放。
- **移动语义**：通过 `std::move` 转移所有权，避免拷贝开销。
- **最佳实践**：
    - 优先使用 `std::make_unique` 创建对象。
    - 按需传递原始指针或引用，避免不必要的所有权转移。
    - 禁止多个智能指针管理同一资源。

通过合理使用 `std::unique_ptr`，可有效避免内存泄漏，提升代码安全性和可维护性。