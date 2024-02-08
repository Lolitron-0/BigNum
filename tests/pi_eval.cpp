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
    bignum::BigNum koeff{
        "42698670."
        "6663333958177128891606596082733208840025090828008380071788526051"
        "5745759421630179991145566860134573716749408041139229273618126672"
        "8193136882170582563460066798766483460795735983552333985484854583"
        "2762473774912507545850325782197456759912124003920153233212768354"
        "4629648583735569730601212345875804914321664042742354797851044822"
        "1162836911053807235838159872646304853335987865686269706977445355"
        "8355991335396786419023123915238298774811088986646222490060213312"
        "3640475004317852138580294466285566561287664084990866080668477800"
        "2991357625433646133139055099023131780968145833996701200122389012"
        "1544217243622840686293294200505214190159390925699071943400294444"
        "3395184862976639746550589509887267697068804437271525728023522738"
        "2872383401509275515634457705197803145721985414408323372552767448"
        "5625623883182211963677365447450162580542514340846860388028410609"
        "1141850281570498384133143209516156684442922928123623464567026873"
        "4321517159131712143438348676514584576378735574108814073595022482"
        "2617863059170606823963307568928054734494021432772379319635163694"
        "3568535236509248454194246209288387776349711384018983557918804101"
        "5469199214591024464903812082236674251398135427633950703414918564"
        "3985359024518359633292259930946209967761946001470275187859964324"
        "74421329"
    };
    auto start{ std::chrono::high_resolution_clock::now() };
    auto [P1n, Q1n, R1n]{ binarySplit(
        1, std::max(80ll, bignum::BigNum::getMinimalPrecision() / 10ll)) };
    auto pi{ (koeff * Q1n) / (13591409 * Q1n + R1n) };
    auto stop{ std::chrono::high_resolution_clock::now() };
    auto duration{ std::chrono::duration_cast<std::chrono::milliseconds>(
        stop - start) };
    std::cout << "Your pi: \n" << pi << std::endl;
    std::cout << "Took " << duration.count() << " milliseconds"
              << std::endl;
    auto piStr{ (std::string)pi };
	std::ifstream piFile{"pi_string"};
	std::string actualPiStr;
	if (piFile.is_open())
	{
		piFile >> actualPiStr;
		piFile.close();
	}
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
