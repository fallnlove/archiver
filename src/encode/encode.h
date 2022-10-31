#pragma once

#include "../binary_code/binary_code.h"
#include "../binary_tree.h"
#include "../bit_input_output.h"
#include "../canonical_code/canonical_code.h"
#include "../priority_queue.h"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

class Encode {
public:
    struct CharBlock {
        int16_t chr = 0;
        size_t count = 0;
        std::shared_ptr<BinaryTree<int16_t>::Node> node = nullptr;

        CharBlock(int16_t chr, size_t count, std::shared_ptr<BinaryTree<int16_t>::Node> node = nullptr)
            : chr(chr), count(count), node(node) {
        }

        bool operator<(const CharBlock& other) const {
            return std::tie(count, chr) < std::tie(other.count, other.chr);
        }
    };

    Encode(std::string archive_name, std::vector<std::string> files);

    void StartEncoding();

private:
    std::ofstream stream_;
    BitOutput<> output_;
    std::vector<std::string> files_;
    std::string file_name_;

    void EncodeFile(bool is_end);

    void EncodeAlphabet(std::vector<BinaryCode>& codes, std::vector<SizeCodeOfChar>& chars_code_length);

    void HuffmanAlgorithm(std::vector<size_t>& code_char_length);

    void InitQueue(PriorityQueue<CharBlock>& priority_queue);

    void CountCharsFrequency(std::vector<size_t>& char_frequency);
};
