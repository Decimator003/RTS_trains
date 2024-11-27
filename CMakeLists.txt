cmake_minimum_required(VERSION 3.10)
project(rts_trains)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the Allegro5 directory explicitly
set(ALLEGRO5_DIR "/usr/lib/x86_64-linux-gnu")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${ALLEGRO5_DIR})

# Add pkg-config support
find_package(PkgConfig REQUIRED)

# Find Allegro and its components
pkg_check_modules(ALLEGRO5 REQUIRED
    allegro-5
    allegro_primitives-5
    allegro_font-5
)

# Include directories
include_directories(
    ${ALLEGRO5_INCLUDE_DIRS}
    include
)

# Collect source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(train_station ${SOURCES})

# Link libraries
target_link_libraries(train_station
    ${ALLEGRO5_LIBRARIES}
)

# Add compiler flags
target_compile_options(train_station PRIVATE
    ${ALLEGRO5_CFLAGS_OTHER}
)

# Set library search paths
link_directories(
    ${ALLEGRO5_LIBRARY_DIRS}
)

# Print debug information (optional, can be removed later)
#message(STATUS "Allegro Include Dirs: ${ALLEGRO5_INCLUDE_DIRS}")
#message(STATUS "Allegro Libraries: ${ALLEGRO5_LIBRARIES}")
#message(STATUS "Allegro Library Dirs: ${ALLEGRO5_LIBRARY_DIRS}")
