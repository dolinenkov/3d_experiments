include(setup_cmake_package)

# jansson package provides imported target jansson::jansson

message(STATUS "searching for jansson")
find_package(jansson CONFIG NO_DEFAULT_PATH PATHS ${CMAKE_BINARY_DIR}/externals/jansson)
if(NOT jansson_FOUND)

  message(STATUS "jansson not found, building from sources")

  setup_cmake_package(
    ${CMAKE_SOURCE_DIR}/externals/jansson
    ${CMAKE_BINARY_DIR}/tmp/jansson
    ${CMAKE_BINARY_DIR}/externals/jansson
    Release
    "-DJANSSON_BUILD_SHARED_LIBS=FALSE;-DJANSSON_EXAMPLES=FALSE;-DJANSSON_BUILD_DOCS=FALSE;-DJANSSON_WITHOUT_TESTS=TRUE;"
  )

  find_package(jansson CONFIG REQUIRED NO_DEFAULT_PATH PATHS ${CMAKE_BINARY_DIR}/externals/jansson)
endif()

