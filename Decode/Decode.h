#pragma once

#include <iostream>
#include <fstream>
#include <map>

#include "../Encode/Encode.h"

void RebuildHuffmanTable(std::map<char, std::map<std::string, char>>& code, std::ifstream& input_encode);

void WriteSourceFile(const std::string& decode_path, std::map<char, std::map<std::string, char>>& code, std::ifstream& input_encode);