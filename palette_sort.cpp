#include "palette_sort.h"

#include <algorithm>
#include <iostream>

void find_path(std::vector<int>& point_index, std::vector<int>& best_index,
               const d2array<SCALAR>& dismap, int pos, SCALAR dis,
               SCALAR& min_dis) {
  if (pos == dismap.size_x()) {
    if (min_dis == -1.0f || min_dis > dis) {
      min_dis = dis;
      best_index.clear();
      // std::copy(best_index.begin(), point_index.begin(), point_index.end() -
      // 1);
      for (int i = 0; i < pos; i++) best_index.emplace_back(point_index[i]);
    }
    return;
  }
  for (int ins = 0; ins < dismap.size_x(); ins++) {
    bool exist = false;
    for (int i = 0; i < pos; i++)
      if (point_index[i] == ins) {
        exist = true;
        break;
      }
    if (!exist) {
      point_index.at(pos) = ins;
      SCALAR new_dis =
          (pos == 0 ? 0.0f : dismap(point_index[pos - 1], point_index[pos]));
      find_path(point_index, best_index, dismap, pos + 1, dis + new_dis,
                min_dis);
      point_index.at(pos) = -1;
    }
  }
}

void paletteSort(std::vector<ColorPoint>& list, d2array<SCALAR>& dismap) {
  int size = list.size();
  dismap.resize(size, size, 0.0f);
  for (int i = 0; i < size; i++)
    for (int j = i; j < size; j++) {
      if (i == j) {
        dismap(i, j) = 0.0f;
      } else {
        dismap(i, j) = list[i].distanceE2000(list[j].lab());
        dismap(j, i) = dismap(i, j);
      }
    }
  std::vector<int> point_index(size, -1);
  std::vector<int> best_index;
  SCALAR min_dis = -1.0f;
  find_path(point_index, best_index, dismap, 0, 0.0f, min_dis);
  for (int i = 0; i < size; i++) std::cout << best_index[i] << " ";
  std::cout << min_dis << "\n";
  std::vector<ColorPoint> tmp;
  tmp.resize(size);
  // for (int i = 0; i < size; i++) tmp.emplace_back(list[i]);
  // list = tmp;
  std::copy(list.begin(), list.end(), tmp.begin());
  list.clear();
  for (int i = 0; i < size; i++) list.emplace_back(tmp[best_index[i]]);
}
