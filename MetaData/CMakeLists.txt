# Papillon SDK - CMake File
# -----------------------------------------------------------------------------
# Description: Papillon - Examples
# -----------------------------------------------------------------------------

# An example showing how to get at the meta data provided by a detector 
add_executable(doMetaDataDetector doMetaDataDetector.cpp)
target_link_libraries(doMetaDataDetector ${PAPILLON_LIBRARIES})
