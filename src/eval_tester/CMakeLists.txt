# EXTERNAL DEPS
libhandler_eigen3 ()

# BUILD EXE
add_executable (eval_tester
    main.cpp
)
target_link_libraries (eval_tester
  irp-imgeval++
  ${IRPLIB_OPENCV}
)

## BUIlD LIB
#add_library (gp_optimize
#    gp_optimize.cpp gp_optimize.h
#)
#target_link_libraries(gp_optimize
#  ${IRPLIB_EIGEN3}
#)
