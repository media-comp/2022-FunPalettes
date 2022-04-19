#ifndef CAMERA_H_
#define CAMERA_H_

#include <Eigen/Core>

class Camera {
 public:
  Camera(const Eigen::Vector3f& pos, const Eigen::Vector3f& look_at,
         float length, float camera_width, float ratio);
  void setCamera(const Eigen::Vector3f& pos, const Eigen::Vector3f& look_at,
                 float length, float camera_width, float ratio);
  void updateCameraProj();
  Eigen::Vector2f worldToScreen(const Eigen::Vector3f& point);
  void enableDrag();
  void disableDrag();
  void rotateCamera(Eigen::Vector2f drag, float s);

  inline Eigen::Vector3f get_pos() { return m_pos; }
  inline Eigen::Vector3f get_look_at() { return m_look_at; }
  inline Eigen::Vector3f get_dir() { return m_dir; }

 private:
  Eigen::Vector3f m_pos;
  Eigen::Vector3f m_look_at;
  Eigen::Vector3f m_dir;
  Eigen::Matrix3f m_proj;
  Eigen::Vector3f m_origin_pos;
  Eigen::Vector3f m_origin_dir;
  float m_length;
  float m_width;
  float m_height;
};

#endif  // CAMERA_H_
