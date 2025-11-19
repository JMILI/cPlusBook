#include <iostream>
using namespace std;

class BaseA {
public:
  virtual void show() { cout << "BaseA::show" << endl; }
};


class Derived : public BaseA{
public:
  void show() override { cout << "Derived::show" << endl; }
};

int main() {
  Derived d;
  BaseA* a = &d;
  a->show();                     // 输出: Derived::show
  return 0;
}