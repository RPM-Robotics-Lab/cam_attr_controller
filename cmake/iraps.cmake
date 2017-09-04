################################################################################
# GENERAL GROUP/TARGET UTILS (SRC/JAVA/PYTHON)
################################################################################

#===============================================================================
function (_irap_update_active_libraries)
  # update list of active libraries
  set (IRAP_LIBRARIES ${IRAP_LIBRARIES_ALL})
  foreach (LIB ${IRAP_LIBRARIES_ALL})
    list (FIND IRAP_LIBRARIES_ALL ${LIB} INDEX)
    if (INDEX GREATER -1)
      list (GET IRAP_LIBRARIES_GROUP_OPTNAMES ${INDEX} GROUP_OPTNAME)
      list (GET IRAP_LIBRARIES_TARGET_OPTNAMES ${INDEX} TARGET_OPTNAME)
      if (NOT (${GROUP_OPTNAME} AND ${TARGET_OPTNAME}))
        # remove LIB from the list of active LIBS
        list (REMOVE_ITEM IRAP_LIBRARIES ${LIB})
      endif ()
    endif ()
  endforeach ()
  set (IRAP_LIBRARIES ${IRAP_LIBRARIES} CACHE INTERNAL "irap active libraries")

  # update compile defines
  set (IRAP_ACTIVE_LIB_DEFINES "")
  foreach (LIB ${IRAP_LIBRARIES})
    string (REPLACE "-" "_" LIB ${LIB})
    string (REPLACE "+" "p" LIB ${LIB})
    string (TOUPPER ${LIB} LIB)
    set (IRAP_ACTIVE_LIB_DEFINES "${IRAP_ACTIVE_LIB_DEFINES} -D __${LIB}__")
  endforeach ()
  set (IRAP_ACTIVE_LIB_DEFINES "${IRAP_ACTIVE_LIB_DEFINES}" CACHE INTERNAL "irap active libraries defines")
endfunction ()

#===============================================================================
function (irap_add_group GROUP_DIR GROUP_PREFIX GROUP_DESCRIPTION GROUP_ONOFF)
  # set some build vars that are viewable only by subdirs
  string (TOUPPER ${GROUP_DIR} IRAP_GROUP_NAME)
  set (IRAP_GROUP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${GROUP_DIR})
  set (IRAP_GROUP_PREFIX ${GROUP_PREFIX})

  # add group build option
  option (BUILD_${IRAP_GROUP_NAME} ${GROUP_DESCRIPTION} ${GROUP_ONOFF})
  set (IRAP_GROUP_BUILD_OPTNAME BUILD_${IRAP_GROUP_NAME})
  add_subdirectory (${IRAP_GROUP_DIR})
  _irap_update_active_libraries ()
endfunction ()

#===============================================================================
function (irap_add_folder FOLDER_DIR FOLDER_DESCRIPTION FOLDER_ONOFF)
  set (IRAP_FOLDER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER_DIR})
  set (ADVANCED_REGEX "" CACHE STRING "regex for showing advanced build options (only active when in non-advanced mode)")

  if (IRAP_GROUP_NAME) # add intra-group build folder option
    set (IRAP_FOLDER_BUILD_OPTNAME ${IRAP_GROUP_BUILD_OPTNAME}_${FOLDER_DIR})
    option (${IRAP_FOLDER_BUILD_OPTNAME} ${FOLDER_DESCRIPTION} ${FOLDER_ONOFF})
    mark_as_advanced (FORCE ${IRAP_FOLDER_BUILD_OPTNAME})

    if (ADVANCED_REGEX)
      string (TOUPPER ${ADVANCED_REGEX} ADVANCED_REGEX_UPPER)
      if (IRAP_GROUP_NAME MATCHES ${ADVANCED_REGEX_UPPER})
        mark_as_advanced (CLEAR ${IRAP_FOLDER_BUILD_OPTNAME})
      else ()
        mark_as_advanced (FORCE ${IRAP_FOLDER_BUILD_OPTNAME})
      endif ()
    endif ()

    if (${IRAP_GROUP_BUILD_OPTNAME} AND ${IRAP_FOLDER_BUILD_OPTNAME})
      add_subdirectory (${IRAP_FOLDER_DIR})
    endif ()

  else ()
    set (IRAP_FOLDER_BUILD_OPTNAME BUILD_${FOLDER_DIR})
    option (${IRAP_FOLDER_BUILD_OPTNAME} ${FOLDER_DESCRIPTION} ${FOLDER_ONOFF})
    if (${IRAP_FOLDER_BUILD_OPTNAME})
      add_subdirectory (${IRAP_FOLDER_DIR})
    endif ()
  endif ()

  #_irap_update_active_libraries ()
