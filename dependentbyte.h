/******************************************************************************
 * File Name    : dependentbyte.h
 * Coder        : Aziz Gökhan NARİN
 * E-Mail       : azizgokhannarin@yahoo.com
 * Explanation  : Dependent Byte Pairs
 * Versiyon     : 1.0.0
 ******************************************************************************/

#ifndef DEPENDENTBYTE_H
#define DEPENDENTBYTE_H

#include <cinttypes>

class DependentByte
{
public:
    explicit DependentByte();
    virtual ~DependentByte();

    void encode(const uint8_t &firstValue, const uint8_t &secondValue, uint16_t &encodedPair);
    void decode(uint8_t &firstValue, uint8_t &secondValue, const uint16_t &encodedPair, const bool &bigger);

protected:
    void maskValue(const uint8_t &value, uint8_t &mask, uint8_t &R);
    void unmaskValue(uint8_t &value, const uint8_t &mask, const uint8_t &R);
    void findCoordinates(const uint8_t &value, uint8_t &X, uint8_t &Y, uint8_t &Z, uint8_t &R);
    void findValue(uint8_t &value, const uint8_t &X, const uint8_t &Y, const uint8_t &Z, const uint8_t &R);

private:
    void selectS(uint8_t &X2, const uint8_t &Z2, uint8_t &S);
    void guessX2Z2(const uint8_t &C, const uint8_t &S, const bool &bigger);
    void setResult(const uint8_t X2, const uint8_t Y2, const uint8_t Z2);

    uint8_t finalResult;
};

#endif // DEPENDENTBYTE_H
