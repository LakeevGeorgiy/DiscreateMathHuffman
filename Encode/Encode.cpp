#include "Encode.h"

#include <bitset>

void ReadFromFile(const std::string& path, std::map<char, std::vector<std::pair<char, uint32_t>>>& symbols) {
    std::ifstream in(path);

    // Использование кодировки UTF-8, поэтому все символы точно помещаются в диапозон от 0 до 256
    // used[i][j] хранит информацию о том, сколько раз символ j встречался после символа i
    std::vector<std::vector<uint32_t>> used(256, std::vector<uint32_t>(256));
    char symbol;
    char last_symbol;
    in.get(last_symbol);

    // Считывание и просчет used
    while (in.get(symbol)) {
        used[(int) last_symbol][(int) symbol]++;
        last_symbol = symbol;
    }

    // В массив letters добавляются только те буквы, которые встречаются > 0 раз
    // Если массив letters не пустой, то он добавляется в общую таблицу по всем буквам
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

    // Если мы спустились до листа, где хранится уже символ встречающийся в файле
    // Выбран символ ~ потому что вероятность встретить этот символ мала
    if (head->symbol != '~') {
        huffman_table.emplace(head->symbol, code);
        return;
    }

    // Если идем по левому поддереву добавляем к коду 0, инача 1
    HuffmanCodes(head->left, code + "0", huffman_table);
    HuffmanCodes(head->right, code + "1", huffman_table);

}

void BuildHuffmanCodes(const std::vector<std::pair<char, uint32_t>>& symbols,
                       std::map<char, std::string>& huffman_table){

    // Приоритетная очередь на минимум
    std::priority_queue<MinHeadNode*, std::vector<MinHeadNode*>, compare> min_heap;

    MinHeadNode* left;
    MinHeadNode* right;

    // Добавляем символ и сколько раз встречается
    for (const auto& symbol : symbols){
        min_heap.push(new MinHeadNode(symbol.first, symbol.second));
    }

    // Если символ всего один, то присвоим ему код 1
    if (min_heap.size() == 1){
        huffman_table.emplace(symbols[0].first, "1");
        return;
    }

    // Достаем минимальных два элемента, складываем сумму их частот, в качестве символа выставляем ~
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

    // Вызываем функцию для прохода по дереву и восстановлению кодовых слов для каждого символа
    HuffmanCodes(min_heap.top(), "", huffman_table);
}

void WriteTable(std::ofstream& encode_out,
                std::map<char, std::vector<std::pair<char, uint32_t>>>& symbols){

    //Записываем размер всей таблицы частот
    uint32_t size_of_table = symbols.size();
    encode_out.write((char*)&size_of_table, 4);

    // Записываем размер частотной таблицы для отдельного элемента и символ, который был предыдущий для последующих
    // Записываем все символы и их частоту
    for (auto& symbol : symbols){

        char last_symbol = symbol.first;
        uint32_t size = symbol.second.size();

        encode_out.write((char*)&last_symbol, 1);
        encode_out.write((char*)&size, 4);

        for (auto& character : symbol.second){
            encode_out.write((char*)&character.first, 1);
            encode_out.write((char*)&character.second, 4);
        }
    }
}

void WriteToFile(const std::string& source_path, std::ofstream& out, std::map<char, std::map<char, std::string>>& huffman_table) {

    std::ifstream in(source_path, std::ios::binary);

    char symbol;
    uint8_t last_symbol;
    int i = 7;
    uint8_t code = 0;

    // Записываем первый символ в файле без кодирования, так как для него нет предыдущего и соответственно кодового слова
    in.read((char*)&last_symbol, 1);
    out.write((char*)&last_symbol, 1);

    // Считывание по одному байту из исходного файла, нахождение кодового слова для этого символа с учетом предыдущего символа
    // По битовая запись кода в байт с помощью побитовых сдвигов
    while (in.read((char*)&symbol, 1)) {


        for (int j = 0; j < huffman_table[last_symbol][symbol].size(); j++){
            if (i == -1){
                i = 7;
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
}