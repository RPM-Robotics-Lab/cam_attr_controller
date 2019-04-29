# EXTERNAL DEPS
libhandler_opencv ()

# BUILD EXE
add_executable (img_synthesizer main.cpp)
target_link_libraries (img_synthesizer
  irp-imgeval++
  ${IRPLIB_OPENCV}
  )
