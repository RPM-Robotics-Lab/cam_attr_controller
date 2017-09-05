# SOURCE FILES
set (HEADER 
  BotUtils.h
  TimeStamp.h
  ImageConversion.h
  Datatypes.h
#  EigenSophusConversion.h
)
set (SRC
  TimeStamp.cpp
  ImageConversion.cpp
#  EigenSophusConversion.cpp
)

# externals
#libhandler_tbb ()
libhandler_opencv ()
#libhandler_glib ()
libhandler_lcm ()
libhandler_eigen3 ()
#libhandler_boost (COMPONENTS system)
#libhandler_sophus ()

#libhandler_cholmod ()

#include_directories(${G2O_INCLUDE_DIR})
#set(IRPLIB_G2O ${G2O_STUFF_LIBRARY} ${G2O_CORE_LIBRARY} ${G2O_SOLVER_CHOLMOD} ${G2O_TYPES_SBA})

## Bot Utility
## -----------------------------------
add_library (bot_util ${HEADER} ${SRC})
target_link_libraries (bot_util
#   param-util
#   StereoUtility
  ${IRPLIB_OPENCV}
#   ${IRPLIB_TBB}
#   ${IRPLIB_G2O}
  ${IRAP_LCMTYPES}
)

# then pod install
pods_install_libraries (bot_util)

pods_install_headers(${HEADER} DESTINATION bot_util)
