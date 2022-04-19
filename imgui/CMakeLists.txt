add_library(imgui STATIC)

set(imgui_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(imgui_backend_dir ${CMAKE_CURRENT_SOURCE_DIR}/backends)

set(imgui_base_src
  ${imgui_dir}/imgui_demo.cpp
  ${imgui_dir}/imgui_draw.cpp
  ${imgui_dir}/imgui_tables.cpp
  ${imgui_dir}/imgui_widgets.cpp
  ${imgui_dir}/imgui.cpp
)

# check what kind of backend script you need in imgui examples
set(imgui_backend_src
  ${imgui_backend_dir}/imgui_impl_sdl.cpp
  ${imgui_backend_dir}/imgui_impl_opengl2.cpp
)
# link necessary libraries to backends
set(imgui_link "")

target_sources(imgui
  PRIVATE
  ${imgui_base_src}
  ${imgui_backend_src}
)

target_include_directories(imgui
  PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/backends"
)

target_link_libraries(imgui
  PRIVATE
  ${imgui_link}
  SDL2::SDL2
  OpenGL::GL
)
