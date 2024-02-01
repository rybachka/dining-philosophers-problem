cmake_minimum_required(VERSION 3.26)
project(Dinning_philosophers_problem)

set(CMAKE_CXX_STANDARD 17)

add_executable(Dinning_philosophers_problem main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(Dinning_philosophers_problem sfml-system sfml-window sfml-graphics sfml-audio sfml-network)