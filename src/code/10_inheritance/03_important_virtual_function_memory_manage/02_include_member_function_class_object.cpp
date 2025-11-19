#include <iostream>

class Basic {
public:
  int a;
  double b;

  void setB(double value) {
    b = value; // 直接访问成员变量b
  }
};

int main() {
  using MemberFunc = void (Basic::*)(double);
  Basic obj1, obj2;
  obj1.a = 10;
  obj1.setB(3.14);
  obj2.a = 12;
  obj2.setB(3.15);
  return 0;
}