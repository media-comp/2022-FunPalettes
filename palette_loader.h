#ifndef PALETTE_LOADER_H_
#define PALETTE_LOADER_H_

#include <vector>

#include "data_type.h"
#include "color_point.h"

void savePalette(const std::vector<ColorPoint>& color_list);
void loadPalette(std::vector<ColorPoint>& color_list);

#endif  // PALETTE_LOADER_H_
