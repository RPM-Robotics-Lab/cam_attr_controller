# SOURCE FILES
set (SRC
  main.cpp 
  Parser.cpp     Parser.h
  param_util.cpp param_util.h
)


# externals
libhandler_opencv ()
libhandler_mvimpact()
libhandler_eigen3 ()
libhandler_boost (COMPONENTS program_options system filesystem)
libhandler_glib ()

add_executable (exp_ctrl ${SRC})
target_link_libraries (exp_ctrl
  Bluefox2Driver
  irp-imgeval++
  irp-common
  gp_optimize
  ${IRPLIB_BOOST}
  ${IRAP_LCMTYPES}
  ${IRPLIB_LCM}
  ${IRPLIB_MVIMPT}
)

