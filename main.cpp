#include "Encode/Encode.h"
#include "Decode/Decode.h"

#include <fstream>

int main() {

    /*std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);*/

    std::string source_path = "C:\\Users\\lakee\\CLionProjects\\DiscreateMath\\Harry Potter discreate.txt";
    std::string encode_path = "C:\\Users\\lakee\\CLionProjects\\DiscreateMath\\~Harry Potter encode.txt";
    std::string decode_path = "C:\\Users\\lakee\\CLionProjects\\DiscreateMath\\~Harry Potter decode.txt";

    std::vector<std::pair<char, uint32_t>> symbols;
    ReadFromFile(source_path, symbols);

    std::map<char, std::string> huffman_table;
    BuildHuffmanCodes(symbols, huffman_table);

    std::cout << "huffman table begin: \n";
    for (auto now : huffman_table){
        std::cout << now.first << " " << now.second << "\n";
    }
    std::cout << "huffman table end.\n";

    WriteToFile(source_path, encode_path, huffman_table, symbols);

    std::ifstream input_encode(encode_path, std::ios::binary);
    std::map<std::string, char> code;

    RebuildHuffmanTable(code, input_encode);

    WriteSourceFile(decode_path, code, input_encode);

    return 0;
}