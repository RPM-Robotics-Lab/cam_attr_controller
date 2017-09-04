# EXTERNAL DEPS
libhandler_opencv ()

# BUILD EXE
add_executable (exp_tester main.cpp)
target_link_libraries (exp_tester
  ${IRPLIB_OPENCV}
  )
