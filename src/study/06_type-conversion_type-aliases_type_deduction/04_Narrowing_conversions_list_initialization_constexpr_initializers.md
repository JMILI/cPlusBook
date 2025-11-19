### 窄化转换、列表初始化和`constexpr`初始化详解

在C++中，窄化转换（narrowing conversion）是一种潜在不安全的**数值转换**，其中目标类型可能无法保存源类型的所有值。为了确保代码的安全性和可维护性，理解窄化转换的概念及其相关的规则非常重要。

#### 窄化转换的定义

以下情况被视为窄化转换：

1. **从浮点类型到整型**：例如，将`double`转换为`int`。
2. **从浮点类型到较窄或较低级别的浮点类型**：除非转换的值是`constexpr`并且在目标类型的范围内。
3. **从整数类型转换为浮点类型**：除非转换的值是`constexpr`且其值可以精确存储在目标类型中。
4. **从整型转换为目标类型不能表示原始类型所有值的另一整型**：包括宽到窄的整数转换以及整数符号转换（有符号到无符号，反之亦然），除非转换的值是`constexpr`且其值可以精确存储在目标类型中。

#### 示例代码展示

```cpp
#include <iostream>

void someFcn(int i) {
}

int main() {
    double d{ 5.0 };

    // 不好的做法：隐式窄化转换可能导致编译器警告
    someFcn(d); 

    // 好的做法：使用static_cast进行显式窄化转换
    someFcn(static_cast<int>(d)); 

    return 0;
}
```

#### 列表初始化不允许窄化转换

使用大括号初始化时不允许窄化转换，尝试这样做会导致编译错误。

```cpp
int main() {
    // 编译失败：窄化转换
    int i { 3.5 }; 

    double d { 3.5 };
    // 正确的做法：使用static_cast进行显式转换
    int j { static_cast<int>(d) }; 

    return 0;
}
```

#### `constexpr`转换不被认为是窄化

当转换的源值是`constexpr`时，编译器可以在编译时确定是否会发生数据丢失。如果值能被完全保留，则这种转换不被认为是窄化的。

```cpp
#include <iostream>

int main() {
    constexpr int n1{ 5 };   // note: constexpr
    unsigned int u1 { n1 };  // okay: 这里的转换不被认为窄化

    constexpr int n2 { -5 }; // note: constexpr
    // 编译失败: 原值n2无法在u2里完全保存下来
    unsigned int u2 { n2 };  

    return 0;
}
```

#### 使用`constexpr`进行列表初始化

当使用列表初始化非`int`/非`double`对象时，可以利用`constexpr`来避免字面值后缀和`static_cast`的使用。

```cpp
int main() {
    // 免除字面值后缀
    unsigned int u { 5 }; // okay (不需要使用 `5u`)
    float f { 1.5 };      // okay (不需要使用 `1.5f`)

    // 可以不用 static_casts
    constexpr int n{ 5 };
    double d { n };       // okay (不需要 static_cast)
    short s { 5 };        // okay (不需要 static_cast)

    return 0;
}
```

#### 关键点总结

- **浮点类型按以下顺序排序（从大到小）**：
    - Long double
    - Double
    - Float

因此，这样的操作是合法的，不会产生错误：

```cpp
int main() {
    float f { 1.23456789 }; // 不是窄化转换, 即使发生了精度丢失!

    return 0;
}
```

然而，在这种情况下，编译器可能仍然会发出警告（如果使用-Wconversion编译标志，GCC和Clang会发出警告）。

### 表格归纳

| 转换类型 | 描述 | 示例 |
| --- | --- | --- |
| 浮点到整型 | 将浮点数转换为整数 | `int i = static_cast<int>(3.5);` |
| 浮点到较窄浮点类型 | 将较高精度浮点数转换为较低精度浮点数 | `float f = static_cast<float>(3.14159265358979323846);` |
| 整型到浮点类型 | 将整数转换为浮点数 | `float f = static_cast<float>(5);` |
| 整型到不同范围的整型 | 将一个整数转换为另一个范围不同的整数 | `unsigned int u = static_cast<unsigned int>(-5);` |

