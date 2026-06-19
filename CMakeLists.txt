cmake_minimum_required(VERSION 3.28.0)
set (CMAKE_C_STANDARD 17)
project(3Sisters-Engine-App VERSION 1.0.0 LANGUAGES C)

# Include all files
file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/app/*
)

# Define the destination build folder
set(BUILD_DIR ${CMAKE_BINARY_DIR})

# include Engine
add_subdirectory(3Sisters-Engine)

# Create executable
add_executable(3Sisters-Engine-App
${SOURCE_FILES}
)

# Link the extra packages their include directories to avoid linking errors
target_include_directories(3Sisters-Engine-App PRIVATE 3Sisters-Engine)
# Link the 3Sisters-Engine to executable
target_link_libraries(3Sisters-Engine-App PRIVATE 3Sisters-Engine)
