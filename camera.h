#ifndef CAMERA_H_
#define CAMERA_H_

#include <Eigen/Core>

#include "data_type.h"

class Camera {
 public:
  Camera(const VEC3& pos, const VEC3& look_at, SCALAR length,
         SCALAR camera_width, SCALAR ratio);
  void setCamera(const VEC3& pos, const VEC3& look_at, SCALAR length,
                 SCALAR camera_width, SCALAR ratio);
  void updateCameraProj();
  VEC3 worldToScreen(const VEC3& point);
  void enableDrag();
  void disableDrag();
  void rotateCamera(VEC2 drag, SCALAR s);

  inline VEC3 get_pos() { return m_pos; }
  inline VEC3 get_look_at() { return m_look_at; }
  inline VEC3 get_dir() { return m_dir; }

 private:
  VEC3 m_pos;
  VEC3 m_look_at;
  VEC3 m_dir;
  Eigen::Matrix3f m_proj;
  Eigen::Matrix3f m_origin_proj;
  VEC3 m_origin_pos;
  VEC3 m_origin_dir;
  SCALAR m_length;
  SCALAR m_width;
  SCALAR m_height;
};

#endif  // CAMERA_H_
