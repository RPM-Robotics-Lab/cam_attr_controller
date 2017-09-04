# helpful functions for finding irap external dependencies

#===============================================================================
macro (libhandler_error)
  message (FATAL_ERROR "${ARGN}\n") # wrapper so that all libhandler error messages have same level
endmacro ()

#===============================================================================
macro (libhandler_warning)
  message (AUTHOR_WARNING "${ARGN}\n") # wrapper so that all libhandler error messages have same level
endmacro ()

#===============================================================================
macro (libhandler_find_library LIB INSTALLMSG)
  string (TOUPPER ${LIB} LIBUP)
  set (IS_REQUIRED true)
  foreach(arg ${ARGN})
    if ("${arg}" MATCHES "OPTIONAL")
      set (IS_REQUIRED false)
    else () # pass to find_library()
      if (${LIBARGN})
        set(LIBARGN "${LIBARGN} ${arg}")
      else ()
        set(LIBARGN "${arg}") # No space in front of PKGARGN
      endif()
    endif()
  endforeach(arg ${ARGN})
  
  # no need to try again if already found
  if (NOT ${LIBUP}_LIBRARIES)
    find_library (${LIBUP}_LIBRARIES ${LIB} # mimick find_package lib var name
      DOC "Fullpath to ${LIB} library."
      ${LIBARGN}
      )
    mark_as_advanced (FORCE ${LIBUP}_LIBRARIES)
  endif ()

  if (${LIBUP}_LIBRARIES)
    set (${LIBUP}_FOUND TRUE)
  else ()
    set (${LIBUP}_FOUND FALSE)
      if (IS_REQUIRED)
        libhandler_error ("lib${LIB} not found; ${INSTALLMSG}")
      endif()
  endif ()

endmacro ()

#===============================================================================
macro (libhandler_find_package PKG INSTALLMSG)
  string (TOUPPER ${PKG} PKGUP)

  set (IS_REQUIRED true)
  set (PKGARGN "")
  foreach(arg ${ARGN})
    if ("${arg}" MATCHES "OPTIONAL")
      set (IS_REQUIRED false)
    else () # pass to find_package()
      if (PKGARGN)
        set(PKGARGN ${PKGARGN} ${arg})
      else ()
        set(PKGARGN ${arg}) # No space in front of PKGARGN
      endif()
    endif()
  endforeach(arg ${ARGN})

  # no need to try again if already found
  if ( (NOT ${PKG}_FOUND) AND (NOT ${PKGUP}_FOUND) )
    find_package (${PKG} ${PKGARGN})
  endif ()

  if ( (NOT ${PKG}_FOUND) AND (NOT ${PKGUP}_FOUND) AND ${IS_REQUIRED})
    libhandler_error ("package ${PKG} not found; ${INSTALLMSG}")
  else ()
    set (${PKGUP}_FOUND TRUE)
  endif ()
endmacro ()

# =============================================================================== 
macro (libhandler_find_python_module PKG INSTALLMSG)
  libhandler_pythoninterp ()
  execute_process(COMMAND "${PYTHON_EXECUTABLE}" "-c"
    "import ${PKG}"
    RESULT_VARIABLE result
    OUTPUT_QUIET
    ERROR_QUIET)
  set (MODULE_FOUND {"${result}" MATCHES "0" })
  if (NOT ${result} EQUAL 0)
    libhandler_error ("could not find python module ${PKG}; ${INSTALLMSG}")
  endif ()
endmacro ()

macro (libhandler_find_program CMD INSTALLMSG)
  find_program (IRPLIB_FIND_PROG_TMP "${CMD}")
  if (${IRPLIB_FIND_PROG_TMP} MATCHES ".*NOTFOUND")
    unset (IRPLIB_FIND_PROG_TMP CACHE)
    libhandler_error ("could not find program ${CMD}; ${INSTALLMSG}")
  endif ()
  unset (IRPLIB_FIND_PROG_TMP CACHE)
endmacro ()
