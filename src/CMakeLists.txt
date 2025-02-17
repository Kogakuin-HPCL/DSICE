set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# For Windows (dll)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(DSICE SHARED
    around_points.cpp
    around_searcher.cpp
    average_double.cpp
    base_point_log.cpp
    coordinate_line.cpp
    d_spline_c_wrapper.cpp
    d_spline_core.cpp
    direction_line.cpp
    dsice_c_wrapper.cpp
    dsice_logging_database.cpp
    dsice_operator_p_2024b.cpp
    dsice_operator_s_2017.cpp
    dsice_operator_s_2018.cpp
    dsice_operator_s_ippe.cpp
    dsice_process_agent.cpp
    dsice_process_launcher.cpp
    dsice_process_manager.cpp
    dsice_searchers_manager.cpp
    dsice_standard_database.cpp
    dsice_tuner.cpp
    dsice_viewer_background.cpp
    equally_spaced_d_spline.cpp
    full_searcher.cpp
    line_iterative_tri_searcher.cpp
    line_single_tri_searcher.cpp
    log_printer.cpp
    observed_d_spline.cpp
    one_dim_dsp_searcher.cpp
    operation_integrator.cpp
    overwritten_double.cpp
    radial_dsp_searcher.cpp
    sampling_log.cpp
    simple_direction_searcher.cpp
    simple_lhd_searcher.cpp
    suggest_group_log.cpp
    tri_equally_indexer.cpp
    uni_measurer.cpp
)

target_compile_options(DSICE PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -O3>
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8 /W4>
    $<$<CXX_COMPILER_ID:NVHPC>:-fast -O4>
    $<$<CXX_COMPILER_ID:IntelLLVM>:-O3>
)

install(TARGETS DSICE
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

if (UNIX)
    install(CODE "
        get_filename_component(ACTUAL_INSTALL_PREFIX \${CMAKE_INSTALL_PREFIX} REALPATH)
        file(WRITE \"\${CMAKE_SOURCE_DIR}/setup_env.sh\" \"#!/bin/sh\\n\\n\")
        file(APPEND \"\${CMAKE_SOURCE_DIR}/setup_env.sh\" \"export PATH=\\\$PATH:\${ACTUAL_INSTALL_PREFIX}/bin\\n\")
        file(APPEND \"\${CMAKE_SOURCE_DIR}/setup_env.sh\" \"export C_INCLUDE_PATH=\\\$C_INCLUDE_PATH:\${ACTUAL_INSTALL_PREFIX}/include\\n\")
        file(APPEND \"\${CMAKE_SOURCE_DIR}/setup_env.sh\" \"export CPLUS_INCLUDE_PATH=\\\$CPLUS_INCLUDE_PATH:\${ACTUAL_INSTALL_PREFIX}/include\\n\")
        file(APPEND \"\${CMAKE_SOURCE_DIR}/setup_env.sh\" \"export LD_LIBRARY_PATH=\\\$LD_LIBRARY_PATH:\${ACTUAL_INSTALL_PREFIX}/lib\\n\")
        file(APPEND \"\${CMAKE_SOURCE_DIR}/setup_env.sh\" \"export LIBRARY_PATH=\\\$LIBRARY_PATH:\${ACTUAL_INSTALL_PREFIX}/lib\\n\")
    ")
endif()
