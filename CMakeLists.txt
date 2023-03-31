cmake_minimum_required(VERSION 3.16.3)
project(Barnes_Hut_Algoritm)

set(CMAKE_CXX_STANDARD 20)

find_library(UNIVERSAL universal)
find_package(ImGui-SFML REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(bha
        Source/main.cpp
        Source/Simulation/QuadTree/Node.cpp
        Source/Simulation/QuadTree/Node.h
        Source/Simulation/QuadTree/QuadTree.cpp
        Source/Simulation/QuadTree/QuadTree.h
        Source/Utils/Utils.cpp
        Source/Utils/Utils.h
        Source/Application/Camera.cpp
        Source/Application/Camera.h
        Source/Simulation/Entity/Entity.cpp
        Source/Simulation/Entity/Entity.h
        Source/Simulation/Simulation.cpp
        Source/Simulation/Simulation.h
        Source/Simulation/Entity/Tracker.cpp
        Source/Simulation/Entity/Tracker.h
        Source/Simulation/Entity/CircEntity.cpp
        Source/Simulation/Entity/CircEntity.h
        Source/Simulation/Preset/SimulationPresetSerializer.cpp
        Source/Simulation/Preset/SimulationPresetSerializer.h
        Source/Simulation/Preset/SimulationPreset.h
        Source/Simulation/Preset/SimulationPresetManager.cpp
        Source/Simulation/Preset/SimulationPresetManager.h
        Source/Simulation/Preset/SimulationPresetGenerator.cpp
        Source/Simulation/Preset/SimulationPresetGenerator.h
        Source/Simulation/Preset/SimulationPresetUtils.cpp
        Source/Simulation/Preset/SimulationPresetUtils.h
        Source/Application/Application.cpp
        Source/Application/Application.h
        Source/Simulation/Preset/SimulationPresetEditor.cpp
        Source/Simulation/Preset/SimulationPresetEditor.h)

target_link_libraries(bha PRIVATE ImGui-SFML::ImGui-SFML ${UNIVERSAL} nlohmann_json::nlohmann_json)

target_include_directories(bha PRIVATE Source)
