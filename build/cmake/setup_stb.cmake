message(STATUS "searching for stb")
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${PROJECT_SOURCE_DIR}/externals/stb/)
#target_compile_features(asio INTERFACE cxx_std_17)
#target_compile_definitions(asio INTERFACE  _WIN32_WINNT=0x0601)
