#pragma once

#include <array>
#include <bitset>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

#include "binary_code/binary_code.h"
#include "exceptions.h"

const int MAX_BUFFER_SIZE = 1024;

template <typename Stream = std::basic_ifstream<char>>
class BitInput {
public:
    BitInput() = delete;

    explicit BitInput(Stream& stream, const std::string& file_name = "")
        : bit_in_(stream), file_name_(file_name), first_buffer_element_(0), last_buffer_element_(0) {
        bit_in_.unsetf(std::ios_base::skipws);
    }

    BinaryCode ReadNineBits() {
        std::vector<bool> input;
        for (size_t i = 0; i < 9; ++i) {
            input.push_back(ReadBit());
        }
        return BinaryCode(input);
    }

    bool ReadBit() {
        if (IsEndFile()) {
            throw IncorrectArchive(file_name_);
        }
        bool bit = char_buffer_[first_buffer_element_ / 8] & (1 << bits_reversed_[first_buffer_element_ % 8]);
        ++first_buffer_element_;
        return bit;
    }

    bool IsEndFile() {
        if (first_buffer_element_ == last_buffer_element_) {
            bit_in_.read(char_buffer_, MAX_BUFFER_SIZE);
            last_buffer_element_ = bit_in_.gcount() * 8;
            first_buffer_element_ = 0;
        }
        return 0 == last_buffer_element_;
    }

    unsigned char ReadChar() {
        unsigned char character = char_buffer_[first_buffer_element_ / 8];
        first_buffer_element_ += 8;
        return character;
    }

private:
    Stream& bit_in_;
    std::string file_name_;
    char char_buffer_[MAX_BUFFER_SIZE];
    size_t first_buffer_element_;
    size_t last_buffer_element_;
    const std::array<int, 8> bits_reversed_ = {7, 6, 5, 4, 3, 2, 1, 0};
};

template <typename Stream = std::basic_ofstream<char>>
class BitOutput {
public:
    BitOutput() = delete;

    explicit BitOutput(Stream& stream, const std::string& file_name = "")
        : bit_out_(stream), file_name_(file_name), first_buffer_element_(0) {
        bit_out_.unsetf(std::ios_base::skipws);
        Clear();
    }

    ~BitOutput() {
        if (first_buffer_element_ != 0) {
            bit_out_.write(char_buffer_, first_buffer_element_ / 8 + (first_buffer_element_ % 8 ? 1 : 0));
        }
    }

    void PrintBits(BinaryCode bit) {
        for (size_t i = 0; i < bit.Size(); ++i) {
            PrintBit(bit.At(i));
        }
    }

    void PrintChar(unsigned char character) {
        Check();
        char_buffer_[first_buffer_element_ / 8] = static_cast<char>(character);
        first_buffer_element_ += 8;
    }

private:
    Stream& bit_out_;
    std::string file_name_;
    char char_buffer_[MAX_BUFFER_SIZE];
    size_t first_buffer_element_;
    const std::array<int, 8> bits_reversed_ = {7, 6, 5, 4, 3, 2, 1, 0};

    void PrintBit(bool bit) {
        Check();
        char_buffer_[first_buffer_element_ / 8] |= (bit << bits_reversed_[first_buffer_element_ % 8]);
        ++first_buffer_element_;
    }

    void Check() {
        if (first_buffer_element_ == MAX_BUFFER_SIZE * 8) {
            bit_out_.write(char_buffer_, MAX_BUFFER_SIZE);
            first_buffer_element_ = 0;
            Clear();
        }
    }

    void Clear() {
        for (size_t i = 0; i < MAX_BUFFER_SIZE; ++i) {
            char_buffer_[i] = '\0';
        }
    }
};
