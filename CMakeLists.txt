﻿# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
# enable C and C++ language
project (vkRenderer)

cmake_minimum_required (VERSION 3.8)

enable_testing()

# Include sub-projects.
add_subdirectory (Sandbox)
add_subdirectory (vkRenderer)
