#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../binary_code/binary_code.h"
#include "../binary_tree.h"
#include "../bit_input_output.h"
#include "../canonical_code/canonical_code.h"

class Decode {
public:
    explicit Decode(const std::string& archive_name);

    void StartDecoding();

private:
    std::ifstream stream_;
    BitInput<> input_;
    std::string file_name_;

    void GetDecodeInformation(std::vector<SizeCodeOfChar>& length_encoded_chars);

    void GetCodes(std::vector<CharToBinaryCode>& codes);

    bool DecodeFile();

    int16_t DecodeChar(BinaryTree<int16_t, CharToBinaryCode>& bor);
};
