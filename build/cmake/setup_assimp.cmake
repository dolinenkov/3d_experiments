include(setup_cmake_package)

message(STATUS "searching for assimp")
find_package(assimp CONFIG NO_DEFAULT_PATH PATHS ${CMAKE_BINARY_DIR}/externals/assimp)
if(NOT assimp_FOUND)

  message(STATUS "assimp not found, building from sources")

  setup_cmake_package(
    ${CMAKE_SOURCE_DIR}/externals/assimp
    ${CMAKE_BINARY_DIR}/tmp/assimp
    ${CMAKE_BINARY_DIR}/externals/assimp
    "Debug;Release"
    "-DASSIMP_BUILD_TESTS=FALSE;-DBUILD_SHARED_LIBS=TRUE;"
  )

  find_package(assimp CONFIG REQUIRED NO_DEFAULT_PATH PATHS ${CMAKE_BINARY_DIR}/externals/assimp)
endif()
