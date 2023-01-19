#include "Decode.h"

void RebuildHuffmanTable(std::map<std::string, char>& code, std::ifstream& input_encode) {

    std::map<char, std::string> huffman_table;
    std::vector<std::pair<char, uint32_t>> symbols;

    uint32_t size_of_table;
    input_encode.read((char*)&size_of_table, 4);

    for (int i = 0; i < size_of_table; i++) {
        char symbol;
        uint32_t freq;
        input_encode.read((char*)&symbol, 1);
        input_encode.read((char*)&freq, 4);
        symbols.emplace_back(symbol, freq);
    }

    BuildHuffmanCodes(symbols, huffman_table);
    for (auto& symbol : huffman_table){
        code.emplace(symbol.second, symbol.first);
    }
}

void WriteSourceFile(const std::string& decode_path, std::map<std::string, char>& code, std::ifstream& input_encode) {

    std::ofstream out(decode_path, std::ios::binary);

    std::string word;
    uint8_t symbol;
    int cnt = 0;

    while (input_encode.read((char*)&symbol, 1)) {
        for (int i = 7; i >= 0; i--){

            if (code.find(word) != code.end()){
                char letter = code.find(word)->second;
                cnt++;
                out.write((char*)&letter, 1);
                word = "";
            }

            if ((symbol & (1 << i)) == 0)
                word += '0';
            else
                word += '1';

        }
    }
}