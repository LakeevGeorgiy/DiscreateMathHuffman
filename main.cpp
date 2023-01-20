#include "Encode/Encode.h"
#include "Decode/Decode.h"

#include <fstream>

int main() {

    std::string source_path = "C:\\Users\\lakee\\CLionProjects\\DiscreateMath\\Harry Potter discreate.txt";
    std::string encode_path = "C:\\Users\\lakee\\CLionProjects\\DiscreateMath\\~Harry Potter encode.txt";
    std::string decode_path = "C:\\Users\\lakee\\CLionProjects\\DiscreateMath\\~Harry Potter decode.txt";

    char command;
    std::cout << "Choose what you want to do: encode file(e), decode file(d)? (e // d) ";
    std::cin >> command;

    if (command == 'e') {

        std::cout << "What file you want to encode? (enter the full path to the file): \n";
        std::cin.ignore(1);
        std::getline(std::cin, source_path);
        std::cout << "Where to save encode file? (enter the full path to the file): \n";
        //std::cin.ignore(1);
        std::getline(std::cin, encode_path);

        std::map<char, std::vector<std::pair<char, uint32_t>>> symbols;
        ReadFromFile(source_path, symbols);

        std::map<char, std::map<char, std::string>> huffman_table;
        for (auto &code: symbols) {
            std::map<char, std::string> table;
            BuildHuffmanCodes(code.second, table);
            huffman_table.emplace(code.first, table);
        }

        std::ofstream encode_out(encode_path, std::ios::binary);
        WriteTable(encode_out, symbols);
        WriteToFile(source_path, encode_out, huffman_table);
        encode_out.close();
    } else if (command == 'd') {

        std::cout << "What file you want to decode? (enter the full path to the file)\n";
        std::cin.ignore(1);
        std::getline(std::cin, encode_path);
        std::cout << "Where to save decode file? (enter the full path to the file)\n";
        //std::cin.ignore(1);
        std::getline(std::cin, decode_path);

        std::ifstream input_encode(encode_path, std::ios_base::in | std::ios_base::binary);
        std::map<char, std::map<std::string, char>> code;

        RebuildHuffmanTable(code, input_encode);

        WriteSourceFile(decode_path, code, input_encode);

    } else {
        std::cerr << "Incorrect input!!!\n";
    }

    return 0;
}