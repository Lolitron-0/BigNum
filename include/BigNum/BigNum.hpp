#pragma once
#include <cstdint>
#include <ostream>
#include <string>
#include <type_traits>
#include <vector>

namespace bignum
{

class ZeroDivisionException : public std::exception
{
public:
    const char* what() const noexcept override;
};

class InvalidInputException : public std::exception
{
public:
    const char* what() const noexcept override;
};

class BigNum
{
    using DigitType = int64_t;

public:
    BigNum() = default;

    template <typename Number,
              typename std::enable_if<
                  std::is_integral<Number>::value ||
                  std::is_floating_point<Number>::value>::type* = nullptr>
    BigNum(Number number) : BigNum{ std::to_string(number) }
    {
    }

    BigNum(const std::string&);

    ~BigNum() = default;

    BigNum(const BigNum&);
    BigNum(BigNum&&);

    // development only function
    static void initializePyTests();

    // Set pricision in digits
    static void setPrecision(int64_t);
    // Retrieve precision in degits
    static int64_t getPrecision();

public:
    BigNum& operator=(const BigNum&) = default;
    BigNum& operator=(BigNum&&) = default;
    BigNum operator-() const;

    friend BigNum& operator-=(BigNum&, const BigNum&);
    friend BigNum operator+(const BigNum&, const BigNum&);
    friend BigNum operator-(const BigNum&, const BigNum&);
    friend BigNum operator*(const BigNum&, const BigNum&);
    friend BigNum operator/(const BigNum&, const BigNum&);
    friend int32_t operator<=>(const BigNum&, const BigNum&);
    friend bool operator>(const BigNum&, const BigNum&) = default;
    friend bool operator<(const BigNum&, const BigNum&) = default;
    friend bool operator==(const BigNum&, const BigNum&) = default;
    operator std::string() const;

    // Find inverse of the number
    BigNum inverse() const;

private:
    template <typename Iterator>
    BigNum(const Iterator& begin, const Iterator& end)
        : m_Digits(begin, end)
    {
    }

    static void _commonExponent(BigNum&, BigNum&);
    static void _commonLength(BigNum&, BigNum&);

    void _removeInsignificantZeroes();
    void _shiftReprRight();
    void _normalize();

    constexpr const DigitType& operator[](int32_t i) const;
    constexpr DigitType& operator[](int32_t i);

private:
    static int64_t s_Precision;
    static constexpr int64_t s_Base{ 10 };
    static constexpr size_t s_MinKaratsubaSize{ 1 };

    std::vector<DigitType> m_Digits{ 1, 0 };
    bool m_Negative{ false };
    int64_t m_Exponent{ 1 };
};

std::ostream& operator<<(std::ostream&, const BigNum&);

int32_t operator<=>(const BigNum&, const BigNum&);

BigNum operator+(const BigNum&, const BigNum&);
BigNum operator-(const BigNum&, const BigNum&);
BigNum operator*(const BigNum&, const BigNum&);
BigNum operator/(const BigNum&, const BigNum&);
BigNum& operator-=(BigNum&, const BigNum&);

namespace literals
{
inline BigNum operator""_BN(unsigned long long num)
{
    return BigNum{ num };
}
inline BigNum operator"" _BN(const char* str, size_t len)
{
    return BigNum{ str };
}
inline BigNum operator""_BN(long double num) { return BigNum{ num }; }
} // namespace literals

} // namespace bignum
