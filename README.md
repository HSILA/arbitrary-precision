# BigInt for Arbitrary-Precision Integers

## Introduction
C++ supports different types of integer types with different capacities, but it lacks a primitive data type with arbitrary precision that can handle huge integers and their arithmetic. Using this project, you can define a signed integer without being limited to a range, the only limit is your computer's memory! This documentation demonstrates the features and usage of the mentioned class and will briefly go through some design decisions and the inner working of this tool. 

## Features
- You can instantiate bigint number whether with an integer (signed 64-bit int) or with an string.
- You can compare different bigint numbers with comparison operators such as `==`, `!=`, `>`, `<`, `<=` and `>=`.
- Use arithmetic operators such as `+=`, `+`, `-=`, `-`, `*=` and `*` on bigint numbers.
- Just like primitive integer types, you can insert a bigint number to any output stream, being it terminal or a file.
- This tool can recognize invalid input strings and throw an appropriate exception.
- There is no limit on the size and range of integers being defined with this tool, the only limit is your system memory.
- This tool is self-contained, it only relies on standard C++ libraries and does not use any external dependencies.

## Usage

### Constructors
There are three constructors that you can instantiate an object with:
- Default Constructor (`bigint()`): It will instantiate a bigint object with zero value.
- Integer Constructor (`bigint(const int64_t &)`): It will instantiate a bigint object with a signed 64-bit integer.
- String Constructor (`bigint(const string &)`): It will instantiate a bigint object with a string.

```cpp
#include "bigint.hpp"
using namespace std;

int main()
{
    bigint a;
    cout << a; \\ 0

    bigint b(9223372036854775807);
    cout << b; \\ 9223372036854775807

    bigint c("-12345678909876543210")
    cout << c; \\ -12345678909876543210
}
```

### Setter Functions
After defining a bigint number, we can change its value using `set` function. Just like the constructors, `set` function can accept either a signed 64-bit integer or a string. Keep in mind that calling `set` without any argument will not set the number to zero, to set any bigint number to zero, you should explicitly use `set(0)` or `set("0")`.

```cpp
#include "bigint.hpp"
using namespace std;

int main()
{
    bigint d(12);
    cout << d; // 12

    d.set("-13131313");
    cout << d; // -13131313

    d.set(13);
    cout << d; // 13
}
```

### Class Invariant & Exceptions
Both constructors and setter functions maintain the class invariant and will throw an exception upon receiving invalid input. Specifically, the exceptions thrown when instantiating the string setter and constructor with invalid input are:
- `non_digit`: This exception will be thrown if the string constructor or setter function is instantiated with a string containing non-digit characters. It does not apply to `+` and `-` characters in the beginning of the number. Examples of invalid inputs are: `"331313.1"`, `"AB31311"`, and `"gk%45#^$#!"`.
- `leading_zeros`: This exception will be thrown if the string constructor or setter function is instantiated with a number with leading zeros. It includes but is not limited to: `"000000"`, and `"0031313"`. This is a design decision that I made because in C++, if an integer starts with zero, it means that it represents the int in the octal base, but the string parser of this class can only parse decimal integers, so if the user starts the string with zero, it might imply an octal representation, which can confuse other users.
- `empty_string`: This exception will be thrown if the string constructor or setter function is instantiated with an empty string `"`. 

### Output Stream
Just like any primitive type of C++, you can use insertion operator `<<` in order to insert a bigint number into any output stream, being terminal or a file:
```cpp
#include "bigint.hpp"
#include <fstream>;
using namespace std;

int main()
{
    bigint c("516");
    cout << c; // 516

    ofstream output_file("output.txt");
    if (output_file.is_open())
    {
        output_file << c;
        output_file.close();
    } 
}
```

### Assignment & Negation
You can assign a bigint number to another and negate an existing bigint number:
```cpp
#include "bigint.hpp"
using namespace std;

int main()
{
    bigint j(123);
    bigint k("-456");
    cout << "j = " << j; // 123
    cout << "k = " << k; // -456
    j = k;
    cout << "j = " << j; // -456
    cout << "j = " << -j; // 456
}
```

### Comparison
You can compare two bigint numbers:
```cpp
#include "bigint.hpp"
using namespace std;

int main()
{
    bigint j(123);
    bigint k("-456");
    cout << "j == k? " << (j == k) << '\n'; // 0
    cout << "j != k? " << (j != k) << '\n'; // 1
    cout << "j < k? " << (j < k) << '\n';   // 0
    cout << "j > k? " << (j > k) << '\n';   // 1
    cout << "j <= k? " << (j <= k) << '\n'; // 0
    cout << "j >= k? " << (j >= k) << '\n'; // 1
}
```

