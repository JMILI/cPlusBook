

### C++ 输入验证与流状态管理详解

输入验证是确保用户输入符合预期格式的关键步骤，而流状态管理则用于处理输入过程中可能出现的错误。以下是对这两个主题的详细解析：

---

#### **1. 流的状态标志**
流（如 `std::cin`）通过状态标志反映当前状态，包含以下四个标志：
- **`goodbit`**：一切正常（值为0）。
- **`badbit`**：严重错误（如流损坏）。
- **`eofbit`**：到达文件末尾（如输入结束）。
- **`failbit`**：非致命错误（如类型不匹配）。

##### **关键成员函数**
- **`good()`**：若流处于正常状态返回 `true`。
- **`fail()`**：若 `failbit` 或 `badbit` 被设置返回 `true`。
- **`clear()`**：重置所有标志为 `goodbit`。
- **`ignore(n, delim)`**：忽略最多 `n` 个字符，直到遇到 `delim`。

```cpp
int age;
while (true) {
    std::cout << "Enter age: ";
    std::cin >> age;
    if (std::cin.fail()) { // 输入失败（如非数字）
        std::cin.clear(); // 清除错误状态
        std::cin.ignore(1000, '\n'); // 清空缓冲区
    } else {
        break;
    }
}
```

---

#### **2. 输入验证策略**
##### **字符串验证**
验证字符串格式，如姓名只含字母和空格：
```cpp
#include <algorithm>
#include <cctype>

bool isValidName(const std::string& name) {
    return std::all_of(name.begin(), name.end(), [](char c) {
        return std::isalpha(c) || std::isspace(c);
    });
}

// 使用示例
std::string name;
do {
    std::cout << "Enter name: ";
    std::getline(std::cin, name);
} while (!isValidName(name));
```

##### **模板匹配验证**
验证固定格式（如电话号码 `(###) ###-####`）：
```cpp
#include <map>
#include <cctype>

bool inputMatches(const std::string& input, const std::string& pattern) {
    if (input.size() != pattern.size()) return false;
    static std::map<char, int(*)(int)> validators {
        {'#', &std::isdigit}, {'@', &std::isalpha}, {'_', &std::isspace}, {'?', [](int){return 1;}}
    };
    for (size_t i = 0; i < input.size(); ++i) {
        char mask = pattern[i];
        if (validators.count(mask)) {
            if (!validators[mask](input[i])) return false;
        } else if (input[i] != mask) {
            return false;
        }
    }
    return true;
}

// 使用示例
std::string phone;
do {
    std::cout << "Enter phone (###) ###-####: ";
    std::getline(std::cin, phone);
} while (!inputMatches(phone, "(###) ###-####"));
```

---

#### **3. 数字输入验证**
##### **直接提取法**
使用 `>>` 提取数字，处理失败情况：
```cpp
int age;
while (true) {
    std::cout << "Enter age: ";
    if (std::cin >> age) { // 提取成功
        std::cin.ignore(1000, '\n'); // 清空可能残留的输入
        if (age > 0) break;
    } else { // 提取失败
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }
    std::cout << "Invalid input. ";
}
```

##### **字符串转换法**
将输入读取为字符串，再严格转换为数字：
```cpp
#include <charconv>

std::optional<int> convertToInt(const std::string& s) {
    int result;
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), result);
    if (ec == std::errc{} && ptr == s.data() + s.size()) {
        return result;
    }
    return std::nullopt;
}

// 使用示例
std::string input;
int age;
do {
    std::cout << "Enter age: ";
    std::getline(std::cin, input);
    auto num = convertToInt(input);
    if (num && *num > 0) {
        age = *num;
        break;
    }
} while (true);
```

---

#### **4. 处理部分有效输入**
当用户输入如 `34abcd` 时，`std::cin >> age` 会提取 `34`，但后续字符仍留在流中。需清空缓冲区以避免后续问题：
```cpp
std::cin >> age;
std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清空所有残留字符
if (std::cin.gcount() > 1) { // 若有残留字符被清空
    std::cout << "Invalid trailing characters.\n";
}
```

---

#### **5. 总结与最佳实践**
1. **流状态管理**：
   - 每次输入操作后检查 `fail()` 或 `good()`。
   - 错误时调用 `clear()` 重置状态，并用 `ignore()` 清空缓冲区。

2. **输入验证**：
   - **字符串验证**：使用 `std::all_of` 或正则表达式。
   - **模板匹配**：自定义规则检查每个字符。
   - **数字验证**：优先使用字符串转换法确保严格格式。

3. **错误恢复**：
   - 始终处理无效输入，提供明确提示。
   - 避免残留数据影响后续输入，彻底清空缓冲区。

通过合理运用流状态管理和验证策略，可显著提升程序的健壮性和用户体验。