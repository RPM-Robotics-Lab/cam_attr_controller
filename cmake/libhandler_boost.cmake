macro (libhandler_boost)

  set (Boost_USE_STATIC_LIBS        ON)
  set (Boost_USE_MULTITHREADED      ON)
  set (Boost_USE_STATIC_RUNTIME    OFF)
  set (Boost_DETAILED_FAILURE_MSG   ON)

  foreach (arg ${ARGN})
    if ("${arg}" MATCHES "COMPONENTS")
      # continue
    else () # pass to find_library()
      if (COMPONENTS)
        set (COMPONENTS "${COMPONENTS} ${arg}")
        set (DEB_PACKAGES "${DEB_PACKAGES} libboost-${arg}-dev") # debian package name formatting assumes
                                                                 # that programmer passed component name correctly
      else ()
        set (COMPONENTS "${arg}")
        set (DEB_PACKAGES "libboost-${arg}-dev")
      endif ()
    endif ()
  endforeach ()

  if (COMPONENTS)
    string (REPLACE "_" "-" DEB_PACKAGES ${DEB_PACKAGES})
    libhandler_find_package (Boost "on ubuntu `sudo apt-get install libboost-dev ${DEB_PACKAGES}`" ${ARGN})
  else ()
    libhandler_find_package (Boost "on ubuntu `sudo apt-get install libboost-dev`" ${ARGN})
  endif ()

  if (Boost_FOUND)
    include_directories (${Boost_INCLUDE_DIRS})
    set (IRPLIB_BOOST ${Boost_LIBRARIES})
    mark_as_advanced (Boost_DIR)
  endif ()
endmacro ()
