cmake_minimum_required(VERSION 3.14)
project(DSICE
    VERSION 1.0.1
    DESCRIPTION "The automatic parameter tuning library."
    LANGUAGES CXX
)

add_subdirectory(src)
add_subdirectory(python)
add_subdirectory(include)
add_subdirectory(bin)

add_dependencies(copy_shared_library DSICE)

MESSAGE(STATUS "--------------------------------------------")
MESSAGE(STATUS "Information :")
MESSAGE(STATUS "    Project Name =          ${PROJECT_NAME}")
MESSAGE(STATUS "    Version =               ${PROJECT_VERSION}")
MESSAGE(STATUS "    Description =           ${PROJECT_DESCRIPTION}")
MESSAGE(STATUS "    CMAKE_VERSION =         ${CMAKE_VERSION}")
MESSAGE(STATUS "    CMAKE_CXX_COMPILER =    ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "    CMAKE_INSTALL_PREFIX =  ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "    CMAKE_BUILD_TYPE =      ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "    CMAKE_INSTALL_LIBDIR =  ${CMAKE_INSTALL_LIBDIR}")
