include(setup_cmake_package)

# entt package provides imported target EnTT::EnTT

message(STATUS "searching for entt")
find_package(entt CONFIG NO_DEFAULT_PATH PATHS ${CMAKE_BINARY_DIR}/externals/entt)
if(NOT entt_FOUND)

  message(STATUS "entt not found, building from sources")

  setup_cmake_package(
    ${CMAKE_SOURCE_DIR}/externals/entt
    ${CMAKE_BINARY_DIR}/tmp/entt
    ${CMAKE_BINARY_DIR}/externals/entt
    Release
    "-DBUILD_SHARED_LIBS=FALSE;"
  )

  find_package(entt CONFIG REQUIRED NO_DEFAULT_PATH PATHS ${CMAKE_BINARY_DIR}/externals/entt)
endif()

