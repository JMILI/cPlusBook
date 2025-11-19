#include <iostream>
using namespace std;

class Interface1 {
public:
  virtual void method1() = 0;
};

class Interface2 {
public:
  virtual void method2() = 0;
};

class Concrete : public Interface1, public Interface2 {
public:
  void method1() override { cout << "Implemented method1" << endl; }
  void method2() override { cout << "Implemented method2" << endl; }
};

int main() {
  Concrete c;
  c.method1();                  // 输出: Implemented method1
  c.method2();                  // 输出: Implemented method2
  return 0;
}