
### C++ 字符串流（String Stream）详解

字符串流类（如 `std::istringstream`、`std::ostringstream`、`std::stringstream`）允许将字符串作为流处理，支持类似 `cin`/`cout` 的输入输出操作。它们常用于字符串与数据的转换、格式化处理或临时缓冲数据。

---

#### **1. 字符串流类的基本用法**
##### **头文件与类型**
```cpp
#include <sstream>  // 必须包含此头文件

// 常用类型：
std::istringstream;  // 输入字符串流（只读）
std::ostringstream;  // 输出字符串流（只写）
std::stringstream;   // 双向字符串流（可读可写）
```

##### **示例：写入数据到字符串流**
```cpp
std::ostringstream oss;
oss << "Hello, " << 42 << " " << 3.14;  // 插入数据
std::string result = oss.str();         // 获取字符串："Hello, 42 3.14"
```

##### **示例：从字符串流读取数据**
```cpp
std::string input = "123 45.67";
std::istringstream iss(input);
int n;
double d;
iss >> n >> d;  // 提取数据：n=123, d=45.67
```

---

#### **2. 字符串与数字的转换**
字符串流简化了字符串和基本数据类型（如 `int`、`double`）之间的转换。

##### **将数字转换为字符串**
```cpp
std::ostringstream oss;
int age = 25;
double salary = 8500.50;
oss << "Age: " << age << ", Salary: " << salary;
std::cout << oss.str();  // 输出："Age: 25, Salary: 8500.5"
```

##### **将字符串解析为数字**
```cpp
std::string data = "42 3.14";
std::istringstream iss(data);
int num;
double pi;
iss >> num >> pi;  // num=42, pi=3.14
```

---

#### **3. 清空字符串流**
字符串流的缓冲区通过 `str("")` 清空，同时需调用 `clear()` 重置错误状态。

##### **错误示例（未清空状态）**
```cpp
std::stringstream ss;
ss << "Hello";
ss >> std::string{};      // 读取数据，流状态可能为 eof
ss << "World";             // 写入失败（流处于错误状态）
std::cout << ss.str();     // 输出仍为 "Hello"
```

##### **正确做法**
```cpp
std::stringstream ss;
ss << "Hello";
ss.str("");                // 清空缓冲区
ss.clear();                // 重置错误标志（重要！）
ss << "World";
std::cout << ss.str();     // 输出："World"
```

---

#### **4. 高级应用场景**
##### **解析复杂字符串**
```cpp
std::string input = "Name=Alice;Age=30;Salary=50000";
std::stringstream ss(input);
std::string token;
while (std::getline(ss, token, ';')) {  // 按';'分割
    std::istringstream iss(token);
    std::string key, value;
    std::getline(iss, key, '=');       // 按'='分割键值对
    std::getline(iss, value);
    std::cout << key << ": " << value << "\n";
}
// 输出：
// Name: Alice
// Age: 30
// Salary: 50000
```

##### **构建格式化字符串**
```cpp
std::ostringstream oss;
oss << std::fixed << std::setprecision(2)
    << "Total: $" << 1234.567 << "\n"
    << "Date: " << std::setfill('0') 
    << std::setw(2) << 5 << "/" << std::setw(2) << 12;
std::cout << oss.str();
// 输出：
// Total: $1234.57
// Date: 05/12
```

---

#### **5. 注意事项**
1. **宽字符流**：  
   若需处理宽字符（如 `wchar_t`），使用 `std::wistringstream`、`std::wostringstream`、`std::wstringstream`。

2. **性能**：  
   频繁创建/销毁字符串流可能影响性能，建议复用流对象并清空。

3. **错误处理**：  
   提取数据时需检查流状态：
   ```cpp
   std::istringstream iss("123 abc");
   int num;
   iss >> num;
   if (iss.fail()) {
       std::cerr << "解析失败！";
   }
   ```

---

#### **总结**
- **字符串流的核心功能**：  
- 将字符串作为流处理，支持 `<<` 和 `>>` 操作。  
- 实现字符串与数字的灵活转换。  
- **关键操作**：  
- `str()` 获取/设置缓冲区内容。  
- `clear()` 重置流状态。  
- **适用场景**：  
- 解析复杂格式的字符串（如 CSV、配置文件）。  
- 动态构建格式化输出（如日志、错误消息）。  
- 临时缓存数据供后续处理。

通过合理使用字符串流，可以简化字符串操作，提升代码的可读性和维护性。