## **std::array 与 类类型元素**

### 1. **基本概念**
`std::array` 是 C++11 中引入的容器类，类似于 C 风格的数组，但具备更多的功能和更好的类型安全。它是一个固定大小的数组类型，允许我们存储固定数量的元素，而且其大小在编译时就已确定。

数组中的元素可以是任意类型，不仅限于基本数据类型。换句话说，`std::array` 可以存储类类型的元素，如指针、结构体（struct）或类（class）。

### 2. **类类型元素的 `std::array` 初始化问题**
虽然 `std::array` 可以存储类类型的元素，但对新手来说，如何初始化一个含有结构体或类类型元素的 `std::array` 常常会导致混淆。让我们通过一个结构体示例来深入探讨这个话题。

---

### **2.1 定义并分配结构体给 `std::array`**

首先，我们定义一个结构体 `House`，它有三个成员：`number`、`stories` 和 `roomsPerStory`。

```cpp
struct House {
    int number{};
    int stories{};
    int roomsPerStory{};
};
```

接下来，我们创建一个 `std::array` 来存储3个 `House` 对象，并对每个元素进行赋值：

```cpp
#include <array>
#include <iostream>

int main() {
    std::array<House, 3> houses{};
    
    houses[0] = {13, 1, 7};
    houses[1] = {14, 2, 5};
    houses[2] = {15, 2, 4};

    for (const auto& house : houses) {
        std::cout << "House number " << house.number
                  << " has " << (house.stories * house.roomsPerStory)
                  << " rooms.\n";
    }

    return 0;
}
```

**输出：**
```
House number 13 has 7 rooms.
House number 14 has 10 rooms.
House number 15 has 8 rooms.
```

这里使用了 `houses[0] = {13, 1, 7};` 来直接为结构体数组的元素赋值。

---

### **2.2 使用 `CTAD` 自动推导类型初始化**

如果不想显式地指定数组的类型，我们可以通过 `CTAD`（类模板参数推导）让编译器自动推导类型。

```cpp
#include <array>
#include <iostream>

int main() {
    constexpr std::array houses { 
        House{13, 1, 7},
        House{14, 2, 5},
        House{15, 2, 4}
    };

    for (const auto& house : houses) {
        std::cout << "House number " << house.number
                  << " has " << (house.stories * house.roomsPerStory)
                  << " rooms.\n";
    }

    return 0;
}
```

在这个例子中，编译器会推导出 `std::array<House, 3>`，因此我们无需显式声明类型。

---

### **2.3 不显式指定每个初始值的元素类型**

有时你希望通过 `{13, 1, 7}` 的方式初始化数组中的结构体，但这样做会导致编译错误。

```cpp
constexpr std::array<House, 3> houses {
    {13, 1, 7},
    {14, 2, 5},
    {15, 2, 4}
};
```

上面的代码无法按预期工作，因为 `std::array` 内部实际上是一个 C 样式的数组（即 `T implementation_defined_name[N]`）。因此，编译器会错误地把 `{13, 1, 7}` 当作初始化 `implementation_defined_name` 的值，而不是初始化 `House` 的元素。

### **2.4 正确的初始化方式**

要正确初始化 `std::array`，需要添加额外的大括号。具体如下：

```cpp
constexpr std::array<House, 3> houses {
    {13, 1, 7},  // 初始化元素 0
    {14, 2, 5},  // 初始化元素 1
    {15, 2, 4}   // 初始化元素 2
};
```

此时，编译器能够正确地识别每个元素的类型并进行初始化。

---

### **3. 大括号使用规则**

#### **3.1 大括号省略与双大括号**

通常情况下，我们会遇到单大括号和双大括号的使用。为了避免潜在的错误，可以始终使用双大括号进行初始化。

- **单大括号：** 如果不涉及嵌套初始化，使用单大括号即可。

```cpp
constexpr std::array<int, 5> arr { 1, 2, 3, 4, 5 };
```

- **双大括号：** 当嵌套结构体初始化时，必须使用双大括号。

```cpp
constexpr std::array<House, 3> houses {{ 
    {13, 1, 7}, 
    {14, 2, 5}, 
    {15, 2, 4} 
}};
```

---

### **3.2 大括号省略的规则**

C++ 中的聚合初始化支持“大括号省略”的特性，允许在某些情况下省略大括号。具体来说：

- **单个值初始化：** `std::array` 可以用单大括号初始化。
- **类类型元素初始化：** 在初始化 `std::array` 中的类类型元素时，如果没有显式提供类型信息，必须使用双大括号。

---

### **4. 另一个例子：学生结构体**

下面是一个用 `Student` 结构初始化 `std::array` 的示例：

```cpp
#include <array>
#include <iostream>
#include <string_view>

struct Student {
    int id{};
    std::string_view name{};
};

constexpr std::array students{
    Student{0, "Alex"},
    Student{1, "Joe"},
    Student{2, "Bob"}
};

const Student* findStudentById(int id) {
    for (auto& s : students) {
        if (s.id == id) return &s;
    }
    return nullptr;
}

int main() {
    constexpr std::string_view nobody { "nobody" };
    
    const Student* s1 = findStudentById(1);
    std::cout << "You found: " << (s1 ? s1->name : nobody) << '\n';
    
    const Student* s2 = findStudentById(3);
    std::cout << "You found: " << (s2 ? s2->name : nobody) << '\n';

    return 0;
}
```

**输出：**
```
You found: Joe
You found: nobody
```

### **5. 关键点总结**

- **结构体/类元素初始化：** 当使用 `std::array` 存储结构体或类时，确保正确使用大括号以初始化元素。
- **双大括号：** 用于聚合类型元素（如结构体）初始化时，始终使用双大括号，避免初始化失败。
- **类型推导：** 使用 `CTAD` 时，编译器可以自动推导出 `std::array` 的类型。

---

