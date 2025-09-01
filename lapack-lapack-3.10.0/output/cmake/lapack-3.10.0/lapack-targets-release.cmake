#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "blas" for configuration "Release"
set_property(TARGET blas APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(blas PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "Fortran"
  IMPORTED_LOCATION_RELEASE "/home/zext/outlibraries/lapack-lapack-3.10.0/output/libblas.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS blas )
list(APPEND _IMPORT_CHECK_FILES_FOR_blas "/home/zext/outlibraries/lapack-lapack-3.10.0/output/libblas.a" )

# Import target "lapack" for configuration "Release"
set_property(TARGET lapack APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(lapack PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "Fortran"
  IMPORTED_LOCATION_RELEASE "/home/zext/outlibraries/lapack-lapack-3.10.0/output/liblapack.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS lapack )
list(APPEND _IMPORT_CHECK_FILES_FOR_lapack "/home/zext/outlibraries/lapack-lapack-3.10.0/output/liblapack.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
