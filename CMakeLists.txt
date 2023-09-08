cmake_minimum_required(VERSION 3.22.0)
project(sen0251-server VERSION 0.1 LANGUAGES CXX)

option(USE_SUBMODULE "Use dependencies from submodule" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${CMAKE_BUILD_TYPE} EQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
else ()
  add_definitions("-DNODEBUG")
endif()

if(USE_SUBMODULE)
  set(WITH_MICRO_LOGGER_TESTS OFF)
  set(WITH_SEN0251_TESTS OFF)
  add_subdirectory(dependencies/micro-logger)
  include_directories(dependencies/micro-logger/includes)
  add_subdirectory(dependencies/dfrobot-sen0251)
  include_directories(dependencies/dfrobot-sen0251/include)
else()
  include(FindPkgConfig)
  pkg_check_modules(MICRO_LOGGER REQUIRED micro_logger)
  include_directories(${MICRO_LOGGER_INCLUDE_DIRS})
  pkg_check_modules(DFROBOT_SEN02051 REQUIRED dfrobot-sen0251)
  include_directories(${DFROBOT_SEN02051_INCLUDE_DIRS})
endif()

include_directories(include)
aux_source_directory(src SRC)

add_executable(${PROJECT_NAME} ${SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-li2c")
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC micro_logger++)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC dfrobot-sen0251)

