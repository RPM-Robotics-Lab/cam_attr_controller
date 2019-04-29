# EXTERNAL DEPS
libhandler_eigen3 ()

# BUILD EXE
add_executable (img_evaluator
    main.cpp
)
target_link_libraries (img_evaluator
  irp-imgeval++
  ${IRPLIB_OPENCV}
)
