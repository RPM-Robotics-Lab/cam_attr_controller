# SOURCE FILES
set (SRC
    gp_optimize.h gp_optimize.cpp
    config.h
    kernelfunction.h kernelfunction.cpp
)

libhandler_eigen3 ()

# BUIlD LIB
add_library (gp_optimize ${SRC})
target_link_libraries(gp_optimize
  ${IRPLIB_EIGEN3}
)

# then pod install
pods_install_libraries (gp_optimize)

file(GLOB HEADER *.h)
pods_install_headers(${HEADER} DESTINATION gp_optimize)
