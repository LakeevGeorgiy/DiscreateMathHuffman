#include "Encode/Encode.h"
#include "Decode/Decode.h"

#include <fstream>

int main() {

    std::string source_path = "C:\\Users\\lakee\\CLionProjects\\DiscreateMath\\Harry Potter discreate.txt";
    std::string encode_path = "C:\\Users\\lakee\\CLionProjects\\DiscreateMath\\~Harry Potter encode.txt";
    std::string decode_path = "C:\\Users\\lakee\\CLionProjects\\DiscreateMath\\~Harry Potter decode.txt";

    std::map<char, std::vector<std::pair<char, uint32_t>>> symbols;
    ReadFromFile(source_path, symbols);

    std::map<char, std::map<char, std::string>> huffman_table;
    for (auto& code : symbols){
        std::map<char, std::string> table;
        BuildHuffmanCodes(code.second, table);
        huffman_table.emplace(code.first, table);
    }

    /*for (auto& now : huffman_table){
        std::cout << "last symbol: " << now.first << "\n";
        for (auto& cur : now.second){
            std::cout << "\tsymbol: " << cur.first << " code: " << cur.second << "\n";
        }
        std::cout << "\n";
    }*/

    std::ofstream encode_out(encode_path, std::ios::binary);
    WriteTable(encode_out, symbols);
    WriteToFile(source_path, encode_out, huffman_table);
    encode_out.close();

    std::ifstream input_encode(encode_path, std::ios_base::in | std::ios_base::binary);
    std::map<char, std::map<std::string, char>> code;

    RebuildHuffmanTable(code, input_encode);

    WriteSourceFile(decode_path, code, input_encode);

    return 0;
}