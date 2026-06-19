# This file handles downloading all dependencies

# To be used to include all dependencies of the engine
include(FetchContent)
# Configure Fetch Content
set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/vendor CACHE PATH "Missing Desription." FORCE)
# Disable network updates for all dependencies after the initial fetch
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "" FORCE)

# show what process is being done, downloading dependencies
message(STATUS "Downloading and building engine dependencies")

FetchContent_Declare(
    sdl
    GIT_REPOSITORY https://github.com/Rihay66/SDL.git
    GIT_TAG main
    GIT_SHALLOW TRUE
)

# Set CMAKE arguments
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# ? setting CMAKE arguments for "SDL"
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)

# Make dependencies available
FetchContent_MakeAvailable(sdl)