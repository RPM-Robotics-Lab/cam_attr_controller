# SOURCE FILES
set (BLUEFOX2DRV_SRC
  bluefox2-drv.cpp Bluefox2Parser.cpp
)

# externals
libhandler_opencv ()
libhandler_mvimpact()
libhandler_lcm ()
libhandler_boost (COMPONENTS program_options)

## bluefox2 driver
## -----------------------------------
add_executable (bluefox2-drv ${BLUEFOX2DRV_SRC})
target_link_libraries (bluefox2-drv
  Bluefox2Driver
  param-util
  ${IRPLIB_BOOST}  
  ${IRAP_LCMTYPES}
  ${IRPLIB_LCM}
  ${IRPLIB_MVIMPT}  
)

# then pod install
pods_install_executables (bluefox2-drv)

