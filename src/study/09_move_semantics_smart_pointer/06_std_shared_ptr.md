### C++ `std::shared_ptr` 详解

---

#### 一、核心概念
`std::shared_ptr` 是 C++11 引入的智能指针，用于多个指针共享同一资源的所有权。其核心特性包括：
- **共享所有权**：多个 `std::shared_ptr` 可指向同一资源，引用计数自动管理。
- **自动释放**：当最后一个 `std::shared_ptr` 超出作用域时，资源被释放。
- **线程安全**：引用计数操作是原子的，但资源访问需额外同步。
- **支持移动语义**：允许所有权转移，但拷贝操作会增加引用计数。

---

#### 二、基本用法
##### 1. 创建与共享所有权
```cpp
#include <memory>

class Resource {
public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
};

int main() {
    auto ptr1 = std::make_shared<Resource>(); // 正确创建方式
    {
        auto ptr2 = ptr1; // 共享所有权，引用计数+1
        std::cout << "引用计数: " << ptr2.use_count() << "\n"; // 输出 2
    } // ptr2 销毁，引用计数-1
    std::cout << "引用计数: " << ptr1.use_count() << "\n"; // 输出 1
} // ptr1 销毁，引用计数归零，资源释放
```
**输出**：
```
Resource acquired
引用计数: 2
引用计数: 1
Resource destroyed
```

##### 2. 避免独立创建导致重复释放
```cpp
Resource* raw = new Resource();
std::shared_ptr<Resource> ptr1(raw);
std::shared_ptr<Resource> ptr2(raw); // 错误！独立创建导致重复释放
```
- **正确做法**：始终通过拷贝或`std::make_shared`创建共享指针。

---

#### 三、`std::make_shared` 的优势
##### 1. 优化内存分配
```cpp
auto ptr = std::make_shared<Resource>(); // 单次内存分配（资源 + 控制块）
```
- **性能提升**：减少内存碎片，提升访问局部性。

##### 2. 异常安全
```cpp
func(std::make_shared<Resource>(), otherFunction()); // 避免参数求值顺序问题
```

---

#### 四、与 `std::unique_ptr` 的交互
##### 1. 从 `std::unique_ptr` 转移所有权
```cpp
std::unique_ptr<Resource> unique = std::make_unique<Resource>();
std::shared_ptr<Resource> shared = std::move(unique); // 正确转移所有权
```

##### 2. 不可反向转换
```cpp
std::shared_ptr<Resource> shared = std::make_shared<Resource>();
std::unique_ptr<Resource> unique = std::move(shared); // 错误！语法不支持
```

---

#### 五、数组支持（C