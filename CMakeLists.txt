# Papillon SDK - CMake File
# -----------------------------------------------------------------------------
# Description: Papillon Core Library
# -----------------------------------------------------------------------------

project(papillon-examples C CXX)

cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")

include(FindPackageHandleStandardArgs)

# Enqueues all the Cmake variables starting with given prefix into given list
function (papillon_get_variables_with_prefix _prefix _varResult)
    get_cmake_property(_vars VARIABLES)
    string (REGEX MATCHALL "(^|;)${_prefix}[A-Za-z0-9_]*" _matchedVars "${_vars}")
    set (${_varResult} ${_matchedVars} PARENT_SCOPE)
endfunction()

# Prints all the Cmake variables starting with given prefix
function (papillon_print_variables_with_prefix _prefix)
    if(VERBOSE)
        papillon_get_variables_with_prefix(${_prefix} matchedVars)
        foreach (_var IN LISTS matchedVars)
            message("${_var}=${${_var}}")
        endforeach()
    endif(VERBOSE)
endfunction()


find_package(SWIG REQUIRED)

find_package(Papillon)
papillon_print_variables_with_prefix(PAPILLON)
include_directories(${PAPILLON_INCLUDE_DIR})

add_subdirectory(FaceLogServer)

