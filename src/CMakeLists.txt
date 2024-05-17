set(CMAKE_CXX_STANDARD 11)

find_package(Catch2 3 REQUIRED)
add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

cmake_minimum_required(VERSION 3.29.3)

project(matching-engine)

add_executable(${PROJECT_NAME} main.cpp)
