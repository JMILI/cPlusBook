### 命令行参数

**为什么需要命令行参数？**

命令行参数允许用户在启动程序时向程序传递输入，而不是在程序运行后等待用户输入。这对于自动化任务、处理批量文件或在服务器环境中运行程序特别有用。

#### 示例：需要用户输入的程序

考虑一个生成图像缩略图的程序：

```cpp
// Program: Thumbnail
#include <iostream>
#include <string>

int main()
{
    std::cout << "Please enter an image filename to create a thumbnail for: ";
    std::string filename{};
    std::cin >> filename;

    // 这里执行生成缩略图的逻辑
}
```

这种方式适用于手动执行一次，但不适合批量处理多个文件或服务器自动化。

#### 使用命令行参数

- **传递参数**：在命令行中，参数直接跟随程序名：

  ```
  WordCount Myfile.txt
  ```

  多个参数用空格分隔：

  ```
  WordCount Myfile.txt Myotherfile.txt
  ```

- **从IDE传递参数**：在Visual Studio中，通过项目属性设置命令参数。

#### `main()` 函数的变体

为了使用命令行参数，`main()` 函数需要修改为：

```cpp
int main(int argc, char* argv[])
```

- **argc**：表示参数的数量（包括程序名），因此至少为1。
- **argv**：指针数组，其中每个元素都是一个C风格字符串，指向每个参数。

#### 示例程序：打印命令行参数

```cpp
// Program: MyArgs
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "There are " << argc << " arguments:\n";

    for (int count{ 0 }; count < argc; ++count)
    {
        std::cout << count << ' ' << argv[count] << '\n';
    }

    return 0;
}
```

输出示例：

```
There are 3 arguments:
0 C:\MyArgs
1 Myfile.txt
2 100
```

**注意**：`argv[0]` 是程序本身的路径和名称。

#### 处理数值参数

命令行参数都是字符串，需要转换为数字：

```cpp
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        std::cout << "Usage: " << (argv[0] ? argv[0] : "<program name>") << " <number>" << '\n';
        return 1;
    }

    std::stringstream convert{ argv[1] };
    int myint{};
    if (!(convert >> myint)) 
        myint = 0; // 默认值如果转换失败

    std::cout << "Got integer: " << myint << '\n';

    return 0;
}
```

#### 操作系统的预处理

操作系统对命令行参数进行预处理，包括处理引号和转义字符：

- **引号处理**：将引号内的内容作为单一参数。

  ```
  MyArgs "Hello world!"
  ```

- **转义字符**：如使用反斜杠来包含引号。

  ```
  MyArgs \"Hello world!\"
  ```

#### 结论

命令行参数为程序提供一种灵活的输入方式，允许程序在启动时接收数据，从而提高程序的自动化和批处理能力。在设计程序时，记得验证这些参数的有效性。