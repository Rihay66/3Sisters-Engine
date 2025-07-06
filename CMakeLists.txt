cmake_minimum_required(VERSION 3.28.0)
set (CMAKE_CXX_STANDARD 20)
project(3Sisters-Engine-Testing VERSION 0.0.1 LANGUAGES C CXX)
cmake_policy(SET CMP0072 NEW)

# Include all files
file(GLOB SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/test/src/main.cpp
    ${CMAKE_SOURCE_DIR}/test/src/test_window.cpp
)

# Define resource folders
set(SHADERS_DIR ${CMAKE_SOURCE_DIR}/3Sisters-Engine/shaders)
set(FONTS_DIR ${CMAKE_SOURCE_DIR}/3Sisters-Engine/fonts)
set(TEXTURES_DIR "${CMAKE_SOURCE_DIR}/test/textures")
set(SOUNDS_DIR "${CMAKE_SOURCE_DIR}/test/sounds")

# Define the destination build folder
set(BUILD_DIR ${CMAKE_BINARY_DIR})

# Create folders and their sub files for runtime loading
# Function to copy files if they are different
function(copy_if_different SRC_DIR DEST_DIR)
    # Remove the existing destination directory
    if(EXISTS ${DEST_DIR})
        file(REMOVE_RECURSE ${DEST_DIR})
    endif()

    # Create the destination directory if it doesn't exist
    file(MAKE_DIRECTORY ${DEST_DIR})

    # Copy over the files
    file(GLOB_RECURSE FILES_LIST "${SRC_DIR}/*")
    foreach(FILE_PATH ${FILES_LIST})
        get_filename_component(FILE_NAME ${FILE_PATH} NAME)
        set(DEST_FILE_PATH "${DEST_DIR}/${FILE_NAME}")
        configure_file(${FILE_PATH} ${DEST_FILE_PATH} COPYONLY)
    endforeach()
endfunction()

# include Engine
add_subdirectory(3Sisters-Engine)

# Check if detected OS is LINUX
if(UNIX)

# Create executable
add_executable(3Sisters-Engine-Testing
${SOURCE_FILES}
)

# Call the function for each resource folder
copy_if_different(${SHADERS_DIR} ${BUILD_DIR}/shaders)
copy_if_different(${TEXTURES_DIR} ${BUILD_DIR}/textures)
copy_if_different(${SOUNDS_DIR} ${BUILD_DIR}/sounds)
copy_if_different(${FONTS_DIR} ${BUILD_DIR}/fonts)

# Link the extra packages their include directories to avoid linking errors
target_include_directories(3Sisters-Engine-Testing PRIVATE 3Sisters-Engine)
# Link the 3Sisters-Engine to executable
target_link_libraries(3Sisters-Engine-Testing PRIVATE 3Sisters-Engine)

endif ()

if(WIN32)

# Create executable
add_executable(3Sisters-Engine-Testing
${SOURCE_FILES}
)

# Call the function for each resource folder
copy_if_different(${SHADERS_DIR} ${BUILD_DIR}/shaders)
copy_if_different(${TEXTURES_DIR} ${BUILD_DIR}/textures)
copy_if_different(${SOUNDS_DIR} ${BUILD_DIR}/sounds)
copy_if_different(${FONTS_DIR} ${BUILD_DIR}/fonts)

if(MSVC) # msvc

# Link the extra packages their include directories to avoid linking errors
target_include_directories(3Sisters-Engine-Testing PRIVATE 3Sisters-Engine)
# Link the 3Sisters-Engine to executable
target_link_libraries(3Sisters-Engine-Testing PRIVATE 3Sisters-Engine)

else () # mingw

# Link the extra packages their include directories to avoid linking errors
target_include_directories(3Sisters-Engine-Testing PRIVATE 3Sisters-Engine)
# Link the 3Sisters-Engine to executable
target_link_libraries(3Sisters-Engine-Testing PRIVATE 3Sisters-Engine)

endif()

endif()