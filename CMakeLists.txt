cmake_minimum_required(VERSION 3.7)

project(sdljam)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/data.cpp
    src/renderer.cpp
    src/audio.cpp
    src/level.cpp
    src/game.cpp
    src/game/factory.cpp
    # src/inputs.cpp
    )

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2MIXER_LIBRARIES})

