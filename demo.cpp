#include "bigint.hpp"
using namespace std;

int main()
{
    // Default constructor creates a bigint object with value of 0,
    bigint a;
    cout << "a = " << a;

    // Integer constructor: takes a signed 64-bit integer and converts it to a bigint object.
    bigint b(267481);
    cout << "b = " << b;
    bigint c(-31642);
    cout << "c = " << c;

    // String constructor: takes a string and converts it to a bigint object.
    bigint d("295712491461964816498164981");
    cout << "d = " << d;
    bigint e("-343284521048104795104781");
    cout << "e = " << e;
    bigint f("+572907418046716498164891");
    cout << "f = " << f;

    // The string constructor will throw exceptions upon receiving an invalid input:
    try
    {
        bigint g("13816361.3131");
    }
    catch (const invalid_argument &error)
    {
        cout << "Error: " << error.what() << '\n';
    }
    try
    {
        bigint g("00000313131");
    }
    catch (const invalid_argument &error)
    {
        cout << "Error: " << error.what() << '\n';
    }

    // If any error occurs, no object will be created:
    try
    {
        bigint g("gk%45#^$#!");
        cout << "g = " << a;
    }
    catch (const invalid_argument &error)
    {
        cout << "Error: " << error.what() << '\n';
    }

    // We can change the value of a bigint number with `set` function, whether with a string or int:
    a.set(123321);
    cout << "a = " << a;
    a.set("-9888898888");
    cout << "a = " << a;

    // The setter function also ensures that the class invariant is satisfied:
    try
    {
        a.set("AB131351");
        cout << "a = " << a;
    }
    catch (const invalid_argument &error)
    {
        cout << "Error: " << error.what() << '\n';
    }
    // We can negate a bigint number with unary negate operator:
    bigint h = -a;
    cout << "h = " << -a; // 9888898888
    a.set(0);

    cout << "Comparing two bigint numbers:" << '\n';
    // We can check if two bigint numbers are equal or not:
    bigint i(267481);
    cout << "267481 == 267481? " << (b == i) << '\n'; // 1
    cout << "267481 == -31642? " << (b == c) << '\n'; // 0
    cout << "267481 != 267481? " << (b != i) << '\n'; // 0
    cout << "267481 != -31642? " << (b != c) << '\n'; // 1

    // Or whether one is less than the other:
    cout << "267481 < 267481? " << (b < i) << '\n'; // 0
    cout << "267481 < -31642? " << (b < c) << '\n'; // 0
    cout << "-31642 < 267481? " << (c < b) << '\n'; // 1

    // Or greater than the other:
    cout << "267481 > -31642? " << (b > c) << '\n'; // 1
    cout << "-31642 > 267481? " << (c > b) << '\n'; // 0

    // Less than or equal:
    cout << "267481 <= 267481? " << (b <= i) << '\n'; // 1
    cout << "267481 <= -31642? " << (b <= c) << '\n'; // 0

    // Greater than or equal:
    cout << "267481 >= 267481? " << (b >= i) << '\n'; // 1
    cout << "267481 >= -31642? " << (b >= c) << '\n'; // 1

    // Assign an existing bigint object to another.
    bigint j(123);
    bigint k("-456");
    cout << "j = " << j;
    cout << "k = " << k;
    j = k;
    cout << "After j = k Assignment: j = " << j; //-456

    cout << "\nAdding two bigint numbers:\n";
    b.set("186418");
    cout << "a = " << a;
    cout << "b = " << b;
    a += b;
    cout << "a += b : a = " << a; // 186418
    b.set("-186418");
    cout << "b = " << b;
    a += b;
    cout << "a += b : a = " << a; // 0
    a.set("12345678910111213141516");
    b.set("-161718192021222324252627");
    cout << "a = " << a;
    cout << "b = " << b;
    a += b;
    cout << "a += b : a = " << a; // -149372513111111111111111
    a.set("9999");
    b.set(-9999);
    cout << "a = " << a;
    cout << "b = " << b;
    cout << "a + b = " << a + b; // 0

    cout << "\nSubtracting two bigint numbers: \n";
    a.set("10000000000000000000000");
    b.set(1);
    cout << "a = " << a;
    cout << "b = " << b;
    a -= b;
    cout << "a -= b : a = " << a; // 9999999999999999999999
    b.set("9999999999999999999999");
    cout << "b = " << b;
    a -= b;
    cout << "a -= b : a = " << a; // 0
    a.set("10000000000000000000000");
    b.set("-10000000000000000000000");
    cout << "a = " << a;
    cout << "b = " << b;
    cout << "a - b = " << a - b; // 20000000000000000000000
    a.set("-10000000000000000000000");
    b.set("10000000000000000000000");
    cout << "a = " << a;
    cout << "b = " << b;
    cout << "a - b = " << a - b; // -20000000000000000000000
    b.set(0);
    cout << "b = " << b;
    cout << "a - b = " << a - b; // -10000000000000000000000
    cout << "b - a = " << b - a; // -10000000000000000000000

    cout << "\nMultiplying two bigint numbers:\n";
    a.set(0);
    b.set("500");
    a *= b;
    cout << "a = " << a;
    cout << "b = " << b;
    a *= b;
    cout << "a *= b : a = " << a; // 0
    a.set("-32674816684");
    cout << "a = " << a;
    a *= b;
    cout << "a *= b : a = " << a; // -16337408342000
    a.set("592491734917987491");
    b.set("999999988888888888");
    cout << "a = " << a;
    cout << "b = " << b;
    cout << "a * b = " << a * b; // 592491728334745991384590780072900008
    a.set("592491734917987491");
    b.set("-999999988888888888");
    cout << "a = " << a;
    cout << "b = " << b;
    cout << "a * b = " << a * b; // -592491728334745991384590780072900008
}
