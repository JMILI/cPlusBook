### lambda（匿名函数）简介

参考链接：https://zhuanlan.zhihu.com/p/666631927

Lambda表达式（或称为闭包）在 C++ 中允许你定义匿名函数，这些函数可以直接在需要的地方定义，避免全局污染和增加代码的可读性。

#### 基本 lambda 结构

lambda 表达式的形式如下：

```cpp
[捕获列表] (参数列表) -> 返回类型
{
    函数语句;
}
```

- **捕获列表**：可以是空的，如果 lambda 不需要捕获任何外层作用域的变量。
- **参数列表**：可以为空，如果 lambda 不需要参数。
- **返回类型**：可选，如果省略，会自动推导为 `auto`。
- **函数语句**：lambda 的执行体。

#### 示例：使用 lambda 重写函数

考虑之前的例子，使用 `std::find_if` 查找数组中包含 "nut" 的字符串：

```cpp
#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>

int main()
{
    constexpr std::array<std::string_view, 4> arr{ "apple", "banana", "walnut", "lemon" };

    auto found = std::find_if(arr.begin(), arr.end(),
        [](std::string_view str)
        {
            return str.find("nut") != std::string_view::npos;
        });

    if (found == arr.end())
    {
        std::cout << "No nuts\n";
    }
    else
    {
        std::cout << "Found " << *found << '\n';
    }

    return 0;
}
```

输出:

```
Found walnut
```

#### Lambda的类型和存储

- **lambda 没有明确的类型**：编译器为每个 lambda 生成一个独特的类型。
- **存储 lambda**：
    - **使用函数指针**：仅限于没有捕获的 lambda。
    - **使用 `std::function`**：可以处理有捕获的 lambda。
    - **使用 `auto`**：存储 lambda 的真实类型，最高效。

#### Lambda 传递给函数

- **使用 `std::function`**：
  ```cpp
  void repeat(int repetitions, const std::function<void(int)>& fn)
  ```
- **使用模板参数**：
  ```cpp
  template <typename T>
  void repeat(int repetitions, const T& fn)
  ```
- **使用 C++20 的 `auto` 参数**：
  ```cpp
  void repeat(int repetitions, const auto& fn)
  ```
- **使用函数指针**（仅适用于无捕获的 lambda）：
  ```cpp
  void repeat(int repetitions, void (*fn)(int))
  ```

#### 泛型 Lambda

自 C++14 起，lambda 可以使用 `auto` 作为参数类型，允许编译器推导类型：

```cpp
auto sameLetter = std::adjacent_find(months.begin(), months.end(),
    [](const auto& a, const auto& b) {
        return a[0] == b[0];
    });
```

#### Constexpr Lambda

从 C++17 开始，如果满足条件，lambda 可以是 `constexpr`：

```cpp
constexpr auto fiveLetterMonths = std::count_if(months.begin(), months.end(),
    [](std::string_view str) {
        return str.length() == 5;
    });
```

#### Lambda 与静态变量

注意，泛型 lambda 会为每种类型生成不同的实例：

```cpp
auto print = [](auto value) {
    static int callCount{ 0 };
    std::cout << callCount++ << ": " << value << '\n';
};
```

每次对不同类型的调用会创建新的静态变量。

#### 返回类型推导和尾部返回类型

如果没有明确声明返回类型，lambda 将根据 `return` 语句推导：

```cpp
auto divide = [](int x, int y, bool intDivision) {
    if (intDivision) return x / y;  // int
    else return static_cast<double>(x) / y;  // Error: 不匹配
};
```

解决方案是显式声明返回类型：

```cpp
auto divide = [](int x, int y, bool intDivision) -> double {
    if (intDivision) return x / y;  // 隐式转换为 double
    else return static_cast<double>(x) / y;
};
```

#### 标准库函数对象

许多常见操作可以通过标准库中的函数对象完成，如 `std::greater`：

```cpp
std::sort(arr.begin(), arr.end(), std::greater{});
```

#### 结论

