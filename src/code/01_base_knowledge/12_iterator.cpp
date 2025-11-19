#include <array>
#include <iostream>

int main(){
  std::array data{1,2,3,4,5,6,7,8,9};//36个字节

  //指针作为迭代器
  {
    auto begin{&data[0]};//取数组第一个元素的地址，使用指针
    auto end {begin + std::size(data)};
    std::cout<<"begin: "<<begin<<" end: "<<end<<"\n";
    for (auto ptr{begin};ptr!=end;ptr++){
      std::cout<<*ptr<<" ";
    }
    std::cout<<"\n";
  }
  //std标准库提供begin()和end()成员函数(std::array,std::vector都有)，可以获取迭代器的起点和重点
  {
    auto begin {data.begin()};
    auto end {data.end()};
    std::cout<<"begin: "<<begin<<" end: "<<end<<"\n";
    for (auto ptr{begin};ptr!=end;ptr++){
      std::cout<<*ptr<<" ";
    }
    std::cout<<"\n";
  }
  //泛型函数std::begin(object),std::end()
  {
    auto begin {std::begin(data)};
    auto end {std::end(data)};
    std::cout<<"begin: "<<begin<<" end: "<<end<<"\n";
    for (auto ptr{begin};ptr!=end;ptr++){
      std::cout<<*ptr<<" ";
    }
    std::cout<<"\n";
  }
  //基于范围的for循环
  {
    for(auto item:data){
      std::cout<<item<<" ";
    }
  }
  return 0;
}