message(STATUS "searching for imgui")
add_library(imgui
  ${PROJECT_SOURCE_DIR}/externals/imgui/imconfig.h
  ${PROJECT_SOURCE_DIR}/externals/imgui/imgui.cpp
  ${PROJECT_SOURCE_DIR}/externals/imgui/imgui.h
  ${PROJECT_SOURCE_DIR}/externals/imgui/imgui_draw.cpp
  ${PROJECT_SOURCE_DIR}/externals/imgui/imgui_internal.h
  ${PROJECT_SOURCE_DIR}/externals/imgui/imgui_widgets.cpp
  ${PROJECT_SOURCE_DIR}/externals/imgui/imstb_rectpack.h
  ${PROJECT_SOURCE_DIR}/externals/imgui/imstb_textedit.h
  ${PROJECT_SOURCE_DIR}/externals/imgui/imstb_truetype.h
)
target_include_directories(imgui INTERFACE ${PROJECT_SOURCE_DIR}/externals/imgui/)
