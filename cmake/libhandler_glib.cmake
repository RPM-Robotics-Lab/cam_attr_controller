macro (libhandler_glib)
  # note, not using libhandler_find_package() here, b/c we are only
  # checking for the GLIB *subset* of the GTK package
  if (NOT GTK2_GLIB_LIBRARY)
    find_package (GTK2)
  endif ()

  if (GTK2_GLIB_LIBRARY)
    include_directories (${GTK2_GLIB_INCLUDE_DIR} ${GTK2_GLIBCONFIG_INCLUDE_DIR})
    set (GLIB_FOUND TRUE)
    set (IRPLIB_GLIB ${GTK2_GLIB_LIBRARY})
  else ()
    libhandler_error ("on ubuntu `sudo apt-get install libglib2.0-dev`")
  endif ()
endmacro ()
