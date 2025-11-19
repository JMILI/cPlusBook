#include <iostream>
using namespace std;

class VirtualBase {
public:
  int data = 100;
};

class Derived : virtual public VirtualBase {
public:
  void useData() { cout << data << endl; }
};

int main() {
  Derived d;
  d.useData();                   // 输出: 100
  return 0;
}