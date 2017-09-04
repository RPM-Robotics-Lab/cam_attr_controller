# SOURCE FILES
set (HEADER 
  img_eval.h
)
set (SRC
  img_eval.cpp
  main.cpp
)

# EXTERNAL DEPS
libhandler_opencv ()

# BUILD EXE
add_executable (exp_tester img_eval.cpp main.cpp)
target_link_libraries (exp_tester
  ${IRPLIB_OPENCV}
  )
