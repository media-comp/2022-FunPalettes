#include <cstdio>
#include <vector>
#include <iostream>

#include "d2array.hpp"
#include "context.h"
#include "color_point.h"
#include "camera.h"
#include "palette_loader.h"
#include "palette_sort.h"
#include "palette_interpolate.h"

using VEC = Eigen::Vector3f;

int main() {
  SDL_SetMainReady();

  Context context;
  const int width = 1366;
  const int height = 768;
  context.createContext(width, height);

  std::vector<ColorPoint> color_list;
  color_list.clear();
  d2array<bool> connect_map(0, 0);
  d2array<float> dis_map(0, 0);

  VEC camera_pos(180, 120, -90);
  VEC camera_look_at(0, 0, 0);
  float camera_length = 1.2f;
  float camera_width = 10.0f;
  float camera_ratio = static_cast<float>(height) / width;
  Camera* camera = context.createCamera(
      camera_pos, camera_look_at, camera_length, camera_width, camera_ratio);
  Canvas* canvas = context.createCanvas(width * 0.7f, height * 0.7f,
                                        width * 0.24f, height * 0.26f);
  Canvas* palette_canvas = context.createCanvas(width * 0.7f, height * 0.13f,
                                                width * 0.24f, height * 0.09f);

  const float nanf = 9999.0f;
  Eigen::Vector2f drag_start(nanf, nanf);
  Eigen::Vector2f drag_end;
  float dis = 100000.0f;
  int interpolate_num = 0;

  // Main loop
  bool done = false;
  while (!done) {
    // on_event
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT) {
        done = true;
      } else if (event.type == SDL_WINDOWEVENT &&
                 event.window.event == SDL_WINDOWEVENT_CLOSE) {
        done = true;
      } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                 event.button.button == 1) {
        Eigen::Vector2f screen_pos = canvas->sdlToScreen(
            Eigen::Vector2i(event.button.x, event.button.y));
        if (canvas->checkInCanvas(screen_pos)) {
          drag_start = screen_pos;
          camera->enableDrag();
        }
      } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == 1) {
        drag_start = Eigen::Vector2f(nanf, nanf);
      } else if (drag_start(0) != nanf && event.type == SDL_MOUSEMOTION) {
        drag_end = canvas->sdlToScreen(
            Eigen::Vector2i(event.motion.x, event.motion.y));
      }
    }

    context.newFrame();

    bool sort_palette = false;
    bool interpolate_palette = false;

    // set imgui
    ImGui::SetNextWindowPos(ImVec2(width * 0.02f, height * 0.04f));
    ImGui::SetNextWindowSize(ImVec2(width * 0.2f, height * 0.92f));
    ImGui::Begin("Palette Editor");
    /*ImGui::Text("__________________");
    ImGui::Text("Debug Editor");
    ImGui::InputFloat3("Debug Window", camera->get_pos().data());*/

    if (ImGui::Button("Load Color")) loadPalette(color_list);
    if (ImGui::Button("Save Color")) savePalette(color_list);
    if (ImGui::Button("Sort Color")) sort_palette = true;
    ImGui::Text("__________________");
    ImGui::Text("Color Palette Editor:");
    ImGui::InputInt("##1", &interpolate_num);
    ImGui::SameLine();
    if (ImGui::Button("Fill Palette")) {
      interpolate_palette = true;
    }
    if (ImGui::Button("Add Color")) {
      color_list.emplace_back(ColorPoint());
    }
    auto iter = color_list.begin();
    char index[] = "##a";
    char remove_label[] = "delete##a";
    while (iter != color_list.end()) {
      ImGui::ColorEdit3(index, iter->data());
      ImGui::SameLine();
      if (ImGui::Button(remove_label)) {
        color_list.erase(iter);
      } else {
        iter++;
        index[2]++;
        remove_label[8]++;
      }
    }
    ImGui::Text("%d Colors", color_list.size());
    ImGui::End();

    context.clearScreen(0.84f, 0.84f, 0.84f);

    canvas->clearCanvas(VEC(33, 37, 43) / 255.0f);

    for (int i = 0; i + 1 < color_list.size(); i++) {
      canvas->drawLine(camera->worldToScreen(color_list[i].lab()),
                       camera->worldToScreen(color_list[i + 1].lab()), 2.0f,
                       color_list[i].rgb3f());
    }

    for (auto& color : color_list) {
      canvas->drawPoint(camera->worldToScreen(color.lab()), 15.0f,
                        color.rgb3f());
    }
    palette_canvas->clearCanvas(VEC(240, 240, 240) / 255.0f);
    float indentx = 0.005f;
    float paddingx = 0.03f;
    float paddingy = 0.1f;
    float width =
        (1.0f - indentx * (static_cast<float>(color_list.size()) - 1) -
         paddingx * 2.0f) /
        color_list.size();
    float pos = paddingx;
    for (int i = 0; i < color_list.size(); i++) {
      palette_canvas->drawRect(Eigen::Vector2f(pos, paddingy),
                               Eigen::Vector2f(pos + width, paddingy),
                               Eigen::Vector2f(pos + width, 1.0f - paddingy),
                               Eigen::Vector2f(pos, 1.0f - paddingy),
                               color_list[i].rgb3f());
      pos += width + indentx;
    }

    context.renderImgui();
    context.swapBuffer();

    if (sort_palette) {
      paletteSort(color_list, connect_map, dis_map);
      sort_palette = false;
    }
    if (interpolate_palette) {
      paletteInterpolate(color_list, interpolate_num);
      interpolate_palette = false;
    }

    if (drag_start(0) != nanf)
      camera->rotateCamera(drag_end - drag_start, 1.0f);
    for (auto& color : color_list) color.updateColor();
  }

  context.destoryContext();

  return 0;
}
