function(process_dtm_includes)
  # Get the current directory
  get_filename_component(CURRENT_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
  
  # Find all .cmake files in the current directory
  file(GLOB cmake_files "${CURRENT_DIR}/*.cmake")
  
  # Include each .cmake file
  foreach(cmake_file IN LISTS cmake_files)
      if(NOT cmake_file STREQUAL "${CMAKE_CURRENT_LIST_FILE}")
          include(${cmake_file})
      endif()
  endforeach()
endfunction()

process_dtm_includes()
