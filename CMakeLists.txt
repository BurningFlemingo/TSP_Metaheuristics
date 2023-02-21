cmake_minimum_required(VERSION 3.7)

project(TSP_Metaheuristics)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS  ${CMAKE_CXX_FLAGS} -O3)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/)

find_package(SDL2 REQUIRED)
include_directories(TSP_Metaheuristics ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm/glm/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/xlnt/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/xlnt/lib/)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/matplotplusplus/)

add_executable(TSP_Metaheuristics
    src/Main.cpp
    src/pch.h
    src/Utils.cpp
    src/Metaheuristic.cpp
    src/Application.cpp
    src/ICompute.cpp
    src/TSP.cpp
    src/ComputeShaders/SlimeMold.cpp
    src/ComputeShaders/AntColony.cpp
    src/Core/Window.cpp
    src/Core/EventHandler.cpp
    src/Renderer/Renderer.cpp
    src/Renderer/Shader.cpp
    src/Renderer/Shapes.cpp
    src/Renderer/VertexBufferLayout.cpp
    vendor/glad/src/glad.c
)
target_link_libraries(TSP_Metaheuristics PUBLIC SDL2::SDL2
    SDL2::SDL2main
    matplot
    xlnt)

