#include "Decode.h"

void RebuildHuffmanTable(std::map<char, std::map<std::string, char>>& code, std::ifstream& input_encode) {

    std::map<char, std::map<char, std::string>> huffman_table;
    std::map<char, std::vector<std::pair<char, uint32_t>>> symbols;

    uint32_t size_of_table;
    input_encode.read((char*)&size_of_table, 4);

    for (int i = 0; i < size_of_table; i++) {

        char last_symbol;
        uint32_t size;
        input_encode.read((char *) &last_symbol, 1);
        input_encode.read((char *) &size, 4);

        std::vector<std::pair<char, uint32_t>> table;
        for (int j = 0; j < size; j++) {
            char symbol;
            uint32_t freq;
            input_encode.read((char *) &symbol, 1);
            input_encode.read((char *) &freq, 4);
            table.emplace_back(symbol, freq);
        }
        symbols.emplace(last_symbol, table);
    }

    for (auto& it : symbols){
        std::map<char, std::string> table;
        BuildHuffmanCodes(it.second, table);
        huffman_table.emplace(it.first, table);
    }

    for (auto& it : huffman_table){
        std::map<std::string, char> temp;
        for (auto& itr : it.second)
            temp.emplace(itr.second, itr.first);
        code.emplace(it.first, temp);
    }

    /*std::cout << "Rebuild\n";
    for (auto& now : code){
        std::cout << "last symbol: " << now.first << "\n";
        for (auto& cur : now.second){
            std::cout << "\tsymbol: " << cur.second << " code: " << cur.first << "\n";
        }
        std::cout << "\n";
    }*/
}

void WriteSourceFile(const std::string& decode_path, std::map<char, std::map<std::string, char>>& code, std::ifstream& input_encode) {

    std::ofstream out(decode_path, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);

    std::string word;
    uint8_t symbol;
    uint8_t last_symbol;

    input_encode.read((char*)&last_symbol, 1);
    out.write((char*)&last_symbol, 1);
    int cnt = 0;

    while (input_encode.read((char*)&symbol, 1)) {
        cnt++;
        for (int i = 7; i >= 0; i--){

            if (code[last_symbol].find(word) != code[last_symbol].end()){
                char letter = code[last_symbol].find(word)->second;
                out.write((char*)&letter, 1);
                word = "";
                last_symbol = letter;
            }

            if ((symbol & (1 << i)) == 0)
                word += '0';
            else
                word += '1';

        }
    }
    std::cout << cnt << "\n";
}