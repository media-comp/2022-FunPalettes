#ifndef CAMERA_H_
#define CAMERA_H_

#include <Eigen/Core>

#include "data_type.h"

class Camera {
 public:
  Camera(const VEC3& pos, const VEC3& look_at, float length, float camera_width,
         float ratio);
  void setCamera(const VEC3& pos, const VEC3& look_at, float length,
                 float camera_width, float ratio);
  void updateCameraProj();
  VEC2 worldToScreen(const VEC3& point);
  void enableDrag();
  void disableDrag();
  void rotateCamera(VEC2 drag, float s);

  inline VEC3 get_pos() { return m_pos; }
  inline VEC3 get_look_at() { return m_look_at; }
  inline VEC3 get_dir() { return m_dir; }

 private:
  VEC3 m_pos;
  VEC3 m_look_at;
  VEC3 m_dir;
  Eigen::Matrix3f m_proj;
  VEC3 m_origin_pos;
  VEC3 m_origin_dir;
  float m_length;
  float m_width;
  float m_height;
};

#endif  // CAMERA_H_
