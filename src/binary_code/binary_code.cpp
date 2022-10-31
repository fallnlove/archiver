#include "binary_code.h"

const int MIN_NINE_CODE_SIZE = 9;

BinaryCode::BinaryCode() {
}

BinaryCode::BinaryCode(bool code) {
    code_.push_back(code);
}

BinaryCode::BinaryCode(std::vector<bool> code) : code_(code) {
}

bool BinaryCode::Empty() {
    return code_.empty();
}

size_t BinaryCode::Size() {
    return code_.size();
}

BinaryCode& BinaryCode::operator<<=(const size_t num) {
    std::reverse(code_.begin(), code_.end());
    for (size_t i = 0; i < num; ++i) {
        code_.push_back(false);
    }
    std::reverse(code_.begin(), code_.end());
    return *this;
}

BinaryCode& BinaryCode::operator+=(const size_t num) {  // this function works only for num = 1
    bool next_bit = true;
    for (size_t i = 0; i < code_.size(); ++i) {
        if (next_bit) {
            if (!code_[i]) {
                next_bit = false;
            }
            code_[i] = !code_[i];
        } else {
            break;
        }
    }
    if (next_bit) {
        code_.push_back(true);
    }
    return *this;
}

BinaryCode::BinaryCode(size_t num) {
    while (num > 0) {
        code_.push_back(num % 2);
        num /= 2;
    }
}

BinaryCode::BinaryCode(int16_t num) {
    while (num > 0) {
        code_.push_back(num % 2);
        num /= 2;
    }
}

size_t BinaryCode::CastToSizeT() {
    std::reverse(code_.begin(), code_.end());
    size_t num = 0;
    size_t pow_two = 1;
    for (const auto& number : code_) {
        if (number) {
            num += pow_two;
        }
        pow_two *= 2;
    }
    std::reverse(code_.begin(), code_.end());
    return num;
}

int16_t BinaryCode::CastToInt() {
    std::reverse(code_.begin(), code_.end());
    int16_t num = 0;
    int16_t pow_two = 1;
    for (const auto& number : code_) {
        if (number) {
            num += pow_two;
        }
        pow_two *= 2;
    }
    std::reverse(code_.begin(), code_.end());
    return num;
}

bool BinaryCode::At(size_t position) const {
    return code_.at(position);
}

BinaryCode BinaryCode::ToNine() {
    while (MIN_NINE_CODE_SIZE > code_.size()) {
        code_.push_back(false);
    }
    return *this;
}

BinaryCode BinaryCode::Reverse() {
    BinaryCode now(*this);
    std::reverse(now.code_.begin(), now.code_.end());
    return now;
}
