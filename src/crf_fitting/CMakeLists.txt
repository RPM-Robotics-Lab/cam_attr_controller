# EXTERNAL DEPS
libhandler_opencv ()
# BUILD EXE
add_executable (crf_fitting crf_fitting.cpp)
target_link_libraries (crf_fitting
  irp-imgeval++
  ${IRPLIB_OPENCV}
  )
