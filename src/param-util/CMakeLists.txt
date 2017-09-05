# externals
libhandler_boost (COMPONENTS system)

add_library(param-util param_util.cpp)

pods_install_libraries(param-util)
file(GLOB h_files *.h)
pods_install_headers(${h_files} DESTINATION param-util)
