#include <iostream>

// 第一个基类（包含纯虚函数）
class Base1 {
public:
  // 纯虚函数（必须被子类实现）
  virtual void pure_virtual() = 0;

  // 虚函数（可被子类覆盖）
  virtual void virtual_func() {
    std::cout << "Base1::virtual_func()" << std::endl;
  }

  // 普通函数（静态绑定）
  void normal_func() {
    std::cout << "Base1::normal_func()" << std::endl;
  }
};

// 第二个基类（包含纯虚函数）
class Base2 {
public:
  // 另一个纯虚函数
  virtual void another_pure_virtual() = 0;

  // 另一个虚函数
  virtual void another_virtual_func() {
    std::cout << "Base2::another_virtual_func()" << std::endl;
  }

  // 另一个普通函数
  void another_normal_func() {
    std::cout << "Base2::another_normal_func()" << std::endl;
  }
};

// 派生类（多继承）
class Derived : public Base1, public Base2 {
public:
  // 实现基类的纯虚函数
  void pure_virtual() override {
    std::cout << "Derived::pure_virtual()" << std::endl;
  }

  void another_pure_virtual() override {
    std::cout << "Derived::another_pure_virtual()" << std::endl;
  }

  // 覆盖基类的虚函数
  void virtual_func() override {
    std::cout << "Derived::virtual_func()" << std::endl;
  }
};

int main() {
  Derived d;

  // 通过Base1指针操作
  Base1* b1 = &d;
  b1->pure_virtual();      // 输出: Derived::pure_virtual()
  b1->virtual_func();      // 输出: Derived::virtual_func()
  b1->normal_func();       // 输出: Base1::normal_func()

  // 通过Base2指针操作
  Base2* b2 = &d;
  b2->another_pure_virtual();   // 输出: Derived::another_pure_virtual()
  b2->another_virtual_func();   // 输出: Base2::another_virtual_func()
  b2->another_normal_func();    // 输出: Base2::another_normal_func()

  return 0;
}