# Papillon SDK - CMake File
# -----------------------------------------------------------------------------
# Description: Papillon - Examples
# -----------------------------------------------------------------------------

# An example showing how to use PMemoryStream
add_executable(doMemoryStream doMemoryStream.cpp)
target_link_libraries(doMemoryStream ${PAPILLON_LIBRARIES})
