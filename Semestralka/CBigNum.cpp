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
#include "Exceptions.h"

CBigNum::CBigNum()
        : m_sgn(false)
{
    m_integer.push_back(0);
    m_fraction.push_back(0);
}

CBigNum::CBigNum(int val)
{
    if(val < 0) {
        m_sgn = true;
        val *= -1;
    } else
        m_sgn = false;
    m_integer.push_back((uint32_t)val);
    m_fraction.push_back(0);
}

CBigNum::CBigNum(bool sgn, const std::vector<uint32_t> & integer)
{
    m_sgn = sgn;
    m_integer = integer;
    m_fraction.push_back(0);
}

CBigNum::CBigNum(bool sgn, const std::vector<uint32_t> &integer, const std::vector<uint32_t> & fraction)
{
    m_sgn = sgn;
    m_integer = integer;
    m_fraction = fraction;
}

void CBigNum::print() const
{
    std::cout << "----------" << std::endl;

    if(m_sgn)
        std::cout << "-";

    std::string output = toString(m_integer);

    if(!(m_fraction.size() == 1 && m_fraction[0] == 0)) {
        output += ".";
        output += toString(m_fraction);
    }

    std::cout << output;

    std::cout << std::endl;
}

CBigNum CBigNum::operator+(const CBigNum &other) const
{
    if(m_sgn == true && other.m_sgn == false) {
        CBigNum negativeThis(!m_sgn, m_integer, m_fraction);
        return other.operator-(negativeThis);
    }
    else if(m_sgn == false && other.m_sgn == true)
        return operator-(other.operator-());

    std::vector<uint32_t> integer1 = m_integer;
    std::vector<uint32_t> integer2 = other.m_integer;

    std::vector<uint32_t> fractoin1 = m_fraction;
    std::vector<uint32_t> fractoin2 = other.m_fraction;

    expandFewerNumberWithZeros(integer1, integer2);

    unsigned short carry = 0;

    std::vector<uint32_t> fractionSum = addFractions(fractoin1, fractoin2, carry);
    std::vector<uint32_t> exponentSum = additionAlgorithm(integer1, integer2, carry);

    if(carry)
        exponentSum.push_back(carry);

    bool sgn = false;
    if(m_sgn && other.m_sgn)
        sgn = true;
    else if(!m_sgn && !other.m_sgn)
        sgn = false;

    return CBigNum(sgn, exponentSum, fractionSum);
}

CBigNum CBigNum::operator-(const CBigNum &other) const
{
    if(m_sgn == false && other.m_sgn == true) {
        CBigNum copy = other;
        return operator+(copy.operator-());
    }
    else if (m_sgn == true && other.m_sgn == false) {
        CBigNum copy(!m_sgn, m_integer, m_fraction);
        CBigNum result = copy.operator+(other);
        return result.operator-();
    }

    std::vector<uint32_t> integer1 = m_integer;
    std::vector<uint32_t> integer2 = other.m_integer;

    std::vector<uint32_t> fractoin1 = m_fraction;
    std::vector<uint32_t> fractoin2 = other.m_fraction;

    expandFewerNumberWithZeros(integer1, integer2);

    unsigned short carry = 0;
    bool sgn;

    std::vector<uint32_t> fractionDifference = subtractFractions(fractoin1, fractoin2, carry);
    std::vector<uint32_t> exponentDifference = subtractionAlgorithm(integer1, integer2, carry, sgn);

    return CBigNum(sgn, exponentDifference, fractionDifference);
}

CBigNum CBigNum::operator-() const
{
    return CBigNum(!m_sgn, m_integer, m_fraction);
}

