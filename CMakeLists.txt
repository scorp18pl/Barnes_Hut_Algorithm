cmake_minimum_required(VERSION 3.16.3)
project(Barnes_Hut_Algoritm)

set(CMAKE_CXX_STANDARD 20)

find_library(UNIVERSAL universal)
find_package(ImGui-SFML REQUIRED)
message("${UNIVERSAL}")

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(bha
        Source/main.cpp
        Source/QuadTree/Node.cpp
        Source/QuadTree/Node.h
        Source/QuadTree/QuadTree.cpp
        Source/QuadTree/QuadTree.h
        Source/Utils/Utils.cpp
        Source/Utils/Utils.h
        Source/Camera.cpp
        Source/Camera.h
        Source/Entity/Entity.cpp
        Source/Entity/Entity.h
        Source/Simulation.cpp
        Source/Simulation.h
        Source/Tracker.cpp
        Source/Tracker.h
        Source/Entity/CircEntity.cpp
        Source/Entity/CircEntity.h
        Source/SimulationPreset/SimulationPresetSerializer.cpp
        Source/SimulationPreset/SimulationPresetSerializer.h
        Source/SimulationPreset/SimulationPreset.h
        )

target_link_libraries(bha PRIVATE ImGui-SFML::ImGui-SFML ${UNIVERSAL} nlohmann_json::nlohmann_json)

target_include_directories(bha PRIVATE Source)
