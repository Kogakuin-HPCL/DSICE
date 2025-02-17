set(SCRIPT_FILES)

if(UNIX)
    list(APPEND SCRIPT_FILES
        ${CMAKE_SOURCE_DIR}/bin/Linux/dsice_version
        ${CMAKE_SOURCE_DIR}/bin/Linux/dsice_launch
        ${CMAKE_SOURCE_DIR}/bin/Linux/dsice_submit_job
        ${CMAKE_SOURCE_DIR}/bin/Linux/dsice_del
        ${CMAKE_SOURCE_DIR}/bin/Linux/dsice_env_clean
    )
endif()

install(FILES ${SCRIPT_FILES}
    DESTINATION bin
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                GROUP_EXECUTE GROUP_READ
                WORLD_EXECUTE WORLD_READ
)
