#pragma once

#include <algorithm>
#include <vector>

#include "../binary_code/binary_code.h"

struct SizeCodeOfChar {
    int16_t character;
    size_t size;
};

void ConvertToCanonicalCode(std::vector<SizeCodeOfChar>& chars, std::vector<BinaryCode>& codes, bool is_sorted = false);
