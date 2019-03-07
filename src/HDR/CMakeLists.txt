# EXTERNAL DEPS
libhandler_opencv ()
# BUILD EXE
add_executable (HDR hdr.cpp)
target_link_libraries (HDR
  irp-imgeval++
  ${IRPLIB_OPENCV}
  )
