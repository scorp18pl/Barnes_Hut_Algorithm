cmake_minimum_required(VERSION 3.16.3)
project(Barnes_Hut_Algoritm)

set(CMAKE_CXX_STANDARD 20)

link_libraries(sfml-graphics sfml-system sfml-window)
file(GLOB SRC_FILES CONFIGURE_DEPENDS "Barnes_Hut_Algorithm/*.h" "Barnes_Hut_Algorithm/*.cpp")

add_executable(bha ${SRC_FILES})