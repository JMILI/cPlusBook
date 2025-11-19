### 省略号与可变参数（以及为什么要避免它们）

**省略号（Variadic Functions）**允许函数接受不定数量的参数，这在某些情况下非常有用，但它们也带来了一些显著的风险和复杂性。

#### 定义使用省略号的函数

省略号函数的基本形式如下：

```cpp
返回类型 函数名(正常参数列表, ...)
```

- **注意**：
    - 必须至少有一个非省略号参数。
    - 省略号必须是参数列表的最后一个参数。

#### 使用省略号的示例

让我们看一个简单示例，计算整数的平均值：

```cpp
#include <iostream>
#include <cstdarg>

double findAverage(int count, ...)
{
    int sum{ 0 };
    std::va_list list;
    va_start(list, count);

    for (int arg{ 0 }; arg < count; ++arg)
        sum += va_arg(list, int);

    va_end(list);

    return static_cast<double>(sum) / count;
}

int main()
{
    std::cout << findAverage(5, 1, 2, 3, 4, 5) << '\n';
    std::cout << findAverage(6, 1, 2, 3, 4, 5, 6) << '\n';
    return 0;
}
```

#### 访问省略号参数

- **使用 `va_list`**：
    - `va_start(list, count);` 初始化 `va_list`。
    - `va_arg(list, int);` 获取下一个参数。
    - `va_end(list);` 清理 `va_list`。

提供一个使用 `va_list`、`va_start`、`va_arg` 和 `va_end` 来访问省略号参数的具体例子，该例子实现一个计算多个整数之和的函数。

```cpp
#include <iostream>
#include <cstdarg>

// 计算多个整数之和的函数
int sum(int count, ...) {
    // 用于存储可变参数列表
    std::va_list args;
    // 初始化 va_list，count 是已知的参数数量
    va_start(args, count);

    int total = 0;
    // 循环获取每个可变参数并累加
    for (int i = 0; i < count; ++i) {
        // 获取下一个参数，这里假设参数类型为 int
        int num = va_arg(args, int);
        total += num;
    }

    // 清理 va_list
    va_end(args);

    return total;
}

int main() {
    // 调用 sum 函数，传入参数数量和具体的参数值
    int result1 = sum(3, 1, 2, 3);
    std::cout << "Sum of 1, 2, 3 is: " << result1 << std::endl;

    int result2 = sum(5, 10, 20, 30, 40, 50);
    std::cout << "Sum of 10, 20, 30, 40, 50 is: " << result2 << std::endl;

    return 0;
}
```

#### 为什么省略号可能很危险

1. **缺乏类型检查**：
    - 编译器不会检查省略号参数的类型，这意味着可以传递任何数据，导致可能的类型错误或垃圾数据。

   ```cpp
   std::cout << findAverage(6, 1.0, 2, 3, 4, 5, 6) << '\n'; // 会导致奇怪的结果
   ```

2. **不知道参数数量**：
    - 省略号不提供参数数量信息，因此需要提供额外的机制来确定参数的数量：

    - **方法1：传递长度参数**：
        - 需要准确传递参数数量，否则会处理垃圾数据。

    - **方法2：使用哨兵值**：
        - 使用特殊值（如 `-1`）作为参数列表的结束标志。此方法需要注意选择合适的哨兵值。

      ```cpp
      double findAverage(int first, ...)
      {
          int sum{ first };
          std::va_list list;
          va_start(list, first);
 
          int count{ 1 };
          while (true)
          {
              int arg{ va_arg(list, int) };
              if (arg == -1) break;
              sum += arg;
              ++count;
          }
          va_end(list);
          return static_cast<double>(sum) / count;
      }
      ```

    - **方法3：使用解码字符串**：
        - 传递一个字符串来描述参数类型和数量。

      ```cpp
      double findAverage(std::string_view decoder, ...)
      {
          double sum{ 0 };
          std::va_list list;
          va_start(list, decoder);
 
          for (auto codetype : decoder)
          {
              switch (codetype)
              {
              case 'i': sum += va_arg(list, int); break;
              case 'd': sum += va_arg(list, double); break;
              }
          }
          va_end(list);
          return sum / std::size(decoder);
      }
      ```

#### 安全使用省略号的建议

- **尽量避免使用省略号**：如果可以，使用替代解决方案，如传递动态数组或使用C++17的折叠表达式和可变参数模板。
- **一致性**：如果必须使用，确保所有省略号参数都是同一类型。
- **使用计数参数或解码字符串**：比哨兵值更安全，因为它强制用户正确提供参数信息。

#### 高级选项

C++11 引入了可变参数模板，提供了一种更类型安全的方式来处理可变参数。在 C++17 中，折叠表达式进一步简化了其使用，使之成为更实际的选择。

总的来说，虽然省略号提供了灵活性，但其复杂性和安全隐患使得在现代 C++ 中，应优先考虑更安全的替代方案。