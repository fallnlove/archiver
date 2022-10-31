#include "decode.h"

const int FILENAME_END = 256;
const int ONE_MORE_FILE = 257;
const int ARCHIVE_END = 258;
const size_t MAX_CHAR_NUM = 259;

Decode::Decode(const std::string& archive_name) : stream_(archive_name, std::ios::binary), input_(stream_) {
}

void Decode::StartDecoding() {
    bool is_end = false;
    while (!is_end) {
        is_end = DecodeFile();
    }
}

bool Decode::DecodeFile() {
    std::vector<CharToBinaryCode> codes;
    GetCodes(codes);
    BinaryTree<int16_t, CharToBinaryCode> bor(codes);
    bor.MakeRoot(std::make_shared<BinaryTree<int16_t, CharToBinaryCode>::Node>());
    bor.BuildTree();
    std::string file_name;
    // decode file name
    int16_t character = DecodeChar(bor);
    while (character != FILENAME_END) {
        file_name += static_cast<char>(character);
        character = DecodeChar(bor);
    }
    std::ofstream stream(file_name, std::ios::binary);
    BitOutput output(stream, file_name);
    file_name_ = file_name;
    // decode file
    character = DecodeChar(bor);
    while (character != ONE_MORE_FILE && character != ARCHIVE_END) {
        output.PrintChar(character);
        character = DecodeChar(bor);
    }
    if (character == ONE_MORE_FILE) {
        return false;
    }
    return true;
}

int16_t Decode::DecodeChar(BinaryTree<int16_t, CharToBinaryCode>& bor) {
    std::weak_ptr<BinaryTree<int16_t, CharToBinaryCode>::Node> node = bor.GetRoot();
    bool node_is_char = false;
    while (!node_is_char) {
        if (!node.lock()) {
            throw IncorrectFile(file_name_);
        }
        node_is_char = bor.DfsToNode(input_.ReadBit(), node);
    }
    return node.lock()->val;
}

void Decode::GetCodes(std::vector<CharToBinaryCode>& codes) {
    std::vector<SizeCodeOfChar> length_encoded_chars;
    GetDecodeInformation(length_encoded_chars);  // get information important to decode
    std::vector<BinaryCode> all_codes(MAX_CHAR_NUM);
    ConvertToCanonicalCode(length_encoded_chars, all_codes, true);  // converting to canonical code
    for (const auto& [character, _] : length_encoded_chars) {
        codes.push_back({character, all_codes[character].Reverse()});
    }
}

void Decode::GetDecodeInformation(std::vector<SizeCodeOfChar>& length_encoded_chars) {
    size_t symbols_count = input_.ReadNineBits().CastToSizeT();  // get alphabet size
    for (size_t i = 0; i < symbols_count; ++i) {
        length_encoded_chars.push_back({input_.ReadNineBits().CastToInt(), 0});  // get symbols
    }
    size_t symbols_viewed = 0;
    for (size_t i = 1; i < MAX_CHAR_NUM; ++i) {  // get symbols length code
        if (symbols_viewed == symbols_count) {
            break;
        }
        size_t num_chars_same_len = input_.ReadNineBits().CastToSizeT();
        for (size_t j = 0; j < num_chars_same_len; ++j) {
            length_encoded_chars[symbols_viewed].size = i;
            ++symbols_viewed;
        }
    }
}
