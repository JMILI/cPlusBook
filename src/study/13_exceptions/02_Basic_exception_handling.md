

### 基本异常处理详解

---

#### **1. 异常处理的基本结构**
C++异常处理基于三个关键字：**`throw`**、**`try`**、**`catch`**。它们协作实现错误的抛出、监控和处理。

- **`throw`**：抛出异常信号，类似裁判吹哨暂停比赛。
- **`try`**：定义监控区域，检测内部代码是否抛出异常。
- **`catch`**：捕获并处理特定类型的异常，类似裁判判罚。

```cpp
try {
    // 可能出错的代码
    throw 42; // 抛出int类型异常
} catch (int e) {
    // 处理int类型异常
    std::cerr << "错误码：" << e << std::endl;
}
```

---

#### **2. 异常处理流程**
1. **抛出异常**：代码执行到 `throw` 时，立即停止当前流程。
2. **查找匹配的 `catch`**：从最近的 `try` 块开始，按顺序检查 `catch` 块类型是否匹配。
3. **处理异常**：找到匹配的 `catch` 块后，执行其代码，之后继续执行后续程序。

**示例**：
```cpp
#include <iostream>

int main() {
    try {
        throw "文件打开失败"; // 抛出const char*异常
    } catch (const char* msg) { // 匹配类型
        std::cerr << "错误信息：" << msg << std::endl;
    }
    std::cout << "程序继续运行" << std::endl;
    return 0;
}
```

**输出**：
```
错误信息：文件打开失败
程序继续运行
```

---

#### **3. 异常类型匹配规则**
- **严格类型匹配**：不会自动转换类型（如 `int` 异常不会被 `double` 的 `catch` 捕获）。
- **支持继承**：派生类异常可被基类 `catch` 捕获（后续课程详解）。

**示例**：
```cpp
try {
    throw 3.14; // 抛出double类型异常
} catch (int e) {   // 不匹配
    std::cerr << "整数错误：" << e;
} catch (double e) { // 匹配
    std::cerr << "浮点数错误：" << e;
}
```

---

#### **4. 实际应用示例**
##### **场景**：计算平方根时处理负数输入
```cpp
#include <cmath>
#include <iostream>

int main() {
    std::cout << "输入一个数：";
    double num;
    std::cin >> num;

    try {
        if (num < 0) {
            throw "不能对负数求平方根"; // 抛出异常
        }
        std::cout << "结果：" << std::sqrt(num) << std::endl;
    } catch (const char* error) { // 捕获const char*异常
        std::cerr << "错误：" << error << std::endl;
    }
    return 0;
}
```

**测试情况**：
- 输入正数（如 `9`）：正常计算并输出结果。
- 输入负数（如 `-4`）：抛出异常，捕获后输出错误信息。

---

#### **5. `catch` 块的四大用途**
##### **(1) 记录错误并继续**
```cpp
catch (const std::exception& e) {
    std::cerr << "日志：发生异常 - " << e.what() << std::endl;
    // 记录后继续执行其他逻辑
}
```

##### **(2) 返回错误码**
```cpp
int readFile() {
    try {
        // 可能抛出异常的操作
    } catch (const FileError& e) {
        return -1; // 返回错误码
    }
    return 0; // 成功返回0
}
```

##### **(3) 重新抛出异常**
```cpp
try {
    // 某些操作
} catch (const NetworkError& e) {
    // 处理部分逻辑后，重新抛出给上层处理
    throw; // 重新抛出原异常
}
```

##### **(4) 终止程序**
```cpp
int main() {
    try {
        // 关键初始化操作
    } catch (const FatalError& e) {
        std::cerr << "致命错误，程序终止：" << e.what() << std::endl;
        return 1; // 非正常退出
    }
    return 0;
}
```

---

#### **6. 注意事项**
- **资源释放**：利用 **RAII**（如智能指针）确保异常发生时资源自动释放。
- **避免空 `catch`**：空的 `catch` 块会“吞掉”异常，导致难以调试的问题。
- **异常安全**：确保代码在异常发生后仍保持一致性（如数据不变性）。

```cpp
class Database {
public:
    Database() {
        if (!connect()) {
            throw DatabaseException("连接失败"); // 构造函数抛出异常
        }
    }
    ~Database() {
        disconnect(); // 析构函数确保资源释放
    }
};
```

---

#### **7. 总结**
- **异常优势**：分离错误处理逻辑，支持跨函数传播，提升代码可读性和安全性。
- **适用场景**：处理不可恢复错误、构造函数失败、复杂逻辑中的错误传播。
- **核心原则**：合理使用 `try`/`catch`，结合 RAII 确保资源安全，避免滥用异常影响性能。

通过灵活运用异常机制，可以构建更健壮、易维护的 C++ 程序，有效应对各种运行时错误。