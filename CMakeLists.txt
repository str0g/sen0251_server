cmake_minimum_required(VERSION 3.5)
project(sen0251-server VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

add_subdirectory(dependecies/micro-logger)

include_directories(include)
include_directories(include dependecies/micro-logger/includes)
aux_source_directory(src SRC)

add_executable(${PROJECT_NAME} ${SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-li2c")
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC micro_logger++)

