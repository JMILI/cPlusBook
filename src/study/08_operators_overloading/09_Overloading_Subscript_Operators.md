### C++ 下标运算符重载详解

---

#### 一、核心概念
下标运算符（`operator[]`）允许类提供类似数组的访问方式。通过重载此运算符，可以控制对类内部数据的读写，增强封装性和安全性。

---

#### 二、基础重载实现
**目标**：允许通过 `list[2] = 3` 语法访问私有数组元素。  
**关键点**：
- 返回引用以支持左值赋值。
- 非 `const` 版本用于读写，`const` 版本用于只读。

```cpp
#include <iostream>

class IntList {
private:
    int m_list[10]{};
public:
    // 非 const 版本，允许读写
    int& operator[](int index) {
        return m_list[index];
    }

    // const 版本，只读
    const int& operator[](int index) const {
        return m_list[index];
    }
};

int main() {
    IntList list;
    list[2] = 3;          // 调用非 const 版本
    std::cout << list[2]; // 调用非 const 版本

    const IntList clist;
    std::cout << clist[2]; // 调用 const 版本
    return 0;
}
```

---

#### 三、避免代码重复：非 const 调用 const 版本
通过 `const_cast` 复用 `const` 版本的逻辑，减少代码冗余。

```cpp
class IntList {
private:
    int m_list[10]{};
public:
    // 非 const 版本调用 const 版本
    int& operator[](int index) {
        return const_cast<int&>(static_cast<const IntList&>(*this)[index]);
    }

    // const 版本实现核心逻辑
    const int& operator[](int index) const {
        return m_list[index];
    }
};
```

---

#### 四、边界检查增强安全性
在重载时加入索引有效性验证，防止越界访问。

```cpp
#include <cassert>
#include <iterator> // for std::size

class SafeArray {
private:
    int m_data[5]{};
public:
    int& operator[](int index) {
        assert(index >= 0 && static_cast<size_t>(index) < std::size(m_data));
        return m_data[index];
    }

    const int& operator[](int index) const {
        assert(index >= 0 && static_cast<size_t>(index) < std::size(m_data));
        return m_data[index];
    }
};
```

---

#### 五、处理指针对象的正确用法
指向对象的指针需先解引用再调用 `operator[]`。

```cpp
int main() {
    SafeArray* arr = new SafeArray;
    (*arr)[2] = 3; // 正确：解引用后调用 operator[]
    delete arr;
    return 0;
}
```

---

#### 六、支持非整数索引参数
下标运算符的参数可以是任意类型，提供灵活设计。

```cpp
#include <string>
#include <unordered_map>

class WordMap {
private:
    std::unordered_map<std::string, int> m_data;
public:
    int& operator[](const std::string& key) {
        return m_data[key];
    }
};

int main() {
    WordMap map;
    map["apple"] = 5;
    std::cout << map["apple"]; // 输出 5
    return 0;
}
```

---

#### 七、最佳实践与注意事项
1. **返回引用**：确保支持左值赋值操作。
2. **提供 `const` 版本**：允许 `const` 对象安全访问。
3. **边界检查**：提高程序健壮性，防止未定义行为。
4. **避免指针误用**：明确解引用指针后再访问。
5. **参数灵活性**：根据需求设计不同类型的索引参数。

---

#### 八、总结
通过合理重载下标运算符，可以：
- 提供直观的类数组访问接口。
- 增强数据访问的安全性（如边界检查）。
- 支持灵活的索引类型（整数、字符串等）。
- 保持代码简洁和高效，通过复用逻辑减少冗余。