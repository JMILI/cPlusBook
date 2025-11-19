### 友元非成员函数（Friend Non-Member Functions）

#### 1. 友元的概念
在C++中，`friend`关键字允许类的成员函数、非成员函数或其他类访问类的私有成员和受保护成员。友元并不需要是类的成员函数，但它们具有访问类私有数据的权限，就像它们是类的成员一样。

友元的概念主要用于在不暴露类的内部实现的情况下，允许特定的函数或类能够直接访问类的私有成员。

#### 2. 友元非成员函数的定义与使用
友元非成员函数是指一个非成员函数被某个类声明为友元，这样该函数就可以访问该类的私有和受保护成员，尽管它不是该类的成员。

##### 示例 1：简单的友元非成员函数
```cpp
#include <iostream>

class Accumulator {
private:
    int m_value {0};

public:
    void add(int value) { m_value += value; }

    // 声明友元非成员函数
    friend void print(const Accumulator& accumulator);
};

void print(const Accumulator& accumulator) {
    // 访问 Accumulator 的私有成员
    std::cout << accumulator.m_value;
}

int main() {
    Accumulator acc;
    acc.add(5);  // 将 5 加到 accumulator
    print(acc);  // 调用友元非成员函数
    return 0;
}
```
在上面的例子中，`print()` 函数是 `Accumulator` 类的友元。虽然 `print()` 不是 `Accumulator` 的成员函数，它仍然能够访问 `m_value`，因为它是友元。

##### 友元声明
友元声明在类的定义中进行，通常在类的 `private` 或 `protected` 区域。使用 `friend` 关键字，告诉编译器哪个非成员函数可以访问类的私有成员。

#### 3. 在类中定义友元非成员函数
友元非成员函数不仅可以在类外部定义，也可以在类内部定义。尽管函数是在类内部定义的，它仍然被视为非成员函数。

##### 示例 2：在类内定义友元非成员函数
```cpp
#include <iostream>

class Accumulator {
private:
    int m_value {0};

public:
    void add(int value) { m_value += value; }

    // 类内定义友元非成员函数
    friend void print(const Accumulator& accumulator) {
        // 直接访问 Accumulator 的私有成员
        std::cout << accumulator.m_value;
    }
};

int main() {
    Accumulator acc;
    acc.add(5);
    print(acc);  // 调用友元非成员函数
    return 0;
}
```
尽管 `print()` 在 `Accumulator` 类的内部定义，它仍然是一个非成员函数，因为它没有作为成员函数来定义，而是作为友元函数访问类的私有数据。

#### 4. 使用友元非成员函数的场景
有时我们需要将某些功能（例如打印、比较等）作为非成员函数实现，而这些功能需要访问类的私有成员。例如，进行运算符重载或比较操作时，可以使用友元非成员函数。

##### 示例 3：友元非成员函数的对称性
```cpp
#include <iostream>

class Value {
private:
    int m_value {};

public:
    explicit Value(int v): m_value { v }  { }

    bool isEqualToMember(const Value& v) const;  // 成员函数
    friend bool isEqualToNonmember(const Value& v1, const Value& v2);  // 友元非成员函数
};

bool Value::isEqualToMember(const Value& v) const {
    return m_value == v.m_value;
}

bool isEqualToNonmember(const Value& v1, const Value& v2) {
    return v1.m_value == v2.m_value;
}

int main() {
    Value v1 {5};
    Value v2 {6};

    std::cout << v1.isEqualToMember(v2) << '\n';
    std::cout << isEqualToNonmember(v1, v2) << '\n';

    return 0;
}
```
在这个例子中，`isEqualToMember()` 是成员函数，而 `isEqualToNonmember()` 是友元非成员函数。非成员函数通常提供更好的对称性，因为两个对象都是显式传递给函数的。

#### 5. 友元函数访问多个类的成员
一个函数可以同时是多个类的友元，这使得它能够跨多个类访问私有数据。

##### 示例 4：友元函数访问多个类
```cpp
#include <iostream>

class Humidity;  // 前向声明 Humidity

class Temperature {
private:
    int m_temp { 0 };

public:
    explicit Temperature(int temp) : m_temp { temp } { }

    friend void printWeather(const Temperature& temperature, const Humidity& humidity);  // 友元函数声明
};

class Humidity {
private:
    int m_humidity { 0 };

public:
    explicit Humidity(int humidity) : m_humidity { humidity } { }

    friend void printWeather(const Temperature& temperature, const Humidity& humidity);  // 友元函数声明
};

void printWeather(const Temperature& temperature, const Humidity& humidity) {
    std::cout << "The temperature is " << temperature.m_temp << " and the humidity is " << humidity.m_humidity << '\n';
}

int main() {
    Humidity hum {10};
    Temperature temp {12};

    printWeather(temp, hum);  // 访问多个类的私有数据
    return 0;
}
```
在此示例中，`printWeather()` 是 `Temperature` 和 `Humidity` 的友元，它可以访问这两个类的私有成员。

#### 6. 友元函数与数据隐藏
使用友元函数并不违反数据隐藏原则，因为类本身授予友元访问权限。这是类的一种设计选择，它允许在特定情况下，允许外部函数或类访问类的私有成员。

#### 7. 友元函数的最佳实践
- **尽可能避免使用友元函数**：友元函数直接访问类的私有成员，减少了封装性和数据隐藏的优势。因此，应该优先考虑通过公共接口而不是友元函数访问类的数据。
- **使用公共接口**：如果可能，使用公共访问函数而非友元函数。这有助于隔离实现细节，使得修改类实现时对外部代码的影响最小化。
- **慎用多个友元函数**：当有多个友元函数时，类的设计变得复杂。应避免过多地依赖友元函数，以免产生链式的依赖关系。

#### 总结
- **友元非成员函数**使得外部函数可以访问类的私有成员，通常用于分离责任或提供更好的对称性。
- **优先考虑公共接口**，以减少友元函数的使用，从而保留类的封装性和数据隐藏。