Lambda 提供了一种强大、灵活的编程工具，特别在与标准库算法结合使用时，可以简化代码并提升可读性。然而，对于复杂或需要重用的逻辑，普通函数可能更合适。







---


### Lambda捕获

#### 捕获子句和按值捕获

捕获子句允许 lambda 访问 lambda 定义之外的变量。这些变量在 lambda 创建时被捕获。

**示例**：

```cpp
#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>
#include <string>

int main()
{
  std::array<std::string_view, 4> arr{ "apple", "banana", "walnut", "lemon" };

  std::cout << "search for: ";
  std::string search{};
  std::cin >> search;

  auto found = std::find_if(arr.begin(), arr.end(), [search](std::string_view str) {
    return str.find(search) != std::string_view::npos;
  });

  if (found == arr.end())
  {
    std::cout << "Not found\n";
  }
  else
  {
    std::cout << "Found " << *found << '\n';
  }

  return 0;
}
```

在此例中，`search` 被按值捕获，这意味着在 lambda 创建时，`search` 的值被复制到 lambda 的内部。

#### 捕获的工作原理

- **克隆变量**：lambda 捕获一个变量时，它实际上是创建了该变量的一个副本。
- **类型不匹配**：捕获的变量类型可能会与外部作用域中的类型不同。

#### 默认捕获为 const

默认情况下，捕获的变量是 const 的，这意味着在 lambda 内不能修改它们。

**错误示例**：

```cpp
int main()
{
  int ammo{ 10 };
  auto shoot = [ammo]() {
    --ammo; // 错误：不能修改 const 变量
    std::cout << "Pew! " << ammo << " shot(s) left.\n";
  };
  return 0;
}
```

#### 可修改的捕获

使用 `mutable` 关键字可以使捕获的变量在 lambda 内可修改。

```cpp
int main()
{
  int ammo{ 10 };
  auto shoot = [ammo]() mutable {
    --ammo; // 现在合法
    std::cout << "Pew! " << ammo << " shot(s) left.\n";
  };
  shoot();
  shoot();
  std::cout << ammo << " shot(s) left\n"; // 10，因为修改了副本
  return 0;
}
```

#### 通过引用捕获

通过引用捕获变量允许 lambda 修改外部变量。

```cpp
int main()
{
  int ammo{ 10 };
  auto shoot = [&ammo]() {
    --ammo; // 修改外部的 ammo
    std::cout << "Pew! " << ammo << " shot(s) left.\n";
  };
  shoot();
  std::cout << ammo << " shot(s) left\n"; // 9
  return 0;
}
```

#### 捕获多个变量

可以捕获多个变量，混合通过值和引用的方式：

```cpp
int health{ 33 };
int armor{ 100 };
std::vector<CEnemy> enemies{};
// 按值捕获 health 和 armor, 按引用捕获 enemies.
[health, armor, &enemies](){};
```

#### 默认捕获

使用 `=` 按值捕获所有变量，或 `&` 按引用捕获所有变量：

```cpp
[=](){}; // 默认按值捕获
[&](){}; // 默认按引用捕获
```

#### 在 lambda 捕获中定义新变量

可以定义新变量直接在捕获子句中：

```cpp
auto found = std::find_if(areas.begin(), areas.end(),
                           [userArea{ width * height }](int knownArea) {
                             return userArea == knownArea;
                           });
```

#### 悬空捕获的变量

通过引用捕获的变量必须在 lambda 的整个生命周期内保持有效，否则会导致未定义行为：

```cpp
auto makeWalrus(const std::string& name)
{
  return [&]() { // 通过引用捕获，可能会导致悬空引用
    std::cout << "I am a walrus, my name is " << name << '\n';
  };
}
```

#### 可变 lambda 的意外副本

当 lambda 可变时，复制 lambda 会导致意外行为，因为每个副本都有自己的一组捕获变量：

```cpp
auto count = [i]() mutable { 
  std::cout << ++i << '\n'; 
};

auto otherCount = count; // 这里创建了 count 的副本
```

#### 避免副本问题

- **使用 `std::function` 包装**：

