### 结构体指针和引用的成员选择操作

在 C++ 中，结构体可以作为对象、指针或引用存在，且每种情况下的成员访问方式有所不同。理解如何通过不同的方式访问结构体成员非常重要。接下来，我们将详细探讨结构体引用和指针的成员选择操作，并提供相关示例和最佳实践。

#### 1. 结构体引用的成员选择

当通过引用访问结构体时，引用就像一个对象本身一样。我们可以直接使用成员选择操作符（`.`）来访问结构体的成员。通过引用访问时，不需要进行任何特殊操作，因为引用本身就指向原对象。

**示例代码：**
```cpp
#include <iostream>

struct Employee {
    int id{};
    int age{};
    double wage{};
};

void printEmployee(const Employee& e) {
    std::cout << "Id: " << e.id << '\n';
    std::cout << "Age: " << e.age << '\n';
    std::cout << "Wage: " << e.wage << '\n';
}

int main() {
    Employee joe{ 1, 34, 65000.0 };
    ++joe.age;  // Joe 大了一岁
    joe.wage = 68000.0;  // Joe 升职加薪了

    printEmployee(joe);  // 打印 joe 的信息

    return 0;
}
```

在上面的示例中，通过 `joe` 的引用来访问和修改结构体成员，直接使用成员选择操作符 `.`。

#### 2. 结构体指针的成员选择

当结构体通过指针引用时，我们不能直接使用 `.` 操作符。因为指针存储的是对象的地址，所以我们需要先解引用指针，然后才能使用 `.` 来访问成员。

**错误示范：**
```cpp
#include <iostream>

struct Employee {
    int id{};
    int age{};
    double wage{};
};

int main() {
    Employee joe{ 1, 34, 65000.0 };

    Employee* ptr{ &joe };
    std::cout << ptr.id << '\n'; // 编译错误：不能直接对指针使用 operator. 

    return 0;
}
```

上面代码会编译失败，因为我们尝试直接对指针 `ptr` 使用 `.` 操作符，而 `ptr` 存储的是 `joe` 对象的地址。

#### 3. 通过指针访问结构体成员（解引用）

为了通过指针访问结构体的成员，我们需要先解引用指针，获取指向的对象，然后才能访问其成员。我们可以使用 `(*ptr).成员名` 来进行成员访问。

**示例代码：**
```cpp
#include <iostream>

struct Employee {
    int id{};
    int age{};
    double wage{};
};

int main() {
    Employee joe{ 1, 34, 65000.0 };

    Employee* ptr{ &joe };
    std::cout << (*ptr).id << '\n';  // 解引用指针后使用 . 操作符访问成员

    return 0;
}
```

在这个示例中，我们通过解引用指针 `(*ptr)` 来获取 `joe` 对象，然后使用 `.` 操作符访问其成员。

#### 4. 使用箭头操作符 `->` 简化代码

C++ 提供了一个简洁的操作符 `->`，用于通过指针访问结构体成员。这个操作符会自动执行解引用操作，相当于 `(*ptr).成员`，但语法更加简洁和易读。

**示例代码：**
```cpp
#include <iostream>

struct Employee {
    int id{};
    int age{};
    double wage{};
};

int main() {
    Employee joe{ 1, 34, 65000.0 };

    Employee* ptr{ &joe };
    std::cout << ptr->id << '\n';  // 使用 -> 操作符访问成员

    return 0;
}
```

在这个示例中，使用 `ptr->id` 来访问 `joe` 对象的 `id` 成员。这比 `(*ptr).id` 更简洁，而且避免了括号的使用，使代码更加清晰。

#### 5. 混合使用指针与非指针

有时，我们会在同一个表达式中混合使用指针和非指针类型。需要注意的是，成员选择运算符会根据变量类型来选择使用 `.` 或 `->`。

**示例代码：**
```cpp
#include <iostream>
#include <string>

struct Paw {
    int claws{};
};

struct Animal {
    std::string name{};
    Paw paw{};
};

int main() {
    Animal puma{ "Puma", { 5 } };

    Animal* ptr{ &puma };

    // ptr 是指针, 使用 ->
    // paw 不是指针, 使用 .

    std::cout << (ptr->paw).claws << '\n';  // 访问指针中的 paw 成员

    return 0;
}
```

在这段代码中，`ptr` 是一个指向 `Animal` 对象的指针，因此我们使用 `->` 来访问 `paw` 成员。而 `paw` 本身不是指针，因此我们继续使用 `.` 来访问 `claws` 成员。`(ptr->paw).claws` 语法是合法的，其中的括号是可选的，但有助于提高代码可读性。

#### 6. 最佳实践

- **通过指针访问成员时，始终使用箭头操作符 `->`**，避免手动解引用，简化代码并减少出错的机会。
- **通过引用或对象访问成员时，使用成员选择操作符 `.`**，这符合语法规范。
- **当混合使用指针和非指针时，确保正确使用 `.` 或 `->` 操作符。** 使用括号有助于提高代码的可读性。

### 总结

在 C++ 中，通过引用和指针访问结构体成员时有一些细微的差别。引用与对象的使用方式相同，可以直接使用 `.` 操作符。指针则需要解引用后才能访问成员，或者更简便地使用箭头操作符 `->`。理解这些基本操作对编写高效、清晰的 C++ 代码至关重要。