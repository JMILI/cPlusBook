### C++中的命名常量

#### 1. **常量的定义**
- **常量**是在程序执行期间不能更改的值。
- C++支持两种常量：
    - **命名常量**（Named constants）：与标识符关联的常量值。
    - **字面值常量**（Literal constants）：与标识符无关的常量值。

#### 2. **命名常量的类型**
- 在C++中，定义命名常量有三种方法：
    1. **常变量（Constant variables）**：最常见的命名常量类型。
    2. **类对象宏（Object-like macros）**：通过预处理器定义的宏。
    3. **枚举常数（Enumerated constants）**：通过枚举类型定义的常量。

#### 3. **常变量（Constant variables）**
- **常变量**是值不能改变的变量。
- 使用`const`关键字声明常变量：
  ```cpp
  const double gravity { 9.8 };  // 推荐将 const 放在类型前
  int const sidesInSquare { 4 }; // 可以但不推荐
  ```
- **必须初始化**：常变量在定义时必须初始化，且之后不能更改其值。
  ```cpp
  const double gravity { 9.8 }; // 正确
  gravity = 9.9; // 错误：常变量的值不能改变
  ```

#### 4. **命名约定**
- 常变量的命名约定：
    - C风格：带下划线的大写名称（如`EARTH_GRAVITY`）。
    - C++风格：带“k”前缀的插入名称（如`kEarthGravity`）。
    - 推荐使用与普通变量相同的命名约定（如`earthGravity`）。

#### 5. **const函数参数**
- 函数参数可以通过`const`关键字设置为常量：
  ```cpp
  void printInt(const int x) {
      std::cout << x << '\n';
  }
  ```
- **最佳实践**：通过值传递时不要使用`const`。

#### 6. **常量返回值**
- 函数的返回值也可以设置为常量：
  ```cpp
  const int getValue() {
      return 5;
  }
  ```
- **最佳实践**：按值返回时不要使用`const`。

#### 7. **类对象宏**
- 类对象宏是预处理器定义的常量：
  ```cpp
  #define MY_NAME "Fly"
  ```
- **不推荐使用**：宏不遵循作用域规则，调试困难，容易引发错误。

#### 8. **在多文件程序中使用常变量**
- 常变量可以在多个文件中共享，通常通过内联变量实现。

#### 9. **类型限定符**
- **类型限定符**是用于修改类型行为的关键字。
- C++中的类型限定符：
    - `const`：常量限定符。
    - `volatile`：用于告诉编译器对象的值可以随时更改。

#### 10. **最佳实践**
- 定义常量时，推荐使用常变量，而不是类对象宏。
- 通过值传递时不要使用`const`。
- 按值返回时不要使用`const`。

### 代码示例
```cpp
#include <iostream>

// 常变量
const double gravity { 9.8 };

// const函数参数
void printInt(const int x) {
    std::cout << x << '\n';
}

// 常量返回值
const int getValue() {
    return 5;
}

int main() {
    // 常变量
    std::cout << "Gravity: " << gravity << '\n';

    // const函数参数
    printInt(5);
    printInt(6);

    // 常量返回值
    std::cout << "Value: " << getValue() << '\n';

    return 0;
}
```

### 总结
- **常变量**是C++中最常见的命名常量类型，使用`const`关键字声明。
- **类对象宏**不推荐使用，因为它们不遵循作用域规则且难以调试。
- **const函数参数**和**常量返回值**在大多数情况下不推荐使用。
- **类型限定符**`const`和`volatile`用于修改类型的行为。