# EXTERNAL DEPS
libhandler_eigen3 ()

# BUILD EXE
add_executable (gp_exp_tester
    main.cpp
)
target_link_libraries (gp_exp_tester
  irp_gpgo++
  ${IRPLIB_EIGEN3}
)

## BUIlD LIB
#add_library (gp_optimize
#    gp_optimize.cpp gp_optimize.h
#)
#target_link_libraries(gp_optimize
#  ${IRPLIB_EIGEN3}
#)
