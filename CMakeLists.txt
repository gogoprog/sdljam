cmake_minimum_required(VERSION 3.10)

project(sdljam)

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

target_compile_features(sdljam PUBLIC cxx_std_20)

if(WIN32)
    set(SDL2MIXER_INCLUDE_DIRS deps/SDL2_mixer-2.0.4/include)
    set(SDL2MIXER_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/deps/SDL2_mixer-2.0.4/lib/x64/SDL2_mixer.lib")
    set(SDL2_INCLUDE_DIRS deps/SDL2-2.0.20/include)
    set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/deps/SDL2-2.0.20/lib/x64/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/deps/SDL2-2.0.20/lib/x64/SDL2main.lib")
endif()

if(EMSCRIPTEN)
    target_compile_definitions(sdljam PUBLIC EMSCRIPTEN)
    set(USE_FLAGS "-s USE_SDL=2 -s USE_SDL_MIXER=2")
    set(PRELOAD_FLAGS "--preload-file ${CMAKE_CURRENT_LIST_DIR}/res@/res --shell-file ${CMAKE_CURRENT_LIST_DIR}/deps/shell.html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS} ${PRELOAD_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
elseif(UNIX)
    find_package(SDL3 REQUIRED CONFIG)
    find_package(SDL3_mixer REQUIRED CONFIG)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_mixer::SDL3_mixer)
endif()
