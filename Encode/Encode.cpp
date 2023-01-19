#include "Encode.h"

#include <bitset>

void ReadFromFile(const std::string& path, std::vector<std::pair<char, uint32_t>>& symbols){
    std::ifstream in(path);

    std::vector <uint32_t> used(256);
    char symbol;

    while (in.get(symbol)){
        used[(int)symbol]++;
    }

    for (int i = 0; i < 128; i++){
        if (used[i] == 0)
            continue;
        symbols.emplace_back((char)i, used[i]);
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

void WriteToFile(const std::string& source_path, const std::string& encode_path, std::map<char, std::string>& huffman_table,
                 std::vector<std::pair<char, uint32_t>>& symbols) {

    std::ifstream in(source_path);
    std::ofstream out(encode_path, std::ios::binary);

    size_t size = symbols.size();
    out.write((char*)&size, 4);
    std::cout << "size: " << size << "\n";

    for (auto& symbol : symbols){
        out.write((char*)&symbol.first, 1);
        out.write((char*)&symbol.second, 4);
        std::cout << "symbol: " << symbol.first << " freq: " << symbol.second << "\n";
    }

    char symbol;
    int i = 7;
    uint8_t code = 0;

    int cnt = 0;

    while (in.read((char*)&symbol, 1)) {
        //std::cout << ++cnt << "\n";

        for (int j = 0; j < huffman_table[symbol].size(); j++){
            if (i == -1){
                i = 7;
                out.write((char*)&code,1);
                code = 0;
            }
            if (huffman_table[symbol][j] == '0'){
                i--;
                continue;
            }
            uint8_t bit = 1 << (i);
            code += bit;
            i--;
        }
    }
    out.write((char*)&code, 1);
}