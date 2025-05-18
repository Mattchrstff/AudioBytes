# Minimum CMake version required
cmake_minimum_required(VERSION 3.22.1)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Set the project name
project("audiobytesadroid")

# Create the shared library from native-lib.cpp
add_library(${CMAKE_PROJECT_NAME} SHARED
        native-lib.cpp
)

# Find the Android log library
find_library(
        log-lib
        log
)

# ✅ Link the correct C++ standard library (shared)
# You can switch to c++_static if you want to statically link it instead
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lc++_shared")

# Link the libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        ${log-lib}
        c++_shared
)
