# SOURCE FILES
#set (SRC
#  bluefox2-mono.cpp Parser.cpp
#)

# externals
libhandler_opencv ()
libhandler_mvimpact()
libhandler_eigen3 ()
libhandler_boost (COMPONENTS program_options system filesystem)

# calib data acquisition
set (BLUEFOX2CALIB_SRC
  bluefox2calib_daq.cpp Parser.cpp
)
add_executable (expos_adjust ${BLUEFOX2CALIB_SRC})
target_link_libraries (expos_adjust
	Bluefox2Driver
	param-util
	${IRPLIB_BOOST}
	${IRAP_LCMTYPES}
  ${IRPLIB_LCM}
	${IRPLIB_MVIMPT}
)

pods_install_executables (expos_adjust)
