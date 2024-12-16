cmake_minimum_required(VERSION 3.30)

project(hunting_season VERSION 0.1.0 LANGUAGES C CXX)

# Libraries to install
include(FetchContent)
set(RAYLIB_VERSION 5.0)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS ${RAYLIB_VERSION} EXACT
)
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(raylib)

# Our project
set(TO_COMPILE_FILES main.cpp)
option(DONT_SHOW_DETAILS OFF)

add_subdirectory(srcs)

add_executable(hunting_season ${TO_COMPILE_FILES})
target_link_libraries(hunting_season raylib classes)

if (WIN32)
    target_link_libraries(hunting_season -lwinmm -lgdi32 -lopengl32)
    if(DONT_SHOW_DETAILS)
        target_link_options(hunting_season PUBLIC -mwindows)
    endif()
endif()

target_include_directories(hunting_season PUBLIC "${PROJECT_BINARY_DIR}")

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)

add_dependencies(hunting_season copy_assets)