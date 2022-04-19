#ifndef COLOR_POINT_H_
#define COLOR_POINT_H_

#include <Eigen/Core>
#include "imgui.h"

class ColorPoint {
 public:
  ColorPoint() {}
  explicit ColorPoint(unsigned int hexcolor);
  explicit ColorPoint(ImVec4 rgb);
  explicit ColorPoint(const ColorPoint& copyfrom);
  explicit ColorPoint(const Eigen::Vector3f& lab);

  inline ImVec4 rgb() const { return m_rgb; }
  inline Eigen::Vector3f rgb3f() const {
    return Eigen::Vector3f(m_rgb.x, m_rgb.y, m_rgb.z);
  }
  inline Eigen::Vector3i rgbi() const {
    return Eigen::Vector3i(static_cast<int>(m_rgb.x * 255.0f),
                           static_cast<int>(m_rgb.y * 255.0f),
                           static_cast<int>(m_rgb.z * 255.0f));
  }
  inline Eigen::Vector3f lab() const { return m_lab; }
  inline float* data() { return reinterpret_cast<float*>(&m_rgb); }

  std::string hex() const;

  void updateColor();

  float distanceE2000(const Eigen::Vector3f o_lab) const;

 private:
  ImVec4 m_rgb;
  Eigen::Vector3f m_lab;
};

#endif  // COLOR_POINT_H_