endfunction ()

################################################################################
# C/C++ UTILS
################################################################################

#===============================================================================
function (irap_add_executable EXE)
  # build exe
  add_executable (${EXE} ${ARGN})
  if (IRAP_GROUP_PREFIX)
    set_target_properties (${EXE}
      PROPERTIES
      PREFIX irap-${IRAP_GROUP_PREFIX}-
      )
  else ()
    set_target_properties (${EXE}
      PROPERTIES
      PREFIX irap-
      )
  endif ()

  # install exe
  install (TARGETS ${EXE}
    DESTINATION ${EXECUTABLE_INSTALL_PATH}
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    )
endfunction ()

#===============================================================================
function (irap_add_library LIB)

  # be helpful and store our include dependencies for targets that link against us
  get_directory_property (MY_INCLUDES INCLUDE_DIRECTORIES)
  set (IRAP_INCLUDES_${LIB} ${MY_INCLUDES} CACHE INTERNAL "${LIB} includes")

  # add ourselves to the list of irap libs
  list (FIND IRAP_LIBRARIES_ALL ${LIB} INDEX)
  if (INDEX EQUAL -1)
    list (APPEND IRAP_LIBRARIES_ALL ${LIB})
    if (IRAP_GROUP_BUILD_OPTNAME)
      list (APPEND IRAP_LIBRARIES_GROUP_OPTNAMES ${IRAP_GROUP_BUILD_OPTNAME})
    else ()
      list (APPEND IRAP_LIBRARIES_GROUP_OPTNAMES "")
    endif ()
    list (APPEND IRAP_LIBRARIES_TARGET_OPTNAMES ${IRAP_FOLDER_BUILD_OPTNAME})
  endif ()
  set (IRAP_LIBRARIES_ALL ${IRAP_LIBRARIES_ALL} CACHE INTERNAL "irap known libraries")
  set (IRAP_LIBRARIES_GROUP_OPTNAMES ${IRAP_LIBRARIES_GROUP_OPTNAMES} CACHE INTERNAL "irap library group option names")
  set (IRAP_LIBRARIES_TARGET_OPTNAMES ${IRAP_LIBRARIES_TARGET_OPTNAMES} CACHE INTERNAL "irap library target option names")

  # build library
  add_library (${LIB} ${ARGN})

  # copy our headers locally
  set (INCLUDE_HEADERS)
  file (GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hh *.hpp)
  foreach (HEADER ${HEADERS})
    set (INCLUDE_HEADER ${INCLUDE_OUTPUT_PATH}/${LIB}/${HEADER})
    set (INCLUDE_HEADERS ${INCLUDE_HEADERS} ${INCLUDE_HEADER})

    get_filename_component (DIR ${INCLUDE_HEADER} PATH)
    if (NOT EXISTS ${DIR})
      file (MAKE_DIRECTORY ${DIR})
    endif ()
    add_custom_command (
      OUTPUT ${INCLUDE_HEADER}
      COMMAND cp -u -f ${HEADER} ${INCLUDE_HEADER}
      DEPENDS ${HEADER}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      )
  endforeach ()
  add_custom_target (${LIB}-includes ALL
    DEPENDS ${INCLUDE_HEADERS}
    )
  add_dependencies (${LIB} ${LIB}-includes)

  # install library
  install (TARGETS ${LIB}
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH}
    ARCHIVE DESTINATION ${LIBRARY_INSTALL_PATH}
    )
  install (DIRECTORY ${INCLUDE_OUTPUT_PATH}/${LIB}
    DESTINATION ${INCLUDE_INSTALL_PATH}
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    FILES_MATCHING REGEX "^.*\\.h$|^.*\\.hpp$|^.*\\.hh$"
    PATTERN ".svn" EXCLUDE
    )
