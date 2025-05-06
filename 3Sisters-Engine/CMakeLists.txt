cmake_minimum_required(VERSION 3.28.0)
set (CMAKE_CXX_STANDARD 20)
project(3Sisters-Engine VERSION 0.1.2 LANGUAGES C CXX)
cmake_policy(SET CMP0072 NEW)

# Set the install prefix to the build folder's lib directory
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}")

# Set the library output to the build folder's bin directory
set(CMAKE_LIBRARY_PREFIX "${CMAKE_BINARY_DIR}/bin")

# Grab all engine source files

set(GLAD
    src/glad/glad.c)

set(WINDOW
    src/window/sisters_glfw_window.cpp
    src/window/sisters_sdl_window.cpp)

set(STB 
    src/stb/stb_image.cpp
    src/stb/stb_truetype.cpp
    src/stb/stb_image_write.cpp)

set(CAMERAS 
    src/cameras/ortho_camera.cpp)

set(RESOURCESYS
    src/resourceSystems/resource_shader.cpp
    src/resourceSystems/resource_texture.cpp
    src/resourceSystems/managers/shader_manager.cpp
    src/resourceSystems/managers/texture_manager.cpp)

set(ECS 
    src/ecs/ecs.cpp
    src/ecs/managers/entity_manager.cpp)

set(INPUT
    src/input/sisters_sdl_gamepad.cpp
    src/input/sisters_glfw_gamepad.cpp
    src/input/sisters_sdl_keyboard.cpp
    src/input/sisters_glfw_keyboard.cpp)

set(SOUND 
    src/soundSystems/sound_source.cpp
    src/soundSystems/managers/sound_manager.cpp)

set(ENGINE 
    src/engine/line_renderer.cpp
    src/engine/quad_renderer.cpp
    src/engine/text_renderer.cpp)

# Create engine as a static library and add source files
add_library(3Sisters-Engine STATIC ${ENGINE} ${ECS} ${STB} ${CAMERAS} ${RESOURCESYS} ${INPUT} ${GLAD} ${WINDOW} ${SOUND})

# Find all dependencies needed to compile

# include subdirectory
add_subdirectory(vendor)

# include all dependencies directories
target_include_directories(3Sisters-Engine PRIVATE
    vendor/glfw-src/include
    vendor/glm-src
    vendor/openalsoft-src/include
    vendor/sdl-src/include
    vendor/sndfile-src/include
)

# include all dependencies
target_link_libraries(3Sisters-Engine glm glfw SDL3::SDL3-static OpenAL::OpenAL sndfile)

# Include directory for engine's headers
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)

# Include directories for header files
target_include_directories(3Sisters-Engine PUBLIC
$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
$<BUILD_INTERFACE:${ENGINE_INCLUDE_DIR}>
$<INSTALL_INTERFACE:include>
)

# Install the library and CMake configuration files
install(TARGETS 3Sisters-Engine EXPORT 3Sisters-EngineConfig
RUNTIME_DEPENDENCY_SET engine_depen
ARCHIVE DESTINATION ${CMAKE_LIBRARY_PREFIX}
LIBRARY DESTINATION ${CMAKE_LIBRARY_PREFIX}
RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Include all headers of the engine and dependecies (only the ones that don't get included)
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/inc/
    DESTINATION "${CMAKE_SOURCE_DIR}/build/include"
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/sdl-src/include/SDL3
    DESTINATION "${CMAKE_SOURCE_DIR}/build/include"
    FILES_MATCHING PATTERN "*.h"
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw-src/include/GLFW
    DESTINATION "${CMAKE_SOURCE_DIR}/build/include"
    FILES_MATCHING PATTERN "*.h"
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm-src/glm
    DESTINATION "${CMAKE_SOURCE_DIR}/build/include"
    FILES_MATCHING PATTERN "*.hpp"
)
