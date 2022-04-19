#include "palette_loader.h"
#include <fstream>
#include <string>
#include <iostream>

unsigned int stringToHex(const std::string& input) {
  unsigned int mul = 1, sum = 0;
  for (int i = input.length() - 1; i >= 0; i--) {
    char c = input.at(i);
    unsigned int digit = 0;
    if (c >= '0' && c <= '9')
      digit = c - '0';
    else if (c >= 'a' && c <= 'f')
      digit = (c - 'a') + 10;
    else if (c >= 'A' && c <= 'F')
      digit = c - 'A';
    sum += mul * digit;
    mul = mul << 4;
  }
  return sum;
}

void savePalette(const std::vector<ColorPoint>& color_list) {
  std::ofstream file;
  file.open("resources/output_palette.txt", std::ios::out);
  if (file.is_open()) {
    for (const ColorPoint& color : color_list) {
      file << color.hex() << std::endl;
    }
    file.close();
  }
}

void loadPalette(std::vector<ColorPoint>& color_list) {
  color_list.clear();
  std::ifstream file;
  file.open("resources/input_palette.txt", std::ios::in);
  std::string input;
  if (file.is_open()) {
    while (file) {
      std::getline(file, input);
      if (input.length() == 6)
        color_list.emplace_back(ColorPoint(stringToHex(input)));
    }
    file.close();
  } else {
    std::cout << "cannot find resources/input_palette.txt\n";
  }
}
