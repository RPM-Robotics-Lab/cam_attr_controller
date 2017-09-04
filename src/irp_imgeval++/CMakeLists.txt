# SOURCE FILES
set (SRC
    img_eval.h  img_eval.cpp
  )

file(GLOB h_files *.h)

# EXTERNAL DEPS
libhandler_opencv ()

# BUILD LIBRARY
add_library (irp-imgeval++ ${SRC})
target_link_libraries (irp-imgeval++
  ${IRPLIB_OPENCV}
  )
