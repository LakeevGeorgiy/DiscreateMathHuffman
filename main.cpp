#include "Encode/Encode.h"
#include "Decode/Decode.h"

#include <fstream>

int main() {

    std::string source_path = "C:\\Users\\lakee\\CLionProjects\\DiscreateMath\\Harry Potter discreate.txt";
    std::string encode_path = "C:\\Users\\lakee\\CLionProjects\\DiscreateMath\\~Harry Potter encode.txt";
    std::string decode_path = "C:\\Users\\lakee\\CLionProjects\\DiscreateMath\\~Harry Potter decode.txt";

    std::vector<std::pair<char, uint32_t>> symbols;
    ReadFromFile(source_path, symbols);

    std::map<char, std::string> huffman_table;
    BuildHuffmanCodes(symbols, huffman_table);

    WriteToFile(source_path, encode_path, huffman_table, symbols);

    std::ifstream input_encode(encode_path, std::ios::binary);
    std::map<std::string, char> code;

    RebuildHuffmanTable(code, input_encode);

    WriteSourceFile(decode_path, code, input_encode);

    return 0;
}