#include <BigNum/BigNum.hpp>
#include <gtest/gtest.h>

using namespace bignum::literals;

TEST(Operations, Division)
{
    bignum::BigNum::setMinimalPrecision(100);
    EXPECT_EQ(
        bignum::BigNum{
            "0.10111213141516171819201234567"
            "8910111213141516171819201234567891011121314151617181920" } /
            bignum::BigNum{ "0.201918171615141312111098720191817161514131"
                            "211109872019181716151413121110987" },
        bignum::BigNum{
            "0."
            "500757958564931783728889871545981936661372011697163059987918"
            "4106819551768882935118580178634347498945" });
}

TEST(Operations, DivisionZero)
{
    bignum::BigNum::setMinimalPrecision(100);
    EXPECT_THROW("1"_BN / "0"_BN, bignum::ZeroDivisionException);
}

TEST(Operations, Inverse)
{
    bignum::BigNum::setMinimalPrecision(100);
    EXPECT_EQ(
        bignum::BigNum{
            "0.10111213141516171819201234567"
            "8910111213141516171819201234567891011121314151617181920" }
            .inverse(),
        bignum::BigNum{
            "9."
            "890010090817356205853542311693048335116960897173948164121655"
            "288718316265523871663315438708642078117834" });
}

TEST(Operations, Multiplication_integer)
{
    EXPECT_EQ(
        bignum::BigNum{
            "123456789101112131415161718192012345678910111213141516171819"
            "201234567"
            "8910111213141516171819201234567891011121314151617181920" } *
            bignum::BigNum{ "20191817161514131211109872019181716151413121"
                            "11098720191817161514131"
                            "211109872019181716151413121110987" },
        bignum::BigNum{
            "249281691287726668779424042525121949014343401458257698913954"
            "59671636431347812147674009"
            "984760595955020398474003974655773560954653085126970133709671"
            "33693084449510006055072545"
            "716956859606360429537726369278920725477508489755040" });
}

TEST(Operations, Multiplication_decimal)
{
    bignum::BigNum::setMinimalPrecision(100);
    EXPECT_EQ(
        bignum::BigNum{
            "0.10111213141516171819201234567"
            "8910111213141516171819201234567891011121314151617181920" } *
            bignum::BigNum{ "0.201918171615141312111098720191817161514131"
                            "211109872019181716151413121110987" },
        bignum::BigNum{
            "0."
            "020416376703459344995620984065805322697519844202274376108734"
            "686517219578761881212093693084449510006055072545716956859606"
            "360429537726369278920725477508489755040" });
}

TEST(Operations, Sum_Positive)
{
    bignum::BigNum::setMinimalPrecision(100);
    EXPECT_EQ(
        bignum::BigNum{
            "0.10111213141516171819201234567"
            "8910111213141516171819201234567891011121314151617181920" } +
            bignum::BigNum{ "0.201918171615141312111098720191817161514131"
                            "211109872019181716151413121110987" },
        bignum::BigNum{ "0."
                        "303030303030303030303111065870727272727272727281"
                        "691220416284042424242425138617181920" });
}

TEST(Operations, Sum_Negative)
{
    EXPECT_EQ("-21.5"_BN + -"24.5"_BN, "-46"_BN);
}

TEST(Operations, Sum_PositiveNegative)
{
    EXPECT_EQ("21.5"_BN + "-24.5"_BN, "-3"_BN);
}

TEST(Operations, Sum_DigitAdd)
{
    EXPECT_EQ("0.99999999"_BN + "0.00000001"_BN, 1_BN);
}

TEST(Operations, Difference_Positive)
{
    bignum::BigNum::setMinimalPrecision(100);
    EXPECT_EQ(
        bignum::BigNum{
            "0.10111213141516171819201234567"
            "8910111213141516171819201234567891011121314151617181920" } -
            bignum::BigNum{ "0.201918171615141312111098720191817161514131"
                            "211109872019181716151413121110987" },
        bignum::BigNum{ "-0."
                        "100806040199979593919086374512907050300989694938"
                        "05281794714826040199979683538281808" });
}

TEST(Operations, Difference_PositiveNegative)
{
    EXPECT_EQ("20.001"_BN - "-1.1"_BN, "21.101"_BN);
}

TEST(Operations, Pow)
{
    EXPECT_EQ(bignum::BigNum::pow("2.5"_BN, 4_BN), "39.0625"_BN);
}

TEST(Operations, Factorial)
{
    EXPECT_EQ("12"_BN.factorial(), "479001600"_BN);
}

TEST(Comparison, LT)
{
	EXPECT_LT(2934992.000344_BN, 100000000000_BN);
}

TEST(Comparison, LE_Equal)
{
	EXPECT_LE(1_BN, 1_BN);
}

TEST(Comparison, LE_Less)
{
	EXPECT_LE(1_BN, "1.000000000001"_BN);
}

TEST(Comparison, GT)
{
	EXPECT_GT(100_BN, "0.00000000001"_BN);
}

TEST(Comparison, EQ)
{
    EXPECT_EQ(bignum::BigNum{"123.00000123"}, "123.00000123"_BN);
}

TEST(Comparison, EQ_Substract)
{
    EXPECT_EQ("1.001"_BN - "0"_BN, "1.001"_BN);
}

TEST(Comparison, EQ_Zero)
{
    EXPECT_EQ(bignum::BigNum{"-0"}, "0"_BN);
}

TEST(Comparison, NE)
{
    EXPECT_NE(bignum::BigNum{"-1.1"}, "1.1"_BN);
}

TEST(General, ToString)
{
    EXPECT_EQ((std::string) bignum::BigNum{-123}, "-123");
}

TEST(General, ToString_Zero)
{
    EXPECT_EQ((std::string) bignum::BigNum{-0}, "0");
}

TEST(General, ToString_SmallExp)
{
    EXPECT_EQ((std::string) bignum::BigNum{"0.000001"}, "0.000001");
}


TEST(General, Cout)
{
    EXPECT_NO_THROW(std::cout << bignum::BigNum{-123});
}

TEST(General, GetPrecision)
{
    bignum::BigNum::setMinimalPrecision(125);
    EXPECT_EQ(bignum::BigNum::getMinimalPrecision(), 125);
}

TEST(General, DemoCase)
{
    bignum::BigNum numFromString{ "-234.567" };
    bignum::BigNum numFromBaseType1{ 12345 };
    bignum::BigNum numFromBaseType2{ 12345.333 };
    1_BN - 0.43_BN;
    "6514.22"_BN;

    1 / numFromString;
    numFromString.inverse();

    bignum::BigNum::setMinimalPrecision(1000);
    auto a{ -322.322_BN };
    bignum::BigNum b{ 121.21 };
    (a + b);
    (a - b);
    (a * b);
    (a / b);

    (a < b);
    (a >= b);
    (a == b);
    (a <=> b);
}
