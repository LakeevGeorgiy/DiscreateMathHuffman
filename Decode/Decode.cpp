#include "Decode.h"

void RebuildHuffmanTable(std::map<char, std::map<std::string, char>>& code, std::ifstream& input_encode) {

    // symbols - частотная таблица, аналогичная кодировщику
    // huffman_table[i][j] - кодовое слово для символа j, если предыдущий был i
    std::map<char, std::map<char, std::string>> huffman_table;
    std::map<char, std::vector<std::pair<char, uint32_t>>> symbols;

    // Считывание размеров всей таблицы
    uint32_t size_of_table;
    input_encode.read((char*)&size_of_table, 4);

    // Восстановление частотной таблицы: считывание предыдущего символа, последующего символа и его частоту
    for (int i = 0; i < size_of_table; i++) {

        char last_symbol;
        uint32_t size;
        input_encode.read((char *) &last_symbol, 1);
        input_encode.read((char *) &size, 4);

        std::vector<std::pair<char, uint32_t>> table;
        for (int j = 0; j < size; j++) {
            char symbol;
            uint32_t freq;
            input_encode.read((char *) &symbol, 1);
            input_encode.read((char *) &freq, 4);
            table.emplace_back(symbol, freq);
        }
        symbols.emplace(last_symbol, table);
    }

    // Построение дерева Хаффмана для каждого символа, аналогично кодировщику
    for (auto& it : symbols){
        std::map<char, std::string> table;
        BuildHuffmanCodes(it.second, table);
        huffman_table.emplace(it.first, table);
    }

    // Перезапись дерева: теперь ключом является кодовое слово, а значением - буква, которую этот код кодирует
    for (auto& it : huffman_table){
        std::map<std::string, char> temp;
        for (auto& itr : it.second)
            temp.emplace(itr.second, itr.first);
        code.emplace(it.first, temp);
    }

}

void WriteSourceFile(const std::string& decode_path, std::map<char, std::map<std::string, char>>& code, std::ifstream& input_encode) {

    std::ofstream out(decode_path, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);

    std::string word;
    uint8_t symbol;
    uint8_t last_symbol;

    // Считывание первой буквы, так как она есть, потому что она не кодируется
    input_encode.read((char*)&last_symbol, 1);
    out.write((char*)&last_symbol, 1);

    // Считывание из файла побайтово, побитовыми сдивигами формируем кодовое слово, которое записываем в строку word
    // На каждой итерации проверяем, встречается ли word, как кодовое слово после символа last_symbol
    // Если такой элемент находится, то печатаем результат декодирования, иначе дальше формируем кодовое слово
    while (input_encode.read((char*)&symbol, 1)) {
        for (int i = 7; i >= 0; i--){

            if (code[last_symbol].find(word) != code[last_symbol].end()){
                char letter = code[last_symbol].find(word)->second;
                out.write((char*)&letter, 1);
                word = "";
                last_symbol = letter;
            }

            if ((symbol & (1 << i)) == 0)
                word += '0';
            else
                word += '1';

        }
    }
}