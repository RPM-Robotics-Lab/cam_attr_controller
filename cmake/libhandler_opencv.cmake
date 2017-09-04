macro (libhandler_opencv)
  libhandler_find_package (OpenCV "
on ubuntu <12.04 `sudo apt-get install libcv-dev libcvaux-dev libhighgui-dev`
on ubuntu >=12.04 `sudo apt-get install libcv-dev libcvaux-dev libhighgui-dev libopencv-dev`" ${ARGN})
  if (OpenCV_FOUND)
    include (${OpenCV_CMAKE_INCLUDE})
    include_directories (${OpenCV_INCLUDE_DIR})
    set (IRPLIB_OPENCV ${OpenCV_LIBS})
    #message ("version ${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}")
    add_definitions ("-D OpenCV_VERSION=${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}")

    # for many case we use cuda with opencv
    libhandler_find_library (cuda "opencv exclude cuda library" ${ARGN} OPTIONAL)
    if (CUDA_FOUND)
      set (IRPLIB_OPENCV ${OpenCV_LIBS} ${CUDA_LIBRARIES})
    endif ()

  endif ()

endmacro ()
