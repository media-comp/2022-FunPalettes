#include "context.h"

#include <GL/gl.h>
#include <iostream>

////////////////////////////////////////////////
////////////////// Canvas /////////////////////
//////////////////////////////////////////////

Canvas::Canvas(VEC2I window_size, VEC2I canvas_size, VEC2I canvas_pos)
    : m_window_size(window_size), m_canvas_size(canvas_size) {
  float coordx = static_cast<float>(canvas_pos(0)) / window_size(0);
  float coordy = static_cast<float>(canvas_pos(1)) / window_size(1);
  m_canvas_coord = VEC2(coordx, coordy);
  m_ratio = VEC2(static_cast<float>(canvas_size(0)) / window_size(0),
                 static_cast<float>(canvas_size(1)) / window_size(1));
  padding = 0.1f;
}

void Canvas::clearCanvas(VEC3 color) {
  glBegin(GL_QUADS);
  glColor3d(color(0), color(1), color(2));
  VEC2 p = canvasToScreen(VEC2(0, 0));
  glVertex3f(p(0), p(1), 0);
  p = canvasToScreen(VEC2(0, 1));
  glVertex3f(p(0), p(1), 0);
  p = canvasToScreen(VEC2(1, 1));
  glVertex3f(p(0), p(1), 0);
  p = canvasToScreen(VEC2(1, 0));
  glVertex3f(p(0), p(1), 0);
  glEnd();
}

void Canvas::drawRect(VEC2 lb, VEC2 rb, VEC2 rt, VEC2 lt, VEC3 color) {
  glBegin(GL_QUADS);

  glColor3d(color(0), color(1), color(2));
  VEC2 p = canvasToScreen(lb);
  glVertex3f(p(0), p(1), 0);
  p = canvasToScreen(rb);
  glVertex3f(p(0), p(1), 0);
  p = canvasToScreen(rt);
  glVertex3f(p(0), p(1), 0);
  p = canvasToScreen(lt);
  glVertex3f(p(0), p(1), 0);
  glEnd();
}

VEC2 Canvas::canvasToScreen(VEC2 point) {
  float x = (point(0) * m_ratio(0) + m_canvas_coord(0)) * 2.0f - 1.0f;
  float y = (point(1) * m_ratio(1) + m_canvas_coord(1)) * 2.0f - 1.0f;
  return VEC2(x, y);
}

bool Canvas::checkInCanvas(VEC2 screen_pos) {
  return screen_pos(0) >= m_canvas_coord(0) &&
         screen_pos(1) >= m_canvas_coord(1) &&
         screen_pos(0) <= m_canvas_coord(0) + m_ratio(0) &&
         screen_pos(1) <= m_canvas_coord(1) + m_ratio(1);
}

VEC2 Canvas::sdlToScreen(VEC2I point) {
  return VEC2(static_cast<float>(point(0)) / m_window_size(0),
              1.0f - static_cast<float>(point(1)) / m_window_size(1));
}

void Canvas::drawPoint(VEC2 pos, float radius, VEC3 color) {
  if (!inCanvas(pos)) return;
  pos = canvasToScreen(pos);
  glColor3f(color(0), color(1), color(2));
  glEnable(GL_POINT_SMOOTH);
  glPointSize(radius);
  glBegin(GL_POINTS);
  glVertex3f(pos(0), pos(1), 0);
  glEnd();
  glDisable(GL_POINT_SMOOTH);
}
void Canvas::drawLine(VEC2 from, VEC2 to, float width, VEC3 color) {
  static auto min = [](float x, float y) { return x < y ? x : y; };
  if (!(inCanvas(from) || inCanvas(to))) return;
  if (!inCanvas(from) && inCanvas(to)) {
    VEC2 dft = from - to;
    dft.normalize();
    float ms = 1000.0f;
    if (from(0) < padding)
      ms = min(ms, -to(0) / dft(0));
    else if (from(0) > 1.0f - padding)
      ms = min(ms, (1.0f - padding - to(0)) / dft(0));
    if (from(1) < padding)
      ms = min(ms, -to(1) / dft(1));
    else if (from(1) > 1.0f - padding)
      ms = min(ms, (1.0f - padding - to(1)) / dft(1));
    from = to + dft * ms;
  } else if (inCanvas(from) && !inCanvas(to)) {
    VEC2 dft = to - from;
    dft.normalize();
    float ms = 1000.0f;
    if (to(0) < padding)
      ms = min(ms, -from(0) / dft(0));
    else if (to(0) > 1.0f - padding)
      ms = min(ms, (1.0f - padding - from(0)) / dft(0));
    if (to(1) < padding)
      ms = min(ms, -from(1) / dft(1));
    else if (to(1) > 1.0f - padding)
      ms = min(ms, (1.0f - padding - from(1)) / dft(1));
    to = from + dft * ms;
  }
  from = canvasToScreen(from);
  to = canvasToScreen(to);
  glColor3f(color(0), color(1), color(2));
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(width);
  glBegin(GL_LINES);
  glVertex3f(from(0), from(1), 0);
  glVertex3f(to(0), to(1), 0);
  glEnd();
  glDisable(GL_LINE_SMOOTH);
}

