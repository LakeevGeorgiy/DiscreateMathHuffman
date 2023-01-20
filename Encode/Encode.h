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

void ReadFromFile(const std::string& path, std::map<char, std::vector<std::pair<char, uint32_t>>>& symbol);

void HuffmanCodes(MinHeadNode* head, std::string code, std::map<char, std::string>& huffman_table);

void BuildHuffmanCodes(const std::vector<std::pair<char, uint32_t>>& symbols,
                       std::map<char, std::string>& huffman_table);

void WriteTable(std::ofstream& encode_out,
                std::map<char, std::vector<std::pair<char, uint32_t>>>& symbols);

void WriteToFile(const std::string& source_path, std::ofstream& out, std::map<char, std::map<char, std::string>>& huffman_table);