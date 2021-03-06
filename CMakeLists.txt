cmake_minimum_required(VERSION 3.8)

project(xrndr VERSION 0.0.1 LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "xrndr")

message(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/build/cmake/)

include(setup_zlib)
include(setup_asio)
include(setup_glm)
include(setup_glad)
include(setup_entt)
include(setup_fmt)
include(setup_jansson)
include(setup_sdl2)
include(setup_assimp)
include(setup_stb)
include(setup_imgui)

add_executable(xrndr
    xrndr/Camera.cc
    xrndr/json.cc
    xrndr/main.cc
    xrndr/MatrixStack.cc
    xrndr/Mesh.cc
    xrndr/Model.cc
    xrndr/Program.cc
    xrndr/Projection.cc
    xrndr/Scene.cc
    xrndr/Settings.cc
    xrndr/Texture2D.cc
    xrndr/TextureCache.cc
    xrndr/detail/stb.cc
    include/xrndr/detail/Stack.hh
    include/xrndr/Camera.hh
    include/xrndr/config.hh
    include/xrndr/json.hh
    include/xrndr/Light.hh
    include/xrndr/Material.hh
    include/xrndr/MatrixStack.hh
    include/xrndr/Mesh.hh
    include/xrndr/Model.hh
    include/xrndr/Program.hh
    include/xrndr/Projection.hh
    include/xrndr/Scene.hh
    include/xrndr/Settings.hh
    include/xrndr/Texture2D.hh
    include/xrndr/TextureCache.hh
    include/xrndr/VertexFormat.hh

    include/xrndr/async/Async.hh
    xrndr/async/Async.cc
)

#source_group(TREE include/ PREFIX "include")
#source_group(TREE xrndr/ PREFIX "src")
set_target_properties(xrndr PROPERTIES FOLDER xrndr)
set_target_properties(xrndr PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

target_include_directories(xrndr PRIVATE include)
target_compile_features(xrndr PRIVATE cxx_std_17)
target_link_libraries(xrndr PRIVATE
    ZLIB::ZLIB
    assimp::assimp
    EnTT::EnTT
    fmt::fmt
    glad::glad
    glm::glm
    jansson::jansson
    SDL2::SDL2-static SDL2::SDL2main
    asio
    stb
    imgui
)
