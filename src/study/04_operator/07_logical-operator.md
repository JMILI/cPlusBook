
### 逻辑运算符

#### 逻辑运算符概述
C++ 提供了3种逻辑运算符：

| 运算符     | 符号 | 使用形式 | 结果                                       |
|------------|------|----------|--------------------------------------------|
| 逻辑非     | !    | !x       | x 为 true 时返回 false，x 为 false 时返回 true |
| 逻辑与     | &&   | x && y   | x 和 y 都为 true 时返回 true，否则返回 false  |
| 逻辑或     | \|\| | x \|\| y | x 或 y 为 true 时返回 true，否则返回 false    |

#### 逻辑非
逻辑非运算符 `!` 用于翻转布尔值。

**示例代码：**
```cpp
bool tooLarge{ x > 100 };
if (!tooLarge) {
    // do something with x
} else {
    // print an error
}
```

#### 逻辑与
逻辑与运算符 `&&` 用于测试两个条件是否都为 true。

**示例代码：**
```cpp
if (value > 10 && value < 20) {
    std::cout << "Your value is between 10 and 20\n";
} else {
    std::cout << "Your value is not between 10 and 20\n";
}
```

#### 逻辑或
逻辑或运算符 `||` 用于测试两个条件中是否有一个为 true。

**示例代码：**
```cpp
if (value == 0 || value == 1) {
    std::cout << "You picked 0 or 1\n";
} else {
    std::cout << "You did not pick 0 or 1\n";
}
```

#### 短路求值
逻辑与和逻辑或运算符支持短路求值：
- 对于 `&&`，如果左操作数为 false，则不会计算右操作数。
- 对于 `||`，如果左操作数为 true，则不会计算右操作数。

**示例代码：**
```cpp
if (x == 1 && ++y == 2) {
    // do something
}
```

#### 混合使用逻辑与和逻辑或
在同一表达式中混合使用逻辑与和逻辑或时，建议使用括号明确优先级。

**示例代码：**
```cpp
if ((value1 && value2) || (value3 && value4)) {
    // do something
}
```

#### 德摩根定律
德摩根定律描述了逻辑非与逻辑与、逻辑或之间的关系：
- `!(x && y)` 等价于 `!x || !y`
- `!(x || y)` 等价于 `!x && !y`

**示例代码：**
```cpp
if (!(x > 10 && y < 20)) {
    // do something
}
```

#### 逻辑异或（XOR）
C++ 中没有专门的逻辑异或运算符，但可以通过 `!=` 实现。

**示例代码：**
```cpp
if (a != b) {
    // a XOR b
}
```
