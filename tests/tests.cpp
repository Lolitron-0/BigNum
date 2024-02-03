#include <BigNum/BigNum.hpp>
#include <gtest/gtest.h>

using namespace bignum::literals;

TEST(Operations, Division)
{
    bignum::BigNum::setMinimalPrecision(100);
    EXPECT_EQ(
        bignum::BigNum{
            "0.10111213141516171819201234567"
            "8910111213141516171819201234567891011121314151617181920" }
            /
            bignum::BigNum{ "0.201918171615141312111098720191817161514131"
                            "211109872019181716151413121110987" },
        bignum::BigNum{
            "0.5007579585649317837288898715459819366613720116971630599879184106819551768882935118580178634347498945" });
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
            "2887183162655238716633154387086420781178" });
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

TEST(Operations, Sum)
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

TEST(Operations, Difference)
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
