# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/kuvatesti_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/kuvatesti_autogen.dir/ParseCache.txt"
  "kuvatesti_autogen"
  )
endif()
