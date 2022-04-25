#ifndef PALETTE_SORT_H_
#define PALETTE_SORT_H_

#include <vector>

#include "data_type.h"
#include "color_point.h"
#include "d2array.hpp"

void paletteSort(std::vector<ColorPoint>& list, d2array<SCALAR>& dismap);

#endif  // PALETTE_SORT_H_
