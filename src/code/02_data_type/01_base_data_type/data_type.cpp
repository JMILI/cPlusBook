#include "iostream"
using namespace std;
// Define a function type
typedef int (*FuncPtr)(int, double);

int exampleFunction(int a, double b) {
  return a + static_cast<int>(b);
}

int main() {
  {
    cout << "--------------------------Basic Data Type Sizes---------------" << endl;
    cout << "--------------------------char---------------------------" << endl;
    cout << "Size of char:" << sizeof(char) << endl;
    cout << "Size of unsigned char:" << sizeof(unsigned char) << endl;
    cout << "Size of signed char:" << sizeof(signed char) << endl;
    cout << "---------------------------int--------------------------" << endl;
    cout << "Size of int : " << sizeof(int) << endl;
    cout << "Size of unsigned int:" << sizeof(unsigned int) << endl;
    cout << "Size of signed int:" << sizeof(signed int) << endl;

    cout << "Size of short int : " << sizeof(short int) << endl;
    cout << "Size of unsigned short int:" << sizeof(unsigned short int) << endl;
    cout << "Size of signed short int:" << sizeof(signed short int) << endl;

    cout << "Size of long int : " << sizeof(long int) << endl;
    cout << "Size of unsigned long int:" << sizeof(unsigned long int) << endl;
    cout << "Size of signed long int:" << sizeof(signed long int) << endl;
    cout << "---------------------------float--------------------------" << endl;
    cout << "Size of float : " << sizeof(float) << endl;
    cout << "---------------------------double--------------------------" << endl;
    cout << "Size of double : " << sizeof(double) << endl;
    cout << "---------------------------wchar_t--------------------------" << endl;
    cout << "Size of wchar_t : " << sizeof(wchar_t) << endl;
    std::cout << sizeof(std::size_t) << '\n';
  }

  // Pointers, references, enums,
  {
    enum Color { RED, GREEN, BLUE };
    cout<< "---------------------------int* int& enum--------------------------" << endl;
    cout << "Size of pointer to int: " << sizeof(int *) << endl;
    cout << "Size of reference to int: " << sizeof(int &) << " (This is the same as sizeof(int))" << endl;
    cout << "Size of reference to const int: " << sizeof(const int &) << " (This is the same as sizeof(int))" << endl;
    cout << "Size of pointer to double: " << sizeof(double *) << endl;
    cout << "Size of reference to double: " << sizeof(double &) << " (This is the same as sizeof(double))" << endl;
    cout << "Size of reference to const double: " << sizeof(const double &) << " (This is the same as sizeof(double))" << endl;
    cout << "Size of pointer to char: " << sizeof(char *) << endl;
    cout << "Size of reference to char: " << sizeof(char &) << " (This is the same as sizeof(char))" << endl;
    // Results show that the size of basic type references is consistent with the size of the basic types.
    // The size of pointers is usually 8 bytes.
    // Enums are typically the same size as an int.
    cout << "Size of enum Color: " << sizeof(Color) << " (This is usually the same as sizeof(enum))" << endl;
  }
  // (Aligned classes and structs), conclusion: Pointer sizes are generally 8 bytes, the size of references is consistent with data types
  // And it is related to the order of member variables,
  {
    cout<< "---------------------------struct--------------------------\n";
    struct MyStruct {
      char c;   // 1 byte
      int i;    // 4 bytes (on most systems)
      double d; // 8 bytes (on most systems)
      short s;  // 2 bytes (on most systems)
    };
    std::cout << "Size of MyStruct: " << sizeof(MyStruct) << " bytes" << std::endl;
    cout << "Size of pointer to MyStruct: " << sizeof(MyStruct *) << endl;
    cout << "Size of reference to MyStruct: " << sizeof(MyStruct &) << endl;

    struct MyStruct1 {
      char c;   // 1 byte
      short s;  // 2 bytes (on most systems)
      int i;    // 4 bytes (on most systems)
      double d; // 8 bytes (on most systems)
    };
    std::cout << "Size of MyStruct1: " << sizeof(MyStruct1) << " bytes" << std::endl;
    cout << "Size of pointer to MyStruct1: " << sizeof(MyStruct1 *) << endl;
    cout << "Size of reference to MyStruct1: " << sizeof(MyStruct1 &) << endl;

    cout<< "---------------------------class--------------------------\n";
    class MyClass {
    public:
      int x;        // 4 bytes (on most systems)
      float y;      // 4 bytes (on most systems)
      char z[10];   // 10 bytes
      char pad;     // 1 byte (might be padding to align next member)
      long long ll; // 8 bytes (on 64-bit systems)
    };
    std::cout << "Size of MyClass: " << sizeof(MyClass) << " bytes" << std::endl;
    cout << "Size of pointer to MyClass: " << sizeof(MyClass *) << endl;
    cout << "Size of reference to MyClass: " << sizeof(MyClass &) << endl;

    class MyClass1 {
    public:
      char pad;     // 1 byte (might be padding to align next member)
      int x;        // 4 bytes (on most systems)
      float y;      // 4 bytes (on most systems)
      long long ll; // 8 bytes (on 64-bit systems)
      char z[10];   // 10 bytes
    };
    std::cout << "Size of MyClass1: " << sizeof(MyClass1) << " bytes" << std::endl;
    cout << "Size of pointer to MyClass1: " << sizeof(MyClass1 *) << endl;
    cout << "Size of reference to MyClass1: " << sizeof(MyClass1 &) << endl;

    class MyClass3 {
    public:
      char pad;     // 1 byte (might be padding to align next member)
      int x;        // 4 bytes (on most systems)
      float y;      // 4 bytes (on most systems)
      long long ll; // 8 bytes (on 64-bit systems)
      char z[10];   // 10 bytes
      MyStruct1 myStruct1; // 16 bytes
    };
    std::cout << "Size of MyClass3: " << sizeof(MyClass3) << " bytes" << std::endl;
    cout << "Size of pointer to MyClass3: " << sizeof(MyClass3 *) << endl;
    cout << "Size of reference to MyClass3: " << sizeof(MyClass3 &) << endl;
  }

  // Arrays, array pointers, pointer arrays, and their sizes
  {
    cout<< "---------------------------array | array pointer | pointer array--------------------------\n";
    // Define an array
    int arr[5] = {1, 2, 3, 4, 5};

    // Define an array pointer
    int* arrPtr = arr; // Or use int* arrPtr = &arr[0];

    // Define a pointer array
    int* ptrArr[3] = {new int(10), new int(20), new int(30)};

    // Output the size of the entire array in bytes
    std::cout << "Size of array arr: " << sizeof(arr) << " bytes" << std::endl;
    // Output the size of one element in the array
    std::cout << "Size of one element in array arr: " << sizeof(arr[0]) << " bytes" << std::endl;

    // Output the size of the array pointer (pointer itself)
    std::cout << "Size of array pointer arrPtr: " << sizeof(arrPtr) << " bytes" << std::endl;
    // Note: sizeof(arrPtr) is the same as sizeof(&arr[0]), because both are pointers

    // Output the size of the pointer array (total bytes occupied by the pointer array)
    std::cout << "Size of pointer array ptrArr: " << sizeof(ptrArr) << " bytes" << std::endl;
    // Output the size of one element (i.e., one pointer) in the pointer array
    std::cout << "Size of one element (pointer) in pointer array ptrArr: " << sizeof(ptrArr[0]) << " bytes" << std::endl;

    // Size of a reference (actually returns the size of the referenced type)
    int& arrRef = arr[0];
    std::cout << "Size of reference to int (arrRef): " << sizeof(arrRef) << " bytes (same as sizeof(int))" << std::endl;

    // Size of a pointer to a pointer
    int** ptrToPtr = &arrPtr;
    std::cout << "Size of pointer to pointer (ptrToPtr): " << sizeof(ptrToPtr) << " bytes" << std::endl;

    // Clean up dynamically allocated memory
    for (int i = 0; i < 3; ++i) {
      delete ptrArr[i];
    }
  }

  // Function pointer sizes
  {
    cout<< "---------------------------function pointer--------------------------\n";
    // Create a function pointer and point it to exampleFunction
    FuncPtr funcPointer = &exampleFunction;
    // Output the size of the function pointer
    std::cout << "Size of function pointer: " << sizeof(funcPointer) << " bytes" << std::endl;
    // Call the function pointed to by the function pointer
    int result = funcPointer(5, 3.2);
    std::cout << "Result of function call: " << result << std::endl;
  }

  // Further discussion on C++ data alignment rules
  {

  }
  return 0;
}