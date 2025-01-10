cmake_minimum_required(VERSION 3.10)
project(RaylibProject)

# Set C standard
set(CMAKE_CXX_STANDARD 20)

# Define source files
set(SOURCE_FILES
        src/main.cc
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Platform-specific configuration
if(WIN32)
    # Set paths for Raylib downloaded from GitHub
    set(RAYLIB_DIR "${CMAKE_SOURCE_DIR}/lib/raylib-5.5_win64_mingw-w64")  # Replace with your actual path
    target_include_directories(${PROJECT_NAME} PRIVATE "${RAYLIB_DIR}/include")
    target_link_directories(${PROJECT_NAME} PRIVATE "${RAYLIB_DIR}/lib")

    # Link Raylib and dependencies
    target_link_libraries(${PROJECT_NAME} raylib winmm)
else()
    # Require Raylib 5.5 or newer
    find_package(raylib 5.5 REQUIRED)

    # Link Raylib
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
endif()