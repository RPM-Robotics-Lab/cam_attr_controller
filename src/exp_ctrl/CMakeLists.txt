# EXTERNAL DEPS
libhandler_opencv ()

# BUILD EXE
add_executable (exp_ctrl main.cpp)
target_link_libraries (exp_ctrl
  ${IRPLIB_OPENCV}
  )
