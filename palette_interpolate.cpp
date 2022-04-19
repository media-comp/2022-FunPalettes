#include "palette_interpolate.h"

#include <Eigen/Core>
#include <algorithm>

#include <iostream>

void paletteInterpolate(std::vector<ColorPoint>& color_list, int num) {
  if (num == 0) return;

  int size = color_list.size();
  float indent = 1.0f / static_cast<float>(num + 1);
  std::cout << indent << " \n";

  std::vector<ColorPoint> reference;
  reference.resize(size);
  std::copy(color_list.begin(), color_list.end(), reference.begin());
  color_list.clear();

  for (int i = 0; i + 1 < reference.size(); i++) {
    Eigen::Vector3f start(reference[i].lab());
    Eigen::Vector3f end(reference[i + 1].lab());

    float lerp = 0.0f;

    for (int j = 0; j <= num; j++) {
      Eigen::Vector3f p = start * (1.0f - lerp) + end * lerp;
      color_list.emplace_back(ColorPoint(p));
      lerp += indent;
    }
  }

  color_list.emplace_back(ColorPoint(reference[size - 1]));
}
