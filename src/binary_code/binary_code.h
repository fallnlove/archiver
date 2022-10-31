#pragma once

#include <fstream>
#include <vector>

class BinaryCode {
public:
    BinaryCode();

    explicit BinaryCode(bool code);

    explicit BinaryCode(size_t num);

    explicit BinaryCode(int16_t num);

    explicit BinaryCode(std::vector<bool> code);

    BinaryCode& operator+=(const size_t num);

    BinaryCode& operator<<=(const size_t num);

    bool Empty();

    size_t Size();

    size_t CastToSizeT();

    int16_t CastToInt();

    bool At(size_t position) const;

    BinaryCode ToNine();

    BinaryCode Reverse();

private:
    std::vector<bool> code_;
};
