
get_filename_component(CONFIG_PATH "${CMAKE_CURRENT_LIST_FILE}" PATH)
find_path(sonatareport_INCLUDE_DIR reportinglib/records.h HINTS ${CONFIG_PATH}/../../../include)
find_path(sonatareport_LIB_DIR NAMES libsonatareport.so HINTS ${CONFIG_PATH}/../../../lib)
find_library(sonatareport_LIBRARY sonatareport HINTS ${CONFIG_PATH}/../../../lib)

include("${CMAKE_CURRENT_LIST_DIR}/sonata-targets.cmake")