# This file handles downloading all dependencies

# To be used to include all dependencies of the engine
include(FetchContent)
# Configure Fetch Content
set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/vendor CACHE PATH "Missing Desription." FORCE)

# show what process is being done, downloading dependencies
message(STATUS "Downloading and building engine dependencies")

# Fetch all dependencies
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/Rihay66/glfw.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/Rihay66/glm.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)
FetchContent_Declare(
    sndfile
    GIT_REPOSITORY https://github.com/Rihay66/libsndfile.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)
FetchContent_Declare(
    sdl
    GIT_REPOSITORY https://github.com/Rihay66/SDL.git
    GIT_TAG main
    GIT_SHALLOW TRUE
)
FetchContent_Declare(
    openalsoft
    GIT_REPOSITORY https://github.com/Rihay66/openal-soft.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)

# get openal soft properties
FetchContent_GetProperties(openalsoft)

# Set CMAKE arguments
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# ? setting CMAKE arguments for "GLFW"
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES Off CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS Off CACHE BOOL "" FORCE)
set(GLFW_INSTALL Off CACHE BOOL "" FORCE)

# ? setting CMAKE arguments for "GLM"
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)

# ? Setting CMAKE arguments for "sndfile", the set arguments may or may not work
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

# ! Currently loading MP3s and other libraries like FLAC are not available, will be implement at a later time
set(ENABLE_MPEG OFF CACHE BOOL "" FORCE)
set(ENABLE_EXTERNAL_LIBS OFF CACHE BOOL "" FORCE)

# ? setting CMAKE arguments for "SDL"
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)

# ? setting CMAKE arguments for "OPENAL"
set(ALSOFT_UTILS OFF CACHE BOOL "" FORCE)
set(ALSOFT_TESTS OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_UTILS OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_CONFIG OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_HRTF_DATA OFF CACHE BOOL "" FORCE)
set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LIBTYPE STATIC CACHE INTERNAL "" FORCE)

# Make dependencies available
FetchContent_MakeAvailable(glfw glm sndfile openalsoft sdl)