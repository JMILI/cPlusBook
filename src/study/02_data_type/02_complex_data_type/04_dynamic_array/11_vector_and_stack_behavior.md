## **std::vector与栈行为的应用**

在编程中，存储动态数据结构时，使用合适的容器非常重要，尤其是当数据的数量在编译时未知时。C++标准库中的`std::vector`是一个非常灵活的容器，能够处理动态大小的数据存储。在本节中，我们将探讨如何使用`std::vector`模拟栈的行为，并讨论其相关特性和应用。

### **栈（Stack）的定义**

栈是一种具有后进先出（LIFO）性质的容器。可以类比为自助餐厅中的盘子堆，操作只能在堆顶进行：

- **Push**：将一个新的盘子放置到栈顶。
- **Pop**：从栈顶移除一个盘子。

这种行为可以表示为**后进先出**（LIFO，Last-In-First-Out）。栈中最晚被添加的元素最先被移除。

### **编程中的栈**

栈的常见实现方式通过两个基本操作：
- **push**：将元素添加到栈顶。
- **pop**：从栈顶移除元素。

有些栈还提供：
- **top**：查看栈顶元素但不移除它。
- **empty**：检查栈是否为空。
- **size**：获取栈中元素的数量。

栈在计算机科学中应用广泛，特别是在函数调用的管理中。例如，函数调用栈跟踪了函数的调用顺序，每次进入一个新函数时，将一个记录压入栈顶，函数返回时，该记录被移除。

### **栈在C++中的实现**

C++并没有提供一个专门的栈容器类型，但`std::vector`、`std::deque` 和 `std::list` 等容器类可以被用来模拟栈行为。`std::vector`在实现栈行为时最为常见，利用其提供的尾部插入和删除操作。

### **使用`std::vector`模拟栈行为**

`std::vector`是一个动态数组，它能够根据需要自动扩展容量。其常用的栈操作函数如下表所示：

| 操作              | 函数名             | 行为描述                                        |
|-------------------|--------------------|------------------------------------------------|
| Push              | `push_back()`       | 将一个元素添加到栈顶。                         |
| Pop               | `pop_back()`        | 移除栈顶的元素。                              |
| Top               | `back()`            | 获取栈顶元素但不移除它。                      |
| Size              | `size()`            | 获取栈中元素的数量。                          |
| Empty             | `empty()`           | 检查栈是否为空。                              |
| Capacity          | `capacity()`        | 获取当前栈的容量（内部分配的空间）。          |

#### **示例：使用`std::vector`实现栈**

```cpp
#include <iostream>
#include <vector>

void printStack(const std::vector<int>& stack)
{
    if (stack.empty()) {
        std::cout << "Empty";
    }

    for (auto element : stack) {
        std::cout << element << ' ';
    }

    std::cout << "\tCapacity: " << stack.capacity() << "  Length: " << stack.size() << "\n";
}

int main() {
    std::vector<int> stack{}; // 空栈

    printStack(stack);
    stack.push_back(1); // Push 1
    printStack(stack);
    stack.push_back(2); // Push 2
    printStack(stack);
    stack.push_back(3); // Push 3
    printStack(stack);

    std::cout << "Top: " << stack.back() << '\n'; // Top元素

    stack.pop_back(); // Pop 3
    printStack(stack);
    stack.pop_back(); // Pop 2
    printStack(stack);
    stack.pop_back(); // Pop 1
    printStack(stack);

    return 0;
}
```

**输出：**

```
Empty   Capacity: 0  Length: 0
1       Capacity: 1  Length: 1
1 2     Capacity: 2  Length: 2
1 2 3   Capacity: 4  Length: 3
Top: 3
1 2     Capacity: 4  Length: 2
1       Capacity: 4  Length: 1
Empty   Capacity: 4  Length: 0
```

### **push_back与emplace_back的区别**

`push_back`和`emplace_back`都可以将元素添加到`std::vector`的末尾。两者的主要区别在于：
- **`push_back()`**：需要先创建一个对象，然后将该对象拷贝或移动到`std::vector`中。
- **`emplace_back()`**：直接在`std::vector`内部构造对象，避免了临时对象的创建和拷贝，因此更加高效。

#### **示例：`push_back`与`emplace_back`的差异**

```cpp
#include <iostream>
#include <vector>

class Foo {
public:
    Foo(int b) : m_b(b) {}
private:
    int m_b;
};

int main() {
    std::vector<Foo> stack;

    Foo f(1);
    stack.push_back(f);  // 复制f到vector
    stack.emplace_back(1); // 在vector内直接构造对象

    return 0;
}
```

在处理需要大量临时对象的情况下，`emplace_back()`提供了性能上的优势。

### **容量管理与避免多次重新分配**

`std::vector`会自动调整其容量以适应增加的元素。如果容量不够，`std::vector`会重新分配更多内存，这可能会带来性能开销。为了避免这种情况，可以使用`reserve()`方法预先为`std::vector`分配足够的内存。

- **`reserve()`**：确保`std::vector`有足够的容量来容纳指定数量的元素，但不会改变其长度。
- **`resize()`**：改变`std::vector`的长度并填充新元素。

#### **示例：使用`reserve()`优化性能**

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> stack;

    stack.reserve(10); // 为10个元素预留空间

    for (int i = 0; i < 10; ++i) {
        stack.push_back(i);
    }

    std::cout << "Capacity after reserve: " << stack.capacity() << std::endl;

    return 0;
}
```

**输出：**

```
Capacity after reserve: 10
```

### **动态输入与栈操作**

当用户输入数据时，`std::vector`的栈操作非常有用。例如，在一个不确定用户输入数据数量的情况下，可以使用`push_back()`不断添加元素，直到用户输入终止信号。

#### **示例：动态输入处理**

```cpp
#include <iostream>
#include <limits>
#include <vector>

int main() {
    std::vector<int> scoreList;

    while (true) {
        std::cout << "Enter a score (or -1 to finish): ";
        int score;
        std::cin >> score;

        if (score == -1) {
            break; // 输入结束
        }

        scoreList.push_back(score); // 将分数添加到vector中
    }

    std::cout << "Your list of scores: \n";
    for (const auto& score : scoreList) {
        std::cout << score << ' ';
    }

    return 0;
}
```

此程序允许用户输入一个未知数量的分数，并在输入完成后输出所有分数。

### **总结与最佳实践**

1. **使用`std::vector`模拟栈行为**：`push_back`、`pop_back` 和 `back` 是实现栈操作的核心函数。
2. **性能优化**：使用`emplace_back`避免不必要的复制，使用`reserve`避免多次内存重新分配。
3. **动态数据输入**：通过栈操作，可以轻松处理不确定数量的数据输入。

通过这些方法，我们可以轻松地管理动态数据存储，同时保持程序的简洁与高效。