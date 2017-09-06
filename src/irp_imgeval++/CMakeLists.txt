# SOURCE FILES
set (SRC
    img_eval.h  img_eval.cpp
  )

# EXTERNAL DEPS
libhandler_opencv ()

# BUILD LIBRARY
add_library (irp-imgeval++ ${SRC})
target_link_libraries (irp-imgeval++
  ${IRPLIB_OPENCV}
  )

# then pod install
pods_install_libraries (irp-imgeval++)

file(GLOB HEADER *.h)
pods_install_headers(${HEADER} DESTINATION irp-imgeval++)
