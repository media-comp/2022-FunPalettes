#include "palette_interpolate.h"

#include <Eigen/Core>
#include <algorithm>

#include <iostream>

void paletteInterpolate(std::vector<ColorPoint>& color_list, int num) {
  int size = color_list.size();

  if (num == 0 || size == 0) return;

  if (num + 1 >= size) num = size - 1;

  int indent = 1;
  int start_index = 0;
  while (indent <= num) {
    int index = start_index;
    while (index + indent < color_list.size()) {
      VEC3 left(color_list[index].lab());
      VEC3 right(color_list[index + indent].lab());
      left = (left + right) / 2.0f;
      color_list.insert(color_list.begin() + index + 1, ColorPoint(left));
      index += indent + 1;
    }
    start_index++;
    indent++;
  }
}
