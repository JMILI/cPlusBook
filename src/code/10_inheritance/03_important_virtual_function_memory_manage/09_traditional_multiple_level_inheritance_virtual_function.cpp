#include <iostream>
using namespace std;

class GrandBase {
public:
  virtual void action() { cout << "GrandBase action" << endl; }
};

class Parent : public GrandBase {
public:
  void action() override { cout << "Parent action" << endl; }
};

class Child : public Parent {
public:
  void action() override { cout << "Child action" << endl; }
};

int main() {
  Child c;
  GrandBase* ptr = &c;
  ptr->action();                // 输出: Child action
  return 0;
}