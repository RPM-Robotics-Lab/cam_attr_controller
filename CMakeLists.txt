cmake_minimum_required(VERSION 2.8)

# CMAKE TWEAKS
#========================================================================
cmake_minimum_required (VERSION 2.8)
cmake_policy (VERSION 2.8)
cmake_policy (SET CMP0054 OLD)

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
set (CMAKE_C_CXX_COMMON_FLAGS "-Wall -Wfloat-equal -Wundef -Wno-strict-aliasing -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-macros -Wno-unknown-pragmas")
set (CMAKE_C_FLAGS "-u MAIN__ -std=gnu99 -fopenmp ${CMAKE_C_CXX_COMMON_FLAGS}")
set (CMAKE_CXX_FLAGS "-u MAIN__ -std=gnu++11 -fopenmp -msse4.1 ${CMAKE_C_CXX_COMMON_FLAGS}")


# CONFIG FILES
#========================================================================
set (CONFIG_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/config)

add_custom_target (config ALL
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/build/share
  COMMAND ln -sf ${CONFIG_SOURCE_PATH} ${CMAKE_SOURCE_DIR}/build/share/.
)

install (DIRECTORY ${CONFIG_SOURCE_PATH}
  DESTINATION ${CMAKE_SOURCE_DIR}/build/share/.
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ WORLD_WRITE
  DIRECTORY_PERMISSIONS  OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE  GROUP_EXECUTE WORLD_READ WORLD_WRITE WORLD_EXECUTE
  PATTERN ".svn" EXCLUDE
  )

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
set(CMAKE_GENERATOR "Unix Makefiles" CACHE INTERNAL "" FORCE)
set(CMAKE_EXTRA_GENERATOR "Sublime Text 2" CACHE INTERNAL "" FORCE)


# Gen and cook lcmtypes
#========================================================================
include(${PROJECT_SOURCE_DIR}/lcmdefs/PACKAGES.cmake)
lcmtypes_build()

# INCLUDE COMMON LIBRARIES
#========================================================================
include_directories(${PROJECT_SOURCE_DIR}/src/)



# OUTPUT DIRECTORIES
#========================================================================
# set where files should be output locally
set (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
if (NOT IS_DIRECTORY ${LIBRARY_OUTPUT_PATH})
  file (MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH})
endif ()

set (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
if (NOT IS_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
  file (MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
endif ()


# INCLUDE SRC
#========================================================================
add_subdirectory(src)