CBigNum CBigNum::operator* (const CBigNum & other) const
{
    CBigNum product;
    std::vector<uint32_t> integer1 = m_integer;
    std::vector<uint32_t> integer2 = other.m_integer;

    std::vector<uint32_t> fractoin1 = m_fraction;
    std::vector<uint32_t> fractoin2 = other.m_fraction;

    if(integer1.size() == 1 && integer1[0] == 0 && fractoin1.size() == 1 && fractoin1[0] == 0
       || integer2.size() == 1 && integer2[0] == 0 && fractoin2.size() == 1 && fractoin2[0] == 0)
        return CBigNum(0);


    //if fraction1 == fraction2 == 0
    if(fractoin1.size() == 1 && fractoin1[0] == 0 && fractoin2.size() == 1 && fractoin2[0] == 0){
        int tens = 0;
        tens = eliminateEndingZeros(integer1, integer2);
        expandFewerNumberWithZeros(integer1, integer2);

        product = multiplicationAlgorithm(integer1, integer2);

        while(tens) {
            product.multiplyByTen();
            tens--;
        }
    }else {
        expandFewerNumberWithZeros(integer1, integer2);

        std::string integer1String = toString(integer1);
        std::string integer2String = toString(integer2);

        std::string fraction1String = toString(fractoin1);
        std::string fraction2String = toString(fractoin2);

        if(fraction1String == "0")
            fraction1String.clear();

        if(fraction2String == "0")
            fraction2String.clear();

        size_t fraction1Len = fraction1String.size();
        size_t fraction2Len = fraction2String.size();
        size_t productFractionLen = fraction1Len + fraction2Len;

        std::string noPoint1 = integer1String + fraction1String;
        std::string noPoint2 = integer2String + fraction2String;

        std::vector<uint32_t> num1 = toBigInt(noPoint1);
        std::vector<uint32_t> num2 = toBigInt(noPoint2);

        product = multiplicationAlgorithm(num1, num2);

        std::string productString = toString(product);
        std::string productFractionString = productString.substr(productString.size() - productFractionLen);
        productString.erase(productString.size() - productFractionLen);

        std::vector<uint32_t> productExponent = toBigInt(productString);
        std::vector<uint32_t> productFraction = toBigInt(productFractionString);

        product = CBigNum(false, productExponent, productFraction);
    }

    if(m_sgn && !other.m_sgn || !m_sgn && other.m_sgn)
        return product.operator-();

    return product;
}

CBigNum CBigNum::operator/(const CBigNum &other) const
{
    std::vector<uint32_t> integer1 = m_integer;
    std::vector<uint32_t> integer2 = other.m_integer;

    std::vector<uint32_t> fraction1 = m_fraction;
    std::vector<uint32_t> fraction2 = other.m_fraction;

    if(isZero(integer2) && isZero(fraction2))
        throw DivisionByZero();

    if(!isZero(fraction1) || !isZero(fraction2))
        throw DecimalDivision();

    if(integer1.size() == 1 && integer2.size() == 1)
        return CBigNum((int)(integer1[0] / integer2[0]));


    //if dividend is equal to divisor that result is 1
    if(integer1 == integer2)
        return CBigNum(1);


    if(integer1.size() == integer2.size()) {
        int len = (int)integer1.size();

        for(int i = len - 1; i >= 0; i--)
            //if dividend is less than divisor than quotient is 0
            if(integer1[i] < integer2[i])
                return CBigNum(); //default constructor result is 0
            else if(integer1[i] > integer2[i])
                break;
    } else if(integer1.size() < integer2.size())
        return CBigNum(); //default constructor result is 0

    std::string quotient;
    std::string remainder;

    divisionAlgorithm(integer1, integer2, quotient, remainder);

    return CLongInteger(quotient).evaluate();
}

CBigNum CBigNum::operator%(const CBigNum & other) const
{
    std::vector<uint32_t> integer1 = m_integer;
    std::vector<uint32_t> integer2 = other.m_integer;

    std::vector<uint32_t> fraction1 = m_fraction;
    std::vector<uint32_t> fraction2 = other.m_fraction;

    if(isZero(integer2) && isZero(fraction2))
        throw DivisionByZero();

    if(!isZero(fraction1) || !isZero(fraction2))
        throw DecimalDivision();

    //if dividend is equal to divisor than remainder is 0
    if(integer1 == integer2)
        return CBigNum(0);

    if(integer1.size() == integer2.size()) {
        int len = (int)integer1.size();

        for(int i = len - 1; i >= 0; i--)
            //if dividend is less than divisor than remainder is dividend
            if(integer1[i] < integer2[i])
                return CBigNum(false, integer1);
            else if(integer1[i] > integer2[i])
                break;
    } else if(integer1.size() < integer2.size())
        return CBigNum(false, integer1);

    std::string quotient;
    std::string remainder;

    divisionAlgorithm(integer1, integer2, quotient, remainder);

    return CLongInteger(remainder).evaluate();
}

