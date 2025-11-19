#include <iostream>
using namespace std;

class Base {
public:
  int publicVar = 10;
  void publicFunc() { cout << "Base publicFunc" << endl; }
protected:
  int protectedVar = 20;
  void protectedFunc() { cout << "Base protectedFunc" << endl; }
private:
  int privateVar = 30;
};

class Derived : public Base {
public:
  void accessMembers() {
    publicVar = 100;          // 合法：访问公有成员
    privateVar =12;
    protectedFunc();          // 合法：访问基类保护成员
                     // privateVar = 200;      // 非法：不能访问基类私有成员
  }
  static int staticMember;
};
int Derived::staticMember = 50;  // 静态成员定义

int main() {
  Derived d;
  d.publicFunc();              // 输出: Base publicFunc
  d.accessMembers();
  cout << Derived::staticMember << endl; // 输出: 50
  return 0;
}