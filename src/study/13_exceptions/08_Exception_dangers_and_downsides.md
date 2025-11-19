

### 异常处理的风险、缺点与适用场景

---

#### **1. 资源清理问题**
异常可能导致资源未正确释放（如文件未关闭、内存泄漏）。关键在于**异常发生后的资源管理**。

##### **示例：文件操作**
```cpp
try {
    openFile("data.txt");
    writeFile("data.txt", "内容"); // 可能抛出异常
    closeFile("data.txt");         // 若异常发生，此行不会执行
} catch (const FileException& e) {
    std::cerr << "文件操作失败: " << e.what();
}
```
**问题**：若 `writeFile` 抛出异常，`closeFile` 不会执行，文件句柄泄漏。

##### **解决方案**
1. **RAII（资源获取即初始化）**：使用对象管理资源，析构时自动释放。
   ```cpp
   class FileHandler {
   public:
       FileHandler(const std::string& filename) { openFile(filename); }
       ~FileHandler() { closeFile(); } // 析构时自动关闭文件
   };

   try {
       FileHandler file("data.txt"); // 构造时打开文件
       writeFile(file, "内容");      // 析构时自动关闭
   } catch (const FileException& e) {
       std::cerr << "文件操作失败: " << e.what();
   }
   ```
2. **智能指针管理内存**：
   ```cpp
   #include <memory>
   try {
       auto john = std::make_unique<Person>("John", 18); // 自动管理内存
       processPerson(john.get());
   } catch (const PersonException& e) {
       std::cerr << "处理失败: " << e.what();
   } // unique_ptr 析构时自动释放内存
   ```

---

#### **2. 析构函数中的异常**
**绝对避免在析构函数中抛出异常**，否则可能导致程序直接终止。

##### **示例：析构函数异常**
```cpp
class Resource {
public:
    ~Resource() {
        if (清理失败) {
            throw "析构失败"; // 危险：析构函数抛出异常
        }
    }
};

int main() {
    try {
        Resource res;
    } catch (...) {
        // 可能无法捕获，程序直接终止
    }
    return 0;
}
```
**解决方式**：析构函数中仅记录错误，不抛出异常。
```cpp
~Resource() {
    try {
        // 清理逻辑
    } catch (const std::exception& e) {
        logError(e.what()); // 记录日志，但不抛出
    }
}
```

---

#### **3. 性能开销**
异常处理会增加运行时开销，尤其在抛出异常时。

- **零成本异常模型**：无异常时无额外开销，抛出时成本较高。
- **适用场景**：错误罕见且严重，如文件不存在、内存不足。

##### **何时关注性能**：
- **高频循环**：若异常可能频繁触发（如数据校验），改用错误码。
- **实时系统**：需要严格避免不确定性的延迟。

---

#### **4. 何时使用异常？**
异常适用于以下场景：

1. **错误罕见且严重**：如文件不存在、网络连接失败。
2. **无法在本地处理**：需传递给上层调用者处理。
3. **缺乏有效返回码**：如构造函数失败、运算符重载。

##### **示例：构造函数抛出异常**
```cpp
class Database {
public:
    Database(const std::string& config) {
        if (!loadConfig(config)) {
            throw std::runtime_error("配置加载失败");
        }
    }
};

int main() {
    try {
        Database db("config.json");
    } catch (const std::exception& e) {
        std::cerr << "数据库初始化失败: " << e.what();
        return 1;
    }
    return 0;
}
```

---

#### **5. 总结：异常处理的最佳实践**
| **场景**               | **建议**                                                                 |
|-------------------------|--------------------------------------------------------------------------|
| **资源管理**           | 使用 RAII 和智能指针，避免手动清理。                                     |
| **析构函数**           | 禁止抛出异常，仅记录错误日志。                                           |
| **高频或性能敏感代码** | 优先使用错误码，避免异常开销。                                           |
| **构造函数/复杂逻辑**  | 通过异常明确失败，确保对象有效性。                                       |
| **跨层错误传递**       | 异常优于层层返回错误码，提升代码清晰度。                                 |

---

#### **关键要点**
- **资源安全**：依赖 RAII 和智能指针，而非手动管理。
- **析构安全**：禁止在析构函数中抛出异常。
- **性能权衡**：异常适合处理罕见错误，高频场景慎用。
- **清晰错误传递**：异常分离正常逻辑与错误处理，提升代码可维护性。

通过合理使用异常，可以构建更健壮、易维护的程序，但需警惕其潜在风险，遵循最佳实践。