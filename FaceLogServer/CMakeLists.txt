# Papillon SDK - CMake File
# -----------------------------------------------------------------------------
# Description: Papillon Core Library - Examples
# -----------------------------------------------------------------------------

#
# ExampleFace
#

# An example using the FaceLog2, not yet ready for inclusion in SDK.
add_executable(doFaceLogServer doFaceLogServer.cpp)
target_link_libraries(doFaceLogServer PapillonCore)

# An example using the FaceLog2, not yet ready for inclusion in SDK.
add_executable(doFaceLogClient doFaceLogClient.cpp)
target_link_libraries(doFaceLogClient PapillonCore)
