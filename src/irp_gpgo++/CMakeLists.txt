# SOURCE FILES
set (SRC
    gp_optimize.h gp_optimize.cpp
    config.h
    kernelfunction.h kernelfunction.cpp
)

libhandler_eigen3 ()

# BUIlD LIB
add_library (irp_gpgo++ ${SRC})
target_link_libraries(irp_gpgo++
  ${IRPLIB_EIGEN3}
)

# then pod install
pods_install_libraries (irp_gpgo++)

file(GLOB HEADER *.h)
pods_install_headers(${HEADER} DESTINATION irp_gpgo++)
