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
    bool operator==(const bigint &) const;
    friend ostream &operator<<(ostream &, const bigint &);
    inline static invalid_argument invalid_string = invalid_argument("The input string does not represent an integer number!");

private:
    sign numberSign;
    vector<uint8_t> digits;
    void fillDigits(const string);
    bool isDigit(const string);
};

bigint::bigint()
{
    numberSign = sign::zero;
    digits.push_back(0);
}

bigint::bigint(const int64_t number)
{
    if (number == 0)
    {
        numberSign = sign::zero;
        digits.push_back(0);
    }
    else
    {
        if (number < 0)
            numberSign = sign::negative;
        else
            numberSign = sign::positive;
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
            numberSign = sign::zero;
            digits.push_back(0);
        }
        else
            throw invalid_string;
    }
    else if (inputString[0] == '-')
    {
        numberSign = sign::negative;
        fillDigits(inputString.substr(1, inputString.length() - 1));
    }
    else if (inputString[0] == '+')
    {
        numberSign = sign::positive;
        fillDigits(inputString.substr(1, inputString.length() - 1));
    }
    else
    {
        numberSign = sign::positive;
        fillDigits(inputString);
    }
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
    if (number.numberSign == sign::zero)
    {
        out << '0';
        return out;
    }
    else if (number.numberSign == sign::negative)
        out << '-';
    for (size_t i = number.digits.size(); i > 0; i--)
    {
        out << unsigned(number.digits[i - 1]);
    }
    out << '\n';

    return out;
}

bool bigint::operator==(const bigint &rhs) const
{
    if ((digits.size() != rhs.digits.size()) or (numberSign != rhs.numberSign))
        return false;
    for (size_t i = 0; i < digits.size(); ++i)
        if (digits[i] != rhs.digits[i])
            return false;
    return true;
}

bool operator!=(const bigint &lhs, const bigint &rhs)
{
    return !(lhs == rhs);
}