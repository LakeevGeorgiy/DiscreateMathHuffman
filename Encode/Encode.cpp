#include "Encode.h"

#include <bitset>

void ReadFromFile(const std::string& path, std::map<char, std::vector<std::pair<char, uint32_t>>>& symbols) {
    std::ifstream in(path);

    std::vector<std::vector<uint32_t>> used(256, std::vector<uint32_t>(256));
    char symbol;
    char last_symbol;
    in.get(last_symbol);

    while (in.get(symbol)) {
        used[(int) last_symbol][(int) symbol]++;
        last_symbol = symbol;
    }

    for (int i = 0; i < 256; i++) {
        std::vector<std::pair<char, uint32_t>> letters;
        for (int j = 0; j < 256; j++) {
            if (used[i][j] == 0)
                continue;
            letters.emplace_back((char) j, used[i][j]);
        }
        if (!letters.empty())
            symbols.emplace((char)i, letters);
    }
}

void HuffmanCodes(MinHeadNode* head, std::string code, std::map<char, std::string>& huffman_table){

    if (head == nullptr){
        return;
    }

    if (head->symbol != '~') {
        huffman_table.emplace(head->symbol, code);
        return;
    }

    HuffmanCodes(head->left, code + "0", huffman_table);
    HuffmanCodes(head->right, code + "1", huffman_table);

}

void BuildHuffmanCodes(const std::vector<std::pair<char, uint32_t>>& symbols,
                       std::map<char, std::string>& huffman_table){

    std::priority_queue<MinHeadNode*, std::vector<MinHeadNode*>, compare> min_heap;

    MinHeadNode* left;
    MinHeadNode* right;

    for (const auto& symbol : symbols){
        min_heap.push(new MinHeadNode(symbol.first, symbol.second));
    }

    if (min_heap.size() == 1){
        huffman_table.emplace(symbols[0].first, "1");
        return;
    }

    while (min_heap.size() > 1){
        left = min_heap.top();
        min_heap.pop();

        right = min_heap.top();
        min_heap.pop();

        MinHeadNode* new_node = new MinHeadNode('~', left->freq + right->freq);
        new_node->left = left;
        new_node->right = right;

        min_heap.push(new_node);

    }

    HuffmanCodes(min_heap.top(), "", huffman_table);
}

uint32_t CountLetters(const std::string& path, uint32_t tellg){
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    uint32_t end_of_file = file.tellg();
    uint32_t number_of_letters = end_of_file - tellg;

    return number_of_letters;
}

void WriteTable(std::ofstream& encode_out,
                std::map<char, std::vector<std::pair<char, uint32_t>>>& symbols){

    uint32_t size_of_table = symbols.size();
    encode_out.write((char*)&size_of_table, 4);

    for (auto& symbol : symbols){

        char last_symbol = symbol.first;
        uint32_t size = symbol.second.size();

        encode_out.write((char*)&last_symbol, 1);
        encode_out.write((char*)&size, 4);

        //std::cout << "last symbol: " << last_symbol << "\n";
        //std::cout << "size: " << size << "\n";

        for (auto& character : symbol.second){
            encode_out.write((char*)&character.first, 1);
            encode_out.write((char*)&character.second, 4);
           // std::cout << "character: " << character.first << " freq: " << character.second << "\n";
        }
    }
}

void WriteToFile(const std::string& source_path, std::ofstream& out, std::map<char, std::map<char, std::string>>& huffman_table) {

    std::ifstream in(source_path, std::ios::binary);

    char symbol;
    uint8_t last_symbol;
    int i = 7;
    uint8_t code = 0;

    int cnt = 0;

    in.read((char*)&last_symbol, 1);
    out.write((char*)&last_symbol, 1);

    while (in.read((char*)&symbol, 1)) {


        for (int j = 0; j < huffman_table[last_symbol][symbol].size(); j++){
            if (i == -1){
                i = 7;
                cnt++;
                out.write((char*)&code,1);
                code = 0;
            }
            if (huffman_table[last_symbol][symbol][j] == '0'){
                i--;
                continue;
            }
            uint8_t bit = 1 << (i);
            code += bit;
            i--;
        }
        last_symbol = symbol;
    }
    out.write((char*)&code, 1);
    std::cout << cnt << "\n";
}