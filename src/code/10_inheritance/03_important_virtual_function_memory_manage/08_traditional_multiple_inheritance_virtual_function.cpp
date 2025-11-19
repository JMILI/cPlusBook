#include <iostream>
using namespace std;

class BaseA {
public:
  virtual void show() { cout << "BaseA::show" << endl; }
};

class BaseB {
public:
  virtual void print() { cout << "BaseB::print" << endl; }
};

class Derived : public BaseA, public BaseB {
public:
  void show() override { cout << "Derived::show" << endl; }
  void print() override { cout << "Derived::print" << endl; }
};

int main() {
  Derived d;
  BaseA* a = &d;
  BaseB* b = &d;
  a->show();                     // 输出: Derived::show
  b->print();                    // 输出: Derived::print
  return 0;
}