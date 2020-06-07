include(setup_cmake_package)

# FindZLIB cmake standard module provides exported target ZLIB::ZLIB

message(STATUS "searching for zlib")
set(ZLIB_ROOT ${CMAKE_BINARY_DIR}/externals/zlib)
find_package(ZLIB MODULE) # zlib doesn't exports package
if(NOT ZLIB_FOUND)

  message(STATUS "zlib not found, building from sources")

  # zlib ignores CMAKE_INSTALL_PREFIX
  list(APPEND ZLIB_CONFIG_ARGS -DINSTALL_BIN_DIR=${CMAKE_BINARY_DIR}/externals/zlib/bin)
  list(APPEND ZLIB_CONFIG_ARGS -DINSTALL_LIB_DIR=${CMAKE_BINARY_DIR}/externals/zlib/lib)
  list(APPEND ZLIB_CONFIG_ARGS -DINSTALL_INC_DIR=${CMAKE_BINARY_DIR}/externals/zlib/include)
  list(APPEND ZLIB_CONFIG_ARGS -DINSTALL_MAN_DIR=${CMAKE_BINARY_DIR}/externals/zlib/share/man)
  list(APPEND ZLIB_CONFIG_ARGS -DINSTALL_PKGCONFIG_DIR=${CMAKE_BINARY_DIR}/externals/zlib/lib/pkgconfig)

  setup_cmake_package(
    ${CMAKE_SOURCE_DIR}/externals/zlib
    ${CMAKE_BINARY_DIR}/tmp/zlib
    ${CMAKE_BINARY_DIR}/externals/zlib
    Release
    ${ZLIB_CONFIG_ARGS}
  )

  find_package(ZLIB MODULE REQUIRED)
endif()