bool CBigNum::operator>=(const CBigNum & other) const
{

    std::string value1 = toString(m_integer);
    std::string value2 = toString(other.m_integer);

    while(value1[0] == '0')
        value1.erase(0,1);

    while(value2[0] == '0')
        value2.erase(0,1);

    size_t len1 = value1.size();
    size_t len2 = value2.size();

    if(len1 > len2)
        return true;
    else if(len1 < len2)
        return false;
    else {
        size_t len = len1; //len1 == len2
        for(size_t i = 0; i < len; i++)
            if(value1[i] - '0' > value2[i] - '0')
                return true;
            else if(value1[i] - '0' < value2[i] - '0')
                return false;

        return true;
    }
}

std::string CBigNum::toString() const
{
    std::string result;
    if(m_sgn)
        result += "-";
    result += toString(m_integer);
    std::string fraction;
    fraction = toString(m_fraction);
    if(fraction != "0") {
        result += ".";
        result += fraction;
    }
    return result;
}

std::vector<uint32_t> CBigNum::additionAlgorithm(const std::vector<uint32_t> & num1, const std::vector<uint32_t> & num2, unsigned short & carry) const
{
    uint32_t tmp;
    size_t len = num1.size(); //num1.size() == num2.size()
    std::vector<uint32_t> sum;

    for(size_t i = 0; i < len; i++) {
        tmp = num1[i] + num2[i] + carry;
        carry = tmp / 1000000000;//10^9
        tmp %= 1000000000;//10^9
        sum.push_back(tmp);
    }

    return sum;
}

std::vector<uint32_t> CBigNum::subtractionAlgorithm(const std::vector<uint32_t> & num1,
                                                    const std::vector<uint32_t> & num2,
                                                    unsigned short & carry, bool & sgn) const
{
    std::vector<uint32_t> difference;
    long int tmp;

    if(num1 == num2) {
        difference.push_back(0);
        return difference;
    }

    for(int i = (int)num1.size() - 1; i >= 0; i--) {
        if(num1[i] > num2[i]) {
            for(size_t j = 0; j < num1.size(); j++) {
                tmp = (long)num1[j] - (long)num2[j] - carry;
                if(tmp < 0) {
                    carry = 1;
                    tmp = 1000000000 + tmp; //modulo
                } else
                    carry = 0;
                difference.push_back((uint32_t)tmp);
            }
            sgn = false;
            break;
        } else if(num1[i] < num2[i]) {
            for(size_t j = 0; j < num1.size(); j++) {
                tmp = (long)num1[j] - (long)num2[j] + carry;
                if(tmp < 0) {
                    tmp *= -1;
                    carry = 0;
                }
                else {
                    carry = 1;
                    tmp = 1000000000 - tmp;
                }
                difference.push_back((uint32_t)tmp);
            }
            sgn = true;
            break;
        }
    }

    return difference;
}

CBigNum CBigNum::multiplicationAlgorithm(const std::vector<uint32_t> & factor1,
                                         const std::vector<uint32_t> & factor2) const
{
    size_t len = factor1.size();//factor1.size() == factor2.size()
    uint32_t carry = 0;
    std::vector<std::string> intermediateResults;
    std::vector<std::string> intermediateProduct;

    for (size_t i = 0; i < len; i++) {
        if(factor2[i] == 0)
            continue;
        for (size_t j = 0; j < len; j++) {
            unsigned long long tmpProduct = (unsigned long long)factor2[i] * (unsigned long long)factor1[j] + carry;
            intermediateProduct.push_back( toString( (uint32_t)(tmpProduct % 1000000000) ) ); //10 ^ 9
            carry = tmpProduct / 1000000000;
        }
        if(carry) {
            intermediateProduct.push_back( toString(carry) );
            carry = 0;
        }
        std::string intermediateProductString;
        for(auto it = intermediateProduct.rbegin(); it < intermediateProduct.rend(); it++) {
            intermediateProductString += *it;
        }
        for(int k = 0; k < 9 * i; k++)
            intermediateProductString.push_back('0');
        while(intermediateProductString.front() == '0')
            intermediateProductString.erase(0,1);
        intermediateResults.push_back(intermediateProductString);
        intermediateProduct.clear();
    }

    std::stack<CExpression*> exprStack;

    for(const auto & summand : intermediateResults) {
        CExpression * p;
        if(summand.size() > 6)
            p = new CLongInteger(summand);
        else
            p = new CInteger(summand);
        exprStack.push(p);
    }

    CExpression * rVal;
    CExpression * lVal;
    CExpression * sum;

    while(exprStack.size() != 1) {
        rVal = exprStack.top(); exprStack.pop();
        lVal = exprStack.top(); exprStack.pop();

        sum = new CAddExp(lVal, rVal);
        exprStack.push(sum);
    }

    CBigNum product = exprStack.top()->evaluate();
    return product;
}

