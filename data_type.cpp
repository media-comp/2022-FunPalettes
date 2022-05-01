#include "data_type.h"

#include <iostream>

/**
 * @brief cut the line if the line goes across the bound
 *
 * @param n which axis? x=0, y=1, z=2
 * @param x where is the bound
 * @param greater should be larger or smaller than the bound
 * @return true should not show the line
 * @return false should show the line
 */
bool Line3::bound(int n, SCALAR x, bool greater) {
  SCALAR ge = greater ? -1.0f : 1.0f;
  if (from(n) * ge >= x * ge && to(n) * ge >= x * ge) return false;
  if ((from(n) - x) * (x - to(n)) > 0) {
    SCALAR t1 = from(n) - x;
    SCALAR t2 = x - to(n);
    SCALAR t3 = from(n) - to(n);
    if (from(n) * ge > x * ge) {
      from = from * t2 / t3 + to * t1 / t3;
    } else if (to(n) * ge > x * ge) {
      to = from * t2 / t3 + to * t1 / t3;
    }
    return true;
  }
  return true;
}

/**
 * @brief set the line as a ray(extend along the direction)
 */
void Line3::set_as_ray() {
  VEC3 dir = to - from;
  SCALAR norm = sqrt(dir(0) * dir(0) + dir(1) * dir(1));
  dir /= norm;
  to = from + dir * 10.0f;
}

void Line3::print(std::string s) {
  std::cout << s << "\nstart point:\n"
            << from << "\nend point:\n"
            << to << "\n";
}

bool Line3::cast_to_bound(int n, SCALAR x, bool greater) {
  SCALAR ge = greater ? -1.0f : 1.0f;
  if ((from(n) - x) * (from(n) - to(n)) < 0) {
    return false;
  } else if ((from(n) - x) * (x - to(n)) > 0) {
    SCALAR t1 = from(n) - x;
    SCALAR t2 = x - to(n);
    SCALAR t3 = from(n) - to(n);
    if (from(n) * ge > x * ge) {
      from = from * t2 / t3 + to * t1 / t3;
    } else if (to(n) * ge > x * ge) {
      to = from * t2 / t3 + to * t1 / t3;
    }
    return true;
  }
  return true;
}
