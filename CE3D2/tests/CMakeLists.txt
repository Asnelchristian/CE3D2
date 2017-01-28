set(CE3D2_TEST_ROOT ${CMAKE_CURRENT_LIST_DIR})

# Adds a new test target
function(add_ce3d2_test target)
    # Copy test files if existant.
    foreach(file ${ARGN})
        get_filename_component(test_files_directory ${file} NAME_WE)

        if(exists(${test_files_directory}))
            file(GLOB test_files "${test_files_directory}/*")

            foreach(test_file ${test_files})
                add_custom_command(OUTPUT ${test_files_directory}/${test_file}
                                   COMMAND "cp ${test_files_directory}/${test_file}"
                                   DEPENDS ${test_files_directory}/${test_file}
                                   COMMENT "Copying test file ${test_file}...")
            endforeach()
        endif()
    endforeach()

    # TestUtilities are embedded automatically into every test.
    list(INSERT ARGN 0 ${CE3D2_TEST_ROOT}/TestUtilities)

    add_executable(${target} ${ARGN})

    target_link_libraries(${target}
                          ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                          CE3D2)

    add_test(${target} ${target})

    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${target} --log_level=error --report_level=no
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running ${target}..."
        VERBATIM)

endfunction()

# Tests in this directory.
add_ce3d2_test(MatrixTest ${CMAKE_CURRENT_LIST_DIR}/MatrixTest)
add_ce3d2_test(VectorTest ${CMAKE_CURRENT_LIST_DIR}/VectorTest)

add_subdirectory("math")
add_subdirectory("models")
add_subdirectory("render")
add_subdirectory("transformation")
