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
    bigint &operator=(const bigint &);
    bigint &operator+=(const bigint &);
    bool operator==(const bigint &) const;
    bool operator<(const bigint &) const;
    friend ostream &operator<<(ostream &, const bigint &);
    inline static invalid_argument non_digit = invalid_argument("The input string contains non digit characters!");
    inline static invalid_argument leading_zeros = invalid_argument("The input number cannot have leading zeros!");

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

bigint::bigint(const string input_string)
{
    if (input_string[0] == '0')
    {
        if (input_string.length() == 1)
        {
            number_sign = sign::zero;
            digits.push_back(0);
        }
        else
            throw leading_zeros;
    }
    else if (input_string[0] == '-')
    {
        number_sign = sign::negative;
        fill_digits(input_string.substr(1, input_string.length() - 1));
    }
    else if (input_string[0] == '+')
    {
        number_sign = sign::positive;
        fill_digits(input_string.substr(1, input_string.length() - 1));
    }
    else
    {
        number_sign = sign::positive;
        fill_digits(input_string);
    }
}

bool bigint::is_digit(const string input_string)
{
    for (const char &c : input_string)
        if (!isdigit(c))
            return false;
    return true;
}

void bigint::fill_digits(const string input_string)
{
    if (input_string[0] == '0')
        throw leading_zeros;
    else if (!is_digit(input_string))
        throw non_digit;

    digits.resize(input_string.length());

    for (size_t i = 0; i < input_string.length(); i++)
    {
        digits[i] = static_cast<uint8_t>(input_string[input_string.length() - i - 1] - '0');
    }
}

ostream &operator<<(ostream &out, const bigint &number)
{
    if (number.number_sign == sign::zero)
    {
        out << '0' << '\n';
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

bool bigint::operator<(const bigint &rhs) const
{
    if (*this == rhs)
        return false;

    switch (number_sign)
    {
    case sign::negative:
        if (rhs.number_sign == sign::negative)
        {
            if (digits.size() > rhs.digits.size())
                return true;
            else if (digits.size() < rhs.digits.size())
                return false;
            else
            {
                for (size_t i = digits.size(); i > 0; i--)
                {
                    if (digits[i - 1] > rhs.digits[i - 1])
                        return true;
                }
                return false;
            }
        }
        else
            return true;
        break;
    case sign::zero:
        if (rhs.number_sign == sign::positive)
            return true;
        else
            return false;
        break;
    case sign::positive:
        if (rhs.number_sign == sign::positive)
        {
            if (digits.size() > rhs.digits.size())
                return false;
            else if (digits.size() < rhs.digits.size())
                return true;
            else
            {
                for (size_t i = digits.size(); i > 0; i--)
                {
                    if (digits[i - 1] < rhs.digits[i - 1])
                        return true;
                }
                return false;
            }
        }

        else
            return false;
        break;
    }
    return false;
}

bool operator>(const bigint &lhs, const bigint &rhs)
{
    return !((lhs < rhs) or (lhs == rhs));
}

bool operator>=(const bigint &lhs, const bigint &rhs)
{
    return !(lhs < rhs);
}

bool operator<=(const bigint &lhs, const bigint &rhs)
{
    return !(lhs > rhs);
}

bigint &bigint::operator=(const bigint &rhs)
{
    if (this != &rhs)
    {
        digits = rhs.digits;
        number_sign = rhs.number_sign;
    }
    return *this;
}