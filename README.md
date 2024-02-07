# BigNum
C++ library for long floating point arithmetics
## Example

```cpp
#include <BigNum/BigNum.hpp>
#include <iostream>

using namespace bignum::literals;

int main()
{
    bignum::BigNum numFromString{ "-234.567" };
    bignum::BigNum numFromBaseType1{ 12345 };
    bignum::BigNum numFromBaseType2{ 12345.333 };
    // User-defined literals
    std::cout << 1_BN - 0.43_BN;
    std::cout << "6514.22"_BN;

    std::cout << 1 / numFromString; // Implicit conversions
    std::cout << numFromString.inverse();

    // 1000 digit precision (default 100)
    bignum::BigNum::setMinimalPrecision(1000);
    auto a{ -322.322_BN };
    bignum::BigNum b{ 121.21 };
    std::cout << a + b << std::endl;
    std::cout << a - b << std::endl;
    std::cout << a * b << std::endl;
    std::cout << a / b << std::endl;

    std::cout << (a < b) << std::endl;
    std::cout << (a >= b) << std::endl;
    std::cout << (a == b) << std::endl;
    std::cout << (a <=> b) << std::endl;
    return 0;
}

```