### Arithmetic Operations
You can use arithmetic operations such as `+=`, `+`, `-=`, `-`, `*=` and `*` between two bigint numbers:
```cpp
#include "bigint.hpp"
using namespace std;

int main()
{
    bigint a(123456789);
    bigint b("987654321");

    a += b;
    cout << "a = " << a;         // 1111111110
    cout << "a + b = " << a + b; // 2098765431

    a -= b;
    cout << "a = " << a;         // 123456789
    cout << "a - b = " << a - b; // -864197532

    a *= b;
    cout << "a = " << a;         // 121932631112635269
    cout << "a*  b = " << a * b; // 120427289989293261124847349
}
```

## Implementation Details
### Class
In this section, I will delve deeper into the details of my implementation. I have defined two member variables for the `bigint` class:
- `digits`: It is a C++ vector container with the signed 8-bit integer type; each digit of the bigint number will be placed into one element of this vector, so each element contains numbers between (and including) 0 and 9. This vector is filled in reverse order, meaning that the least significant value of it resides in the first element. For example, if we have a number such as 123456789, `digits[0]` would be 9, `digits[1]` would be 8 and so on.
- `sign`: It is an enumeration class denoting the sign of the bigint number. It has three values: `negative`, `zero`, and `positive`. I have considered zero an independent sign because, otherwise, there could be +0 and -0. In C++, we do not have negative and positive zeros for integers, so to be consistent with C++ implementation and handle some scenarios in arithmetic operations, I considered zero to be a distinct type.

Now, I will go through each constructor, function and overloaded operator one by one, and I will explain their inner workings and dependencies (helper functions) as well. But before going through them, I have to mention that in some parts of my code, I have used `static_cast` in order to cast an `int` into `uint8_t`. Without doing so, I would get this warning:
```cpp
conversion from 'int' to '__gnu_cxx::__alloc_traits<std::allocator<unsigned char>, unsigned char>::value_type' {aka 'unsigned char'} may change value [-Wconversion]
```
This happens because in some parts of my code (), the output of certain non-bigint arithmetic operations would be promoted to `int`. Since in the project description, it is mentioned that our code must compile without any warnings, and I am absolutely sure that in every case that I have cast an `int` to an `uint8_t`, that number was in the range of [0,9], I have used this workaround. [Source1](https://stackoverflow.com/questions/57746321/implicit-conversion-warning-int-to-int-lookalike), [Source2](https://stackoverflow.com/questions/19562103/uint8-t-cant-be-printed-with-cout)

### Setter Functions
In order to be able to change a bigint object after defining it, I have defined two setter functions and then used them in constructors. Just like constructors, setter functions can accept two data types to create a bigint object:
- `string`: In [this](https://github.com/HSILA/arbitrary-precision/blob/377fa2506b7e38f39ec80f339abea0aefc43e5bb/bigint.hpp#L123-L158) setter function, the input string is first checked for being empty, and if so, an `empty_string` exception will be thrown. Then, if the string is `"+0"` or `"-0"` or a simple `"0"`, the value of the bigint number will be set to 0. Although this class does not consider a positive or negative sign for zero, the user might mistakenly do it, so we have to handle it as C++ will do (it will see both `+0` and `-0` as a `0`). Also, here, the length of the string will be checked; if it has started with zero and has a length greater than 1 (a number with leading zeros), a `leading_zeros` exception will be thrown. In this setter function,  the helper function `fillDigits` is used. After parsing the first character of the string to see whether the number is positive or negative, the rest of the string will be passed to the private member function `fill_digits` in order to fill in the `digits`. It will also check for leading zeros and throw an exception in that case. Another important check here is to see whether a string contains non-digit characters, which is checked with another helper function called `is_digit`, which will iterate a string character by character and return false if it contains any non-digit characters. `fillDigits` will iterate the string backwards and fill in the `digits` to form the bigint number. Then, upon successful parsing of the string, the sign of the bigint number is assigned in the setter function. The string constructor will simply use this setter function, since it will preserve the class invariant, whether upon creating a new object or altering an existing one.

- `int64_t`: [This] setter function first checks whether the input is zero; if so, it will create a bigint number with the value zero. Otherwise, it will first calculate the number of digits in the input by taking its logarithm in base 10. It will resize the `digits` vector accordingly and fill its elements with consecutive divisions.