#pragma once

#include <iostream>
#include <fstream>
#include <map>

void RebuildHuffmanTable(std::map<std::string, char>& code, std::ifstream& input_encode);

void WriteSourceFile(const std::string& decode_path, std::map<std::string, char>& code, std::ifstream& input_encode);