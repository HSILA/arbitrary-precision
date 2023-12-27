#include <iostream>
#include <stdexcept>
#include <vector>
#include <limits>
#include <cmath>
#include <cctype>
#include <string>
using namespace std;

/**
 * @brief Enumeration to represent the sign of a bigint number.
 *
 * Since built-in integer types in C++ do not distinguish between +0 and -0 as in floating point types,
 * this enumeration allows representing zero as an independent sign along with positive and negative.
 *
 */
enum class sign
{
    negative,
    zero,
    positive
};

/**
 * @brief This class implements the arbitrary precision integers and allows simple arithmetic operations
 * between these integers.
 *
 */
class bigint
{
public:
    bigint();
    bigint(const int64_t &);
    bigint(const string &);
    /**
     * @brief Construct a new bigint object with another (Default copy constructor)
     *
     */
    bigint(const bigint &) = default;
    void set(const int64_t &);
    void set(const string &);
    bigint &operator=(const bigint &);
    bigint operator-() const;
    bigint &operator+=(const bigint &);
    bigint &operator-=(const bigint &);
    bigint &operator*=(const bigint &);
    bool operator==(const bigint &) const;
    bool operator<(const bigint &) const;
    friend ostream &operator<<(ostream &, const bigint &);
    /**
     * @brief Exception thrown when the input string representing a bigint number contains non-digit characters.
     *
     */
    inline static invalid_argument non_digit = invalid_argument("The input string contains non digit characters!");
    /**
     * @brief Exception thrown when the input string contains leading zeros in the beginning.
     *
     */
    inline static invalid_argument leading_zeros = invalid_argument("The input number cannot have leading zeros!");
    /**
     * @brief Exception thrown when the input string is empty.
     *
     */
    inline static invalid_argument empty_string = invalid_argument("The input string is empty!");

private:
    /**
     * @brief Sign of the current bigint object, can be negative, positive or zero.
     *
     */
    sign number_sign;
    /**
     * @brief The vector containing the bigint's digits, each element of the vector is an integer in [0,9].
     *
     */
    vector<uint8_t> digits;
    void fill_digits(const string);
    bool is_digit(const string) const;
    bool is_abs_greater(const bigint &) const;
    void zero_remover();
};

/**
 * @brief Sets the current bigint number to a signed 64-bit integer number.
 *
 * @param number A signed 64-bit integer used to set the bigint value with.
 */
void bigint::set(const int64_t &number)
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

/**
 * @brief Sets the current bigint object to the value represented by a string.
 *
 * It parses the string, assuming it represents a decimal integer number and throws an exception when the number contains non-digit
 * characters, has leading zeros or the string is empty. It will also parse the sign of the number and assign the `bigint::number_sign` accordingly.
 *
 * @param input_string An input string representing a bigint number to set the value of current object with.
 */
void bigint::set(const string &input_string)
{
    if (input_string.empty())
        throw empty_string;
    if (input_string == "+0" or input_string == "-0")
    {
        number_sign = sign::zero;
        digits.push_back(0);
    }

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
        fill_digits(input_string.substr(1, input_string.length() - 1));
        number_sign = sign::negative;
    }
    else if (input_string[0] == '+')
    {
        fill_digits(input_string.substr(1, input_string.length() - 1));
        number_sign = sign::positive;
    }
    else
    {
        fill_digits(input_string);
        number_sign = sign::positive;
    }
}

/**
 * @brief Construct a new bigint::bigint object representing the number zero.
 *
 */
bigint::bigint()
{
    number_sign = sign::zero;
    digits.push_back(0);
}

/**
 * @brief Construct a new bigint::bigint object with a signed 64-bit integer using the `set` member function.
 *
 * @param number A signed 64 input number to initialize the object with.
 */
bigint::bigint(const int64_t &number)
{
    set(number);
}

/**
 * @brief Construct a new bigint::bigint object with a string using the `set` member function.
 *
 * @param input_string An input string representing a bigint number to initialize the object with.
 */
bigint::bigint(const string &input_string)
{
    set(input_string);
}

/**
 * @brief Checks whether a string represents a number.
 *
 * @param input_string An input string to be checked.
 * @return true If all the characters in the string are digits.
 * @return false If the string has any non-digit characters.
 */
bool bigint::is_digit(const string input_string) const
{
    for (const char &c : input_string)
        if (!isdigit(c))
            return false;
    return true;
}

/**
 * @brief Parses a string representing a bigint number and fills it in the `bigint::digits` vector.
 *
 * This function checks the validity of `input_string` and ensures it only contains digit characters.
 * Then it fill fill out the `bigint::digits` vector in a reverse order, meaning that the least significant
 * digit will be filled into the first element of vector.
 *
 * @param input_string An input string representing a bigint number.
 */
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

