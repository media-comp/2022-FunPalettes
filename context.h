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

#include "data_type.h"
#include "camera.h"

class Canvas {
 public:
  Canvas(VEC2I window_size, VEC2I canvas_size, VEC2I canvas_pos);
  void clearCanvas(VEC3 color);
  void drawPoint(VEC3 pos, SCALAR radius, VEC3 color);
  void drawLine(VEC3 from, VEC3 to, SCALAR width, VEC3 color);
  void drawRay(VEC3 from, VEC3 to, SCALAR width, VEC3 color);
  void drawRect(VEC2 lb, VEC2 rb, VEC2 rt, VEC2 lt, VEC3 color);
  bool checkInCanvas(VEC2 screen_pos);
  bool checkInCanvas(VEC3 screen_pos);
  VEC2 canvasToScreen(VEC2 point);
  VEC2 canvasToScreen(VEC3 point);
  VEC2 sdlToScreen(VEC2I point);

 private:
  inline bool inCanvas(VEC2 point) {
    return (point(0) >= padding) && (point(0) <= 1.0f - padding) &&
           (point(1) >= padding) && (point(1) <= 1.0f - padding);
  }

  inline bool inCanvas(VEC3 point) {
    return (point(0) >= padding) && (point(0) <= 1.0f - padding) &&
           (point(1) >= padding) && (point(1) <= 1.0f - padding);
  }

 private:
  VEC2I m_window_size;
  VEC2I m_canvas_size;
  VEC2 m_canvas_coord;
  VEC2 m_ratio;
  SCALAR padding;
};

class Context {
 public:
  Context();
  int createContext(int width, int height);
  void newFrame();
  void swapBuffer();
  void clearScreen(SCALAR color_r, SCALAR color_g, SCALAR color_b);
  void renderImgui();
  void destoryContext();

  Camera* createCamera(const VEC3& pos, const VEC3& dir, SCALAR length,
                       SCALAR camera_width, SCALAR ratio);
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