endfunction ()

#===============================================================================
function (irap_target_link_libraries TARGET)
  # check for any irap libs dependencies and make sure they are being built
  foreach (LIB ${ARGN})
    list (FIND IRAP_LIBRARIES_ALL ${LIB} INDEX)
    if (INDEX GREATER -1)
      list (GET IRAP_LIBRARIES_GROUP_OPTNAMES ${INDEX} GROUP_OPTNAME)
      list (GET IRAP_LIBRARIES_TARGET_OPTNAMES ${INDEX} TARGET_OPTNAME)
      if (${GROUP_OPTNAME} AND ${TARGET_OPTNAME})
        include_directories (${IRAP_INCLUDES_${LIB}})
      else ()
        message (SEND_ERROR "GROUP:${IRAP_GROUP_NAME} TARGET:${TARGET} => LIBRARY:${LIB} required")
      endif ()
    elseif (FIRST_PASS_COMPLETE AND LIB MATCHES "^irap-.*")
      message (SEND_ERROR "GROUP:${IRAP_GROUP_NAME} TARGET:${TARGET} => LIBRARY:${LIB} required")
    endif ()
  endforeach ()

  # build target as usual
  target_link_libraries (${TARGET} ${ARGN})
endfunction ()


################################################################################
# JAVA UTILS
################################################################################

#===============================================================================
function (irap_java_find_library JAR)
  set (JAR_NAME ${JAR})

  # check if it is a system-wide library
  find_file (${JAR} ${JAR_NAME}
    PATHS /usr/share/java /usr/local/share/java ${JAVA_OUTPUT_PATH} ${JAVA_INSTALL_PATH}
    DOC "Fullpath to ${JAR_NAME}"
    )
  if (${JAR})
    mark_as_advanced (${JAR})
    set (JAR_${JAR} ${${JAR}} PARENT_SCOPE)
    if (CLASSPATH)
      set (CLASSPATH "${CLASSPATH}:${${JAR}}" PARENT_SCOPE)
    else ()
      set (CLASSPATH "${${JAR}}" PARENT_SCOPE)
    endif ()
    return ()
  else ()
    # check if it is a irap library first
    string (REPLACE ";" "|" REGEX "${IRAP_JAVA_LIBRARIES}")
    if (${JAR} MATCHES "${REGEX}")
      #message(skipping: for ${JAVA_OUTPUT_PATH}/${JAR})
      set (CLASSPATH "${CLASSPATH}:${JAVA_OUTPUT_PATH}/${JAR}" PARENT_SCOPE)
      return ()
    endif ()

  endif ()

  message (SEND_ERROR "ERROR: ${JAR_NAME} not found.")

endfunction ()


