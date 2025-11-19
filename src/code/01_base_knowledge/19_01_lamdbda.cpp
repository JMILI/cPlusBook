#include <iostream>
#include <string>
#include <vector>

struct ComplexData {
  int id;
  std::string tag;
  std::vector<float> values;
};

int main() {
  {
    int counter = 100;
    double factor = 3.14;
    ComplexData data{42, "lambda", {1.1, 2.2}};
    std::string& ref_str = data.tag;

    // 三层嵌套的复杂lambda
    auto outer_lambda = [=, &ref_str, data](const std::string& param) mutable {
      auto middle_lambda = [&counter, factor, this_data = data]() {
        auto inner_lambda = [this_data, &factor]() {
          std::cout << this_data.id << factor << std::endl;
          return this_data.values.size();
        };
        return inner_lambda;
      };

      data.id += ::atoi(param.c_str());
      ref_str += "_modified";
      return middle_lambda;
    };

    auto result = outer_lambda("5");  // 在此设置断点
    std::cout << "Result: " << result()() << std::endl;
  }

  return 0;
}