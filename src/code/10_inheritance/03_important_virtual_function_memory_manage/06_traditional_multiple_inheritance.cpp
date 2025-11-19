#include <iostream>
using namespace std;

class Base1 {
public:
  void func1() { cout << "Base1::func1" << endl; }
};

class Base2 {
protected:
  void func2() { cout << "Base2::func2" << endl; }
};

class Derived : public Base1, private Base2 {
public:
  void callBase2() { func2(); }  // 合法：通过派生类访问基类保护成员
};

int main() {
  Derived d;
  d.func1();                     // 输出: Base1::func1
  d.callBase2();                 // 输出: Base2::func2
  return 0;
}