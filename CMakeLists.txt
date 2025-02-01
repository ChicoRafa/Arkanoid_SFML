cmake_minimum_required(VERSION 3.23)
project(Arkanoid LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)
FetchContent_Declare(
    TMXLITE
    GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
    GIT_TAG v1.4.4
    SOURCE_SUBDIR tmxlite)
FetchContent_MakeAvailable(TMXLITE)
include_directories(${tmxlite_SOURCE_DIR}/include)

set(PROJECT_HEADERS
    include/Utils/Constants.h

    include/Core/AssetManager.h
    include/Core/LevelManager.h
    include/Core/SoundManager.h
    include/Core/Game.h
    include/Core/World.h

    include/Gameplay/Ball.h
    include/Gameplay/Powerups.h
    include/Gameplay/GameObject.h
    include/Gameplay/PlayerController.h
    include/Gameplay/Paddle.h

    include/Render/SFMLOrthogonalLayer.h
    
    include/UI/UIScreenMainMenu.h
)

set(PROJECT_SOURCES

    src/Core/AssetManager.cpp
    src/Core/LevelManager.cpp
    src/Core/SoundManager.cpp
    src/Core/Game.cpp
    src/Core/World.cpp

    src/Gameplay/Ball.cpp
    src/Gameplay/Powerups.cpp
    src/Gameplay/PlayerController.cpp
    src/Gameplay/Paddle.cpp

    src/main.cpp
)

set(PROJECT_FILES
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

include_directories(${PROJECT_NAME} PRIVATE include)

add_executable(${PROJECT_NAME} ${PROJECT_FILES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    sfml-graphics
    sfml-audio
    tmxlite)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM)
endif()
