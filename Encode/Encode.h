#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <fstream>

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

void ReadFromFile(const std::string& path, std::vector<std::pair<char, uint32_t>>& symbol);

void HuffmanCodes(MinHeadNode* head, std::string code, std::map<char, std::string>& huffman_table);

void BuildHuffmanCodes(const std::vector<std::pair<char, uint32_t>>& symbols,
                       std::map<char, std::string>& huffman_table);

void WriteToFile(const std::string& source_path, const std::string& encode_path, std::map<char, std::string>& huffman_table,
                 std::vector<std::pair<char, uint32_t>>& symbols);