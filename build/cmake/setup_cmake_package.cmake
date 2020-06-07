if(NOT MAX_BUILD_JOBS)
  set(MAX_BUILD_JOBS 15)
endif()

# A simple function for importing a cmake-friendly buildable (and installable!) project
function(setup_cmake_package SRC_DIR BIN_DIR PKG_DIR CFG_TYPE CFG_ARGS)

  set(_CFG_ARGS -G ${CMAKE_GENERATOR})
  if(CMAKE_GENERATOR_PLATFORM)
    set(_CFG_ARGS ${_CFG_ARGS} -A ${CMAKE_GENERATOR_PLATFORM})
  endif()
  if(CMAKE_GENERATOR_TOOLSET)
    set(_CFG_ARGS ${_CFG_ARGS} -T ${CMAKE_GENERATOR_TOOLSET})
  endif()
  set(_CFG_ARGS ${_CFG_ARGS} ${CFG_ARGS})

  # configure step
  message(STATUS "configuring")
  execute_process(
    COMMAND ${CMAKE_COMMAND} -S ${SRC_DIR} -B ${BIN_DIR} -DCMAKE_INSTALL_PREFIX=${PKG_DIR} ${_CFG_ARGS}
    OUTPUT_FILE ${CMAKE_BINARY_DIR}/build_deps.log
    ERROR_FILE ${CMAKE_BINARY_DIR}/build_deps.log
    RESULT_VARIABLE ERR
  )

  if(ERR AND ${ERR})
    message(FATAL_ERROR "configure step failed")
  endif()

  foreach(CFG ${CFG_TYPE})

    # build step
    
    message(STATUS "building: ${CFG}")
    execute_process(
      COMMAND ${CMAKE_COMMAND} --build ${BIN_DIR} --config ${CFG} --parallel ${MAX_BUILD_JOBS}
      OUTPUT_FILE ${CMAKE_BINARY_DIR}/build_deps.log
      ERROR_FILE ${CMAKE_BINARY_DIR}/build_deps.log
      RESULT_VARIABLE ERR
    )

    if(ERR AND ${ERR})
      message(FATAL_ERROR "build step failed")
    endif()
    
    # install step
    message(STATUS "installing: ${CFG}")
    execute_process(
      COMMAND ${CMAKE_COMMAND} --install ${BIN_DIR} --config ${CFG}
      OUTPUT_FILE ${CMAKE_BINARY_DIR}/build_deps.log
      ERROR_FILE ${CMAKE_BINARY_DIR}/build_deps.log
    )

    if(ERR AND ${ERR})
      message(FATAL_ERROR "install step failed")
    endif()

  endforeach()

  file(REMOVE ${CMAKE_BINARY_DIR}/build_deps.log)
endfunction()