```cpp
std::function count{ [i]() mutable { 
  std::cout << ++i << '\n'; 
} };
```

- **使用 `std::ref` 传递引用**：

```cpp
myInvoke(std::ref(count)); // 传递 lambda 的引用
```

#### 最佳实践

- 尽量避免使用可变的 lambdas，因为它们增加了复杂性。
- 使用不可变的 lambda 或在必要时使用 `std::ref` 来避免副本问题。

---

### **问题一：函数与Lambda的对比**

#### **1. 何时使用函数，何时使用Lambda？**

| **场景**                  | **使用函数**                                                                 | **使用Lambda**                                                                 |
|---------------------------|-----------------------------------------------------------------------------|-------------------------------------------------------------------------------|
| **代码复用性**            | 当逻辑需要多次复用，或跨多个文件调用时。                                      | 当逻辑仅在一处使用，且与上下文紧密相关时。                                      |
| **复杂逻辑**              | 适合实现多行、复杂的逻辑（如递归、多分支）。                                  | 适合简单逻辑（如单行表达式）。                                                |
| **命名与调试**            | 需要命名和调试时（函数名在调用栈中可见）。                                    | 匿名特性可能增加调试难度。                                                    |
| **捕获上下文变量**        | 无法直接捕获外部变量，需通过参数传递。                                        | 可以直接通过捕获列表访问外部变量，简化参数传递。                                |
| **与标准库结合**          | 需要定义单独的函数对象（如函数指针或`std::function`）。                       | 可直接内联定义，与`std::algorithm`等库无缝结合。                               |


```cpp
  // 普通函数：可复用、命名清晰
  bool containsNut(std::string_view str) {
      return str.find("nut") != std::string_view::npos;
  }
  
  int main() {
      std::array<std::string_view, 4> arr{ "apple", "banana", "walnut", "lemon" };
      auto found = std::find_if(arr.begin(), arr.end(), containsNut); // 传递函数指针
      // ...
  }
```

```cpp
  int main() {
      std::array<std::string_view, 4> arr{ "apple", "banana", "walnut", "lemon" };
      std::string search = "nut";
      
      // Lambda直接捕获search，无需全局变量
      auto found = std::find_if(arr.begin(), arr.end(), 
          [&search](std::string_view str) { // 捕获外部search
              return str.find(search) != std::string_view::npos;
          });
      // ...
  }
```

---

#### **2. 两者的区别与优缺点**

| **特性**          | **普通函数**                                                                 | **Lambda**                                                                     |
|--------------------|-----------------------------------------------------------------------------|-------------------------------------------------------------------------------|
| **作用域**         | 全局或命名空间作用域，可能污染全局命名空间。                                  | 局部作用域定义，避免命名冲突。                                                |
| **变量访问**       | 只能通过参数传递访问变量。                                                    | 可通过捕获列表直接访问外部变量。                                              |
| **类型**           | 有明确的函数签名（如`void(int)`）。                                           | 匿名类型，每个Lambda有唯一类型。                                              |
| **存储位置**       | 代码段（编译后固定地址）。                                                    | 闭包对象存储在栈或堆中（取决于捕获方式和存储容器）。                          |
| **性能**           | 直接调用，无额外开销。                                                        | 可能因捕获变量或闭包对象生成产生微小开销（优化后通常可忽略）。                |
| **灵活性**         | 无法捕获上下文变量，需显式传递所有参数。                                        | 灵活捕获上下文变量，简化参数传递。                                            |

---

#### **3. Lambda的存储与数据结构**
- **存储位置**：
  - **无捕获的Lambda**：可转换为函数指针，存储在代码段。
  - **有捕获的Lambda**：生成闭包对象（编译器生成的匿名类实例），存储在栈或堆中（如通过`std::function`包装时可能涉及堆分配）。
