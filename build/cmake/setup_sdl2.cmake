include(setup_cmake_package)

# sdl2 package provides imported targets:
# for shared linking: SDL2::SDL2
# for static linking: SDL2::SDL2-static
# for usage with SDL_main: SDL2::SDL2main

message(STATUS "searching for sdl2")
find_package(sdl2 CONFIG NO_DEFAULT_PATH PATHS ${CMAKE_BINARY_DIR}/externals/sdl)
if(NOT sdl2_FOUND)

  message(STATUS "sdl2 not found, building from sources")

  setup_cmake_package(
    ${CMAKE_SOURCE_DIR}/externals/sdl2
    ${CMAKE_BINARY_DIR}/tmp/sdl2
    ${CMAKE_BINARY_DIR}/externals/sdl2
    Release
    "-DSDL_SHARED=FALSE;-DSDL_STATIC=TRUE;"
  )

  find_package(sdl2 CONFIG REQUIRED NO_DEFAULT_PATH PATHS ${CMAKE_BINARY_DIR}/externals/sdl2)
endif()


