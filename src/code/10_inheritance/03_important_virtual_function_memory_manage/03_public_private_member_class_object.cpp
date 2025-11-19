#include <iostream>

class Basic {
private:
  int c;
  double d;

  void secret(int temp) {
    d = temp + c;
  }
public:
  int a;
  double b;

  void setB(double value) {
    b = value; // 直接访问成员变量b
    secret(b);
  }
protected:

};

int main() {
  Basic temp;
  temp.a = 10;
  temp.setB(3.14);
  return 0;
}