make_directory (${CMAKE_CURRENT_BINARY_DIR}/dependency_include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/dependency_include)

function (dependency_include)
    foreach(include_folder ${ARGN})
        execute_process(COMMAND bash -c "cp ${include_folder}/* ${CMAKE_CURRENT_BINARY_DIR}/dependency_include/ -r"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}  )
    endforeach()
endfunction()

function(install_dependency git_repo)

    execute_process(COMMAND basename ${git_repo}
            OUTPUT_VARIABLE repo_name )

    string(REPLACE "\n" "" repo_name ${repo_name})

    message(STATUS "\nConfiguring dependency ${repo_name}")

    set(dependencies_folder "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")

    execute_process(COMMAND mkdir ${dependencies_folder} -p)

    set(dependency_folder "${dependencies_folder}/${repo_name}")

    execute_process(COMMAND bash -c "[ -d ${repo_name} ]"
            WORKING_DIRECTORY ${dependencies_folder}
            RESULT_VARIABLE  folder_exists)

    if (${folder_exists} EQUAL 0)
        execute_process(COMMAND git pull
                WORKING_DIRECTORY ${dependency_folder})
    else()
        execute_process(COMMAND git -C ${dependencies_folder} clone ${git_repo})
    endif()

    set(destination_folder ${CMAKE_CURRENT_BINARY_DIR}/${repo_name})

    execute_process(COMMAND mkdir ${repo_name} -p
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )

    execute_process(COMMAND bash -c "cmake --no-warn-unused-cli -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} '-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}' -G 'CodeBlocks - Unix Makefiles' ${dependency_folder}"
            WORKING_DIRECTORY ${destination_folder})


    execute_process(COMMAND bash -c "if [ -d dependable ]; then echo exists; else echo doesnt; fi"
            WORKING_DIRECTORY ${destination_folder}
            RESULT_VARIABLE  include_folder_exists)

    execute_process(COMMAND bash -c "[ -d dependency_include ]"
            WORKING_DIRECTORY ${destination_folder}
            RESULT_VARIABLE  include_folder_exists)

    if (${include_folder_exists} EQUAL 0)
        dependency_include(${destination_folder}/dependency_include)
    endif()


    execute_process(COMMAND make -j
            WORKING_DIRECTORY ${destination_folder})

    set (repo_targets "${destination_folder}/${repo_name}Targets.cmake")

    set (variadic_args ${ARGN})
    list(LENGTH variadic_args variadic_count)
    if (${variadic_count} GREATER 0)
        list(GET variadic_args 0 package_name)
        set (${package_name}_DIR ${destination_folder})
        find_package (${package_name} REQUIRED)
    endif ()

endfunction()