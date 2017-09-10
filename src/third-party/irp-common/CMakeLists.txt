# SOURCE FILES
add_definitions(-std=gnu99)

file(GLOB c_files *.c)
file(GLOB h_files *.h)

# EXTERNAL DEPS
libhandler_glib ()

# BUILD LIBRARY
add_library (irp-common ${c_files} ${h_files})
target_link_libraries (irp-common 
  ${IRPLIB_GLIB}
  )

pods_install_libraries(irp-common)

pods_install_headers(${h_files} DESTINATION irp-common)
