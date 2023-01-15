#include <iostream>
#include <queue>
#include <fstream>
#include <cassert>
#include <bitset>
#include <map>

struct MinHeadNode{
    char symbol;
    uint32_t freq;
    MinHeadNode* left;
    MinHeadNode* right;
    MinHeadNode(): symbol{'~'}, freq{0}, left{nullptr}, right{nullptr} {}
    MinHeadNode(char symbol, uint32_t freq): symbol{symbol}, freq{freq}, left{nullptr}, right{nullptr} {}
    ~MinHeadNode() {
        delete left;
        delete right;
    }
};

struct compare {
    bool operator()(MinHeadNode* a, MinHeadNode* b) {
        return a->freq > b->freq;
    }
};

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


int main() {
    std::ifstream in("C:\\Users\\lakee\\CLionProjects\\DiscreateMath\\Harry Potter discreate.txt");

    std::vector <uint32_t> used(256);
    char symbol;

    while (in.get(symbol)){
        used[(int)symbol]++;
    }

    std::vector<std::pair<char, uint32_t>> symbols;
    for (int i = 0; i < 128; i++){
        if (used[i] == 0)
            continue;
        symbols.emplace_back((char)i, used[i]);
    }

    std::map<char, std::string> huffman_table;
    BuildHuffmanCodes(symbols, huffman_table);

    assert(huffman_table.size() == symbols.size());

    std::ofstream out("C:\\Users\\lakee\\CLionProjects\\DiscreateMath\\~Harry Potter encode.txt", std::ios::binary);

    out << huffman_table.size() << "\n";
    for (auto& symbol : huffman_table){
        out << symbol.first << " " << symbol.second << "\n";
    }

    in.clear();
    in.seekg(0);

    while (in.get(symbol)) {
        out << huffman_table[symbol];
    }

    std::ifstream in_code("C:\\Users\\lakee\\CLionProjects\\DiscreateMath\\~Harry Potter encode.txt");

    std::map<std::string, char> code;
    uint32_t size_of_table;
    in_code >> size_of_table;
    in_code.ignore(1);

    for (int i = 0; i < size_of_table; i++){
        std::string str;
        in_code.get(symbol);
        in_code >> str;
        code.emplace(str, symbol);
        in_code.ignore(1);
    }

    std::string file;
    in_code >> file;

    std::ofstream decode("C:\\Users\\lakee\\CLionProjects\\DiscreateMath\\~Harry Potter decode.txt");

    std::string word;
    for (int i = 0; i < file.size(); i++){
        word += file[i];
        if (code.find(word) != code.end()) {
            decode << code.find(word)->second;
            word = "";
        }
    }

    return 0;
}
