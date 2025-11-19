

### C++ 输出格式控制详解：`ostream` 与 `ios` 应用指南

---

#### **1. 格式化输出的两种方式**
C++ 提供 **标志（Flags）** 和 **操纵器（Manipulators）** 控制输出格式：
- **标志**：通过 `setf()` 和 `unsetf()` 设置布尔选项。
- **操纵器**：通过插入流中的对象（如 `std::hex`）简化操作。

---

#### **2. 基础格式设置**
##### **显示正数的 `+` 号**
```cpp
#include <iostream>
int main() {
    std::cout.setf(std::ios::showpos);  // 设置标志
    std::cout << 27 << "\n";           // 输出 +27
    std::cout.unsetf(std::ios::showpos); // 取消标志
    std::cout << 28 << "\n";           // 输出 28
    return 0;
}
```
**或使用操纵器**：
```cpp
std::cout << std::showpos << 27 << "\n";  // +27
std::cout << std::noshowpos << 28;        // 28
```

---

#### **3. 数值基数控制（进制转换）**
##### **设置十六进制、八进制**
```cpp
std::cout << std::hex << 27 << "\n";  // 1b（十六进制）
std::cout << std::oct << 27 << "\n";  // 33（八进制）
std::cout << std::dec << 27 << "\n";  // 27（十进制，默认）
```
**注意**：操纵器自动处理进制组（`std::ios::basefield`），避免冲突。

---

#### **4. 布尔值显示**
```cpp
std::cout << std::boolalpha << true << " " << false << "\n";  // true false
std::cout << std::noboolalpha << true << " " << false;         // 1 0
```

---

#### **5. 浮点数格式**
##### **精度与表示法**
- **默认模式**：根据数值大小自动选择小数或科学计数法。
- **固定小数（`fixed`）**：固定小数点后位数。
- **科学计数法（`scientific`）**：指数形式表示。

```cpp
#include <iomanip>
std::cout << std::fixed << std::setprecision(3) << 123.456 << "\n";  // 123.456
std::cout << std::scientific << std::setprecision(3) << 123.456;     // 1.235e+02
```

##### **显示小数点（`showpoint`）**
```cpp
std::cout << std::showpoint << 5.0 << "\n";  // 5.000（精度默认6）
```

---

#### **6. 字段宽度与对齐**
##### **设置宽度与填充**
- **`std::setw(n)`**：设置下一个输出的字段宽度（仅一次有效）。
- **对齐方式**：`left`（左对齐）、`right`（右对齐）、`internal`（符号左对齐，数值右对齐）。
- **填充字符**：`std::setfill(ch)`。

```cpp
std::cout << std::setw(10) << std::left << std::setfill('*') << -12345 << "\n";  // -12345****
std::cout << std::setw(10) << std::right << -12345 << "\n";                     // ****-12345
std::cout << std::setw(10) << std::internal << -12345;                          // -****12345
```

---

#### **7. 综合示例**
```cpp
#include <iostream>
#include <iomanip>

int main() {
    // 布尔值格式
    std::cout << std::boolalpha << true << " " << false << "\n";  // true false

    // 进制与符号
    std::cout << std::showpos << std::hex << 255 << "\n";          // +0xff

    // 浮点数精度与科学计数法
    double pi = 3.1415926535;
    std::cout << std::fixed << std::setprecision(5) << pi << "\n"; // 3.14159
    std::cout << std::scientific << pi;                            // 3.14159e+00

    // 字段对齐与填充
    std::cout << "\n" << std::setw(12) << std::left << std::setfill('-') << "Hello"; // Hello-------

    return 0;
}
```

---

#### **8. 注意事项**
1. **标志组的互斥性**：如设置进制时，需确保同组其他标志已关闭（推荐使用操纵器避免此问题）。
2. **`setw` 的临时性**：仅影响下一次输出。
3. **填充字符持久性**：设置后持续生效，直到再次更改。
4. **浮点模式选择**：`fixed` 和 `scientific` 影响精度含义（小数位 vs 有效数字）。

---

#### **9. 常用操纵器速查表**
| **操纵器**           | **作用**                     |
|-----------------------|------------------------------|
| `std::hex`/`std::oct` | 设置十六/八进制              |
| `std::showpos`        | 显示正数的 `+` 号            |
| `std::boolalpha`      | 布尔值显示为 `true`/`false` |
| `std::fixed`          | 固定小数表示                 |
| `std::scientific`     | 科学计数法                   |
| `std::setprecision(n)`| 设置浮点数精度               |
| `std::setw(n)`        | 设置字段宽度                 |
| `std::setfill(ch)`    | 设置填充字符                 |

---

通过合理组合标志与操纵器，可以精确控制输出格式，满足多样化需求。优先使用操纵器简化代码，确保格式设置安全高效。