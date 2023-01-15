#include "Decode.h"

void RebuildHuffmanTable(std::map<std::string, char>& code, std::ifstream& input_encode) {

    uint32_t size_of_table;
    input_encode >> size_of_table;
    input_encode.ignore(1);

    char symbol;
    for (int i = 0; i < size_of_table; i++) {
        std::string str;
        input_encode.get(symbol);
        input_encode >> str;
        code.emplace(str, symbol);
        input_encode.ignore(1);
    }

}

void WriteSourceFile(const std::string& decode_path, std::map<std::string, char>& code, std::ifstream& input_encode) {

    std::ofstream out(decode_path);

    std::string file;
    input_encode >> file;

    std::string word;
    for (int i = 0; i < file.size(); i++) {
        word += file[i];
        if (code.find(word) != code.end()) {
            out << code.find(word)->second;
            word = "";
        }
    }
}