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
    sign number_sign;
    vector<uint8_t> digits;
    void fill_digits(const string);
    bool is_digit(const string);
};

bigint::bigint()
{
    number_sign = sign::zero;
    digits.push_back(0);
}

bigint::bigint(const int64_t number)
{
    if (number == 0)
    {
        number_sign = sign::zero;
        digits.push_back(0);
    }
    else
    {
        if (number < 0)
            number_sign = sign::negative;
        else
            number_sign = sign::positive;
        uint64_t temp = static_cast<uint64_t>(abs(number));
        size_t number_of_digits = static_cast<size_t>(log10(temp) + 1);
        size_t cursor = 0;
        digits.resize(number_of_digits);

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
            number_sign = sign::zero;
            digits.push_back(0);
        }
        else
            throw invalid_string;
    }
    else if (inputString[0] == '-')
    {
        number_sign = sign::negative;
        fill_digits(inputString.substr(1, inputString.length() - 1));
    }
    else if (inputString[0] == '+')
    {
        number_sign = sign::positive;
        fill_digits(inputString.substr(1, inputString.length() - 1));
    }
    else
    {
        number_sign = sign::positive;
        fill_digits(inputString);
    }
}

bool bigint::is_digit(const string inputString)
{
    for (const char &c : inputString)
        if (!isdigit(c))
            return false;
    return true;
}

void bigint::fill_digits(const string inputString)
{
    if (inputString[0] == '0')
        throw invalid_string;
    else if (!is_digit(inputString))
        throw invalid_string;

    digits.resize(inputString.length());

    for (size_t i = 0; i < inputString.length(); i++)
    {
        digits[i] = static_cast<uint8_t>(inputString[inputString.length() - i - 1] - '0');
    }
}

ostream &operator<<(ostream &out, const bigint &number)
{
    if (number.number_sign == sign::zero)
    {
        out << '0';
        return out;
    }
    else if (number.number_sign == sign::negative)
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
    if ((digits.size() != rhs.digits.size()) or (number_sign != rhs.number_sign))
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