void CBigNum::divisionAlgorithm(const std::vector<uint32_t> &val1,
                                const std::vector<uint32_t> &val2,
                                std::string &quotient, std::string &remainder) const
{
    std::string stringDividend = toString(val1);
    std::string stringDivisor = toString(val2);
//    std::string quotient;
//    std::string remainder;
    std::stringstream ss;

    while(stringDividend[0] == '0')
        stringDividend.erase(0,1);

    while(stringDivisor[0] == '0')
        stringDivisor.erase(0,1);

    size_t len2 = stringDivisor.size();

    std::string dividendChunkString = stringDividend.substr(0, len2);
    for(size_t i = 0; i < dividendChunkString.size(); i++)
        if(dividendChunkString[i] - '0' < stringDivisor[i] - '0') {
            dividendChunkString = stringDividend.substr(0, len2 + 1);
            stringDividend.erase(0, len2 + 1);
            break;
        } else if(dividendChunkString[i] - '0' > stringDivisor[i] - '0') {
            stringDividend.erase(0, len2);
            break;
        }

    CBigNum dividendChunk = CLongInteger(dividendChunkString).evaluate();
    CBigNum divisor = CLongInteger(stringDivisor).evaluate();

    size_t wholePart;

    while(true) {
        wholePart = 0;
        while(dividendChunk >= divisor) {
            wholePart++;
            dividendChunk = dividendChunk - divisor;
        }
        ss << wholePart;
        std::string tmpQuotient;
        ss >> tmpQuotient;
        quotient += tmpQuotient;
        ss.clear();

        if(!stringDividend.empty()) {
            dividendChunk.multiplyByTen();
            dividendChunk = dividendChunk + CBigNum(stringDividend[0] - '0');
            stringDividend.erase(0,1);
        } else
            break;
    }
    remainder = toString(dividendChunk);
    if(remainder.empty())
        remainder = "0";
}

std::vector<uint32_t> CBigNum::addFractions(std::vector<uint32_t> & f1, std::vector<uint32_t> & f2, unsigned short &carry) const
{
    std::vector<uint32_t> sum;
    if(f1.size() == 1 && f1[0] == 0 && f2.size() == 1 && f2[0] == 0) {
        sum.push_back(0);
        return sum;
    }

    std::string fract1String = toString(f1);
    std::string fract2String = toString(f2);

    while(fract1String.length() > fract2String.length())
        fract2String.push_back('0');

    while(fract1String.length() < fract2String.length())
        fract1String.push_back('0');

    std::vector<uint32_t> fract1 = toBigInt(fract1String);
    std::vector<uint32_t> fract2 = toBigInt(fract2String);

    sum = additionAlgorithm(fract1, fract2, carry);
    std::string sumString = toString(sum);

    size_t fractLen = fract1String.size();
    size_t sumLen = sumString.size();

    if(sumLen > fractLen) {
        carry = 1;
        sumString.erase(0,1);
        sum = toBigInt(sumString);
    }

    return sum;
}

