BigNum operator/(const BigNum &a, const BigNum &b)
{
    if (b == 0)
        throw("Division by zero");
    BigNum bAbs{b};
    bAbs.m_Negative = false;
    std::deque<BigNum::DigitType> resultDigits;
    BigNum current;
    for (int32_t i{static_cast<int32_t>(a.m_Digits.size() - 1)}; i >= 0; i--)
    {
        current._shiftReprRight();
        current.m_Digits[0] = a.m_Digits[i];
        BigNum::DigitType x{0};
        BigNum::DigitType left{0};
        BigNum::DigitType right{BigNum::s_Base};
        BigNum::DigitType mid;
        BigNum tmp;
        while (left <= right)
        {
            mid = (left + right) / 2;
            tmp = bAbs * mid;
            if (tmp < current)
            {
                x = mid;
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }
        resultDigits.push_front(x);
        current -= b * x;
    }
    BigNum resultNum{resultDigits.begin(), resultDigits.end()};
    resultNum.m_Negative = a.m_Negative != b.m_Negative;
    return resultNum;
}
