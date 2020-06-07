include(setup_cmake_package)

# fmt package provides imported target fmt::fmt

message(STATUS "searching for fmt")
find_package(fmt CONFIG NO_DEFAULT_PATH PATHS ${CMAKE_BINARY_DIR}/externals/fmt)
if(NOT fmt_FOUND)

  message(STATUS "fmt not found, building from sources")

  setup_cmake_package(
    ${CMAKE_SOURCE_DIR}/externals/fmt
    ${CMAKE_BINARY_DIR}/tmp/fmt
    ${CMAKE_BINARY_DIR}/externals/fmt
    "Debug;Release"
    "-DFMT_DOC=FALSE;-DFMT_TEST=FALSE;-DBUILD_SHARED_LIBS=FALSE;"
  )

  find_package(fmt CONFIG REQUIRED NO_DEFAULT_PATH PATHS ${CMAKE_BINARY_DIR}/externals/fmt)
endif()
