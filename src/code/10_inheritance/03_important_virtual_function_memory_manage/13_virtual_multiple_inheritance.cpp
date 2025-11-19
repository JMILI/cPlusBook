#include <iostream>
using namespace std;

class Animal {
public:
  virtual void breathe() { cout << "Animal breathes" << endl; }
};

class Mammal : virtual public Animal {
public:
  void feedMilk() { cout << "Mammal feeds milk" << endl; }
};

class WingedAnimal : virtual public Animal {
public:
  void flapWings() { cout << "WingedAnimal flaps wings" << endl; }
};

class Bat : public Mammal, public WingedAnimal {
public:
  void breathe() override { cout << "Bat breathes" << endl; }
};

int main() {
  Bat bat;
  bat.breathe();                // 输出: Bat breathes
  bat.feedMilk();               // 输出: Mammal feeds milk
  bat.flapWings();             // 输出: WingedAnimal flaps wings
  Animal* a = &bat;
  a->breathe();                // 输出: Bat breathes
  return 0;
}