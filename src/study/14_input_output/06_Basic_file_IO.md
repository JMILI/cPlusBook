

### C++ 文件 I/O 操作详解

本教程将系统讲解 C++ 中文件输入输出的核心概念与操作方法，涵盖文件读写、缓冲机制、文件模式及显式文件管理等内容。

---

#### **1. 文件流类概览**
C++ 通过 `<fstream>` 头文件提供三个核心文件流类：
- **`ofstream`**：输出文件流（继承自 `ostream`），用于写入文件。
- **`ifstream`**：输入文件流（继承自 `istream`），用于读取文件。
- **`fstream`**：双向文件流（继承自 `iostream`），支持读写操作。

---

#### **2. 写入文件（`ofstream`）**
##### **基本流程**
1. 创建 `ofstream` 对象并打开文件。
2. 检查文件是否成功打开。
3. 使用插入运算符 `<<` 写入数据。
4. 自动或手动关闭文件。

##### **示例代码**
```cpp
#include <fstream>
#include <iostream>

int main() {
    std::ofstream outf{"Sample.txt"};
    if (!outf) {
        std::cerr << "无法打开文件！\n";
        return 1;
    }
    outf << "第一行内容\n";
    outf << "第二行内容\n";
    return 0; // 自动关闭文件
}
```

##### **关键点**
- **构造函数参数**：文件名（可包含路径）。
- **错误检查**：若文件打开失败，流对象转换为 `false`。
- **自动关闭**：对象析构时自动调用 `close()`。

---

#### **3. 读取文件（`ifstream`）**
##### **逐词读取**
```cpp
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream inf{"Sample.txt"};
    if (!inf) {
        std::cerr << "无法打开文件！\n";
        return 1;
    }
    std::string word;
    while (inf >> word) { // 遇到空格/换行停止
        std::cout << word << '\n';
    }
    return 0;
}
```
**输出**：
```
第一行内容
第二行内容
```

##### **逐行读取**
使用 `std::getline()` 处理含空格的整行：
```cpp
std::string line;
while (std::getline(inf, line)) {
    std::cout << line << '\n';
}
```

---

#### **4. 缓冲机制与刷新**
- **缓冲目的**：提升I/O性能，减少磁盘操作次数。
- **自动刷新**：程序正常结束、缓冲区满或关闭文件时。
- **手动刷新**：
  ```cpp
  outf.flush();          // 方法1
  outf << std::flush;    // 方法2
  outf << "内容" << '\n'; // 使用 '\n' 替代 endl 避免频繁刷新
  ```

---

#### **5. 文件模式**
通过第二个参数指定文件打开方式：
| 模式标志       | 描述                       |
|----------------|----------------------------|
| `std::ios::app`  | 追加模式（不覆盖原有内容）  |
| `std::ios::trunc`| 清空文件（默认行为）        |
| `std::ios::binary` | 二进制模式（非文本）      |

##### **追加写入示例**
```cpp
std::ofstream outf{"Sample.txt", std::ios::app};
if (outf) {
    outf << "追加内容\n";
}
```

---

#### **6. 显式文件管理**
##### **手动开闭文件**
```cpp
std::ofstream outf;
outf.open("Data.txt");
if (outf.is_open()) {  // 检查是否打开
    outf << "数据";
    outf.close();      // 显式关闭
}
```

##### **重新打开文件**
```cpp
outf.open("Data.txt", std::ios::app); // 以追加模式重新打开
```

---

#### **7. 综合应用：读写二进制文件**
##### **写入二进制数据**
```cpp
#include <fstream>

struct Person {
    char name[50];
    int age;
};

int main() {
    Person p = {"Alice", 30};
    std::ofstream bf{"person.dat", std::ios::binary};
    bf.write(reinterpret_cast<char*>(&p), sizeof(Person));
    return 0;
}
```

##### **读取二进制数据**
```cpp
Person p;
std::ifstream bf{"person.dat", std::ios::binary};
if (bf.read(reinterpret_cast<char*>(&p), sizeof(Person))) {
    std::cout << p.name << " " << p.age << std::endl;
}
```

---

#### **8. 错误处理与最佳实践**
1. **始终检查文件是否成功打开**。
2. **优先使用RAII**：依赖析构函数自动关闭文件，减少资源泄漏风险。
3. **谨慎处理缓冲**：关键数据及时手动刷新。
4. **明确文件模式**：避免意外覆盖或数据损坏。

---

#### **总结**
- **核心类**：`ofstream`、`ifstream`、`fstream`。
- **基本操作**：`<<` 写入，`>>` 或 `getline` 读取。
- **缓冲机制**：理解自动与手动刷新场景。
- **模式控制**：通过标志位精确控制文件行为。
- **二进制处理**：直接操作内存数据，适合复杂结构存储。

通过掌握这些概念，您将能够高效安全地进行文件操作，满足各类应用场景需求。