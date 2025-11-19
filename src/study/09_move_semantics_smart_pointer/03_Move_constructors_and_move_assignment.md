### C++ 移动构造函数与移动赋值函数详解

---

#### 一、移动语义的核心概念
**移动语义**通过转移资源所有权而非复制来提升性能，适用于管理动态内存或大型对象。其核心思想是：
- **避免深拷贝**：直接接管资源，减少内存分配和数据复制。
- **右值引用**（`&&`）：用于绑定临时对象（右值），标识可移动资源。

---

#### 二、移动构造函数与移动赋值函数
##### 1. 移动构造函数
```cpp
class MyClass {
public:
    // 移动构造函数（接管资源）
    MyClass(MyClass&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;  // 原对象置空，避免重复释放
        other.size_ = 0;
    }
    
private:
    int* data_;
    size_t size_;
};
```
- **作用**：从右值（临时对象）转移资源所有权。
- **关键点**：
    - 参数为右值引用（`MyClass&&`）。
    - 转移后原对象置空，确保析构安全。
    - 标记为 `noexcept` 以优化容器操作（如 `std::vector` 扩容）。

##### 2. 移动赋值函数
```cpp
class MyClass {
public:
    // 移动赋值函数
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {
            delete[] data_;       // 释放当前资源
            data_ = other.data_;  // 接管新资源
            size_ = other.size_;
            other.data_ = nullptr; // 原对象置空
            other.size_ = 0;
        }
        return *this;
    }
};
```
- **作用**：在赋值时转移资源所有权。
- **关键点**：
    - 检查自赋值（`if (this != &other)`）。
    - 释放当前资源后再接管新资源。

---

#### 三、移动语义 vs 拷贝语义
| **特性**               | **拷贝语义**                | **移动语义**                |
|------------------------|----------------------------|----------------------------|
| **资源处理**           | 深拷贝，复制所有数据         | 转移资源所有权               |
| **性能**               | 高开销（内存分配+数据复制）  | 低开销（仅指针操作）         |
| **适用场景**           | 安全共享数据                 | 临时对象或显式转移所有权     |
| **函数签名**           | `const T&`                 | `T&&`                      |

**何时触发移动操作？**
- 对象被右值初始化或赋值（如临时对象、`std::move`结果）。
- 函数返回局部对象时（返回值优化，RVO）。

---

#### 四、实现移动语义的最佳实践
##### 1. 禁用拷贝，启用移动
```cpp
class DynamicArray {
public:
    DynamicArray(const DynamicArray&) = delete;  // 禁用拷贝构造
    DynamicArray& operator=(const DynamicArray&) = delete; // 禁用拷贝赋值
    
    DynamicArray(DynamicArray&&) noexcept = default;  // 启用移动构造
    DynamicArray& operator=(DynamicArray&&) noexcept = default; // 启用移动赋值
};
```
- **适用场景**：资源独占类（如智能指针、文件句柄）。

##### 2. 正确实现 `swap` 函数
```cpp
class Name {
private:
    std::string name_;
    
public:
    friend void swap(Name& a, Name& b) noexcept {
        using std::swap;
        swap(a.name_, b.name_);  // 仅交换成员，避免无限递归
    }
    
    Name(Name&& other) noexcept {
        swap(*this, other);  // 调用自定义swap
    }
};
```
- **避免陷阱**：直接使用 `std::swap` 可能导致递归调用移动操作。

---

#### 五、性能对比：拷贝 vs 移动
##### 动态数组案例
- **深拷贝版本**：每次操作需分配内存并复制所有元素。
  ```cpp
  DynamicArray arr1(1e6); 
  DynamicArray arr2 = arr1; // 深拷贝，耗时约0.008秒
  ```
- **移动语义版本**：仅转移指针，无内存分配。
  ```cpp
  DynamicArray arr1(1e6);
  DynamicArray arr2 = std::move(arr1); // 移动构造，耗时约0.005秒（提升32%）
  ```

---

#### 六、标准库应用
- **`std::unique_ptr`**：独占所有权的智能指针，禁用拷贝，仅支持移动。
  ```cpp
  std::unique_ptr<Resource> p1 = std::make_unique<Resource>();
  std::unique_ptr<Resource> p2 = std::move(p1); // p1变为nullptr
  ```
- **容器操作优化**：`std::vector` 扩容时使用移动语义减少元素拷贝开销。

---

#### 七、常见问题与解决
1. **移动后访问原对象**：
   ```cpp
   MyClass a;
   MyClass b = std::move(a);
   a.use(); // 危险！a的资源已转移，行为未定义
   ```
    - **解决**：移动后置空原对象，避免误用。

2. **未标记 `noexcept`**：
    - 导致 `std::vector` 等容器无法使用移动优化，退化为拷贝。

3. **自定义 `swap` 的递归问题**：
    - 错误示例：`std::swap(*this, other)` 触发移动构造函数，导致无限递归。
    - 正确做法：仅交换成员变量，而非整个对象。

---

#### 八、总结
- **移动语义**是C++11的重要优化手段，通过右值引用实现资源高效转移。
- **移动构造函数/赋值函数**需正确处理资源所有权，确保原对象安全析构。
- **优先使用标准库工具**（如 `std::unique_ptr`、`std::move`）减少手动管理错误。
- **性能关键代码**应禁用拷贝，启用移动，并正确实现 `swap` 函数。

通过合理应用移动语义，可显著提升程序性能，尤其在处理动态内存、大型对象或容器操作时效果显著。