std::vector<uint32_t> CBigNum::subtractFractions(std::vector<uint32_t> & f1, std::vector<uint32_t> & f2, unsigned short &carry) const
{
    std::vector<uint32_t> difference;
    if(f1.size() == 1 && f1[0] == 0 && f2.size() == 1 && f2[0] == 0) {
        difference.push_back(0);
        return difference;
    }

    std::string fract1String = toString(f1);
    std::string fract2String = toString(f2);

    while(fract1String.length() > fract2String.length())
        fract2String.push_back('0');

    while(fract1String.length() < fract2String.length())
        fract1String.push_back('0');

    std::vector<uint32_t> fract1 = toBigInt(fract1String);
    std::vector<uint32_t> fract2 = toBigInt(fract2String);

    bool sgn;
    difference = subtractionAlgorithm(fract1, fract2, carry, sgn);

    if(sgn) {
        modulo(difference);
        carry = 1;
    }

    return difference;
}

void CBigNum::expandFewerNumberWithZeros(std::vector<uint32_t> &integer1, std::vector<uint32_t> &integer2) const
{
    //if lengths are different expand the shorter one with zeros
    size_t n;
    if(integer1.size() == integer2.size())
        n = integer1.size();
    else if(integer1.size() > integer2.size()) {
        n = integer2.size();
        for(size_t i = n; i < integer1.size(); i++)
            integer2.push_back(0);
    }
    else {
        n = integer1.size();
        for(size_t i = n; i < integer2.size(); i++)
            integer1.push_back(0);
    }
}

std::string CBigNum::toString(const std::vector<uint32_t> &value) const
{
    std::stringstream ss;
    for(auto it = value.rbegin(); it < value.rend(); it++)
        if(it == value.rbegin())
            ss << *it;
        else
            ss << std::setw(9) << std::setfill('0') << *it;
    std::string result;
    ss >> result;
    return result;
}

std::string CBigNum::toString(const CBigNum & number) const
{
    std::string result;
    if(number.m_sgn)
        result += "-";
    result += toString(number.m_integer);
    return result;
}

std::string CBigNum::toString(uint32_t num, bool leadingZeros) const
{
    std::stringstream ss;
    if(leadingZeros)
        ss << std::setw(9) << std::setfill('0') << num;
    else
        ss << num;

    return ss.str();
}

void CBigNum::multiplyByTen()
{
    long tmp;
    uint32_t carry = 0;

    for(size_t i = 0; i < m_integer.size(); i++) {
        tmp = (long)m_integer[i] * 10 + carry;
        carry =  tmp / 1000000000; //10 ^ 9
        m_integer[i] = (uint32_t)(tmp % 1000000000); //10 ^ 9
    }

    if(carry)
        m_integer.push_back(carry);
}

int CBigNum::eliminateEndingZeros(std::vector<uint32_t> & integer1, std::vector<uint32_t> & integer2) const
{
    int zerosCnt = 0;
    std::string integer1String = toString(integer1);
    std::string integer2String = toString(integer2);

    for(size_t i = integer1String.size() - 1; i >= 0; i--)
        if(integer1String[i] == '0') {
            zerosCnt++;
            integer1String.erase(i, 1);
        } else
            break;

    for(size_t i = integer2String.size() - 1; i >= 0; i--)
        if(integer2String[i] == '0') {
            zerosCnt++;
            integer2String.erase(i, 1);
        } else
            break;

    integer1 = toBigInt(integer1String);
    integer2 = toBigInt(integer2String);

    return zerosCnt;
}

std::vector<uint32_t> CBigNum::toBigInt(std::string number) const
{
    std::vector<uint32_t> result;
    int len = number.size();
    int portions = len / 9;
    std::stringstream ss;
    uint32_t tmp;

    for(int i = 0; i < portions; i++) {
        ss << number.substr(number.size() - 9, 9);
        ss >> tmp;
        result.push_back(tmp);
        number.erase(number.size() - 9, 9);
        ss.clear();
    }

    if(!number.empty()) {
        ss << number;
        ss >> tmp;
        result.push_back(tmp);
    }

    return result;
}

void CBigNum::modulo(std::vector<uint32_t> & num) const
{
    int carry = 0;
    std::string tmp;
    size_t len;

    for(auto & it : num) {
        tmp = toString(it, false);
        len = tmp.size();
        it = (uint32_t)pow(10, len) - it - carry;
        carry = 1;
    }
}

bool CBigNum::isZero(const std::vector<uint32_t> & val) const
{
    std::string value = toString(val);
    while(value[0] == '0') {
        value.erase(0,1);
        if(value.empty())
            return true;
    }
    return false;
}
