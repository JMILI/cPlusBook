#include <cassert>
#include <iostream>
enum class Type {tInt,tFloat,tCString};
void printValue(void* ptr,Type type){
  switch(type){
  case Type::tInt:
    std::cout<<*static_cast<int*>(ptr)<<"\n";
    break;
  case Type::tFloat:
    std::cout<<*static_cast<float*>(ptr)<<"\n";
    break;
  case Type::tCString:
    std::cout<<*static_cast<char*>(ptr)<<"\n";
    break;
  }
}
int main(){
  int n_value {5};
  float f_value {1.2};
  char c_value[]{"1232323"};
  printValue(&n_value,Type::tInt);
  printValue(&f_value,Type::tFloat);
  printValue(&c_value,Type::tCString);
  return 0;
}