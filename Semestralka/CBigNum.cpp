//
// Created by victor on 22.5.18.
//

#include <algorithm>
#include <stack>
#include <iostream>
#include <iomanip>
#include "CLongInteger.h"
#include "CAddExp.h"
#include "CInteger.h"

void CBigNum::print() const
{
    std::cout << "----------" << std::endl;

    if(m_sgn)
        std::cout << "-";

    for(auto it = m_exponent.rbegin(); it < m_exponent.rend(); it++)
        if(*it) //do not print leading zeros
            std::cout << *it;

    if(m_fraction)
        std::cout << "." << m_fraction;

    std::cout << std::endl;
}

CBigNum & CBigNum::operator+(const CBigNum &other)
{
    if(m_sgn == true && other.m_sgn == false) {
        m_sgn = false;
        CBigNum copy = other;
        return copy.operator-(*this);
    } else if(m_sgn == false && other.m_sgn == true) {
        CBigNum copy = other;
        return operator-(copy.operator-());
    }

    std::vector<uint32_t> exponent1 = m_exponent;
    std::vector<uint32_t> exponent2 = other.m_exponent;

    uint32_t fractoin1 = m_fraction;
    uint32_t fractoin2 = other.m_fraction;

    expandFewerNumberWithZeros(exponent1, exponent2);

    std::vector<uint32_t> result;
    uint32_t tmp;
    unsigned short carry = 0;

    m_fraction = addFractions(fractoin1, fractoin2, carry);

    for(size_t i = 0; i < exponent1.size(); i++) {
        tmp = exponent1[i] + exponent2[i] + carry;
        carry = getCarry(tmp);
        tmp %= 1000000000;
        result.push_back(tmp);
    }

    if(carry)
        result.push_back(carry);

    m_exponent = result;
    return *this;
}

CBigNum &CBigNum::operator-(const CBigNum &other)
{
    if(m_sgn == false && other.m_sgn == true) {
        CBigNum copy = other;
        return operator+(copy.operator-());
    }
    else if (m_sgn == true && other.m_sgn == false) {
        m_sgn = false;
        CBigNum res = operator+(other);
        return res.operator-();
    }

    std::vector<uint32_t> exponent1 = m_exponent;
    std::vector<uint32_t> exponent2 = other.m_exponent;

    uint32_t fractoin1 = m_fraction;
    uint32_t fractoin2 = other.m_fraction;

    expandFewerNumberWithZeros(exponent1, exponent2);

    unsigned short carry = 0;
    m_fraction = subtractFractions(fractoin1, fractoin2, carry);

    std::vector<uint32_t> result;
    long int tmp;
    for(size_t i = exponent1.size() - 1; i >= 0; i--) {
        if(exponent1[i] > exponent2[i]) {
            for(size_t j = 0; j < exponent1.size(); j++) {
                tmp = (long)exponent1[j] - (long)exponent2[j] - carry;
                if(tmp < 0) {
                    carry = 1;
                    tmp = UINT32_MAX + tmp; //modulo
                } else
                    carry = 0;
                result.push_back((uint32_t)tmp);
            }
            m_exponent = result;
            m_sgn = false;
            return *this;
        } else if(exponent1[i] < exponent2[i]) {
            for(size_t j = 0; j < exponent1.size(); j++) {
                tmp = (long)exponent1[j] - (long)exponent2[j] + carry;
                if(tmp < 0) {
                    tmp *= -1;
                    carry = 0;
                }
                else {
                    carry = 1;
                    tmp = UINT32_MAX - tmp;
                }
                result.push_back((uint32_t)tmp);
            }
            m_exponent = result;
            m_sgn = true;
            return *this;
        }
    }
    result.push_back(0);
    m_exponent = result;
    m_sgn = false;
    return *this;
}

CBigNum &CBigNum::operator-()
{
    m_sgn = !m_sgn;
    return *this;
}

