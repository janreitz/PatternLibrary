# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Include sub-projects.
add_subdirectory ("adapter")
add_subdirectory ("observer")
add_subdirectory ("observerCallback")
add_subdirectory ("smartPointer")