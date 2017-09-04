cmake_minimum_required(VERSION 2.8)

# CMAKE TWEAKS
#========================================================================
cmake_minimum_required (VERSION 2.8)
cmake_policy (VERSION 2.8)
cmake_policy (SET CMP0002 OLD) # allow duplicate target names (non-Makefile  generators, e.g., Xcode or VS, will bonk)
set (CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
set (FIND_LIBRARY_USE_LIB64_PATHS 1)
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
    "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
endif ()

# INCLUDE LOCAL MODULES
#========================================================================
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "cmake")
file (GLOB MODULES "${CMAKE_MODULE_PATH}/*.cmake")
foreach (MODULE ${MODULES})
  include (${MODULE})
endforeach ()

find_package(PkgConfig REQUIRED)

# CONFIGURE C/C++
#========================================================================
set (CMAKE_C_CXX_COMMON_FLAGS "-Wall -Wfloat-equal -Wundef -Wno-strict-aliasing -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-macros")
set (CMAKE_C_FLAGS "-u MAIN__ -std=gnu99 -fopenmp ${CMAKE_C_CXX_COMMON_FLAGS}")
set (CMAKE_CXX_FLAGS "-u MAIN__ ${CMAKE_C_CXX_COMMON_FLAGS}")


# INCLUDE SRC
#========================================================================
add_subdirectory(src)
