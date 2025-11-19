#include <iostream>

class Basic {
  // ...
public:
  static float alias;
  static float alias_00;
  static int alias_0;
  static void show() {
    std::cout << alias << std::endl;
  }
};
float Basic::alias_00 = 0.0;
int Basic::alias_0 = 0;
float Basic::alias = 0.233;
int gobal = 100;
int gobal_0 = 0;
int main() {
  // ...
  Basic temp{};
  temp.show();
  return 0;
}