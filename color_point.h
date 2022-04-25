#ifndef COLOR_POINT_H_
#define COLOR_POINT_H_

#include "data_type.h"
#include <Eigen/Core>
#include "imgui.h"

class ColorPoint {
 public:
  ColorPoint() {}
  explicit ColorPoint(unsigned int hexcolor);
  explicit ColorPoint(ImVec4 rgb);
  explicit ColorPoint(const ColorPoint& copyfrom);
  explicit ColorPoint(const VEC3& lab);

  inline ImVec4 rgb() const { return m_rgb; }
  inline VEC3 rgb3f() const { return VEC3(m_rgb.x, m_rgb.y, m_rgb.z); }
  inline VEC3I rgbi() const {
    return VEC3I(static_cast<int>(m_rgb.x * 255.0f),
                 static_cast<int>(m_rgb.y * 255.0f),
                 static_cast<int>(m_rgb.z * 255.0f));
  }
  inline VEC3 lab() const { return m_lab; }
  inline float* data() { return reinterpret_cast<float*>(&m_rgb); }

  std::string hex() const;

  void updateColor();

  float distanceE2000(const VEC3 o_lab) const;

 private:
  ImVec4 m_rgb;
  VEC3 m_lab;
};

#endif  // COLOR_POINT_H_
