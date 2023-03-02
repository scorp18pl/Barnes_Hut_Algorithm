cmake_minimum_required(VERSION 3.16.3)
project(Barnes_Hut_Algoritm)

set(CMAKE_CXX_STANDARD 20)

find_library(UNIVERSAL universal)
find_package(ImGui-SFML REQUIRED)
message("${UNIVERSAL}")

file(GLOB SRC_FILES CONFIGURE_DEPENDS "Barnes_Hut_Algorithm/*.h" "Barnes_Hut_Algorithm/*.cpp")

add_executable(bha ${SRC_FILES})

target_link_libraries(bha ImGui-SFML::ImGui-SFML ${UNIVERSAL})

target_include_directories(bha PRIVATE Barnes_Hut_Algorithm)
