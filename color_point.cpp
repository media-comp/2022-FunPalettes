#include "color_point.h"

#include <cmath>

ColorPoint::ColorPoint(unsigned int hexcolor) {
  unsigned int hc[3];
  hc[2] = hexcolor & 0xff;
  hc[1] = (hexcolor & 0xff00) >> 8;
  hc[0] = (hexcolor & 0xff0000) >> 16;
  float fc[3];
  for (int i = 0; i < 3; i++) {
    fc[i] = static_cast<float>(hc[i]) / 255.0f;
  }
  m_rgb.x = fc[0];
  m_rgb.y = fc[1];
  m_rgb.z = fc[2];
}

ColorPoint::ColorPoint(const ColorPoint& copyfrom) {
  m_rgb = copyfrom.rgb();
  m_lab = copyfrom.lab();
}

ColorPoint::ColorPoint(ImVec4 rgb) : m_rgb(rgb) { updateColor(); }

ColorPoint::ColorPoint(const Eigen::Vector3f& lab) : m_lab(lab) {
  ImVec4 rgb;
  float ciel = lab(0);
  float ciea = lab(1);
  float cieb = lab(2);
  float var_Y = (ciel + 16.0f) / 116.0f;
  float var_X = ciea / 500.0f + var_Y;
  float var_Z = var_Y - cieb / 200.0f;

  if (pow(var_Y, 3.0f) > 0.008856f)
    var_Y = pow(var_Y, 3.0f);
  else
    var_Y = (var_Y - 16.0f / 116.0f) / 7.787f;
  if (pow(var_X, 3) > 0.008856f)
    var_X = pow(var_X, 3);
  else
    var_X = (var_X - 16.0f / 116.0f) / 7.787f;
  if (pow(var_Z, 3.0f) > 0.008856f)
    var_Z = pow(var_Z, 3.0f);
  else
    var_Z = (var_Z - 16.0f / 116.0f) / 7.787f;

  float X = var_X * 95.047f;
  float Y = var_Y * 100.0f;
  float Z = var_Z * 108.883f;

  float var_X2 = X / 100.0f;
  float var_Y2 = Y / 100.0f;
  float var_Z2 = Z / 100.0f;

  float var_R = var_X2 * 3.2406f + var_Y2 * -1.5372f + var_Z2 * -0.4986f;
  float var_G = var_X2 * -0.9689f + var_Y2 * 1.8758f + var_Z2 * 0.0415f;
  float var_B = var_X2 * 0.0557f + var_Y2 * -0.2040f + var_Z2 * 1.0570f;

  if (var_R > 0.0031308f)
    var_R = 1.055f * (pow(var_R, (1.0f / 2.4f))) - 0.055f;
  else
    var_R = 12.92f * var_R;
  if (var_G > 0.0031308f)
    var_G = 1.055f * (pow(var_G, (1.0f / 2.4f))) - 0.055f;
  else
    var_G = 12.92f * var_G;
  if (var_B > 0.0031308f)
    var_B = 1.055f * (pow(var_B, (1.0f / 2.4f))) - 0.055f;
  else
    var_B = 12.92f * var_B;

  m_rgb.x = var_R;
  m_rgb.y = var_G;
  m_rgb.z = var_B;
  m_rgb.w = 1.0f;
}

std::string ColorPoint::hex() const {
  Eigen::Vector3i hc = rgbi();
  unsigned int result = 0;
  for (int i = 0; i < 3; i++) {
    result <<= 4;
    result += hc(i);
  }
  std::string str = "";
  for (int i = 0; i < 6; i++) {
    unsigned int tail = result & 0xf;
    result >>= 4;
    char chara;
    if (tail < 10)
      chara = '0' + tail;
    else
      chara = 'a' + (tail - 10);
    str += chara;
  }
  return str;
}

void ColorPoint::updateColor() {
  float var_R = m_rgb.x;
  float var_G = m_rgb.y;
  float var_B = m_rgb.z;
  if (var_R > 0.04045f)
    var_R = pow(((var_R + 0.055f) / 1.055f), 2.4f);
  else
    var_R = var_R / 12.92f;
  if (var_G > 0.04045f)
    var_G = pow(((var_G + 0.055f) / 1.055f), 2.4f);
  else
    var_G = var_G / 12.92f;
  if (var_B > 0.04045f)
    var_B = pow(((var_B + 0.055f) / 1.055f), 2.4f);
  else
    var_B = var_B / 12.92f;

  var_R = var_R * 100.0f;
  var_G = var_G * 100.0f;
  var_B = var_B * 100.0f;

  float X = var_R * 0.4124f + var_G * 0.3576f + var_B * 0.1805f;
  float Y = var_R * 0.2126f + var_G * 0.7152f + var_B * 0.0722f;
  float Z = var_R * 0.0193f + var_G * 0.1192f + var_B * 0.9505f;

  float var_X = X / 95.047f;
  float var_Y = Y / 100.0f;
  float var_Z = Z / 108.883f;

  if (var_X > 0.008856f)
    var_X = pow(var_X, (1.0f / 3.0f));
  else
    var_X = (7.787f * var_X) + (16.0f / 116.0f);
  if (var_Y > 0.008856f)
    var_Y = pow(var_Y, (1.0f / 3.0f));
  else
    var_Y = (7.787f * var_Y) + (16.0f / 116.0f);
  if (var_Z > 0.008856f)
    var_Z = pow(var_Z, (1.0f / 3.0f));
  else
    var_Z = (7.787f * var_Z) + (16.0f / 116.0f);

  float CIE_L = (116.0f * var_Y) - 16.0f;
  float CIE_a = 500.0f * (var_X - var_Y);
  float CIE_b = 200.0f * (var_Y - var_Z);
  m_lab = Eigen::Vector3f(CIE_L, CIE_a, CIE_b);
}

float ColorPoint::distanceE2000(const Eigen::Vector3f o_lab) const {
  return (m_lab - o_lab).norm();
}
