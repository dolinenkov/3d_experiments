include(setup_cmake_package)

# glad package provides imported target glad::glad

message(STATUS "searching for glad")
find_package(glad CONFIG NO_DEFAULT_PATH PATHS ${CMAKE_BINARY_DIR}/externals/glad)
if(NOT glad_FOUND)

  message(STATUS "glad not found, building from sources")

  setup_cmake_package(
    ${CMAKE_SOURCE_DIR}/externals/glad
    ${CMAKE_BINARY_DIR}/tmp/glad
    ${CMAKE_BINARY_DIR}/externals/glad
    Release
    "-DGLAD_INSTALL=TRUE"
  )

  find_package(glad CONFIG REQUIRED NO_DEFAULT_PATH PATHS ${CMAKE_BINARY_DIR}/externals/glad)
endif()

