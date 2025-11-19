### 布尔值（Boolean）总结

布尔类型在编程中用于表示逻辑上的真（true）或假（false），这在日常生活中类似于“是”或“否”的回答。C++中的布尔类型称为`bool`，它允许变量存储这两个状态之一。

#### 布尔变量的声明与初始化

- **声明**：使用关键字`bool`来声明布尔变量。
  ```cpp
  bool b;
  ```
- **初始化**：可以使用`true`或`false`直接初始化布尔变量。
  ```cpp
  bool b1 { true };
  bool b2 { false };
  b1 = false; // 重新赋值为false
  bool b3 {}; // 默认初始化为false
  ```

#### 逻辑运算符NOT（!）

逻辑非运算符`!`可以用来翻转布尔值的状态：
```cpp
bool b1 { !true }; // 初始化为false
bool b2 { !false }; // 初始化为true
```

#### 布尔值的实际存储形式

布尔值在内存中以整数形式存储，`true`对应整数`1`，`false`对应整数`0`。因此，在进行布尔计算时，结果也是基于这些整数值。

#### 打印布尔值

默认情况下，`std::cout`打印布尔值为`0`（对于`false`）和`1`（对于`true`）。要打印为`true`或`false`，可以使用`std::boolalpha`选项：
```cpp
#include <iostream>

int main() {
    std::cout << std::boolalpha; // 设置为打印true/false而非0/1
    std::cout << true << '\n';   // 输出: true
    std::cout << false << '\n';  // 输出: false
    return 0;
}
```

#### 整数到布尔的转换

- 使用列表初始化不能直接将整数转换为布尔值。
- 在其他上下文中，整数`0`转换为`false`，任何非零整数转换为`true`。
  ```cpp
  bool b1 = 4; // 隐式转换为true
  bool b2 = 0; // 隐式转换为false
  ```

#### 输入布尔值

默认情况下，`std::cin`只接受`0`和`1`作为布尔输入。为了允许输入`true`或`false`，需要启用`std::boolalpha`选项：
```cpp
#include <iostream>

int main() {
    bool b{};
    std::cout << "Enter a boolean value: ";
    std::cin >> std::boolalpha; // 允许输入true/false
    std::cin >> b;
    std::cout << "You entered: " << b << '\n';
    return 0;
}
```
注意：当启用`std::boolalpha`时，`0`和`1`将不再被识别为有效的布尔输入。

#### 布尔返回值

布尔值常用于函数中返回逻辑判断的结果。例如，检查两个整数是否相等：
```cpp
bool isEqual(int x, int y) {
    return (x == y); // 如果x等于y，则返回true，否则返回false
}

int main() {
    std::cout << std::boolalpha;
    std::cout << isEqual(5, 5) << '\n'; // 输出: true
    std::cout << isEqual(6, 4) << '\n'; // 输出: false
    return 0;
}
```
