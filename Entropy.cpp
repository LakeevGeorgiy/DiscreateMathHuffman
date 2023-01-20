#include <iostream>
#include <fstream>
#include <map>
#include <iomanip>
#include <cmath>
using namespace std;

int main(){

    ifstream in("C:\\Users\\lakee\\CLionProjects\\code\\Harry Potter discreate.txt");
    map<char, uint32_t> freq_symbols;
    map<char, map<char, uint32_t>> freq_last_symbols;
    uint32_t cnt_symbols = 1;

    char c;
    char last_symbol;
    in.get(last_symbol);
    freq_symbols[last_symbol] = 1;

    while (in.get(c)){

        freq_symbols[c]++;
        freq_last_symbols[last_symbol][c]++;
        last_symbol = c;

        cnt_symbols++;
    }

    std::map<char, long double> probability;
    for (auto& freq_symbol : freq_symbols){
        probability[freq_symbol.first] = static_cast<long double>(freq_symbol.second) / cnt_symbols;
    }

    long double low_limit = 0;
    for (auto& prob : probability){
        long double bits = log2(prob.second) * prob.second * (-1);
        low_limit += bits;
    }
    std::cout << "low limit: " << low_limit << "\n";

    long double conditional_entropy = 0;
    for (auto& symbol : freq_last_symbols){
        long double sum_bits = 0;
        for (auto& freq : symbol.second){
            long double prob = static_cast<long double>(freq.second) / freq_symbols[symbol.first];
            long double bits = log2(prob) * prob * (-1);
            sum_bits += bits;
        }
        conditional_entropy += probability[symbol.first] * sum_bits;
    }
    std::cout << "conditional entropy: " << conditional_entropy << "\n";
    return 0;
}
