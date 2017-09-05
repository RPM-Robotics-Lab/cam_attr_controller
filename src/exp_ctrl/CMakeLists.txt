# SOURCE FILES
set (SRC
  main.cpp Parser.cpp
)

# externals
libhandler_opencv ()
libhandler_mvimpact()
#libhandler_lcm ()
libhandler_eigen3 ()
libhandler_boost (COMPONENTS program_options system filesystem)

# BUILD EXE
add_executable (exp_ctrl ${SRC})
target_link_libraries (exp_ctrl
    Bluefox2Driver
    param-util
    ${IRPLIB_BOOST}
    ${IRAP_LCMTYPES}
    ${IRPLIB_LCM}
    ${IRPLIB_MVIMPT}
)
