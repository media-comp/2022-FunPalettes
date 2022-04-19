cmake_minimum_required(VERSION 3.20)

project(ClosetColorWarping)

find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
  message("SDL2 founded ${SDL2_INCLUDE_DIRS}")
endif()

find_package(Eigen3 REQUIRED)
if(Eigen3_FOUND)
  message("Eigen3 founded ${EIGEN3_INCLUDE_DIRS}")
endif()

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
  message("OpenGL founded ${OPENGL_LIBRARIES}")
endif()

add_subdirectory(imgui)

add_executable(FunPalettes)

target_sources(FunPalettes
  PRIVATE
  main.cpp
  context.cpp
  color_point.cpp
  camera.cpp
  palette_loader.cpp
  palette_sort.cpp
  palette_interpolate.cpp
)

target_include_directories(FunPalettes
  PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(FunPalettes
  PRIVATE
  SDL2::SDL2
  OpenGL::GL
  Eigen3::Eigen
  imgui
)

add_custom_command(TARGET FunPalettes PRE_BUILD 
  COMMAND ${CMAKE_COMMAND} -E
  copy_directory ${CMAKE_SOURCE_DIR}/resources
  $<TARGET_FILE_DIR:FunPalettes>/resources
)



