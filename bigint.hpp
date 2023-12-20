#include <iostream>
#include <stdexcept>
#include <vector>
#include <limits>
#include <cmath>
#include <cctype>
#include <string>
using namespace std;

enum class sign
{
    negative,
    zero,
    positive
};

class bigint
{
public:
    bigint();
    bigint(const int64_t);
    bigint(const string);

    bigint &operator+=(const bigint &);
    friend ostream &operator<<(ostream &, const bigint &);
    inline static invalid_argument invalid_string = invalid_argument("The input string does not represent an integer number!");

private:
    sign sign;
    vector<uint8_t> digits;
    void fillDigits(const string);
    bool isDigit(const string);
};

bigint::bigint()
{
    sign = sign::zero;
    digits.push_back(0);
}

bigint::bigint(const int64_t number)
{
    if (number == 0)
    {
        sign = sign::zero;
        digits.push_back(0);
    }
    else
    {
        if (number < 0)
            sign = sign::negative;
        else
            sign = sign::positive;
        uint64_t temp = static_cast<uint64_t>(abs(number));
        size_t numberOfDigits = static_cast<size_t>(log10(temp) + 1);
        size_t cursor = 0;
        digits.resize(numberOfDigits);

        while (temp != 0)
        {
            digits[cursor] = static_cast<uint8_t>(temp % 10);
            temp = temp / 10;
            cursor++;
        }
    }
}

bigint::bigint(const string inputString)
{
    if (inputString[0] == '0')
    {
        if (inputString.length() == 1)
        {
            sign = sign::zero;
            digits.push_back(0);
        }
        else
            throw invalid_string;
    }
    else if (inputString[0] == '-')
    {
        sign = sign::negative;
        fillDigits(inputString.substr(1, inputString.length() - 1));
    }
    else if (inputString[0] == '+')
    {
        sign = sign::positive;
        fillDigits(inputString.substr(1, inputString.length() - 1));
    }
    else
        fillDigits(inputString);
}

bool bigint::isDigit(const string inputString)
{
    for (const char &c : inputString)
        if (!isdigit(c))
            return false;
    return true;
}

void bigint::fillDigits(const string inputString)
{
    if (inputString[0] == '0')
        throw invalid_string;
    else if (!isDigit(inputString))
        throw invalid_string;

    digits.resize(inputString.length());

    for (size_t i = 0; i < inputString.length(); i++)
    {
        digits[i] = static_cast<uint8_t>(inputString[inputString.length() - i - 1] - '0');
    }
}

ostream &operator<<(ostream &out, const bigint &number)
{
    if (number.sign == sign::zero)
    {
        out << '0';
        return out;
    }
    else if (number.sign == sign::negative)
        out << '-';
    for (size_t i = number.digits.size(); i > 0; i--)
    {
        out << unsigned(number.digits[i - 1]);
    }
    out << '\n';

    return out;
}
