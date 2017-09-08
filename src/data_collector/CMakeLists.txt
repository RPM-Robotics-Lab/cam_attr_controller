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

add_executable (data_collector ${SRC})
target_link_libraries (data_collector
  Bluefox2Driver
  irp-imgeval++
  ${IRPLIB_BOOST}
  ${IRAP_LCMTYPES}
  ${IRPLIB_LCM}
  ${IRPLIB_MVIMPT}
)

