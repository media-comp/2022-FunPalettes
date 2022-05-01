#ifndef DATA_TYPE_H_
#define DATA_TYPE_H_

#include <Eigen/Core>

using VEC3 = Eigen::Vector3f;
using VEC2 = Eigen::Vector2f;
using VEC2I = Eigen::Vector2i;
using VEC3I = Eigen::Vector3i;
using SCALAR = typename VEC3::Scalar;

struct Line3 {
  VEC3 from;
  VEC3 to;
  void set_as_ray();
  void print(std::string s = "");
  bool bound(int n, SCALAR x, bool greater);
  bool cast_to_bound(int n, SCALAR x, bool greater);
};

#endif  // DATA_TYPE_H_
