# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (Observer)

add_executable( Observer "main.cpp"  "Observer.h" "Observer.cpp" "Subject.h" "Subject.cpp" "FileLogger.h"  "DBLogger.h" )
