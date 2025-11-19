### 数值转换

在上一课中，我们介绍了数值提升（numeric promotion），它将特定的较窄数字类型转换为可以更高效处理的较宽数字类型（通常是 `int` 或 `double`）。除了数值提升外，C++ 还支持另一类更为广泛的数值类型转换，称为 **数值转换**。数值转换涵盖基本类型之间的其他类型转换。

### 数值转换的五种常见示例

1. **从 `int` 到 `short`**：
   ```cpp
   short s = 3; // int to short
   ```

2. **从 `int` 到 `long`**：
   ```cpp
   long l = 3; // int to long
   ```

3. **从 `short` 到 `char`**：
   ```cpp
   char ch = s; // short to char
   ```

4. **从 `int` 到 `unsigned int`**：
   ```cpp
   unsigned int u = 3; // int to unsigned int
   ```

5. **从 `float` 到 `double` 和 `double` 到 `long double`**：
   ```cpp
   float f = 3.0f; // double to float
   long double ld = 3.0; // double to long double
   ```

6. **从 `double` 到 `int`**：
   ```cpp
   int i = 3.5; // double to int
   ```

7. **从 `int` 或 `double` 到 `bool`**：
   ```cpp
   bool b1 = 3; // int to bool
   bool b2 = 3.0; // double to bool
   ```

### 关键点

- **数值提升规则** 覆盖的任何类型转换都被视为数值提升，而不是数值转换。
- 数值转换包括所有其他的类型转换。

### 安全与潜在不安全的转换

#### 安全转换

当目标类型可以精确地保存源类型的所有值时，这种转换是安全的。例如：

- `int` 到 `long`
- `short` 到 `double`

```cpp
int main()
{
    int n { 5 };
    long l = n; // okay, 产出 long 5

    short s { 5 };
    double d = s; // okay, 产出 double 5.0

    return 0;
}
```

编译器通常不会对隐式的保值转换发出警告。

#### 潜在不安全的转换

这些转换在某些情况下不会保持源值。例如，有符号和无符号类型之间的转换：

```cpp
int main()
{
    int n1 { 5 };
    unsigned int u1 { n1 }; // okay: 转换成 unsigned int 5 (值会保留)

    int n2 { -5 };
    unsigned int u2 { n2 }; // bad: 会生成超过 signed int范围的超大数字

    return 0;
}
```

在这种情况下，有符号整数 `-5` 被转换为无符号整数，导致超出有符号整数范围的大数值。

#### 重新解释转换

这类转换可能导致值超出原类型的范围。尽管转换后的值可以转换回源类型并产生原始值，但初始转换可能产生不符合预期的结果。

```cpp
int main()
{
    int u = static_cast<int>(static_cast<unsigned int>(-5)); // 将 '-5' 转换成 unsigned 再转回来
    std::cout << u << '\n'; // 打印 -5
  
    return 0;
}
```

#### 有损转换

这类转换可能导致数据丢失。例如，从 `double` 到 `int` 的转换会丢失小数部分。

```cpp
int i = 3.5; // 数据丢失: 转换到 int 3 (分数部分 0.5 丢失)
```

从双精度到单精度的转换也可能导致数据丢失：

```cpp
float g = 1.23456789; // 数据丢失: 转换到 float 1.23457 (精度丢失)
```

将丢失数据的值转换回源类型将导致与原始值不同的值：

```cpp
#include <iostream>

int main()
{
    double d { static_cast<double>(static_cast<int>(3.5)) }; // 将 double 3.5 转 int 再转回
    std::cout << d << '\n'; // 打印 3

    double d2 { static_cast<double>(static_cast<float>(1.23456789)) }; // 将 double 1.23456789 转 float 再转回
    std::cout << d2 << '\n'; // 打印 1.23457

    return 0;
}
```

### 关于平台差异的注意事项

某些转换在不同平台上可能会分为不同的类别。例如，`int` 到 `double` 在大多数现代系统上是安全的，但在某些架构上可能是有损的。例如：

```cpp
#include <iostream>

int main()
{
    std::cout << static_cast<long long>(static_cast<double>(10000000000000001LL));

    return 0;
}
```

这将打印 `10000000000000000`，最后一个数字已经丢失。

### 最佳实践与总结

- **安全转换**：确保目标类型可以精确表示源类型的所有值。
- **潜在不安全转换**：小心有符号和无符号类型之间的转换，以及可能导致数据丢失的转换。
- **有损转换**：尽量避免可能导致数据丢失的转换，特别是在浮点数和整数之间转换时。
- **编译器警告**：利用编译器警告来捕获潜在的不安全转换。

通过理解和遵循这些原则，您可以编写更健壮和可靠的代码，减少因数值转换引起的意外行为。