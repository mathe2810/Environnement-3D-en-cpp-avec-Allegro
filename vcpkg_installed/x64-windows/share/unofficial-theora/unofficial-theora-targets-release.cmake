#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "unofficial::theora::theora" for configuration "Release"
set_property(TARGET unofficial::theora::theora APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(unofficial::theora::theora PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/theora.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/theora.dll"
  )

list(APPEND _cmake_import_check_targets unofficial::theora::theora )
list(APPEND _cmake_import_check_files_for_unofficial::theora::theora "${_IMPORT_PREFIX}/lib/theora.lib" "${_IMPORT_PREFIX}/bin/theora.dll" )

# Import target "unofficial::theora::theoraenc" for configuration "Release"
set_property(TARGET unofficial::theora::theoraenc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(unofficial::theora::theoraenc PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/theoraenc.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/theoraenc.dll"
  )

list(APPEND _cmake_import_check_targets unofficial::theora::theoraenc )
list(APPEND _cmake_import_check_files_for_unofficial::theora::theoraenc "${_IMPORT_PREFIX}/lib/theoraenc.lib" "${_IMPORT_PREFIX}/bin/theoraenc.dll" )

# Import target "unofficial::theora::theoradec" for configuration "Release"
set_property(TARGET unofficial::theora::theoradec APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(unofficial::theora::theoradec PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/theoradec.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/theoradec.dll"
  )

list(APPEND _cmake_import_check_targets unofficial::theora::theoradec )
list(APPEND _cmake_import_check_files_for_unofficial::theora::theoradec "${_IMPORT_PREFIX}/lib/theoradec.lib" "${_IMPORT_PREFIX}/bin/theoradec.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
