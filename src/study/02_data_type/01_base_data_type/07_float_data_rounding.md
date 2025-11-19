<!-- TOC -->
* [一. 浮点数修约](#一-浮点数修约)
    * [1. **四舍五入修约（Round Half Up）**](#1-四舍五入修约round-half-up)
        * [原理：](#原理)
        * [案例：](#案例)
    * [2. **四舍五入凑偶修约（Round Half to Even，银行家舍入法）**](#2-四舍五入凑偶修约round-half-to-even银行家舍入法)
        * [原理：](#原理-1)
        * [案例：](#案例-1)
    * [3. **向下修约（Round Down，Floor）**](#3-向下修约round-downfloor)
        * [原理：](#原理-2)
        * [案例：](#案例-2)
    * [4. **向上修约（Round Up，Ceiling）**](#4-向上修约round-upceiling)
        * [原理：](#原理-3)
        * [案例：](#案例-3)
    * [5. **向零修约（Round Toward Zero）**](#5-向零修约round-toward-zero)
        * [原理：](#原理-4)
        * [案例：](#案例-4)
    * [6. **远离零修约（Round Away from Zero）**](#6-远离零修约round-away-from-zero)
        * [原理：](#原理-5)
        * [案例：](#案例-5)
    * [7. **截断修约（Truncation）**](#7-截断修约truncation)
        * [原理：](#原理-6)
        * [案例：](#案例-6)
    * [8. **随机修约（Stochastic Rounding）**](#8-随机修约stochastic-rounding)
        * [原理：](#原理-7)
        * [案例：](#案例-7)
    * [9. **自适应修约（Adaptive Rounding）**](#9-自适应修约adaptive-rounding)
        * [原理：](#原理-8)
        * [案例：](#案例-8)
    * [10. **科学修约（Scientific Rounding）**](#10-科学修约scientific-rounding)
        * [原理：](#原理-9)
        * [案例：](#案例-9)
    * [总结](#总结)
        * [代码实现](#代码实现)
        * [测试案例](#测试案例)
            * [输入：](#输入)
            * [输出：](#输出)
        * [代码说明](#代码说明)
        * [总结](#总结-1)
* [二. 修约之前,如果不知道需要精确的小数位数](#二-修约之前如果不知道需要精确的小数位数)
    * [1. **动态确定小数位数修约**](#1-动态确定小数位数修约)
        * [原理：](#原理-10)
        * [案例：](#案例-10)
        * [代码实现：](#代码实现-1)
        * [总结：](#总结-2)
    * [2. **基于数值范围修约**](#2-基于数值范围修约)
        * [原理：](#原理-11)
        * [案例：](#案例-11)
        * [代码实现：](#代码实现-2)
        * [总结：](#总结-3)
    * [总结对比](#总结对比)
* [三. 如果知道需要精确到小数点后几位](#三-如果知道需要精确到小数点后几位)
<!-- TOC -->


> 四舍五入和银行家算法(四舍五入凑偶修约)修约算法，实验代码：[dynamicRound.cpp](../../../code/02_data_type/01_base_data_type/dynamicRound.cpp)

# 一. 浮点数修约
修约（Rounding）是将一个数值近似为指定精度的过程。根据不同的应用场景和需求，存在多种修约方法。以下是常见的修约方法及其原理和案例：

---

## 1. **四舍五入修约（Round Half Up）**
### 原理：
- 当小数部分大于或等于 0.5 时，向上舍入；否则，向下舍入。
- 这是最常见的修约方法。

### 案例：
- 输入：`3.75`，保留 1 位小数。
- 修约过程：
    - 小数部分为 `0.75`，大于 0.5，向上舍入。
- 结果：`3.8`。

---

## 2. **四舍五入凑偶修约（Round Half to Even，银行家舍入法）**
### 原理：
- 当小数部分恰好为 0.5 时，舍入到最近的偶数。
- 其他情况下，与四舍五入修约相同。
- 这种方法可以减少舍入误差的累积。

### 案例：
- 输入：`2.5`，保留 0 位小数。
- 修约过程：
    - 小数部分为 `0.5`，且整数部分为 `2`（偶数），舍入到 `2`。
- 结果：`2`。

- 输入：`3.5`，保留 0 位小数。
- 修约过程：
    - 小数部分为 `0.5`，且整数部分为 `3`（奇数），舍入到 `4`。
- 结果：`4`。

---

## 3. **向下修约（Round Down，Floor）**
### 原理：
- 无论小数部分是多少，都向下舍入到指定精度。

### 案例：
- 输入：`3.75`，保留 1 位小数。
- 修约过程：
    - 直接舍去小数部分。
- 结果：`3.7`。

---

## 4. **向上修约（Round Up，Ceiling）**
### 原理：
- 无论小数部分是多少，都向上舍入到指定精度。

### 案例：
- 输入：`3.25`，保留 1 位小数。
- 修约过程：
    - 直接向上进位。
- 结果：`3.3`。

---

## 5. **向零修约（Round Toward Zero）**
### 原理：
- 对于正数，向下舍入；对于负数，向上舍入。
- 这种方法总是向零的方向舍入。

### 案例：
- 输入：`3.75`，保留 1 位小数。
- 修约过程：
    - 正数，向下舍入。
- 结果：`3.7`。

- 输入：`-3.75`，保留 1 位小数。
- 修约过程：
    - 负数，向上舍入。
- 结果：`-3.7`。

---

## 6. **远离零修约（Round Away from Zero）**
### 原理：
- 对于正数，向上舍入；对于负数，向下舍入。
- 这种方法总是远离零的方向舍入。

### 案例：
- 输入：`3.25`，保留 1 位小数。
- 修约过程：
    - 正数，向上舍入。
- 结果：`3.3`。

- 输入：`-3.25`，保留 1 位小数。
- 修约过程：
    - 负数，向下舍入。
- 结果：`-3.3`。

---

## 7. **截断修约（Truncation）**
### 原理：
- 直接截断小数部分，不进行任何舍入。

### 案例：
- 输入：`3.75`，保留 1 位小数。
- 修约过程：
    - 直接截断小数部分。
- 结果：`3.7`。

---

## 8. **随机修约（Stochastic Rounding）**
### 原理：
- 根据小数部分的大小，随机决定向上或向下舍入。
- 这种方法常用于机器学习中，以减少舍入偏差。

### 案例：
- 输入：`3.75`，保留 1 位小数。
- 修约过程：
    - 小数部分为 `0.75`，有 75% 的概率向上舍入，25% 的概率向下舍入。
- 结果：可能是 `3.8` 或 `3.7`。

---

## 9. **自适应修约（Adaptive Rounding）**
### 原理：
- 根据数据的分布或上下文动态调整修约规则。
- 这种方法通常用于特定领域（如金融、科学计算）。

### 案例：
- 输入：`3.75`，保留 1 位小数。
- 修约过程：
    - 根据上下文规则，可能选择向上或向下舍入。
- 结果：可能是 `3.8` 或 `3.7`。

---

## 10. **科学修约（Scientific Rounding）**
### 原理：
- 根据科学计数法的规则进行修约，通常用于表示非常大或非常小的数。

### 案例：
- 输入：`123456789`，保留 2 位有效数字。
- 修约过程：
    - 转换为科学计数法：`1.23456789e8`。
    - 保留 2 位有效数字：`1.2e8`。
- 结果：`120000000`。

---

## 总结

| 修约方法               | 原理                                                                 | 案例（输入：`3.75`，保留 1 位小数） |
|------------------------|----------------------------------------------------------------------|-------------------------------------|
| 四舍五入修约           | 小数部分 ≥ 0.5 向上舍入，否则向下舍入                                 | `3.8`                              |
| 四舍五入凑偶修约       | 小数部分 = 0.5 时舍入到最近的偶数，否则四舍五入                       | `3.8`                              |
| 向下修约               | 直接向下舍入                                                         | `3.7`                              |
| 向上修约               | 直接向上舍入                                                         | `3.8`                              |
| 向零修约               | 正数向下舍入，负数向上舍入                                           | `3.7`                              |
| 远离零修约             | 正数向上舍入，负数向下舍入                                           | `3.8`                              |
| 截断修约               | 直接截断小数部分                                                     | `3.7`                              |
| 随机修约               | 根据小数部分随机决定向上或向下舍入                                   | `3.7` 或 `3.8`                     |
| 自适应修约             | 根据上下文动态调整修约规则                                           | `3.7` 或 `3.8`                     |
| 科学修约               | 根据科学计数法保留有效数字                                           | `3.8e0`                            |

根据具体需求选择合适的修约方法，可以在精度、性能和适用性之间找到最佳平衡。


---

以下是各种修约方法的代码实现，包括四舍五入修约、四舍五入凑偶修约、向下修约、向上修约、向零修约、远离零修约、截断修约、随机修约和科学修约。每种方法都附有注释和测试案例。

---

### 代码实现

```cpp
#include <iostream>
#include <cmath>
#include <iomanip>
#include <random>
#include <string>

// 1. 四舍五入修约（Round Half Up）
double roundHalfUp(double value, int decimalPlaces) {
    double factor = std::pow(10, decimalPlaces);
    return std::round(value * factor) / factor;
}

// 2. 四舍五入凑偶修约（Round Half to Even，银行家舍入法）
double roundHalfToEven(double value, int decimalPlaces) {
    double factor = std::pow(10, decimalPlaces);
    double scaledValue = value * factor;
    double fractionalPart = scaledValue - std::floor(scaledValue);

    if (fractionalPart > 0.5) {
        scaledValue = std::ceil(scaledValue);
    } else if (fractionalPart < 0.5) {
        scaledValue = std::floor(scaledValue);
    } else {
        if (static_cast<int>(scaledValue) % 2 == 0) {
            scaledValue = std::floor(scaledValue);
        } else {
            scaledValue = std::ceil(scaledValue);
        }
    }

    return scaledValue / factor;
}

// 3. 向下修约（Round Down，Floor）
double roundDown(double value, int decimalPlaces) {
    double factor = std::pow(10, decimalPlaces);
    return std::floor(value * factor) / factor;
}

// 4. 向上修约（Round Up，Ceiling）
double roundUp(double value, int decimalPlaces) {
    double factor = std::pow(10, decimalPlaces);
    return std::ceil(value * factor) / factor;
}

// 5. 向零修约（Round Toward Zero）
double roundTowardZero(double value, int decimalPlaces) {
    double factor = std::pow(10, decimalPlaces);
    if (value > 0) {
        return std::floor(value * factor) / factor; // 正数向下舍入
    } else {
        return std::ceil(value * factor) / factor; // 负数向上舍入
    }
}

// 6. 远离零修约（Round Away from Zero）
double roundAwayFromZero(double value, int decimalPlaces) {
    double factor = std::pow(10, decimalPlaces);
    if (value > 0) {
        return std::ceil(value * factor) / factor; // 正数向上舍入
    } else {
        return std::floor(value * factor) / factor; // 负数向下舍入
    }
}

// 7. 截断修约（Truncation）
double truncate(double value, int decimalPlaces) {
    double factor = std::pow(10, decimalPlaces);
    return static_cast<int>(value * factor) / factor;
}

// 8. 随机修约（Stochastic Rounding）
double stochasticRound(double value, int decimalPlaces) {
    double factor = std::pow(10, decimalPlaces);
    double scaledValue = value * factor;
    double fractionalPart = scaledValue - std::floor(scaledValue);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    if (dis(gen) < fractionalPart) {
        return std::ceil(scaledValue) / factor;
    } else {
        return std::floor(scaledValue) / factor;
    }
}

// 9. 科学修约（Scientific Rounding）
std::string scientificRound(double value, int significantDigits) {
    std::ostringstream oss;
    oss << std::scientific << std::setprecision(significantDigits) << value;
    return oss.str();
}

int main() {
    double value = 3.75;
    int decimalPlaces = 1;
    int significantDigits = 2;

    // 测试各种修约方法
    std::cout << "Original value: " << value << std::endl;
    std::cout << "Round Half Up: " << roundHalfUp(value, decimalPlaces) << std::endl;
    std::cout << "Round Half to Even: " << roundHalfToEven(value, decimalPlaces) << std::endl;
    std::cout << "Round Down: " << roundDown(value, decimalPlaces) << std::endl;
    std::cout << "Round Up: " << roundUp(value, decimalPlaces) << std::endl;
    std::cout << "Round Toward Zero: " << roundTowardZero(value, decimalPlaces) << std::endl;
    std::cout << "Round Away from Zero: " << roundAwayFromZero(value, decimalPlaces) << std::endl;
    std::cout << "Truncate: " << truncate(value, decimalPlaces) << std::endl;
    std::cout << "Stochastic Round: " << stochasticRound(value, decimalPlaces) << std::endl;
    std::cout << "Scientific Round: " << scientificRound(value, significantDigits) << std::endl;

    return 0;
}
```

---

### 测试案例

#### 输入：
```cpp
double value = 3.75;
int decimalPlaces = 1;
int significantDigits = 2;
```

#### 输出：
```
Original value: 3.75
Round Half Up: 3.8
Round Half to Even: 3.8
Round Down: 3.7
Round Up: 3.8
Round Toward Zero: 3.7
Round Away from Zero: 3.8
Truncate: 3.7
Stochastic Round: 3.7 或 3.8（随机）
Scientific Round: 3.8e+00
```

---

### 代码说明

1. **四舍五入修约**：
- 使用 `std::round` 实现。

2. **四舍五入凑偶修约**：
- 当小数部分为 0.5 时，舍入到最近的偶数。

3. **向下修约**：
- 使用 `std::floor` 实现。

4. **向上修约**：
- 使用 `std::ceil` 实现。

5. **向零修约**：
- 正数向下舍入，负数向上舍入。

6. **远离零修约**：
- 正数向上舍入，负数向下舍入。

7. **截断修约**：
- 直接截断小数部分。

8. **随机修约**：
- 根据小数部分随机决定向上或向下舍入。

9. **科学修约**：
- 使用 `std::scientific` 和 `std::setprecision` 实现。

---

### 总结

- 代码实现了常见的修约方法，并提供了测试案例。
- 每种方法都附有注释，便于理解和使用。
- 根据具体需求选择合适的修约方法。

---

# 二. 修约之前,如果不知道需要精确的小数位数
有时候，得到的数据并不知道他需要精确到多少位，可以尝试下面方法

---

## 1. **动态确定小数位数修约**

### 原理：
- 从 0 位小数开始，逐步增加小数位数，直到修约后的值与原始值的误差小于某个阈值（如 `1e-7`）。
- 适用于需要高精度修约的场景。

### 案例：
- 输入：`27723.15999999997`
- 修约过程：
    - 逐步增加小数位数，直到误差小于 `1e-7`。
- 结果：`27723.16`

### 代码实现：
```cpp
#include <iostream>
#include <cmath>
#include <iomanip>

// 动态确定小数位数并修约
std::pair<int, double> determineDecimalPlacesAndRound(double value, double threshold = 1e-7) {
    if (value == 0.0) return {0, 0.0};

    int decimalPlaces = 0;
    double roundedValue = std::round(value);

    while (std::abs(value - roundedValue) > threshold) {
        decimalPlaces++;
        double factor = std::pow(10, decimalPlaces);
        roundedValue = std::round(value * factor) / factor;//这里直接采用的四舍五入修约方法，可以修改为第一章所述各种修约方法
    }

    return {decimalPlaces, roundedValue};
}

int main() {
    double value = 27723.15999999997;
    auto result = determineDecimalPlacesAndRound(value);
    std::cout << "Original value: " << std::fixed << std::setprecision(15) << value << std::endl;
    std::cout << "Rounded value: " << std::fixed << std::setprecision(result.first) << result.second << std::endl;
    return 0;
}
```

### 总结：
- **适用场景**：需要高精度修约的场景。
- **优点**：动态确定小数位数，确保修约后的值在允许的误差范围内。
- **限制**：计算量较大，适用于对精度要求高的场景。

---



## 2. **基于数值范围修约**

### 原理：
- 根据数值的大小动态调整修约规则。
- 例如，对于较大的数值，保留较少的小数位数；对于较小的数值，保留较多的小数位数。

### 案例：
- 输入：`27723.15999999997`
- 修约过程：
    - 数值大于 1000，保留 0 位小数。
- 结果：`27723`

### 代码实现：
```cpp
#include <iostream>
#include <cmath>
#include <iomanip>

// 基于数值范围修约
double adaptiveRound(double value) {
    double magnitude = std::abs(value);
    int decimalPlaces;

    if (magnitude >= 1000) {
        decimalPlaces = 0;
    } else if (magnitude >= 100) {
        decimalPlaces = 1;
    } else if (magnitude >= 10) {
        decimalPlaces = 2;
    } else {
        decimalPlaces = 3;
    }

    double factor = std::pow(10, decimalPlaces);
    return std::round(value * factor) / factor;
}

int main() {
    double value = 27723.15999999997;
    double roundedValue = adaptiveRound(value);

    std::cout << "Original value: " << std::fixed << std::setprecision(15) << value << std::endl;
    std::cout << "Adaptive rounded value: " << roundedValue << std::endl;

    return 0;
}
```

### 总结：
- **适用场景**：数值范围较大的场景。
- **优点**：根据数值大小动态调整修约规则，灵活性高。
- **限制**：修约规则需要根据具体场景调整。



---

## 总结对比

| 方法               | 适用场景                           | 优点                                   | 限制                                   |
|--------------------|------------------------------------|----------------------------------------|----------------------------------------|
| 动态确定小数位数   | 需要高精度修约的场景               | 动态确定小数位数，确保误差在允许范围内 | 计算量较大                             |
| 基于数值范围修约         | 数值范围较大的场景                 | 根据数值大小动态调整修约规则           | 修约规则需要根据具体场景调整           |


---

# 三. 如果知道需要精确到小数点后几位

可以直接使用标准的修约方法（如四舍五入、四舍五入凑偶等）来实现。