#===============================================================================
function (irap_java_add_library TARGET)
  set (JAVA_FILES ${ARGN})

  # compile class definitions
  string (REPLACE ".java" ".class" CLASS_FILES "${JAVA_FILES}")
  string (REPLACE "${CMAKE_CURRENT_BINARY_DIR}/" "" CLASS_FILES "${CLASS_FILES}")

  add_custom_command (
    OUTPUT ${CLASS_FILES}
    COMMAND ${JAVA_COMPILE} -classpath "${CLASSPATH}:${${TARGET}_CLASSPATH}" -d ${CMAKE_CURRENT_BINARY_DIR} ${JAVA_FILES}
    DEPENDS ${JAVA_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

  # build jar
  set (OUTPUT_TARGET "${PROJECT_SOURCE_DIR}/pod-build/${TARGET}" CACHE INTERNAL "${TARGET}")
  add_custom_command (
    OUTPUT ${OUTPUT_TARGET}
    #COMMAND ${CMAKE_Java_ARCHIVE} cf ${OUTPUT_TARGET} ${CLASS_FILES}
    COMMAND ${JAVA_ARCHIVE} cf ${OUTPUT_TARGET} **/*.class
    DEPENDS ${CLASS_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  add_custom_target (${TARGET} ALL
    DEPENDS ${OUTPUT_TARGET}
    )

  # add ourselves to the list of irap jars
  list (FIND _JAVA_LIBRARIES ${TARGET} INDEX)
  if (INDEX EQUAL -1)
    list (APPEND _JAVA_LIBRARIES ${TARGET})
  endif ()
  set (_JAVA_LIBRARIES ${IRAP_JAVA_LIBRARIESS} CACHE INTERNAL "irap java libraries")

  # install
  install (FILES ${OUTPUT_TARGET}
    DESTINATION ${JAVA_INSTALL_PATH}
    )
endfunction ()

#===============================================================================
function (irap_java_target_link_libraries TARGET)
  set (JARS ${ARGN})
  foreach (JAR ${JARS})
    irap_java_find_library (${JAR})
  endforeach ()
  set (${TARGET}_CLASSPATH ${CLASSPATH} CACHE INTERNAL "${TARGET} depedencies")
endfunction ()


#===============================================================================
function (irap_java_add_executable EXE)
  # TODO
endfunction ()


################################################################################
# PYTHON UTILS
################################################################################

#===============================================================================
function (irap_python_get_version RETVAR)
  # which python version?
  find_package(PythonInterp REQUIRED)
  execute_process(COMMAND
    ${PYTHON_EXECUTABLE} -c "import sys; sys.stdout.write(sys.version[:3])"
    OUTPUT_VARIABLE PYTHON_VERSION)
  set (${RETVAR} ${PYTHON_VERSION} PARENT_SCOPE)
endfunction ()

#===============================================================================
function (irap_python_install_script EXE_NAME PYTHON_SCRIPT)
  foreach (HELPER ${ARGN})
    set (PYTHON_SCRIPT_HELPERS ${PYTHON_SCRIPT_HELPERS} ${HELPER})
    set (PYTHON_SCRIPT_HELPERS_OUT ${PYTHON_SCRIPT_HELPERS_OUT} ${SCRIPT_OUTPUT_PATH}/${HELPER})
  endforeach ()

  if (NOT PYTHON_SCRIPT MATCHES ".+\\.py") #ends with a .py
    message (FATAL_ERROR "${PYTHON_FILE} does not end with a .py")
  endif ()

  # exe name
  get_filename_component (FOLDER_NAME ${IRAP_FOLDER_DIR} NAME)
  if (IRAP_GROUP_PREFIX)
    set (EXE irap-${IRAP_GROUP_PREFIX}-${EXE_NAME})
    get_filename_component (GROUP_NAME ${IRAP_GROUP_DIR} NAME)
    set (SCRIPT_SUBDIR ${GROUP_NAME}/${FOLDER_NAME})
  else ()
    set (EXE irap-${EXE_NAME})
    set (SCRIPT_SUBDIR ${FOLDER_NAME})
  endif ()

  # get the directory where we'll install the script
  set (SCRIPT_OUTPUT_PATH  "${PYTHON_OUTPUT_PATH}/irap/${SCRIPT_SUBDIR}/scripts")
  set (SCRIPT_INSTALL_PATH "${PYTHON_INSTALL_PATH}/irap/${SCRIPT_SUBDIR}/scripts")
  if (NOT EXISTS "${SCRIPT_OUTPUT_PATH}")
    file (MAKE_DIRECTORY ${SCRIPT_OUTPUT_PATH})
  endif ()


  # install locally
  get_filename_component (PYTHON_SCRIPT_NAME ${PYTHON_SCRIPT} NAME)
  file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.local
    "#!/usr/bin/env sh\n"
    "export PYTHONPATH=${PYTHON_OUTPUT_PATH}:\${PYTHONPATH}\n"
    "exec ${PYTHON_EXECUTABLE} ${SCRIPT_OUTPUT_PATH}/${PYTHON_SCRIPT_NAME} $*\n"
    )
  add_custom_command (
    OUTPUT ${SCRIPT_OUTPUT_PATH}/${PYTHON_SCRIPT_NAME} ${PYTHON_SCRIPT_HELPERS_OUT} ${EXECUTABLE_OUTPUT_PATH}/${EXE}
    COMMAND cp -f ${PYTHON_SCRIPT} ${PYTHON_SCRIPT_HELPERS} ${SCRIPT_OUTPUT_PATH}/.
    COMMAND cp -f ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.local ${EXECUTABLE_OUTPUT_PATH}/${EXE}
    COMMAND chmod +x -f ${EXECUTABLE_OUTPUT_PATH}/${EXE}
    DEPENDS ${PYTHON_SCRIPT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
  add_custom_target (${EXE} ALL
    DEPENDS ${SCRIPT_OUTPUT_PATH}/${PYTHON_SCRIPT_NAME} ${PYTHON_SCRIPT_HELPERS_OUT} ${EXECUTABLE_OUTPUT_PATH}/${EXE}
    )

  # install system wide
 file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.install
   "#!/usr/bin/env sh\n"
   "export PYTHONPATH=${PYTHON_INSTALL_PATH}:\${PYTHONPATH}\n"
   "exec ${PYTHON_EXECUTABLE} ${SCRIPT_INSTALL_PATH}/${PYTHON_SCRIPT_NAME} $*\n"
   )

 install (FILES ${PYTHON_SCRIPT} ${PYTHON_SCRIPT_HELPERS}
   DESTINATION ${SCRIPT_INSTALL_PATH}
   PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
   )

 install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.install
   DESTINATION ${EXECUTABLE_INSTALL_PATH}
   PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
   RENAME ${EXE}
   )
endfunction ()

#===============================================================================
function (irap_python_add_custom_package PYTHON_PACKAGE_NAME)
  list (APPEND IRAP_PYTHON_PACKAGES ${PYTHON_PACKAGE_NAME})
  list (REMOVE_DUPLICATES IRAP_PYTHON_PACKAGES)
  set (IRAP_PYTHON_PACKAGES ${IRAP_PYTHON_PACKAGES} CACHE INTERNAL "irap active packages")


endfunction ()

################################################################################
# BASH UTILS
################################################################################

#===============================================================================
function (irap_bash_install_script TARGET BASH_FILE)
  if (IRAP_GROUP_PREFIX)
    set (OUTPUT_EXE irap-${IRAP_GROUP_PREFIX}-${TARGET})
  else ()
    set (OUTPUT_EXE irap-${TARGET})
  endif ()

  # local build copy
  get_filename_component (BASH_FILE_NAME ${BASH_FILE} NAME)
  configure_file (${BASH_FILE} ${BASH_FILE_NAME}.local @ONLY)
  add_custom_command (
    OUTPUT ${EXECUTABLE_INSTALL_PATH}/${OUTPUT_EXE}
    COMMAND cp -u -f ${BASH_FILE}.local ${EXECUTABLE_INSTALL_PATH}/${OUTPUT_EXE}
    COMMAND chmod -f +x ${EXECUTABLE_INSTALL_PATH}/${OUTPUT_EXE}
    DEPENDS ${BASH_FILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  add_custom_target (${OUTPUT_EXE} ALL
    DEPENDS ${EXECUTABLE_INSTALL_PATH}/${OUTPUT_EXE}
    )

  # install copy
  configure_file (${BASH_FILE} ${BASH_FILE_NAME}.install @ONLY)
  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${BASH_FILE_NAME}.install
    DESTINATION ${EXECUTABLE_INSTALL_PATH}
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    RENAME ${OUTPUT_EXE}
    )
endfunction ()
