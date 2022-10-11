#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libjpeg-turbo::jpeg-static" for configuration "Debug"
set_property(TARGET libjpeg-turbo::jpeg-static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libjpeg-turbo::jpeg-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "F:/work_space/Corona/External/build/libjpeg-turbo/../../Windows/lib/jpeg-static.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS libjpeg-turbo::jpeg-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_libjpeg-turbo::jpeg-static "F:/work_space/Corona/External/build/libjpeg-turbo/../../Windows/lib/jpeg-static.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
