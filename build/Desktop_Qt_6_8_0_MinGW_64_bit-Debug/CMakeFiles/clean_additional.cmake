# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\PTZControl_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\PTZControl_autogen.dir\\ParseCache.txt"
  "PTZControl_autogen"
  )
endif()