- **数据结构**：
  - 编译器将Lambda转换为一个匿名类，捕获的变量作为该类的成员变量。
  - 例如：`[x, &y](){}` 会被转换为类似以下类：
  ```cpp
    // 编译器为Lambda [x=5](){} 生成的匿名类
    class __Lambda_12345 {
    public:
        __Lambda_12345(int x) : x_(x) {} // 捕获的变量作为成员
        void operator()() const { /* Lambda体 */ }
    private:
        int x_; // 按值捕获的x
    };
    
    // 使用示例
    int x = 5;
    auto lambda = __Lambda_12345(x); // 闭包对象存储在栈中
    lambda();
  ```

---

### **问题二：捕获变量 vs 传参**

#### **1. 核心区别**

| **特性**          | **捕获变量**                                                                 | **传参**                                                                       |
|--------------------|-----------------------------------------------------------------------------|-------------------------------------------------------------------------------|
| **作用域**         | 捕获的是定义Lambda时的外部变量（静态绑定）。                                  | 传递的是调用Lambda时的参数（动态绑定）。                                        |
| **生命周期**       | 需确保捕获变量的生命周期覆盖Lambda的使用期（尤其是引用捕获）。                | 参数的生命周期仅限Lambda执行期间。                                            |
| **修改能力**       | 按值捕获可修改副本（需`mutable`），按引用捕获可修改原变量。                    | 参数传递的是副本或引用（由参数类型决定）。                                      |
| **性能**           | 按值捕获可能涉及拷贝开销，按引用捕获无拷贝但需注意悬空引用。                    | 参数传递可能涉及拷贝开销（如按值传递）。                                        |

---

#### **2. 优缺点对比**

| **方式**          | **优点**                                                                 | **缺点**                                                                 |
|--------------------|-------------------------------------------------------------------------|-------------------------------------------------------------------------|
| **捕获变量**       | 1. 直接访问上下文变量，减少参数传递。<br>2. 可保存状态（如计数器）。       | 1. 可能引入悬空引用（引用捕获）。<br>2. 隐式依赖外部变量，降低可读性。   |
| **传参**           | 1. 显式依赖，代码更清晰。<br>2. 参数生命周期安全。                        | 1. 需显式传递所有依赖变量。<br>2. 无法保存状态（需通过返回值或副作用）。 |



---

#### **3. 能否相互替代？**
- **部分场景可替代**：
  - 若Lambda仅依赖参数，不依赖外部变量，两者等效。
  - 例如：
    ```cpp
    // 通过传参
    std::for_each(vec.begin(), vec.end(), [](int x) { ... });

    // 通过捕获（无意义，此处传参更优）
    int tmp{};
    std::for_each(vec.begin(), vec.end(), [tmp](int x) { ... }); // tmp未被使用
    ```
 
- **不可替代的场景**：
  - **需要保存状态**：如计数器必须通过捕获实现。
    ```cpp
    int count{0};
    auto counter = [count]() mutable { return ++count; }; // 状态保存
    ```
  - **依赖外部变量生命周期**：若外部变量在Lambda调用时已销毁，必须按值捕获。

---

#### **4. 何时使用捕获？何时传参？**

| **场景**                  | **推荐方式**       | **示例**                                                                 |
|---------------------------|-------------------|-------------------------------------------------------------------------|
| **依赖外部变量**          | 捕获              | Lambda需要访问外层循环变量或上下文状态。                                  |
| **需保存状态**            | 捕获（按值或引用）| 实现闭包（如生成累加器、计数器）。                                        |
| **参数与逻辑无关**        | 传参              | 逻辑仅依赖输入参数（如比较函数`[](int a, int b) { return a < b; }`）。    |
| **避免悬空引用**          | 传参              | 外部变量可能提前销毁时，优先按值传参。                                    |

---

### **总结**
- **函数 vs Lambda**：
  - 函数适合复杂、可复用的逻辑；Lambda适合简单、上下文相关的逻辑。
  - Lambda的闭包对象存储在栈/堆中，由捕获变量决定其内存结构。
- **捕获 vs 传参**：
  - 捕获用于绑定外部变量，传参用于动态输入。
  - 捕获可能引入生命周期问题，传参更安全但需显式传递。
  - 在需要保存状态或依赖外部变量时使用捕获，否则优先传参。

---





