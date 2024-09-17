# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\QBlueprint_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\QBlueprint_autogen.dir\\ParseCache.txt"
  "QBlueprint_autogen"
  )
endif()
