cmake_minimum_required(VERSION 3.24)
project(DiscreateMath)

set(CMAKE_CXX_STANDARD 20)

add_executable(DiscreateMath main.cpp)

add_subdirectory(Decode)
add_subdirectory(Encode)

target_link_libraries(${PROJECT_NAME} PUBLIC Encode)
target_link_libraries(${PROJECT_NAME} PUBLIC Decode)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})