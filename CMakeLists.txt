cmake_minimum_required(VERSION 3.10)
project(GameOfLife LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(SFML 3 REQUIRED COMPONENTS  Graphics Window System)

add_executable(gol game_of_life.cpp)

target_link_libraries(gol
    PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)