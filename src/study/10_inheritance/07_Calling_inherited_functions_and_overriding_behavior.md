### C++ 派生类函数调用与重写行为详解

---

#### 一、基类与派生类的函数调用规则
##### 1. 默认调用行为
- **规则**：当调用派生类对象的成员函数时，编译器优先查找派生类中的同名函数。
- **未找到时**：沿继承链向上查找基类中的同名函数。

```cpp
class Base {
public:
    void identify() { cout << "Base::identify()\n"; }
};

class Derived : public Base {};

int main() {
    Derived d;
    d.identify(); // 调用Base::identify()
    return 0;
}
```

##### 2. 派生类重写函数
- **完全覆盖**：在派生类中定义同名函数会隐藏基类版本。
- **示例**：
  ```cpp
  class Derived : public Base {
  public:
      void identify() { cout << "Derived::identify()\n"; }
  };

  int main() {
      Derived d;
      d.identify(); // 调用Derived::identify()
      return 0;
  }
  ```

---

#### 二、部分重用基类功能
##### 1. 显式调用基类函数
- **作用域限定符**：使用`基类名::函数名`显式调用基类版本。
- **应用场景**：在派生类函数中复用基类逻辑，并扩展新功能。

```cpp
class Derived : public Base {
public:
    void identify() {
        cout << "Derived::identify()\n";
        Base::identify(); // 调用基类版本
    }
};

int main() {
    Derived d;
    d.identify(); 
    // 输出:
    // Derived::identify()
    // Base::identify()
    return 0;
}
```

##### 2. 避免无限递归
- **错误示例**：
  ```cpp
  void Derived::identify() {
      identify(); // ❌ 无限递归调用自身
  }
  ```
- **正确做法**：始终使用作用域限定符调用基类函数。

---

#### 三、处理友元函数与运算符重载
##### 1. 友元函数的特殊性
- **不属于类成员**：无法直接通过作用域限定符调用基类友元函数。
- **解决方案**：使用`static_cast`将派生类对象转为基类引用。

```cpp
class Base {
public:
    friend ostream& operator<<(ostream& os, const Base&) {
        os << "Base";
        return os;
    }
};

class Derived : public Base {
public:
    friend ostream& operator<<(ostream& os, const Derived& d) {
        os << "Derived ";
        os << static_cast<const Base&>(d); // 调用基类版本
        return os;
    }
};

int main() {
    Derived d;
    cout << d; // 输出: Derived Base
    return 0;
}
```

---

#### 四、派生类中的重载解析
##### 1. 重载函数隐藏问题
- **现象**：派生类中新增重载函数会隐藏基类所有同名函数。
- **示例**：
  ```cpp
  class Base {
  public:
      void print(int)    { cout << "Base::print(int)\n"; }
      void print(double) { cout << "Base::print(double)\n"; }
  };

  class Derived : public Base {
  public:
      void print(double) { cout << "Derived::print(double)\n"; }
  };

  int main() {
      Derived d;
      d.print(5); // 调用Derived::print(double)，而非Base::print(int)
      return 0;
  }
  ```

##### 2. 恢复基类重载可见性
- **使用`using`声明**：将基类同名函数引入派生类作用域。
- **示例**：
  ```cpp
  class Derived : public Base {
  public:
      using Base::print; // 引入基类所有print重载
      void print(double) { cout << "Derived::print(double)\n"; }
  };

  int main() {
      Derived d;
      d.print(5); // 调用Base::print(int)
      return 0;
  }
  ```

---

#### 五、关键注意事项总结
| 场景                  | 问题                         | 解决方案                          |
|-----------------------|------------------------------|---------------------------------|
| 调用基类函数          | 派生类同名函数隐藏基类版本     | 使用`基类名::函数名`显式调用       |
| 友元函数调用          | 无法直接调用基类友元函数       | `static_cast`转为基类引用调用     |
| 重载函数隐藏          | 派生类新增重载隐藏基类所有版本 | 使用`using 基类名::函数名`引入基类 |
| 无限递归风险          | 错误调用导致递归              | 始终显式指定作用域                |

---

#### 六、最佳实践
1. **谨慎重写函数**：确保理解隐藏基类函数的影响。
2. **优先使用`using`**：在需要基类重载函数时引入作用域。
3. **明确作用域**：调用基类函数时使用作用域限定符避免歧义。
4. **单元测试验证**：确保派生类函数按预期调用基类逻辑。

通过掌握这些规则，可以有效管理派生类与基类的函数交互，构建灵活且可维护的类层次结构。