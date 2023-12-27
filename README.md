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
You can assign (`=`) a bigint number to another and negate (`-`) an existing bigint number:
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
You can compare two bigint numbers with operators such as `==`, `!=`, `<`, `>`, `<=` and `>=`:
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
This happens because in some parts of the code, the output of certain non-bigint arithmetic operations would be promoted to `int`. Since in the project description, it is mentioned that our code must compile without any warnings, and I am absolutely sure that in every case that I have cast an `int` to an `uint8_t`, that number was in the range of [0,9], I have used this workaround ([Source](https://stackoverflow.com/questions/57746321/implicit-conversion-warning-int-to-int-lookalike)).

### Setter Functions & Constructors
In order to be able to change a bigint object after defining it, I have defined two setter functions and then used them in constructors. Just like constructors, setter functions can accept two data types to create a bigint object:
- `string`: In [this](https://github.com/HSILA/arbitrary-precision/blob/377fa2506b7e38f39ec80f339abea0aefc43e5bb/bigint.hpp#L123-L158) setter function, the input string is first checked for being empty, and if so, an `empty_string` exception will be thrown. Then, if the string is `"+0"` or `"-0"` or a simple `"0"`, the value of the bigint number will be set to 0. Although this class does not consider a positive or negative sign for zero, the user might mistakenly do it, so we have to handle it as C++ will do (it will see both `+0` and `-0` as a `0`). Also, here, the length of the string will be checked; if it has started with zero and has a length greater than 1 (a number with leading zeros), a `leading_zeros` exception will be thrown. In this setter function,  the helper function `fillDigits` is used. After parsing the first character of the string to see whether the number is positive or negative, the rest of the string will be passed to the private member function `fill_digits` in order to fill in the `digits`. It will also check for leading zeros and throw an exception in that case. Another important check here is to see whether a string contains non-digit characters, which is checked with another helper function called `is_digit`, which will iterate a string character by character and return false if it contains any non-digit characters. `fillDigits` will iterate the string backwards and fill in the `digits` to form the bigint number. Then, upon successful parsing of the string, the sign of the bigint number is assigned in the setter function. The string constructor will simply use this setter function, since it will preserve the class invariant, whether upon creating a new object or altering an existing one.

- `int64_t`: [This] setter function first checks whether the input is zero; if so, it will create a bigint number with the value zero. Otherwise, it will first calculate the number of digits in the input by taking its logarithm in base 10. It will resize the `digits` vector accordingly and fill its elements with consecutive divisions. The int constructor will use this setter function to instantiate and object with an integer.

- Default Constructor: It will set the sign of the bigint number to `sign::zero` and push a single value of 0 into the `digits` vector.

### Insertion Operator <<
This operator is defined to insert a bigint number into an output stream like a file or terminal. It will first check the sign of the number, if it is zero it will insert `"0"` and if it is negative it will insert a `"-"` in the beginning of the stream. C++ treats an `uint8_t` as an unsigned char and inserts it as an ASCII character into the stream, I casted it into `unsigned` to be printable ([Source](https://stackoverflow.com/questions/19562103/uint8-t-cant-be-printed-with-cout)). This operator is overloaded as a non-member friend function, since it should access private members `sign` and `digits` in order to insert them.

### Comparison Operators
For the comparison operators, I have just implemented `==` and `<`, implementing others (`!=`, `>`, `<=`, `>=`) would be trivial using these two. I have defined them as member functions, since they need to access `sign` and `digits`, but I have set them to be `const` because they should not change anything about an object. Other comparison operators (`!=`, `>`, `<=`, `>=`) are defined as non-member functions.

#### Equality Operator (==)
In order to check whether two bigint numbers are equals or not, this operator first checks whether `digits` vectors in both have a same size, if not, they are not equals. Also, it will check the sign of the two numbers, if they have different signs, the cannot be equal. In both cases, it will return `false`. If two numbers have a same number of digits or same signs, it will iterate over their `digits` vector and compares them element-wise. It will return `false` if any of the corresponding digits are not equal and `true` otherwise.

#### Less Than Operator (<)
This operator, first checks whether two bigint numbers are equals and if so, it will return true. The it will check different scenarios:
- If the current number (left-hand side of the operator) is negative:
  - If the other number (right-hand side of the operator) is positive or zero it will return `true`.
  - If the other number is negative as well, it will first compares the sizes of their `digits` vectors. If current object has more digits than the others, it will return `true`, otherwise it will return `false`. If both of them have the same number of digits, it will compare them digit by digit starting from the most significant digit, the first time where a digit in current number is greater than the corresponding digit in the other number, it will returns `true`, if it is less, it returns false.
- If the current number is zero, if the other is negative it will return `true` otherwise `false`.
- If the current number (left-hand side of the operator) is positive:
  - If the other number (right-hand side of the operator) is negative or zero it will return `true`.
  - If the other number is negative as well, it will first compares the sizes of their `digits` vectors. If current object has fewer digits than the others, it will return `true`, otherwise it will return `false`. If both of them have the same number of digits, it will compare them digit by digit starting from the most significant digit, the first time where a digit in current number is less than the corresponding digit in the other number, it will returns `true`, if it is greater, it returns `false`.

### Assignment Operators
In this section, I will explain assignment operators such as `=`, `+=`, `-=` and `*=`  which are defined as member functions. They are used to define arithmetic operators such as `+`, `-` and `*` afterwards. All of three main arithmetic operators (`+`, `-` and `*`) are defined with simple methods that are taught in elementary schools.

#### Assignment Operator (=)
This operator first checks whether the current object (left-hand side) and the other object (right-hand side) are the same or not, to prevent self assignment ([Source](https://en.cppreference.com/w/cpp/language/copy_assignment)). If not, it will set the `digits` and `sign` of the current object to these values from other object. At the end, it will return a reference to modified current number. After implementing this operator, I got this warning:
```cpp
implicitly-declared 'constexpr bigint::bigint(const bigint&)' is deprecated [-Wdeprecated-copy]
```
Which means that by overloading this assignment operator, the compiler will implicitly define a copy constructor `bigint(const bigint &)`. This behavior is deprecated in C++11 and higher. So in order to prevent this warning, I explicitly defined a default copy constructor: ([Source](https://stackoverflow.com/questions/51863588/warning-definition-of-implicit-copy-constructor-is-deprecated))
```cpp
bigint(const bigint &) = default;
```

#### Addition Assignment Operator (+=)
First, if the other number (right-hand side) is zero, it will simply return current number (left-hand side). Also, if the current number is zero, it will assign it to the other number. I have only implemented the addition for the case where two numbers have the same signs, otherwise the operation could be reduced to a subtraction. First, if both of the numbers have the same sign, it will iterate over both `digits` vectors of the two numbers from the least significant digits to the most significant one, adds the digits element-wise and if the result of this addition is grater than 9, it will save the `result % 10` in the corresponding element of digit and apply the carry `result / 10` to the next digit. If one of the number has more digits than the other, it will simply push the remaining digits into the `digits` vector of current number (with considering carry, of course). At the end, if the carry is not zero, it will be pushed into the end of the `digits`. In this case, we do not have to change the sign, since the sign of the addition of the two numbers with same sign, remains the same. It is worth mentioning that addition in this case is handled in-place and will store the results directly in the current object's `digits` vector.

#### Subtraction Assignment Operator (-=)
This operator, uses two helper functions called `is_abs_greater` and `zero_remover`. The former will check whether the absolute value of current bigint object is greater than other or not. The latter will remove leading zeros from a bigint number after subtraction operation. Both of them are defined as private member function. The subtraction assignment operator will first check if the other number (right-hand side) is zero, it will return current number without any change and if the current number is zero, it will assign the negation of other number to the current number. If two numbers are equal, it will assign the current number to zero. For this operator overload, just like what you saw in `+=`, I will only implement the case where two numbers have the same sign, other cases could be solved with addition. Just like elementary school that we would write the bigger number on top and the smaller number below it and do the subtraction, here the function will first check whether current number has a greater absolute value than the other or not. If so, it will subtract the other from the current number and preserve the sign, if not, it will subtract the current number from the other and reverse the sign. The subtraction takes place with keeping track of the borrow, it will start from the least significant digit, subtract the corresponding digit of subtrahend from the minuend and if the result is less than 0, adds 10 to it and increment the borrow. In the next iterations of the loop, if the borrow is not zero (meaning that we have borrowed from the current digit in the previous iteration and now we have to handle it), it will subtract 1 from the current digit (since borrow is either 0 or 1) and if it becomes negative again, it will keep the borrow as 1 and add 10 to the current digit, otherwise it will set borrow to 0. If one of the `digits` vector ends, we will only work with the bigger one and borrow. Finally, some digits at the end of `digits` might become zero and since we do not have leading zeros in this class, the `zero_remover` is called in order to remove leading zeros. The case where the other number has a greater absolute value is the same, with a slight difference that since we have a constant copy of it, we have to create a new object to do the operations in-place. 

#### Multiplication Assignment Operator (*=)
In this operator, we first check whether either of the operands are 0 or 1 and act accordingly. The sign handling will also takes place, numbers with the same sign will result in positive number and negative otherwise. In this operator, I divide the problem into two cases: one where current number has more digits and otherwise. This way, the operation would be a little bit optimized, since it prevents unnecessary zero placements at the beginning of result in each stage and also reduces the number of bigint additions. Just like the subtraction, assume that we write the number with fewer digits below the number with more digits and do the standard multiplication. Here there are two nested for loops, in the outer loop, the operator iterates over the digits of the smaller number and in the inner loop it iterates over the digits of the bigger number. Both of the loops iterate the `digits` from the least significant digit to the most significant ones. Before each iteration of the inner loop, a temporary bigint number is created to store the result of each iteration in it, and also before the outer loop a `result` bigint number is defined in order to add and store the intermediary temporary bigint numbers in it. The multiplication takes places by multiplying digit by digit and handle the carry just as we did in `+=`. Also, based on the iteration of the outer loop that we are on, we should add zeros in the least significant digits. Since multiplication with this method requires adding different instances of bigint numbers, we cannot store the output in the same object, because we might lose information, so the `result` will be assigned to the current object and a reference to it is returned.

### Arithmetic Operators
The arithmetic operations such as binary `+`, `-` and `*` can be defined by creating a copy of addition assignment, subtraction assignment and multiplication assignment and returning that copy, which is the canonical approach to overload these operations. These overloads are defined as non-member functions.

#### Negation Operator
The unary - operator (`operator-()`) will create a copy of the current bigint number. Then it will negate the number if it is non-zero and return the created copy. Since it should not change the sign of the current number, it is defined as a `const`, as described in the standard prototype [here](https://en.cppreference.com/w/cpp/language/operator_arithmetic).

