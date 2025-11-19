#include <algorithm>
#include <iostream>

class Array {
private:
  int *data_;
  size_t size_;
  static void copy_(const int *src, int *dest, size_t count) {
    for (size_t i = 0; i < count; ++i) {
      dest[i] = src[i];
    }
  }

public:
  // 默认构造函数
  Array() : data_(nullptr), size_(0) {}
  // 带参数的构造函数,explicit 关键字用于
  // 显式声明构造函数或转换函数，其核心作用是 禁止隐式的类型转换
  explicit Array(size_t size) : data_(new int[size]{}), size_(size) {}
  // 拷贝构造函数（深拷贝）
  Array(const Array &other) : size_(other.size_) {
    data_ = new int[size_];
    // copy_(other.data_,data_,size_);
    for (size_t i = 0; i < size_; ++i) {
      data_[i] = other.data_[i];
    }
  }
  // 移动构造函数,当前data_指向other的data_数据地址。
  Array(Array &&other) noexcept : data_(other.data_), size_(other.size_) {
    other.data_ = nullptr;
    other.size_ = 0;
  }
  // 拷贝赋值运算符
  Array &operator=(const Array &other) {
    if (this != &other) {
      std::cout << "this: " << this << " &other " << &other << "\n";
      delete[] data_; // 先清理自己，再赋值
      size_ = other.size_;
      data_ = new int[size_];
      for (size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
      }
    }
    return *this;
  }
  // 移动赋值运算符
  Array &operator=(Array &&other) noexcept {
    if (this != &other) {
      delete[] data_;
      data_ = other.data_;
      size_ = other.size_;
      // 清理右值
      other.data_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }
  int& operator[](size_t index){
    if (index >=size_){
      throw std::out_of_range("index out of range");
    }
    //返回的数组元素的引用，可修改
    return data_[index];
  }
  const int& operator[](size_t index)const {
    if (index >=size_){
      throw std::out_of_range("index out of range");
    }
    //返回的数组元素的引用，可修改
    return data_[index];
  }
  ~Array() { delete[] data_; }
  void print() const {
    std::cout << "size: " << size_ << " data_:\n";
    for (size_t i = 0; i < size_; ++i) {
      std::cout << data_[i] << " ";
    }
    std::cout << "\n";
  }
};

int main() {
  Array arr1;
  arr1.print();
  Array arr2(4);
  arr2.print();
  Array arr3 = arr2;
  arr3.print();
  // 拷贝赋值
  arr1 = arr3;
  arr1.print();
  // 拷贝移动赋值运算符
  Array arr4 = std::move(arr1);
  arr4.print();
  // 移动赋值运算符
  arr3 = std::move(arr4);
  arr3.print();
  arr3[1]=20;//修改
  arr3.print();
  std::cout<<arr3[1];
  return 0;
}