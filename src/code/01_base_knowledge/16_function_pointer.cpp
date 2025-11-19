#include <iostream>

void func1(int x) { std::cout << "func1 :" << x << "\n"; }

void func2(int x) { std::cout << "func1 :" << x << "\n"; }

void (*a(int num, void (*b)(int)))(int) {
  std::cout << "num is :" << num << "\n";
  if (num > 0) {
    return func1;
  } else {
    return func2;
  }
}

int main() {
  void (*result)(int) = a(5, func1);//根据条件使用函数指针获取一个函数，
  result(10);
  result = a(-1, func2);//根据条件使用函数指针获取一个函数，-1是判断条件
  result(20);
  return 0;
}