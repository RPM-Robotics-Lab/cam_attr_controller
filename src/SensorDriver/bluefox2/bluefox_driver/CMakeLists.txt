# SOURCE FILES
set (SRC
  Bluefox2Config.cpp bluefox2.cpp bluefox2_setting.cpp
)

# externals
libhandler_opencv ()
libhandler_mvimpact()
libhandler_lcm ()
libhandler_eigen3 ()
## bluefox_driver
## -----------------------------------
add_library (Bluefox2Driver ${SRC})
target_link_libraries (Bluefox2Driver
  bot_util
  ${IRAP_LCMTYPES}
  ${IRPLIB_LCM}
  ${IRPLIB_MVIMPT}  
)

# then pod install
pods_install_libraries (Bluefox2Driver)

file(GLOB h_files *.h)
pods_install_headers(${h_files} DESTINATION Bluefox2Driver)