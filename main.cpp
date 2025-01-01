#include <iostream>

#include "dependentbyte.h"

int main()
{
    DependentByte dependentByte;
    uint8_t firstValue = 0, secondValue = 0, firstValue_ = 0, secondValue_ = 0;
    uint16_t encodedPair = 0;
    int total = 0, failed = 0;

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            total++;
            firstValue = i, secondValue = j, firstValue_ = 0, secondValue_ = 0;

            dependentByte.encode(firstValue, secondValue, encodedPair);
            dependentByte.decode(firstValue_, secondValue_, encodedPair, encodedPair % 2);

            if (secondValue != secondValue_) {
                failed++;
            }
        }
    }

    std::cout << "Success rate %" << ((total - failed) / (float)total * 100) << std::endl;

    return 0;
}
