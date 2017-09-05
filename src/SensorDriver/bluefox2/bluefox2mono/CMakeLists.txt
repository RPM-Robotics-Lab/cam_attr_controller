# SOURCE FILES
set (SRC
  bluefox2-mono.cpp Parser.cpp
)

# externals
libhandler_opencv ()
libhandler_mvimpact()
libhandler_lcm ()
libhandler_eigen3 ()
libhandler_boost (COMPONENTS program_options system filesystem)

## power controller
## -----------------------------------
add_executable (bluefox2mono ${SRC})
target_link_libraries (bluefox2mono
  Bluefox2Driver
  param-util
  ${IRPLIB_BOOST}  
  ${IRAP_LCMTYPES}
  ${IRPLIB_LCM}
  ${IRPLIB_MVIMPT}  
)

# then pod install
pods_install_executables (bluefox2mono)



# calib data acquisition
set (BLUEFOX2CALIB_SRC
  bluefox2calib_daq.cpp Parser.cpp
)
add_executable (calib_daq ${BLUEFOX2CALIB_SRC})
target_link_libraries (calib_daq
	Bluefox2Driver
	param-util
	${IRPLIB_BOOST}
	${IRAP_LCMTYPES}
  ${IRPLIB_LCM}
	${IRPLIB_MVIMPT}
)

pods_install_executables (calib_daq)
