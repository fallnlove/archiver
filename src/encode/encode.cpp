#include "encode.h"

const int FILENAME_END = 256;
const int ONE_MORE_FILE = 257;
const int ARCHIVE_END = 258;
const size_t MAX_CHAR_NUM = 259;

Encode::Encode(std::string archive_name, std::vector<std::string> files)
    : stream_(archive_name, std::ios::binary), output_(stream_, archive_name), files_(files) {
}

void Encode::StartEncoding() {
    bool is_last_file = false;
    for (size_t i = 0; i < files_.size(); ++i) {
        if (i + 1 == files_.size()) {
            is_last_file = true;
        }
        file_name_ = files_[i];
        EncodeFile(is_last_file);
    }
}

void Encode::EncodeFile(bool is_end) {
    std::vector<BinaryCode> codes(MAX_CHAR_NUM);
    std::vector<SizeCodeOfChar> chars_code_length;
    EncodeAlphabet(codes, chars_code_length);
    // print code to file
    std::ifstream stream(file_name_, std::ios::binary);
    BitInput input(stream, file_name_);
    // start printing information that help decode
    output_.PrintBits(BinaryCode{chars_code_length.size()}.ToNine().Reverse());
    for (size_t i = 0; i < chars_code_length.size(); ++i) {
        output_.PrintBits(BinaryCode{chars_code_length[i].character}.ToNine().Reverse());
    }
    // print MAX_SYMBOL_CODE_SIZE
    std::vector<size_t> max_symbol_code_size(MAX_CHAR_NUM + 1, 0);
    for (const SizeCodeOfChar& char_len : chars_code_length) {
        ++max_symbol_code_size[char_len.size];
    }
    while (!max_symbol_code_size.empty() && max_symbol_code_size[max_symbol_code_size.size() - 1] == 0) {
        max_symbol_code_size.pop_back();
    }
    for (size_t i = 1; i < max_symbol_code_size.size(); ++i) {
        output_.PrintBits(BinaryCode{max_symbol_code_size[i]}.ToNine().Reverse());
    }
    // print file name
    for (const char& character : file_name_) {
        output_.PrintBits(codes[character].Reverse());
    }
    output_.PrintBits(codes[FILENAME_END].Reverse());
    // print encoded file
    unsigned char input_char = 0;
    while (!input.IsEndFile()) {
        input_char = input.ReadChar();
        output_.PrintBits(codes[input_char].Reverse());
    }
    if (is_end) {
        output_.PrintBits(codes[ARCHIVE_END].Reverse());
    } else {
        output_.PrintBits(codes[ONE_MORE_FILE].Reverse());
    }
}

void Encode::EncodeAlphabet(std::vector<BinaryCode>& codes, std::vector<SizeCodeOfChar>& chars_code_length) {
    std::vector<size_t> code_char_length(MAX_CHAR_NUM, 0);
    HuffmanAlgorithm(code_char_length);
    for (size_t i = 0; i < MAX_CHAR_NUM; ++i) {  // generate vector with length of bits of each char
        if (code_char_length[i] > 0) {
            chars_code_length.push_back({static_cast<int16_t>(i), code_char_length[i]});
        }
    }
    ConvertToCanonicalCode(chars_code_length, codes, false);
}

void Encode::HuffmanAlgorithm(std::vector<size_t>& code_char_length) {
    PriorityQueue<CharBlock> priority_queue;
    InitQueue(priority_queue);
    BinaryTree<int16_t> bor(code_char_length);
    while (priority_queue.Size() > 1) {
        CharBlock first = priority_queue.Pop();
        CharBlock second = priority_queue.Pop();
        if (first.node == nullptr) {
            first.node = std::make_shared<BinaryTree<int16_t>::Node>(nullptr);
            first.node->val = first.chr;
        }
        if (second.node == nullptr) {
            second.node = std::make_shared<BinaryTree<int16_t>::Node>(nullptr);
            second.node->val = second.chr;
        }
        auto parent = std::make_shared<BinaryTree<int16_t>::Node>(first.node, second.node, 0);
        priority_queue.Push({std::min(first.chr, second.chr), first.count + second.count, parent});
        bor.Merge(parent, first.node, second.node);
    }
    bor.MakeRoot(priority_queue.Pop().node);
    bor.GetAllLeaves();  // get code length of chars
}

void Encode::InitQueue(PriorityQueue<Encode::CharBlock>& priority_queue) {
    std::vector<size_t> chars_frequency(MAX_CHAR_NUM, 0);
    CountCharsFrequency(chars_frequency);                                // count chars in file
    for (size_t character = 0; character < MAX_CHAR_NUM; ++character) {  // initialise priority queue
        if (chars_frequency[character] > 0) {                            // we should ignore void characters
            priority_queue.Push(CharBlock(static_cast<int16_t>(character), chars_frequency[character]));
        }
    }
}

void Encode::CountCharsFrequency(std::vector<size_t>& char_frequency) {
    std::ifstream stream(file_name_, std::ios::binary);
    BitInput input(stream, file_name_);
    for (const char& character : file_name_) {  // count letters in file name
        ++char_frequency[character];
    }
    while (!input.IsEndFile()) {  // count letters in file
        unsigned char t = input.ReadChar();
        ++char_frequency[t];
    }
    char_frequency[FILENAME_END] = 1;  // count additional(system) symbols
    char_frequency[ONE_MORE_FILE] = 1;
    char_frequency[ARCHIVE_END] = 1;
}
