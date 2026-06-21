cmake_minimum_required(VERSION 3.28.0)
set (CMAKE_C_STANDARD 17)
project(3Sisters-Engine-App VERSION 1.0.0 LANGUAGES C)

# Include all files
file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/app/*
)

# Define the destination build folder
set(BUILD_DIR ${CMAKE_BINARY_DIR})

# Define the destination for LUA scripts
set(LUA_SCRIPTS_DIR ${CMAKE_SOURCE_DIR}/scripts)

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

copy_if_different(${LUA_SCRIPTS_DIR} ${BUILD_DIR}/)

# Create executable
add_executable(3Sisters-Engine-App
${SOURCE_FILES}
)

# Link the extra packages their include directories to avoid linking errors
target_include_directories(3Sisters-Engine-App PRIVATE 3Sisters-Engine)
# Link the 3Sisters-Engine to executable
target_link_libraries(3Sisters-Engine-App PRIVATE 3Sisters-Engine)
