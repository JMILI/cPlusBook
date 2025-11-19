### C++ 构造函数详解：默认、拷贝、赋值、移动

---

#### 一、默认构造函数 (Default Constructor)
**定义**：  
无参数或所有参数均有默认值的构造函数。若未显式定义，编译器会自动生成一个**隐式默认构造函数**。

**用途**：
- 创建对象时不提供初始值。
- 用于初始化对象成员（基础类型未初始化，类类型调用其默认构造函数）。

**优点**：
- 自动生成，无需手动实现（无资源管理时）。
- 支持隐式对象创建（如 `MyClass obj;`）。

**缺点**：
- 隐式生成的默认构造函数不会初始化基础类型成员（如 `int`、指针）。
- 若类包含不可默认构造的成员（如引用），需显式定义。

**限制**：
- 若定义了其他构造函数，编译器不再生成隐式默认构造函数，需手动添加 `= default`。

**代码示例**：
```cpp
class MyClass {
public:
    int* data;
    size_t size;

    // 显式默认构造函数（初始化成员）
    MyClass() : data(nullptr), size(0) {} 

    // 带参数的构造函数
    MyClass(size_t n) : data(new int[n]{}), size(n) {}
};
```

---

#### 二、拷贝构造函数 (Copy Constructor)
**定义**：  
通过同类型对象的**左值引用**创建新对象。格式：  
`ClassName(const ClassName& other)`

**用途**：
- 对象初始化时的拷贝（如 `MyClass a = b;`）。
- 函数传参或返回值（按值传递时触发拷贝）。

**优点**：
- 确保深拷贝，避免共享资源（如动态内存）。

**缺点**：
- 默认实现为**浅拷贝**（逐成员复制），需手动实现深拷贝。
- 深拷贝可能带来性能开销（如复制大型数据）。

**限制**：
- 若未定义且需要深拷贝，浅拷贝会导致重复释放内存（未定义行为）。

**代码示例**：
```cpp
class MyArray {
public:
    int* data;
    size_t size;

    // 拷贝构造函数（深拷贝）
    MyArray(const MyArray& other) : size(other.size) {
        data = new int[size];
        std::copy(other.data, other.data + size, data);
    }
};
```

---

#### 三、拷贝赋值运算符 (Copy Assignment Operator)
**定义**：  
将一个对象的**左值引用**赋值给另一个已存在的对象。格式：  
`ClassName& operator=(const ClassName& other)`

**用途**：
- 对象赋值操作（如 `a = b;`）。

**优点**：
- 提供深拷贝赋值，确保资源独立。

**缺点**：
- 需处理自赋值（`a = a;`）和释放原有资源。
- 深拷贝性能开销大。

**限制**：
- 若未定义且需要深拷贝，默认浅拷贝会导致资源泄漏或重复释放。

**代码示例**：
```cpp
class MyArray {
public:
    // 拷贝赋值运算符
    MyArray& operator=(const MyArray& other) {
        if (this != &other) { // 自赋值检查
            delete[] data;    // 释放原有资源
            size = other.size;
            data = new int[size];
            std::copy(other.data, other.data + size, data);
        }
        return *this;
    }
};
```

---

#### 四、移动构造函数 (Move Constructor)
**定义**：  
通过同类型对象的**右值引用**创建新对象，转移资源所有权。格式：  
`ClassName(ClassName&& other) noexcept`

**用途**：
- 优化临时对象或显式转移资源（如 `MyClass a = std::move(b);`）。

**优点**：
- 避免深拷贝，提升性能（仅转移指针）。
- 支持容器优化（如 `std::vector` 扩容）。

**缺点**：
- 原对象状态变为有效但未定义（通常置空指针）。
- 需手动实现资源转移逻辑。

**限制**：
- 仅适用于右值（临时对象或显式 `std::move` 转换后的对象）。

**代码示例**：
```cpp
class MyArray {
public:
    // 移动构造函数
    MyArray(MyArray&& other) noexcept 
        : data(other.data), size(other.size) {
        other.data = nullptr; // 置空原对象指针
        other.size = 0;
    }
};
```

---

#### 五、移动赋值运算符 (Move Assignment Operator)
**定义**：  
将同类型对象的**右值引用**赋值给已存在的对象，转移资源。格式：  
`ClassName& operator=(ClassName&& other) noexcept`

