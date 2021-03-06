#include "camera.h"

#include <Eigen/Geometry>
#include <iostream>

Camera::Camera(const VEC3& pos, const VEC3& look_at, SCALAR length,
               SCALAR camera_width, SCALAR ratio)
    : m_pos(pos),
      m_look_at(look_at),
      m_length(length),
      m_width(camera_width),
      m_height(camera_width * ratio) {
  m_dir = (m_look_at - m_pos).normalized();
  updateCameraProj();
}

void Camera::setCamera(const VEC3& pos, const VEC3& look_at, SCALAR length,
                       SCALAR camera_width, SCALAR ratio) {
  m_pos = pos;
  m_look_at = look_at;
  m_dir = (look_at - pos).normalized();
  m_length = length;
  m_width = camera_width;
  m_height = camera_width * ratio;
  updateCameraProj();
}

void Camera::updateCameraProj() {
  VEC3 y = m_dir;
  VEC3 z = -y.cross(VEC3(1, 0, 0));
  z.normalize();
  VEC3 x = y.cross(z);
  x.normalize();
  m_proj(0, 0) = x(0);
  m_proj(0, 1) = x(1);
  m_proj(0, 2) = x(2);
  m_proj(1, 0) = y(0);
  m_proj(1, 1) = y(1);
  m_proj(1, 2) = y(2);
  m_proj(2, 0) = z(0);
  m_proj(2, 1) = z(1);
  m_proj(2, 2) = z(2);
}

VEC3 Camera::worldToScreen(const VEC3& point) {
  VEC3 p = point;
  p = m_proj * (p - m_pos);
  SCALAR local_y = p(1);
  p *= m_length / fabs(local_y);
  VEC3 screen_pos((p(0) + 1.0f) / 2.0f, (p(2) + 1.0f) / 2.0f,
                  local_y - m_length);
  return screen_pos;
}

void Camera::enableDrag() {
  m_origin_pos = m_pos;
  m_origin_dir = m_dir;
  m_origin_proj = m_proj;
}

void Camera::disableDrag() {}

void Camera::rotateCamera(VEC2 drag, SCALAR s) {
  if (drag.norm() < 0.01) return;
  SCALAR scale = drag.norm() * s;
  VEC3 axis(drag(0), 0, drag(1));
  axis.normalize();
  axis = m_origin_proj.transpose() * axis;
  axis = axis.cross(m_origin_dir);
  SCALAR w = cos(scale);
  SCALAR ws = sin(scale);
  Eigen::Quaternion<SCALAR> quat(w, axis(0) * ws, axis(1) * ws, axis(2) * ws);
  m_pos = quat * m_origin_pos;
  m_dir = (m_look_at - m_pos).normalized();
  updateCameraProj();
}
