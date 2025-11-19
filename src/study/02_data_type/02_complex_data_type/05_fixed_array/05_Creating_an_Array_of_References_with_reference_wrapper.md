### **通过 `std::reference_wrapper` 创建引用的数组**

在 C++ 中，数组的元素必须是对象，不能是引用类型。虽然数组可以包含基本数据类型和指针类型，但引用类型不能直接存放在数组中。为了解决这个问题，C++ 提供了一个工具——`std::reference_wrapper`，它允许我们将引用类型作为数组元素来使用。

#### **背景：不能创建引用数组**

例如，我们希望在数组中存储对某些变量的引用，然而，C++ 不允许直接创建引用类型的数组：

```cpp
#include <array>
#include <iostream>

int main() {
    int x {1};
    int y {2};

    [[maybe_unused]] std::array<int&, 2> refarr {x, y};  // 编译失败: 不能创建引用的数据

    return 0;
}
```

这种代码会导致编译错误，因为引用类型本身不是对象，数组要求每个元素都是对象，不能重新绑定。

#### **解决方案：使用 `std::reference_wrapper`**

为了解决这个问题，C++ 提供了 `std::reference_wrapper`，它是一个模板类，可以包装任何类型的引用，从而使其可存放在标准容器（如 `std::array`）中。

`std::reference_wrapper` 是一个容器类，它接收一个类型模板参数 `T`，并像 `T&` 一样工作。`std::reference_wrapper` 允许存储引用类型的元素，从而解决了数组无法存储引用类型的问题。

#### **使用 `std::reference_wrapper` 创建引用数组**

以下是一个使用 `std::reference_wrapper` 的例子：

```cpp
#include <array>
#include <functional>  // for std::reference_wrapper
#include <iostream>

int main() {
    int x {1};
    int y {2};
    int z {3};

    std::array<std::reference_wrapper<int>, 3> arr {x, y, z};
    
    arr[1].get() = 5;  // 修改下标 1 处的值

    std::cout << "arr[1]: " << arr[1] << ", y: " << y << '\n';  // 打印 5 5
    
    return 0;
}
```

**输出:**
```
arr[1]: 5, y: 5
```

在这个例子中，我们创建了一个 `std::array`，其中包含了 `std::reference_wrapper<int>` 类型的元素，它们引用了 `x`、`y` 和 `z`。我们通过 `arr[1].get()` 修改了 `y` 的值，并打印了 `arr[1]` 和 `y`，可以看到它们都变为 `5`，这证明 `std::reference_wrapper` 正确地引用了原始变量。

#### **`std::reference_wrapper` 的使用细节**

- **`get()` 方法**：`std::reference_wrapper` 的对象通过 `get()` 方法提供对原始对象的引用，允许我们修改它们的值。

- **隐式转换**：`std::reference_wrapper` 会隐式地转换为其所包装的引用类型，便于直接使用。

#### **简化引用包装：`std::ref` 和 `std::cref`**

为了简化 `std::reference_wrapper` 的创建，C++ 提供了两个函数：`std::ref` 和 `std::cref`，这两个函数分别用于创建一个普通引用的包装器和常量引用的包装器。它们能自动推导模板类型，因此不需要显式地指定类型模板参数。

- **`std::ref`**：用于创建一个包装普通引用的 `std::reference_wrapper`。

  ```cpp
  int x {5};
  auto ref = std::ref(x);  // 推导为 std::reference_wrapper<int>
  ```

- **`std::cref`**：用于创建一个包装常量引用的 `std::reference_wrapper`。

  ```cpp
  const int x {5};
  auto cref = std::cref(x);  // 推导为 std::reference_wrapper<const int>
  ```

#### **`std::ref` 和 `std::cref` 的优势**

1. **简洁性**：这两个函数使得引用的包装更加简洁，避免了显式指定模板参数。
2. **与 `auto` 一起使用**：可以与 `auto` 关键字一起使用，自动推导类型，进一步减少冗长代码。

例如，使用 `std::ref` 和 `std::cref` 的例子：

```cpp
int x {5};
auto ref = std::ref(x);  // 自动推导为 std::reference_wrapper<int>

const int y {10};
auto cref = std::cref(y);  // 自动推导为 std::reference_wrapper<const int>
```

#### **总结**

- 通过 `std::reference_wrapper` 可以存储引用类型的对象，在数组、容器中使用引用类型时非常有用。
- `std::ref` 和 `std::cref` 提供了简便的方法来创建 `std::reference_wrapper` 实例，避免了显式声明模板类型的麻烦。
- `std::reference_wrapper` 和这些辅助函数使得引用类型的管理更加灵活和方便，特别是在容器中使用引用时。



### **`std::reference_wrapper` 与 `std::ref` 和 `std::cref` 对比表格**

| 函数名称         | 功能                          | 用法示例                          | 说明                        |
|----------------|-----------------------------|---------------------------------|---------------------------|
| `std::reference_wrapper` | 创建引用包装器                  | `std::reference_wrapper<int> ref{ x };`  | 显式指定模板参数，创建引用包装器。 |
| `std::ref`      | 创建普通引用的包装器           | `auto ref = std::ref(x);`       | 自动推导模板类型，包装普通引用。    |
| `std::cref`     | 创建常量引用的包装器           | `auto cref = std::cref(x);`     | 自动推导模板类型，包装常量引用。    |

通过使用 `std::reference_wrapper`，我们能够在不修改数据类型的情况下，灵活地将引用存储在标准容器中，使得 C++ 编程更加简洁和强大。