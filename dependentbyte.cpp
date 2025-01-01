/******************************************************************************
 * File Name    : dependentbyte.cpp
 * Coder        : Aziz Gökhan NARİN
 * E-Mail       : azizgokhannarin@yahoo.com
 * Explanation  : Dependent Byte Pairs
 * Versiyon     : 1.0.0
 ******************************************************************************/

#include "dependentbyte.h"

#include <iostream>

DependentByte::DependentByte()
{

}

DependentByte::~DependentByte()
{

}

void DependentByte::encode(const uint8_t &firstValue, const uint8_t &secondValue, uint16_t &encodedPair)
{
    uint8_t X1, Y1, Z1, R1, X2, Y2, Z2, R2, S;

    findCoordinates(firstValue, X1, Y1, Z1, R1);
    findCoordinates(secondValue, X2, Y2, Z2, R2);
    selectS(X2, Z2, S);

    encodedPair &= 0xFFFF;
    encodedPair |= X1;
    encodedPair = (encodedPair << 2) | (Y1 & 0x03);
    encodedPair = (encodedPair << 2) | (Z1 & 0x03);
    encodedPair = (encodedPair << 1) | (X2 & 0x01);
    encodedPair = (encodedPair << 2) | (Y2 & 0x03);
    encodedPair = (encodedPair << 1) | (Z2 & 0x02) >> 1;
    encodedPair = (encodedPair << 1) | (S  & 0x01);
    encodedPair = (encodedPair << 2) | (R1 & 0x03);
    encodedPair = (encodedPair << 2) | (R2 & 0x03);
}

void DependentByte::decode(uint8_t &firstValue, uint8_t &secondValue, const uint16_t &encodedPair, const bool &bigger)
{
    uint8_t X1  = (encodedPair & 0x6000)   >> 13;
    uint8_t Y1  = (encodedPair & 0x1800)   >> 11;
    uint8_t Z1  = (encodedPair & 0x600)    >> 9;
    uint8_t C   = (encodedPair & 0x01E0)   >> 5;
    uint8_t S   = (encodedPair & 0x0010)   >> 4;
    uint8_t R1  = (encodedPair & 0x000C)   >> 2;
    uint8_t R2  = (encodedPair & 0x0003);
    uint8_t X2 = 0, Y2 = 0, Z2 = 0;

    findValue(firstValue, X1, Y1, Z1, R1);

    guessX2Z2(C, S, bigger);

    X2  = (finalResult & 0x30)   >> 4;
    Y2  = (finalResult & 0x0C)   >> 2;
    Z2  = (finalResult & 0x03);

    findValue(secondValue, X2, Y2, Z2, R2);
}

void DependentByte::maskValue(const uint8_t &value, uint8_t &mask, uint8_t &R)
{
    if (value < 64) {
        mask = value;
        R = 0;
    } else if (value >= 64 && value < 128) {
        mask = value - 64;
        R = 1;
    } else if (value >= 128 && value < 192) {
        mask = value - 128;
        R = 2;
    } else { // value >= 192
        mask = value - 192;
        R = 3;
    }
}

void DependentByte::unmaskValue(uint8_t &value, const uint8_t &mask, const uint8_t &R)
{
    if (mask > 63) {
        throw std::out_of_range(std::to_string(mask) + " is wrong to be a mask!");
    } else if (R > 3) {
        throw std::out_of_range(std::to_string(mask) + " is wrong to be a region!");
    } else {
        if (0 == R) {
            value = mask;
        } else if (1 == R) {
            value = mask + 64;
        } else if (2 == R) {
            value = mask + 128;
        } else { // 3 == region
            value = mask + 192;
        }
    }
}

void DependentByte::findCoordinates(const uint8_t &value, uint8_t &X, uint8_t &Y, uint8_t &Z, uint8_t &R)
{
    uint8_t mask = 0;
    maskValue(value, mask, R);

    X = mask % 4;
    Y = (mask / 4) % 4;
    Z = mask / 16;
}

void DependentByte::findValue(uint8_t &value, const uint8_t &X, const uint8_t &Y, const uint8_t &Z, const uint8_t &R)
{
    if ((X < 0) || (X > 3) || (Y < 0) || (Y > 3) || (Z < 0) || (Z > 3)) {
        throw std::out_of_range("Coordinates must be between 0 and 3.");
    } else {
        uint8_t mask = X + Y * 4 + Z * 16;
        unmaskValue(value, mask, R);
    }
}

void DependentByte::selectS(uint8_t &X2, const uint8_t &Z2, uint8_t &S)
{
    if ((X2 % 2 == 0 && Z2 % 2 == 0) || (X2 % 2 == 1 && Z2 % 2 == 1)) {
        S = 0;
    } else {
        S = 1;
    }
}

void DependentByte::guessX2Z2(const uint8_t &C, const uint8_t &S, const bool &bigger)
{
    uint8_t X2 = ((C & 0x08) >> 3), Y2 = ((C & 0x06) >> 1), Z2 = C & 0x01;

    if ((S & 0x01) == 0) {
        if (bigger) {
            if (X2 == 0 && Z2 == 0) {
                X2 = 2;
                Z2 = 0;
            } else if (X2 == 1 && Z2 == 0) {
                X2 = 3;
                Z2 = 1;
            } else if (X2 == 0 && Z2 == 1) {
                X2 = 0;
                Z2 = 2;
            } else { // X2 == 1 && Z2 == 1
                X2 = 1;
                Z2 = 3;
            }
        } else {
            if (X2 == 0 && Z2 == 0) {
                X2 = 0;
                Z2 = 0;
            } else if (X2 == 1 && Z2 == 0) {
                X2 = 1;
                Z2 = 1;
            } else if (X2 == 0 && Z2 == 1) {
                X2 = 2;
                Z2 = 2;
            } else { // X2 == 1 && Z2 == 1
                X2 = 3;
                Z2 = 3;
            }
        }
    } else {
        if (bigger) {
            if (X2 == 0 && Z2 == 0) {
                X2 = 2;
                Z2 = 1;
            } else if (X2 == 1 && Z2 == 0) {
                X2 = 3;
                Z2 = 0;
            } else if (X2 == 0 && Z2 == 1) {
                X2 = 2;
                Z2 = 3;
            } else { // X2 == 1 && Z2 == 1
                X2 = 3;
                Z2 = 2;
            }
        } else {
            if (X2 == 0 && Z2 == 0) {
                X2 = 0;
                Z2 = 1;
            } else if (X2 == 1 && Z2 == 0) {
                X2 = 1;
                Z2 = 0;
            } else if (X2 == 0 && Z2 == 1) {
                X2 = 0;
                Z2 = 3;
            } else { // X2 == 1 && Z2 == 1
                X2 = 1;
                Z2 = 2;
            }
        }
    }

    setResult(X2, Y2, Z2);
}

void DependentByte::setResult(const uint8_t X2, const uint8_t Y2, const uint8_t Z2)
{
    uint8_t result = 0x00;
    result |= (X2 & 0x03);
    result = (result << 2) | (Y2 & 0x03);
    result = (result << 2) | (Z2 & 0x03);
    finalResult = result;
}
