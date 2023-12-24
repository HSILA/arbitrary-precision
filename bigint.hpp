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
    bigint(const bigint &) = default;
    bigint &operator=(const bigint &);
    bigint operator-() const;
    bigint &operator+=(const bigint &);
    bigint &operator-=(const bigint &);
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
    bool is_abs_greater(const bigint &) const;
    bool is_abs_equals(const bigint &) const;
    void zero_remover();
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

bigint bigint::operator-() const
{
    bigint out = *this;
    if (out.number_sign == sign::negative)
        out.number_sign = sign::positive;
    else if (out.number_sign == sign::positive)
        out.number_sign = sign::negative;
    return out;
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
                    else if (digits[i - 1] < rhs.digits[i - 1])
                        return false;
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
                    else if (digits[i - 1] > rhs.digits[i - 1])
                        return false;
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

bigint &bigint::operator+=(const bigint &other)
{
    if (other.number_sign == sign::zero)
        return *this;
    else if (number_sign == sign::zero)
        *this = other;
    else if (number_sign == other.number_sign)
    {
        size_t max_length = max(digits.size(), other.digits.size());
        uint8_t carry = 0;
        for (size_t i = 0; i < max_length; i++)
        {
            if (i >= digits.size())
            {
                uint8_t temp = other.digits[i] + carry;
                carry = temp / 10;
                digits.push_back(temp % 10);
            }
            else if (i >= other.digits.size())
            {
                uint8_t temp = digits[i] + carry;
                carry = temp / 10;
                digits[i] = temp % 10;
            }
            else
            {
                uint8_t temp = static_cast<uint8_t>(digits[i] + other.digits[i] + carry);
                carry = temp / 10;
                digits[i] = temp % 10;
            }
        }
        if (carry > 0)
            digits.push_back(carry);
    }
    else if (number_sign == sign::positive and other.number_sign == sign::negative)
    {
        *this -= -other;
    }
    else if (number_sign == sign::negative and other.number_sign == sign::positive)
    {
        bigint temp = other;
        temp -= -*this;
        *this = temp;
    }
    return *this;
}

bigint operator+(bigint lhs, const bigint &rhs)
{
    lhs += rhs;
    return lhs;
}

bool bigint::is_abs_greater(const bigint &rhs) const
{
    if (digits.size() != rhs.digits.size())
        return digits.size() > rhs.digits.size();
    else
    {
        for (size_t i = digits.size(); i > 0; i--)
        {
            if (digits[i - 1] > rhs.digits[i - 1])
                return true;
            else if (digits[i - 1] < rhs.digits[i - 1])
                return false;
        }
        return false;
    }
}

bool bigint::is_abs_equals(const bigint &rhs) const
{
    if (digits.size() != rhs.digits.size())
        return false;
    else
    {
        for (size_t i = 0; i < digits.size(); ++i)
            if (digits[i] != rhs.digits[i])
                return false;
        return true;
    }
}

void bigint::zero_remover()
{
    if (number_sign == sign::zero)
        return;
    while (digits[digits.size() - 1] == 0)
        digits.pop_back();
}

bigint &bigint::operator-=(const bigint &other)
{
    if (other.number_sign == sign::zero)
        return *this;
    else if (number_sign == sign::zero)
        *this = -other;
    else if (this->is_abs_equals(other))
        *this = bigint();
    else if (number_sign == other.number_sign)
    {
        if (this->is_abs_greater(other))
        {
            uint8_t borrow = 0;
            for (size_t i = 0; i < digits.size(); i++)
            {
                int16_t temp = digits[i];
                if (borrow > 0)
                {
                    temp -= 1;
                    if (temp < 0)
                    {
                        temp += 10;
                        borrow = 1;
                    }
                    else
                        borrow = 0;
                }
                if (i < other.digits.size())
                    temp -= other.digits[i];
                if (temp < 0)
                {
                    temp += 10;
                    borrow = 1;
                }
                digits[i] = static_cast<uint8_t>(temp);
            }
            this->zero_remover();
            return *this;
        }
        else if (!this->is_abs_greater(other))
        {
            bigint _other = other;
            uint8_t borrow = 0;
            for (size_t i = 0; i < _other.digits.size(); i++)
            {
                int16_t temp = _other.digits[i];
                if (borrow > 0)
                {
                    temp -= 1;
                    if (temp < 0)
                    {
                        temp += 10;
                        borrow = 1;
                    }
                    else
                        borrow = 0;
                }
                if (i < digits.size())
                    temp -= digits[i];
                if (temp < 0)
                {
                    temp += 10;
                    borrow = 1;
                }
                _other.digits[i] = static_cast<uint8_t>(temp);
            }
            _other.zero_remover();
            if (number_sign == sign::positive)
                number_sign = sign::negative;
            else
                number_sign = sign::positive;
            digits = _other.digits;
            return *this;
        }
    }
    else if (number_sign == sign::positive and other.number_sign == sign::negative)
    {
        *this += -other;
    }
    else if (number_sign == sign::negative and other.number_sign == sign::positive)
    {
        bigint temp = other;
        temp += -*this;
        *this = -temp;
    }
    return *this;
}

bigint operator-(bigint lhs, const bigint &rhs)
{
    lhs -= rhs;
    return lhs;
}