/**
 * @brief Returns a number that is the negation of current bigint object.
 *
 * @return bigint A copy to a new bigint object that is a negation of the current number.
 */
bigint bigint::operator-() const
{
    bigint out = *this;
    if (out.number_sign == sign::negative)
        out.number_sign = sign::positive;
    else if (out.number_sign == sign::positive)
        out.number_sign = sign::negative;
    return out;
}

/**
 * @brief Inserts a bigint object into an output stream.
 *
 * @param out An output stream.
 * @param number The bigint number to be inserted into the output stream.
 * @return ostream& A reference to the output stream.
 */
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

/**
 * @brief Checks whether current bigint (left-hand side) is equal to another (right-hand side).
 *
 * @param other The right-hand side bigint operand of the comparison.
 * @return true If  bigint operands are equal.
 * @return false If two bigint operands are not equal.
 */
bool bigint::operator==(const bigint &other) const
{
    if ((digits.size() != other.digits.size()) or (number_sign != other.number_sign))
        return false;
    for (size_t i = 0; i < digits.size(); ++i)
        if (digits[i] != other.digits[i])
            return false;
    return true;
}

/**
 * @brief Checks whether two bigint numbers are unequal.
 *
 * @param lhs The left-hand side bigint operand of the comparison.
 * @param rhs The right-hand side bigint operand of the comparison.
 * @return true If the current number is equal to `rhs`.
 * @return false If the current number is not equal to `rhs`.
 */
bool operator!=(const bigint &lhs, const bigint &rhs)
{
    return !(lhs == rhs);
}

/**
 * @brief Checks whether current bigint (left-hand side) is less than the other (right-hand side).
 *
 * @param other The right-hand side bigint operand of the comparison.
 * @return true If left-hand side is less than `rhs`;
 * @return false If current number is greater than or equal to `rhs`.
 */
