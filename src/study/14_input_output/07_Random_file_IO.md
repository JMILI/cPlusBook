
### C++ 随机文件 I/O 操作详解

---

#### **1. 文件指针与随机访问**
- **文件指针**：每个文件流对象内部维护一个指针，指向当前读写位置。
- **随机访问**：允许直接跳转到文件的任意位置进行读写，适用于需要快速定位记录的场景。

---

#### **2. 关键函数：`seekg()` 与 `seekp()`**
- **`seekg(offset, origin)`**：调整输入流（如 `ifstream`）的文件指针。
- **`seekp(offset, origin)`**：调整输出流（如 `ofstream`）的文件指针。
- **参数说明**：
- **`offset`**：偏移量（字节数），可正可负。
- **`origin`**：基准位置，通过 `ios` 标志指定：
  - `std::ios::beg`：文件开头（默认）。
  - `std::ios::cur`：当前位置。
  - `std::ios::end`：文件结尾。

##### **示例代码**
```cpp
std::ifstream inf{"data.txt"};
inf.seekg(10, std::ios::beg);   // 移动到第10字节（从开头）
inf.seekg(-5, std::ios::cur);    // 从当前位置回退5字节
inf.seekg(0, std::ios::end);     // 移动到文件末尾
```

---

#### **3. 获取当前位置：`tellg()` 与 `tellp()`**
- **`tellg()`**：返回输入流当前指针位置。
- **`tellp()`**：返回输出流当前指针位置。
- **应用场景**：计算文件大小或记录位置。

```cpp
inf.seekg(0, std::ios::end);
std::streamsize size = inf.tellg();  // 文件总大小
std::cout << "文件大小: " << size << " 字节\n";
```

---

#### **4. 文本文件与二进制文件的差异**
- **文本文件**：
- 换行符可能因系统不同（Windows为 `\r\n`，Unix为 `\n`），导致偏移量计算复杂。
- 随机访问可能导致不可预测行为，需谨慎使用。
- **二进制文件**：
- 数据按字节精确存储，适合随机访问。
- 使用二进制模式打开：`std::ios::binary`。

```cpp
std::fstream file{"data.bin", std::ios::in | std::ios::out | std::ios::binary};
```

---

#### **5. 综合示例：修改文件内容**
**目标**：将文件中的元音字母替换为 `#`。

```cpp
#include <fstream>
#include <iostream>

int main() {
    std::fstream iofile{"sample.txt", std::ios::in | std::ios::out};
    if (!iofile) {
        std::cerr << "打开文件失败！\n";
        return 1;
    }

    char ch;
    while (iofile.get(ch)) {
        if ("aeiouAEIOU"s.find(ch) != std::string::npos) {
            iofile.seekp(-1, std::ios::cur);  // 回退1字节
            iofile << '#';                    // 写入替换字符
            iofile.seekg(iofile.tellg());      // 同步读指针位置
        }
    }
    return 0;
}
```

**说明**：
1. 以读写模式打开文件。
2. 逐字符读取，遇到元音时回退指针并写入 `#`。
3. 通过 `seekg(tellg())` 同步读指针，确保后续读取位置正确。

---

#### **6. 注意事项与陷阱**
- **跨平台换行符**：文本文件中的换行符差异可能导致偏移量错误，优先使用二进制模式处理结构化数据。
- **指针越界**：移动指针超出文件范围可能导致未定义行为，需结合 `tellg()` 校验位置。
- **读写切换**：在 `fstream` 中切换读写操作前，必须调整指针位置，否则可能覆盖数据或读取错误。
- **内存地址存储**：**切勿将指针值（内存地址）直接写入文件**，因程序每次运行时地址可能变化。

---

#### **7. 错误处理与验证**
- **检查文件是否成功打开**：流对象在布尔上下文中为 `false` 表示失败。
- **验证指针位置**：在 `seekg`/`seekp` 后使用 `tellg`/`tellp` 确认位置。
- **异常处理**：可启用流异常机制，但需谨慎使用。

```cpp
iofile.exceptions(std::ios::failbit | std::ios::badbit); // 启用异常
try {
    iofile.open("data.txt", std::ios::in);
} catch (const std::ios_base::failure& e) {
    std::cerr << "文件操作异常: " << e.what() << '\n';
}
```

---

#### **8. 高级技巧：快速修改二进制记录**
假设文件存储固定长度的结构体，可直接定位修改：

```cpp
struct Record {
    int id;
    char name[50];
};

// 修改第3条记录的name
std::fstream binFile{"data.bin", std::ios::binary | std::ios::in | std::ios::out};
Record rec;
binFile.seekg(2 * sizeof(Record), std::ios::beg);  // 跳过前两条
binFile.read(reinterpret_cast<char*>(&rec), sizeof(rec));
strcpy(rec.name, "New Name");
binFile.seekp(-sizeof(rec), std::ios::cur);  // 回退到记录起始位置
binFile.write(reinterpret_cast<char*>(&rec), sizeof(rec));
```

---

#### **总结**
- **核心函数**：`seekg`、`seekp`、`tellg`、`tellp`。
- **模式选择**：文本模式适合逐行处理，二进制模式适合精确随机访问。
- **同步读写指针**：在 `fstream` 中切换操作时需手动调整指针。
- **错误处理**：始终验证文件操作，避免未定义行为。

通过掌握随机文件访问技术，可以高效处理大型文件或数据库类应用，实现快速数据检索与修改。