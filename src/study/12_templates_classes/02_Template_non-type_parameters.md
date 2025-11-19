

### 模板非类型参数

---

#### **1. 什么是模板非类型参数？**
模板非类型参数允许在模板中传递**具体的值**（而不仅仅是类型），但这些值必须是**编译期常量**（`constexpr`）。它们用于在编译时确定某些参数，从而生成更高效或更灵活的代码。

• **与类型参数的区别**：
• **类型参数**：用`typename T`表示，传入的是类型（如`int`, `string`）。
• **非类型参数**：用具体类型（如`int`, `size_t`）表示，传入的是值（如`5`, `true`）。

---

#### **2. 非类型参数允许的类型**
非类型参数可以是以下类型：
| 类型                | 示例                       | 说明                          |
|---------------------|----------------------------|-------------------------------|
| **整型**            | `int`, `size_t`            | 最常用，如数组大小。          |
| **枚举类型**         | `enum Color { Red, Green }`| 传递枚举值。                  |
| **指针/引用**        | `int*`, `const char&`      | 指向对象或函数的指针/引用。   |
| **浮点型（C++20+）** | `float`, `double`          | C++20后支持。                 |
| `std::nullptr_t`    | `nullptr`                  | 表示空指针。                  |

---

#### **3. 示例代码解析**
以下是一个静态数组模板类的实现，使用**类型参数`T`**和**非类型参数`size`**：
```cpp
#include <iostream>

template <typename T, int size> // size 是一个 int 非类型参数
class StaticArray
{
private:
    // 非类型参数控制数组的大小
    T m_array[size] {};

public:
    T* getArray();
	
    T& operator[](int index)
    {
        return m_array[index];
    }
};

// 这里展示如何在类外部定义带非类型模版参数的函数
template <typename T, int size>
T* StaticArray<T, size>::getArray()
{
    return m_array;
}

int main()
{
    // 声明装了12个int的数组
    StaticArray<int, 12> intArray;

    // 按顺序填装，然后逆序打印
    for (int count { 0 }; count < 12; ++count)
        intArray[count] = count;

    for (int count { 11 }; count >= 0; --count)
        std::cout << intArray[count] << ' ';
    std::cout << '\n';

    // 声明装了4个double的数组
    StaticArray<double, 4> doubleArray;

    for (int count { 0 }; count < 4; ++count)
        doubleArray[count] = 4.4 + 0.1 * count;

    for (int count { 0 }; count < 4; ++count)
        std::cout << doubleArray[count] << ' ';

    return 0;
}
```

• **用法**：
  ```cpp
  StaticArray<int, 12> intArray;     // 创建大小为12的int数组
  StaticArray<double, 4> doubleArray;// 创建大小为4的double数组
  ```

• **关键点**：
• **编译期确定大小**：`size`必须是编译期常量（如`12`），因此数组`m_array`在栈上静态分配，无需动态内存管理。
• **高效安全**：避免动态数组的堆分配开销，且大小固定，防止越界。

---

#### **4. 非类型参数的限制**
• **必须是`constexpr`**：传入的值必须在编译时确定。
  ```cpp
  int x = 5;
  StaticArray<int, x> arr;  // 错误！x不是编译期常量
  ```
**正确做法**：
  ```cpp
  constexpr int x = 5;      // 编译期常量
  StaticArray<int, x> arr;  // 合法
  ```

• **类型严格匹配**：非类型参数的类型必须与模板声明一致。
  ```cpp
  StaticArray<int, 5u> arr; // 错误！5u是unsigned int，模板参数是int
  ```

---

#### **5. 实际应用案例：`std::array`**
标准库中的`std::array`正是使用非类型参数来指定数组大小：
```cpp
#include <array>
std::array<int, 5> arr;  // 类型参数int，非类型参数5
```

• **优势**：
• 替代C风格数组，提供迭代器、边界检查等特性。
• 性能与C数组相当，无额外开销。

---

#### **6. 非类型参数的常见用途**
1. **静态数组/矩阵大小**：如`StaticArray<T, size>`。
2. **模板元编程**：编译期计算，如斐波那契数列。
3. **策略模式**：传递编译期策略（如算法选择）。
4. **硬件寄存器映射**：用固定地址访问硬件。

---

#### **7. 类外定义成员函数**
若成员函数在类外定义，需携带模板参数声明：
```cpp
template <typename T, int size>
T* StaticArray<T, size>::getArray() {
    return m_array;
}
```
• **语法要点**：
• **模板声明**：`template <typename T, int size>`。
• **作用域限定**：`StaticArray<T, size>::`。

---

#### **8. 总结**
• **非类型参数**：允许传递编译期常量，增强模板灵活性。
• **适用场景**：需编译期确定参数（如数组大小、策略标志）。
• **限制**：必须为`constexpr`，类型严格匹配。
• **性能优势**：静态内存分配，避免运行时开销。

通过合理使用非类型参数，可以编写出高效、类型安全且易于复用的模板代码。