bool bigint::operator<(const bigint &other) const
{
    if (*this == other)
        return false;

    switch (number_sign)
    {
    case sign::negative:
        if (other.number_sign == sign::negative)
        {
            if (digits.size() > other.digits.size())
                return true;
            else if (digits.size() < other.digits.size())
                return false;
            else
            {
                for (size_t i = digits.size(); i > 0; i--)
                {
                    if (digits[i - 1] > other.digits[i - 1])
                        return true;
                    else if (digits[i - 1] < other.digits[i - 1])
                        return false;
                }
                return false;
            }
        }
        else
            return true;
        break;
    case sign::zero:
        if (other.number_sign == sign::positive)
            return true;
        else
            return false;
        break;
    case sign::positive:
        if (other.number_sign == sign::positive)
        {
            if (digits.size() > other.digits.size())
                return false;
            else if (digits.size() < other.digits.size())
                return true;
            else
            {
                for (size_t i = digits.size(); i > 0; i--)
                {
                    if (digits[i - 1] < other.digits[i - 1])
                        return true;
                    else if (digits[i - 1] > other.digits[i - 1])
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

/**
 * @brief Checks whether left-hand side is greater than the right-hand side.
 *
 * @param lhs The left-hand side bigint operand of the comparison.
 * @param rhs The right-hand side bigint operand of the comparison.
 * @return true If `lhs` is greater than ‍‍‍`rhs`.
 * @return false If `lhs` is less than or equal to `rhs`.
 */
bool operator>(const bigint &lhs, const bigint &rhs)
{
    return !((lhs < rhs) or (lhs == rhs));
}

/**
 * @brief Checks whether left-hand side is greater than or equal to the right-hand side.
 *
 * @param lhs The left-hand side bigint operand of the comparison.
 * @param rhs The right-hand side bigint operand of the comparison.
 * @return true If `lhs` is greater than or equal to `rhs`.
 * @return false If `lhs` is less than `rhs`.
 */
bool operator>=(const bigint &lhs, const bigint &rhs)
{
    return !(lhs < rhs);
}

/**
 * @brief Checks whether left-hand side is less than or equal to the right-hand side.
 *
 * @param lhs The left-hand side bigint operand of the comparison.
 * @param rhs The right-hand side bigint operand of the comparison.
 * @return true If `lhs` is less than or equal to rhs.
 * @return false If `lhs` is greater than `rhs`.
 */
bool operator<=(const bigint &lhs, const bigint &rhs)
{
    return !(lhs > rhs);
}

/**
 * @brief Assigns the value of another bigint number to the current number.
 *
 * @param other The value to be assigned to the current number.
 * @return bigint& A reference to the modified bigint number.
 */
bigint &bigint::operator=(const bigint &other)
{
    if (this != &other)
    {
        digits = other.digits;
        number_sign = other.number_sign;
    }
    return *this;
}

/**
 * @brief Adds the current bigint number to the other number.
 *
 * @param other The other bigint number (summand).
 * @return bigint& A reference to the current object after summation.
 */
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

/**
 * @brief Adds two bigint numbers.
 *
 * @param lhs The left-hand side operand (summand).
 * @param rhs The right-hand side operand (summand).
 * @return bigint A copy of the result after subtraction.
 */
bigint operator+(bigint lhs, const bigint &rhs)
{
    lhs += rhs;
    return lhs;
}

/**
 * @brief Compares the absolute value of current bigint number with another.
 *
 * @param other The other bigint number, the right-hand side of the comparison.
 * @return true If the absolute value of the current number is greater than the other.
 * @return false If the If the absolute value of the current number is less than the other.
 */
bool bigint::is_abs_greater(const bigint &other) const
{
    if (digits.size() != other.digits.size())
        return digits.size() > other.digits.size();
    else
    {
        for (size_t i = digits.size(); i > 0; i--)
        {
            if (digits[i - 1] > other.digits[i - 1])
                return true;
            else if (digits[i - 1] < other.digits[i - 1])
                return false;
        }
        return false;
    }
}

/**
 * @brief Removes leading zeros of a bigint object.‍ It will iterate the digits starting from the most significant digit, and remove any leading zeros in the start, if available.
 *
 */
void bigint::zero_remover()
{
    if (number_sign == sign::zero)
        return;
    while (digits[digits.size() - 1] == 0)
        digits.pop_back();
}

/**
 * @brief Subtracts the other bigint number from the current bigint number.
 *
 * @param other The other bigint number (subtrahend).
 * @return bigint& A reference to the current object after subtraction.
 */
bigint &bigint::operator-=(const bigint &other)
{
    if (other.number_sign == sign::zero)
        return *this;
    else if (number_sign == sign::zero)
        *this = -other;
    else if (*this == other)
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

/**
 * @brief Subtracts two bigint numbers.
 *
 * @param lhs The left-hand side operand (minuend).
 * @param rhs The right-hand side operand (subtrahend).
 * @return bigint A copy of the result after subtraction.
 */
bigint operator-(bigint lhs, const bigint &rhs)
{
    lhs -= rhs;
    return lhs;
}

/**
 * @brief Multiplies the current bigint number by another and stores the result in the current object.
 *
 * @param other The other bigint number (multiplicand).
 * @return bigint& A reference to the current object after multiplication.
 */
bigint &bigint::operator*=(const bigint &other)
{
    if (number_sign == sign::zero or other.number_sign == sign::zero)
    {
        number_sign = sign::zero;
        digits = {0};
        return *this;
    }
    if (number_sign == other.number_sign)
        number_sign = sign::positive;
    else
        number_sign = sign::negative;

    if (digits.size() == 1 and digits[0] == 1)
    {
        digits = other.digits;
        return *this;
    }
    else if (other.digits.size() == 1 and other.digits[0] == 1)
        return *this;

    if (digits.size() >= other.digits.size())
    {
        bigint result;
        for (size_t i = 0; i < other.digits.size(); i++)
        {
            uint8_t carry = 0;
            bigint temp;
            temp.digits.resize(i, 0);
            for (size_t j = 0; j < digits.size(); j++)
            {
                uint8_t multiply = digits[j] * other.digits[i] + carry;
                carry = multiply / 10;
                temp.digits.push_back(multiply % 10);
            }
            if (carry > 0)
                temp.digits.push_back(carry);
            temp.number_sign = sign::positive;
            result += temp;
        }
        digits = result.digits;
        return *this;
    }
    else
    {
        bigint result;
        for (size_t i = 0; i < digits.size(); i++)
        {
            uint8_t carry = 0;
            bigint temp;
            temp.digits.resize(i, 0);
            for (size_t j = 0; j < other.digits.size(); j++)
            {
                uint8_t multiply = digits[i] * other.digits[j] + carry;
                carry = multiply / 10;
                temp.digits.push_back(multiply % 10);
            }
            if (carry > 0)
                temp.digits.push_back(carry);
            temp.number_sign = sign::positive;
            result += temp;
        }
        digits = result.digits;
        return *this;
    }
}

/**
 * @brief Multiplies two bigint numbers.
 *
 * @param lhs The left-hand side operand (multiplier).
 * @param rhs The right-hand side operand (multiplicand).
 * @return bigint A copy of the result after multiplication.
 */
bigint operator*(bigint lhs, const bigint &rhs)
{
    lhs *= rhs;
    return lhs;
}