CBigNum CBigNum::operator* (const CBigNum & other)
{
    std::vector<uint32_t> exponent1 = m_exponent;
    std::vector<uint32_t> exponent2 = other.m_exponent;

    uint32_t fractoin1 = m_fraction;
    uint32_t fractoin2 = other.m_fraction;

    expandFewerNumberWithZeros(exponent1, exponent2);

    size_t len = exponent1.size();
    uint32_t carry = 0;
    unsigned long long tmpProduct;
    std::string tmpString;
    std::string resultString;
    std::stringstream buffer;
    std::stringstream ss;
    std::vector<std::string> results;

    for (size_t i = 0; i < len; i++) {
        for (size_t j = 0; j < len; j++) {
            tmpProduct = (unsigned long long)exponent2[i] * (unsigned long long)exponent1[j] + carry;
            buffer << std::setw(9) << std::setfill('0') << (uint32_t)(tmpProduct % 1000000000);
            buffer >> tmpString;
            std::reverse(tmpString.begin(), tmpString.end());
            buffer.clear();
            ss << tmpString;
            carry = tmpProduct / 1000000000;
        }
        if(carry) {
            buffer << carry;
            buffer >> tmpString;
            std::reverse(tmpString.begin(), tmpString.end());
            ss << tmpString;
            buffer.clear();
            carry = 0;
        }
        ss >> resultString;
        std::reverse(resultString.begin(), resultString.end());
        for(int k = 0; k < 9 * i; k++)
            resultString.push_back('0');
        while(resultString.front() == '0')
            resultString.erase(0,1);
        results.push_back(resultString);
        tmpString.clear();
        ss.clear();
    }

    std::stack<CExpression*> exprStack;

    CExpression * p;
    for(const auto & it : results) {
        if(it.size() > 6)
            p = new CLongInteger(it);
        else
            p = new CInteger(it);
        exprStack.push(p);
    }

    if(exprStack.size() == 1) {
        CBigNum finalResult = exprStack.top()->evaluate();
        if(m_sgn && !other.m_sgn || !m_sgn && other.m_sgn)
            finalResult.operator-();
        return finalResult;
    }

    CExpression * rVal;
    CExpression * lVal;
    do {
        rVal = exprStack.top(); exprStack.pop();
        lVal = exprStack.top(); exprStack.pop();

        p = new CAddExp(lVal, rVal);
        exprStack.push(p);

    } while(exprStack.size() != 1);

    CBigNum finalResult = exprStack.top()->evaluate();
    if(m_sgn && !other.m_sgn || !m_sgn && other.m_sgn)
        finalResult.operator-();
    return finalResult;
}

uint32_t CBigNum::addFractions(uint32_t f1, uint32_t f2, unsigned short &carry) const
{
    int i = 10;
    int cnt1 = 0, cnt2 = 0;

    while(f1 / i) {
        cnt1++;
        i *= 10;
    }

    i = 10;
    while(f2 / i) {
        cnt2++;
        i *= 10;
    }

    uint32_t result;
    if(cnt1 == cnt2) {
        uint64_t tmp = (uint64_t)f1 + (uint64_t)f2;
        result = (uint32_t)tmp;
        carry = tmp >> 32;
    } else if(cnt1 > cnt2) {
        for(int j = 0; j < abs(cnt1 - cnt2); j++)
            f2 *= 10;
        uint64_t tmp = (uint64_t)f1 + (uint64_t)f2;
        result = (uint32_t)tmp;
        carry = tmp >> 32;
    } else {
        for(int j = 0; j < abs(cnt1 - cnt2); j++)
            f1 *= 10;
        uint64_t tmp = (uint64_t)f1 + (uint64_t)f2;
        result = (uint32_t)tmp;
        carry = tmp >> 32;
        cnt1 = cnt2;
    }

    int cnt3 = 0;
    i = 10;
    while(result / i) {
        cnt3++;
        i *= 10;
    }

    if(cnt3 > cnt1) {
        carry = 1;
        result %= i / 10;
    }

    return result;
}

uint32_t CBigNum::subtractFractions(uint32_t f1, uint32_t f2, unsigned short &carry)
{
    long tmp = (long)f1 - (long)f2;
    if(tmp < 0) {
        carry = 1;
        tmp = 1000000 + tmp;
    }
    return (uint32_t)tmp;
}

unsigned short CBigNum::getCarry(const uint32_t sum) const
{
    if(sum / 1000000000)
        return 1;
    else
        return 0;
}

void CBigNum::expandFewerNumberWithZeros(std::vector<uint32_t> &exponent1, std::vector<uint32_t> &exponent2)
{
    //if lengths are different expand the shorter one with zeros
    size_t n;
    if(exponent1.size() == exponent2.size())
        n = exponent1.size();
    else if(exponent1.size() > exponent2.size()) {
        n = exponent2.size();
        for(size_t i = n; i < exponent1.size(); i++)
            exponent2.push_back(0);
    }
    else {
        n = exponent1.size();
        for(size_t i = n; i < exponent2.size(); i++)
            exponent1.push_back(0);
    }
}

CBigNum::CBigNum(bool sgn, const std::vector<uint32_t> &val)
{
    m_sgn = sgn;
    m_exponent = val;
    m_fraction = 0;
}

CBigNum::CBigNum(double val)
{
    if(val < 0) {
        m_sgn = true;
        val *= -1;
    } else
        m_sgn = false;

    std::string dStr = std::to_string(val);

    char * token;
    token = strtok((char*)dStr.c_str(), ".");
    std::stringstream ss;
    ss << token;
    uint32_t exponent;
    ss >> exponent;
    m_exponent.push_back(exponent);
    ss.clear();

    token = strtok(NULL, ".");
    ss << token;
    uint32_t fraction;
    ss >> fraction;
    m_fraction = fraction;
}

CBigNum::CBigNum(int val)
{
    if(val < 0) {
        m_sgn = true;
        val *= -1;
    } else
        m_sgn = false;
    m_exponent.push_back((uint32_t)val);
    m_fraction = 0;
}
