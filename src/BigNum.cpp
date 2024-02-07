#include "BigNum/Assertions.hpp"
#include <BigNum/BigNum.hpp>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <python3.11/Python.h>
#include <string>

namespace bignum
{

#define BN_CLAMP_TO_ZERO(a) ((a) < 0 ? 0 : (a))

int64_t BigNum::s_Precision{ 100 };

void BigNum::initializePyTests()
{
    Py_Initialize();

    bignum::BigNum a{ "12345678910111213141516171819201234567891011"
                      "1213141516171819201234"
                      "567"
                      "89101112131415161718192012345678910111213141"
                      "51617181920" };
    bignum::BigNum b{ "2019181716151413121110987201918171615141"
                      "31211109872019181716"
                      "1514131"
                      "211109872019181716151413121110987" };
    ASSERT_BN_OP(a, b, *);
}

void BigNum::setMinimalPrecision(int64_t value)
{
    if (value > 0)
    {
        s_Precision = value;
    }
    else
    {
        throw InvalidInputException{};
    }
}

int64_t BigNum::getMinimalPrecision() { return BigNum::s_Precision; }

BigNum::BigNum() : m_Digits(1, 0) {}

BigNum BigNum::pow(const BigNum& base, const BigNum& power)
{
    BigNum result{ 1 };
    for (BigNum i{ 0 }; i < power; i = i + 1)
    {
        result = result * base;
    }
    return result;
}

BigNum::BigNum(const std::string& numStr)
{
    // TODO: string validation: alpha numeric, point pos
    std::string digits{ numStr };

    if (numStr[0] == '-')
    {
        this->m_Negative = true;
        digits = numStr.substr(1);
    }

    m_Exponent = numStr.size() - this->m_Negative;

    for (int32_t i{ 0 }; i < digits.size(); i++)
    {
        if (digits[i] != '.')
        {
            this->m_Digits.push_back(digits[i] - '0');
        }
        else
        {
            this->m_Exponent = i;
        }
    }

    this->_removeInsignificantZeroes();
}

BigNum::BigNum(const BigNum& other)
    : m_Exponent{ other.m_Exponent }, m_Negative{ other.m_Negative },
      m_Digits{ other.m_Digits }
{
}

BigNum::BigNum(BigNum&& other) : m_Digits{ std::move(other.m_Digits) }
{
    std::swap(this->m_Exponent, other.m_Exponent);
    std::swap(this->m_Negative, other.m_Negative);
}

void BigNum::_normalize()
{
    DigitType carry;
    for (int32_t i{ static_cast<int32_t>(m_Digits.size() - 1) }; i > 0;
         i--)
    {
        if (m_Digits[i] >= BigNum::s_Base)
        {
            carry = m_Digits[i] / BigNum::s_Base;
            m_Digits[i - 1] += carry;
            m_Digits[i] -= carry * BigNum::s_Base;
        }
        else if (m_Digits[i] < 0)
        {
            carry = (m_Digits[i] + 1) / BigNum::s_Base - 1;
            m_Digits[i - 1] += carry;
            m_Digits[i] -= carry * BigNum::s_Base;
        }
    }
}

void BigNum::_removeInsignificantZeroes()
{
    while (this->m_Digits.size() > std::max(1L, this->m_Exponent) &&
           !this->m_Digits.back())
    {
        this->m_Digits.pop_back();
    }

    while (this->m_Digits.size() && !this->m_Digits.front())
    {
        this->m_Digits.erase(this->m_Digits.begin());
        this->m_Exponent--;
    }
}

void BigNum::_shiftReprRight()
{
    if (this->m_Digits.empty())
    {
        this->m_Digits.push_back(0);
        return;
    }
    this->m_Digits.push_back(this->m_Digits.back());
    for (int32_t i{ static_cast<int32_t>(this->m_Digits.size()) - 2 };
         i > 0; i--)
    {
        this->m_Digits[i] = this->m_Digits[i - 1];
    }
}

BigNum BigNum::factorial() const
{
    BigNum result{ 1 };
    for (BigNum i{ 2 }; i <= *this; i = i + 1)
    {
        result = result * i;
    }
    return result;
}

BigNum BigNum::inverse() const
{
    if (*this == 0)
    {
        throw bignum::ZeroDivisionException{};
    }

    BigNum copy{ *this };
    BigNum result;
    result.m_Exponent = 1;
    result.m_Negative = this->m_Negative;
    result.m_Digits = {};
    BigNum dividend{ 1 };

    while (copy < 1)
    {
        copy.m_Exponent++;
        result.m_Exponent++;
    }

    while (dividend < copy)
    {
        dividend.m_Exponent++;
    }

    result.m_Exponent -= dividend.m_Exponent - 1;

    int64_t currentDigitsEvald{ 0 };
    int64_t totalDigits{ std::max(0L, result.m_Exponent) +
                         BigNum::s_Precision + 2 }; // for rounding
    BigNum::DigitType digit;
    do
    {
        digit = 0;
        while (dividend >= copy)
        {
            digit++;
            dividend -= copy;
        }

        dividend.m_Exponent++;
        dividend._removeInsignificantZeroes();
        result.m_Digits.push_back(digit);
        currentDigitsEvald++;
    } while (dividend != 0 && currentDigitsEvald < totalDigits);

    return result;
}

void BigNum::_commonExponent(BigNum& num1, BigNum& num2)
{
    while (num1.m_Exponent < num2.m_Exponent)
    {
        num1.m_Digits.insert(num1.m_Digits.begin(), 0);
        num1.m_Exponent++;
    }
    while (num2.m_Exponent < num1.m_Exponent)
    {
        num2.m_Digits.insert(num2.m_Digits.begin(), 0);
        num2.m_Exponent++;
    }
}

void BigNum::_commonLength(BigNum& num1, BigNum& num2)
{
    while (num1.m_Digits.size() < num2.m_Digits.size())
    {
        num1.m_Digits.push_back(0);
    }
    while (num2.m_Digits.size() < num1.m_Digits.size())
    {
        num2.m_Digits.push_back(0);
    }
}

BigNum BigNum::operator-() const
{
    BigNum copy{ *this };
    copy.m_Negative = !copy.m_Negative;
    return copy;
}

BigNum& operator-=(BigNum& a, const BigNum& b) { return (a = a - b); }

BigNum operator+(const BigNum& a, const BigNum& b)
{
    if (a.m_Negative)
    {
        if (b.m_Negative)
        {
            return -(-a + (-b));
        }
        else
        {
            return b - (-a);
        }
    }
    else if (b.m_Negative)
    {
        return a - (-b);
    }

    BigNum num1{ a };
    BigNum num2{ b };
    BigNum::_commonExponent(num1, num2);

    BigNum result;
    result.m_Digits.resize(
        std::max(num1.m_Digits.size(), num2.m_Digits.size()) + 1);

    BigNum::_commonLength(num1, num2);

    for (int32_t i{ 0 }; i < result.m_Digits.size() - 1; i++)
    {
        result[i + 1] = num1[i] + num2[i];
    }
    result.m_Exponent = std::max(a.m_Exponent, b.m_Exponent) + 1;
    result._normalize();
    result._removeInsignificantZeroes();
    return result;
}

BigNum operator-(const BigNum& a, const BigNum& b)
{
    if (b.m_Negative)
    {
        return a + (-b);
    }
    else if (a.m_Negative)
    {
        return -(-a + b);
    }
    else if (a < b)
    {
        return -(b - a);
    }

    BigNum num1{ a };
    BigNum num2{ b };
    BigNum::_commonExponent(num1, num2);

    BigNum result;
    result.m_Digits.resize(
        std::max(num1.m_Digits.size(), num2.m_Digits.size()) + 1);

    BigNum::_commonLength(num1, num2);

    for (int32_t i{ 0 }; i < result.m_Digits.size() - 1; i++)
    {
        result[i + 1] = num1[i] - num2[i];
    }
    result.m_Exponent = std::max(a.m_Exponent, b.m_Exponent) + 1;
    result._normalize();
    result._removeInsignificantZeroes();
    return result;
}

BigNum operator*(const BigNum& a, const BigNum& b)
{
    BigNum result;
    result.m_Digits.resize(a.m_Digits.size() + b.m_Digits.size());

    // TODO: fast multiplication not implemented
    if (1 || std::min(a.m_Digits.size(), b.m_Digits.size()) <=
                 BigNum::s_MinKaratsubaSize)
    {
        for (int32_t i{ 0 }; i < a.m_Digits.size(); i++)
        {
            for (int32_t j{ 0 }; j < b.m_Digits.size(); j++)
            {
                result[i + j + 1] += a[i] * b[j];
            }
        }
    }
    else
    {
        BigNum aPart1{ a.m_Digits.begin(),
                       a.m_Digits.begin() + (a.m_Digits.size() + 1) / 2 };
        BigNum aPart2{ a.m_Digits.begin() + aPart1.m_Digits.size(),
                       a.m_Digits.end() };

        BigNum bPart1{ b.m_Digits.begin(),
                       b.m_Digits.begin() + (b.m_Digits.size() + 1) / 2 };
        BigNum bPart2{ b.m_Digits.begin() + bPart1.m_Digits.size(),
                       b.m_Digits.end() };

        BigNum aPartsSum{ aPart1 + aPart2 };
        // ASSERT_BN_OP(aPart1, aPart2, +);
        BigNum bPartsSum{ bPart1 + bPart2 };
        // ASSERT_BN_OP(bPart1, bPart2, +);
        BigNum productOfPartSums{ aPartsSum * bPartsSum };
        // ASSERT_BN_OP(aPartsSum, bPartsSum, *);

        BigNum productOfFirstParts{ aPart1 * bPart1 };
        // ASSERT_BN_OP(aPart1, bPart1, *);
        BigNum productOfSecondParts{ aPart2 * bPart2 };
        // ASSERT_BN_OP(aPart2, bPart2, *);
        std::vector<BigNum::DigitType> sumOfMiddleTerms{
            productOfPartSums.m_Digits
        };
        for (int32_t i{ 0 }; i < productOfFirstParts.m_Digits.size(); i++)
        {
            sumOfMiddleTerms[i] -= productOfFirstParts[i];
        }
        for (int32_t i{ 0 }; i < productOfSecondParts.m_Digits.size();
             i++)
        {
            sumOfMiddleTerms[i] -= productOfSecondParts[i];
        }
        // BigNum sumOfMiddleTerms{productOfPartSums - productOfFirstParts
        // - productOfSecondParts};
        for (int32_t i{ 0 }; i < productOfFirstParts.m_Digits.size(); i++)
        {
            result[i] = productOfFirstParts[i];
        }
        for (int32_t i{ 0 }; i < productOfSecondParts.m_Digits.size();
             i++)
        {
            result[i + productOfFirstParts.m_Digits.size()] =
                productOfSecondParts[i];
        }
        // for (int32_t i{0}; i < sumOfMiddleTerms.size(); i++)
        // {
        //     result[i + aPart1.m_Digits.size()] += sumOfMiddleTerms[i];
        // }
    }
    result.m_Exponent = a.m_Exponent + b.m_Exponent;
    result.m_Negative = a.m_Negative != b.m_Negative;
    result._normalize();
    result._removeInsignificantZeroes();
    return result;
}

BigNum operator/(const BigNum& a, const BigNum& b)
{
    auto inv{ b.inverse() };
    auto result{ a * inv };
    auto leaveDigits{ (result.m_Exponent > 0 ? result.m_Exponent : 0) +
                      BigNum::s_Precision };
    result.m_Digits.resize(std::min(
        static_cast<int64_t>(result.m_Digits.size()), leaveDigits));
    return result;
}

BigNum::operator std::string() const
{
    if (this->m_Digits.empty())
    {
        return "0";
    }

    std::string result;
    int32_t i{ 0 };
    int64_t exp{ this->m_Exponent };
    if (exp <= 0)
    {
        result += "0.";
        result += std::string(std::abs(exp), '0');
        exp = 0;
    }

    for (; i < std::min(static_cast<int64_t>(this->m_Digits.size()),
                        this->m_Exponent);
         i++)
    {
        result += '0' + this->m_Digits[i];
        exp--;
    }

    while (i < exp)
    {
        result += "0";
    }

    if (this->m_Exponent < this->m_Digits.size() && this->m_Exponent > 0)
    {
        result += ".";
    }

    for (; i < this->m_Digits.size(); i++)
    {
        result += '0' + this->m_Digits[i];
    }

    return (m_Negative ? "-" : "") + result;
}

int32_t operator<=>(const BigNum& a, const BigNum& b)
{
    if (a.m_Negative != b.m_Negative)
    {
        return a.m_Negative < b.m_Negative ? 1 : -1;
    }

    if (a.m_Exponent != b.m_Exponent)
    {
        return ((a.m_Exponent > b.m_Exponent) ^ (a.m_Negative) ? 1 : -1);
    }

    BigNum aCp{ a };
    BigNum bCp{ b };
    while (aCp.m_Digits.size() > bCp.m_Digits.size())
    {
        bCp.m_Digits.push_back(0);
    }

    while (aCp.m_Digits.size() < bCp.m_Digits.size())
    {
        aCp.m_Digits.push_back(0);
    }

    for (int32_t i{ 0 }; i < aCp.m_Digits.size(); i++)
    {
        if (aCp[i] != bCp[i])
        {
            return ((aCp[i] > bCp[i]) ^ (aCp.m_Negative) ? 1 : -1);
        }
    }
    return 0;
}

constexpr const BigNum::DigitType& BigNum::operator[](int32_t i) const
{
    return this->m_Digits[i];
}

constexpr BigNum::DigitType& BigNum::operator[](int32_t i)
{
    return this->m_Digits[i];
}

std::ostream& operator<<(std::ostream& os, const BigNum& n)
{
    return os << (std::string)n;
}

const char* ZeroDivisionException::what() const noexcept
{
    return "Division by zero!";
}

const char* InvalidInputException::what() const noexcept
{
    return "Invalid input!";
}

} // namespace bignum