////////////////////////////////////////////////
////////////////// Context ////////////////////
//////////////////////////////////////////////

Context::Context() {
  window = nullptr;
  m_camera = nullptr;
  gl_context = nullptr;
}

int Context::createContext(int width, int height) {
  if (window != nullptr) {
    printf("Error! multiple window created\n");
    return -1;
  }

  m_width = width;
  m_height = height;
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
      0) {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  // Setup window
  SDL_WindowFlags window_flags =
      (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                        SDL_WINDOW_ALLOW_HIGHDPI);
  window =
      SDL_CreateWindow("ColorWarping", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, width, height, window_flags);

  gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1);  // Enable vsync

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL2_Init();

  StyleColorsSpectrum();

  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  ImFontConfig config;
  config.OversampleH = 3;
  ImFont* font =
      io.Fonts->AddFontFromFileTTF("./resources/micross.ttf", 16.0f, &config);

  return 1;
}

Camera* Context::createCamera(const VEC3& pos, const VEC3& dir, float length,
                              float camera_width, float ratio) {
  m_camera = new Camera(pos, dir, length, camera_width, ratio);
  return m_camera;
}

void Context::newFrame() {
  ImGui_ImplOpenGL2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
}

void Context::clearScreen(float color_r, float color_g, float color_b) {
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  glViewport(0, 0, static_cast<int>(io.DisplaySize.x),
             static_cast<int>(io.DisplaySize.y));
  glClearColor(color_r, color_g, color_b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Context::renderImgui() {
  ImGui::Render();
  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void Context::swapBuffer() { SDL_GL_SwapWindow(window); }

void Context::destoryContext() {
  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

Canvas* Context::createCanvas(int size_x, int size_y, int pos_x, int pos_y) {
  return new Canvas(VEC2I(m_width, m_height), VEC2I(size_x, size_y),
                    VEC2I(pos_x, pos_y));
}

////////////////////////////////////////////////
////////////////// ImGui Style ////////////////
//////////////////////////////////////////////

void StyleColorsSpectrum() {
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowMinSize = ImVec2(160, 20);
  style.FramePadding = ImVec2(4, 2);
  style.ItemSpacing = ImVec2(6, 6);
  style.ItemInnerSpacing = ImVec2(6, 4);
  style.Alpha = 0.95f;
  style.WindowRounding = 4.0f;
  style.FrameRounding = 2.0f;
  style.IndentSpacing = 6.0f;
  style.ItemInnerSpacing = ImVec2(2, 4);
  style.ColumnsMinSpacing = 50.0f;
  style.GrabMinSize = 14.0f;
  style.GrabRounding = 16.0f;
  style.ScrollbarSize = 12.0f;
  style.ScrollbarRounding = 16.0f;

  style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
  style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] =
      ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] =
      ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
  style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  style.Colors[ImGuiCol_Button] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.36f, 0.53f, 0.58f, 1.00f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.36f, 0.53f, 0.58f, 1.00f);
  style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
  style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
  style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
  style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
  ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
  style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogramHovered] =
      ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
}
