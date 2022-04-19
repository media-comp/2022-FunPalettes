#ifndef CONTEXT_H_
#define CONTEXT_H_

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h>
#include <Eigen/Core>
#include <GL/gl.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"

#include "camera.h"

class Canvas {
 public:
  Canvas(Eigen::Vector2i window_size, Eigen::Vector2i canvas_size,
         Eigen::Vector2i canvas_pos);
  void clearCanvas(Eigen::Vector3f color);
  void drawPoint(Eigen::Vector2f pos, float radius, Eigen::Vector3f color);
  void drawLine(Eigen::Vector2f from, Eigen::Vector2f to, float width,
                Eigen::Vector3f color);
  void drawRect(Eigen::Vector2f lb, Eigen::Vector2f rb, Eigen::Vector2f rt,
                Eigen::Vector2f lt, Eigen::Vector3f color);
  bool checkInCanvas(Eigen::Vector2f screen_pos);
  Eigen::Vector2f canvasToScreen(Eigen::Vector2f point);
  Eigen::Vector2f sdlToScreen(Eigen::Vector2i point);

 private:
  inline bool inCanvas(Eigen::Vector2f point) {
    return (point(0) >= padding) && (point(0) <= 1.0f - padding) &&
           (point(1) >= padding) && (point(1) <= 1.0f - padding);
  }

 private:
  Eigen::Vector2i m_window_size;
  Eigen::Vector2i m_canvas_size;
  Eigen::Vector2f m_canvas_coord;
  Eigen::Vector2f m_ratio;
  float padding;
};

class Context {
 public:
  Context();
  int createContext(int width, int height);
  void newFrame();
  void swapBuffer();
  void clearScreen(float color_r, float color_g, float color_b);
  void renderImgui();
  void destoryContext();

  Camera* createCamera(const Eigen::Vector3f& pos, const Eigen::Vector3f& dir,
                       float length, float camera_width, float ratio);
  Canvas* createCanvas(int size_x, int size_y, int pos_x, int pos_y);

  inline int get_width() { return m_width; }
  inline int get_height() { return m_height; }
  inline SDL_Window* get_window() { return window; }

 private:
  SDL_Window* window;
  SDL_GLContext gl_context;
  Camera* m_camera;
  int m_width;
  int m_height;
};

void StyleColorsSpectrum();

#endif  // CONTEXT_H_
