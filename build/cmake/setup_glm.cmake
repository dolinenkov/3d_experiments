# glm package is provided by git repository and provides exported target glm::glm
message(STATUS "searching for glm")
find_package(glm CONFIG NO_DEFAULT_PATH PATHS ${CMAKE_SOURCE_DIR}/externals/glm/cmake)
