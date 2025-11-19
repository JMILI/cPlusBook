### `void` 指针

`void` 指针，或称为泛型指针，是一种可以指向任何数据类型对象的特殊指针类型。其用途包括但不限于在不知道具体类型的情况下进行数据传递或存储。

#### 声明 `void` 指针

```cpp
void* ptr{}; // 声明一个 void 指针
```

#### 赋值

`void` 指针可以指向任何类型的对象：

```cpp
int nValue{};
float fValue{};
struct Something { int n; float f; } sValue{};

void* ptr{};
ptr = &nValue; // 有效
ptr = &fValue; // 有效
ptr = &sValue; // 有效
```

#### 使用 `void` 指针

- **不能直接解引用**：由于 `void` 指针不包含类型信息，尝试直接解引用会导致编译错误。
- **类型转换**：必须先将 `void` 指针转换为具体的指针类型后才能解引用。

```cpp
int value{5};
void* voidPtr{&value};

// std::cout << *voidPtr << '\n'; // 错误: 不能直接解引用 void 指针

int* intPtr{static_cast<int*>(voidPtr)}; // 转换为 int 指针

std::cout << *intPtr << '\n'; // 合法解引用
```

#### 实际使用 `void` 指针的示例

下面的代码示例展示了如何通过一个函数处理不同类型的指针：

```cpp
#include <cassert>
#include <iostream>

enum class Type { tInt, tFloat, tCString };

void printValue(void* ptr, Type type)
{
    switch (type)
    {
    case Type::tInt:
        std::cout << *static_cast<int*>(ptr) << '\n';
        break;
    case Type::tFloat:
        std::cout << *static_cast<float*>(ptr) << '\n';
        break;
    case Type::tCString:
        std::cout << static_cast<char*>(ptr) << '\n';
        break;
    default:
        std::cerr << "printValue(): invalid type provided\n"; 
        assert(false && "type not found");
        break;
    }
}

int main()
{
    int nValue{5};
    float fValue{7.5f};
    char szValue[]{ "Mollie" };

    printValue(&nValue, Type::tInt);
    printValue(&fValue, Type::tFloat);
    printValue(szValue, Type::tCString);

    return 0;
}
```

输出结果：

```
5
7.5
Mollie
```

#### 关于 `void` 指针的注意事项

- **空指针**：可以将 `void` 指针设置为 `nullptr`。
  ```cpp
  void* ptr{nullptr};
  ```

- **删除 `void` 指针**：`delete` 一个 `void` 指针会导致未定义行为。应先将其转换回原类型。
- **指针运算**：不能对 `void` 指针进行指针运算，因为它不包含大小信息。

- **没有 `void` 引用**：`void&` 是不存在的，因为引用必须有明确的类型。

#### 结论

`void` 指针虽然灵活，但由于缺乏类型安全，容易导致错误。现代 C++ 提供了更安全的替代方案，如函数重载和模板，这些方法保留了类型检查，减少了编程错误的可能性。`void` 指针的使用应谨慎，只有在没有更好替代方案时才考虑使用。