cmake_minimum_required(VERSION 3.7)

project(TSP_Metaheuristics)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
include_directories(TSP_Metaheuristics ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glad/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glm/glm/)

add_executable(TSP_Metaheuristics
    Main.cpp
    pch.h
    Utils.cpp
    Metaheuristic.cpp
    Application.cpp
    GravityParticleSystem.cpp
    Core/Window.cpp
    Core/EventHandler.cpp
    Renderer/Renderer.cpp
    Renderer/Shader.cpp
    Renderer/Shapes.cpp
    Renderer/VertexBufferLayout.cpp
    ../vendor/glad/src/glad.c
    TSP/TSP.cpp
)
target_link_libraries(TSP_Metaheuristics PUBLIC SDL2::SDL2 SDL2::SDL2main)
