find_path(GENICAM_WRAPPER_INCLUDE_DIR genicam_wrapper.h PATHS "/home/codac-dev/jszataniak/genicam-wrapper/src")

set(GENICAM_WRAPPER_LIBRARY_DIR "/home/codac-dev/jszataniak/genicam-wrapper/build/src/libgenicam_wrapper.so")

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(GenicamWrapper DEFAULT_MSG GENICAM_WRAPPER_INCLUDE_DIR GENICAM_WRAPPER_LIBRARY_DIR)

mark_as_advanced(GENICAM_WRAPPER_INCLUDE_DIR GENICAM_WRAPPER_LIBRARY_DIR)
