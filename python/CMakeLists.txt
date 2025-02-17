if(WIN32)
    set(DSICE_SHARED_LIBRARY_SRC "${CMAKE_SOURCE_DIR}/build/src/${CMAKE_CFG_INTDIR}/DSICE.dll")
    set(DSICE_SHARED_LIBRARY_DST "${CMAKE_CURRENT_SOURCE_DIR}/dsice/DSICE.dll")
else()
    set(DSICE_SHARED_LIBRARY_SRC "${CMAKE_SOURCE_DIR}/build/src/libDSICE.so")
    set(DSICE_SHARED_LIBRARY_DST "${CMAKE_CURRENT_SOURCE_DIR}/dsice/libDSICE.so")
endif()

add_custom_command(
    OUTPUT "${DSICE_SHARED_LIBRARY_DST}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DSICE_SHARED_LIBRARY_SRC}"
            "${DSICE_SHARED_LIBRARY_DST}"
    DEPENDS "${DSICE_SHARED_LIBRARY_SRC}"
)

add_custom_target(copy_shared_library ALL
    DEPENDS "${DSICE_SHARED_LIBRARY_DST}"
)