**用途**：
- 高效赋值临时对象或显式转移资源（如 `a = std::move(b);`）。

**优点**：
- 避免深拷贝开销，直接接管资源。
- 支持容器元素的高效移动（如 `std::vector` 排序）。

**缺点**：
- 需处理自赋值和释放当前资源。

**限制**：
- 原对象状态不确定，不可再使用其资源。

**代码示例**：
```cpp
class MyArray {
public:
    // 移动赋值运算符
    MyArray& operator=(MyArray&& other) noexcept {
        if (this != &other) { // 自赋值检查
            delete[] data;    // 释放当前资源
            data = other.data; // 接管资源
            size = other.size;
            other.data = nullptr; // 置空原对象
            other.size = 0;
        }
        return *this;
    }
};
```

---

### 六、综合代码示例
```cpp
#include <algorithm>
#include <iostream>

class DynamicArray {
private:
    int* data_;
    size_t size_;

public:
    // 1. 默认构造函数
    DynamicArray() : data_(nullptr), size_(0) {}

    // 2. 带参数的构造函数
    explicit DynamicArray(size_t size) : data_(new int[size]{}), size_(size) {}

    // 3. 拷贝构造函数（深拷贝）
    DynamicArray(const DynamicArray& other) : size_(other.size_) {
        data_ = new int[size_];
        std::copy(other.data_, other.data_ + size_, data_);
    }

    // 4. 拷贝赋值运算符
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            std::copy(other.data_, other.data_ + size_, data_);
        }
        return *this;
    }

    // 5. 移动构造函数（转移资源）
    DynamicArray(DynamicArray&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    // 6. 移动赋值运算符
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            delete[] data_;//三步，先清理自己，再赋值，再清理右值
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    // 析构函数
    ~DynamicArray() {
        delete[] data_;
    }

    void print() const {
        std::cout << "Size: " << size_ << ", Data: ";
        for (size_t i = 0; i < size_; ++i) std::cout << data_[i] << " ";
        std::cout << "\n";
    }
};

int main() {
    // 默认构造
    DynamicArray arr1;
    arr1.print(); // Size: 0, Data: 

    // 带参数构造
    DynamicArray arr2(3);
    arr2.print(); // Size: 3, Data: 0 0 0 

    // 拷贝构造
    DynamicArray arr3 = arr2;
    arr3.print(); // Size: 3, Data: 0 0 0 

    // 移动构造
    DynamicArray arr4 = std::move(arr2);
    arr4.print(); // Size: 3, Data: 0 0 0 
    // arr2 已被移动，data_ 为 nullptr

    // 拷贝赋值
    arr1 = arr3;
    arr1.print(); // Size: 3, Data: 0 0 0 

    // 移动赋值
    arr1 = std::move(arr4);
    arr1.print(); // Size: 3, Data: 0 0 0 
    // arr4 已被移动，data_ 为 nullptr

    return 0;
}
```

---

### 七、总结对比表
| **构造函数/运算符**     | **关键特性**                              | **性能**        | **适用场景**                   |
|-------------------------|------------------------------------------|----------------|-------------------------------|
| **默认构造函数**        | 无参或默认参数初始化                     | 低开销         | 创建未初始化的对象            |
| **拷贝构造函数**        | 深拷贝资源，复制所有数据                 | 高开销（深拷贝）| 对象初始化或按值传递          |
| **拷贝赋值运算符**      | 释放旧资源后深拷贝新数据                 | 高开销（深拷贝）| 对象赋值操作                  |
| **移动构造函数**        | 转移资源所有权，原对象置空               | 低开销（指针操作）| 临时对象或显式移动语义        |
| **移动赋值运算符**      | 释放当前资源后接管新资源                 | 低开销（指针操作）| 高效赋值临时对象              |

---

### 八、最佳实践
1. **Rule of Five**：若定义了拷贝构造函数、拷贝赋值运算符、移动构造函数、移动赋值运算符或析构函数中的任意一个，应同时定义其他四个。
2. **优先使用 `= default`**：对不需要特殊处理的构造函数/运算符，使用 `= default` 让编译器生成。
3. **移动操作标记 `noexcept`**：确保容器（如 `std::vector`）能高效使用移动语义。
4. **避免返回局部对象的引用**：返回值优化（RVO）通常比移动更高效。