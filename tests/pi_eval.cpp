#include <BigNum/BigNum.hpp>

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>

using namespace bignum::literals;

std::tuple<bignum::BigNum, bignum::BigNum, bignum::BigNum>
binarySplit(int32_t a, int32_t b)
{
    bignum::BigNum Pab, Qab, Rab;
    if (b == a + 1)
    {
        Pab = -(6_BN * a - 5) * (2_BN * a - 1) * (6_BN * a - 1);
        Qab = "10939058860032000"_BN * a * a * a;
        Rab = Pab * (545140134_BN * a + 13591409);
    }
    else
    {
        auto m{ (a + b) / 2 };
        auto [Pam, Qam, Ram]{ binarySplit(a, m) };
        auto [Pmb, Qmb, Rmb]{ binarySplit(m, b) };

        Pab = Pam * Pmb;
        Qab = Qam * Qmb;
        Rab = Qmb * Ram + Pam * Rmb;
    }
    return std::make_tuple(Pab, Qab, Rab);
}

std::string readNumFile(const std::string& filename)
{

    std::ifstream file{ filename };
    std::string res;
    if (file.is_open())
    {
        file >> res;
        file.close();
    }
    return res;
}

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        bignum::BigNum::setMinimalPrecision(atoi(argv[1]));
    }
    else
    {
        bignum::BigNum::setMinimalPrecision(1002);
    }
    bignum::BigNum koeff{ readNumFile("coeff_string") };
    auto start{ std::chrono::high_resolution_clock::now() };
    auto [P1n, Q1n, R1n]{ binarySplit(
        1,
        std::max(80ll, bignum::BigNum::getMinimalPrecision() / 10ll)) };
    auto pi{ (koeff * Q1n) / (13591409 * Q1n + R1n) };
    auto stop{ std::chrono::high_resolution_clock::now() };
    auto duration{ std::chrono::duration_cast<std::chrono::milliseconds>(
        stop - start) };
    std::cout << "Your pi: \n" << pi << std::endl;
    std::cout << "Took " << duration.count() << " milliseconds"
              << std::endl;
    auto piStr{ (std::string)pi };
    std::string actualPiStr{ readNumFile("pi_string") };
    int32_t matchCount{ 0 };
    for (int32_t i{ 2 }; i < piStr.length(); i++)
    {
        if (piStr[i] == actualPiStr[i])
        {
            matchCount++;
        }
        else
        {
            break;
        }
    }
    std::cout << "Matched " << matchCount << " digits" << std::endl;
    return 0;
}
