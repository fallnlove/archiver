#include "canonical_code.h"

bool CompareChars(const SizeCodeOfChar& lhs, const SizeCodeOfChar& rhs) {
    return std::tie(lhs.size, lhs.character) < std::tie(rhs.size, rhs.character);
}

void ConvertToCanonicalCode(std::vector<SizeCodeOfChar>& chars, std::vector<BinaryCode>& codes, bool is_sorted) {
    BinaryCode code;
    if (!is_sorted) {
        std::sort(chars.begin(), chars.end(), CompareChars);
    }
    for (const auto& [character, size] : chars) {
        if (!code.Empty()) {
            code += 1;
        }
        if (size > code.Size()) {
            code <<= (size - code.Size());
        }
        codes[character] = code;
    }
}
