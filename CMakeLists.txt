cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

project(cellworld
        VERSION 2019.1.0
        DESCRIPTION "Agent cell world simulation framework"
        LANGUAGES CXX)

####
#### DEPENDENCIES
####

find_package (Dependencies QUIET)

if (NOT ${Dependencies_FOUND})
    if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/DependenciesConfig.cmake)
        file(DOWNLOAD https://raw.githubusercontent.com/germanespinosa/dependencies/main/DependenciesConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/DependenciesConfig.cmake)
    endif()
    set(Dependencies_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    find_package (Dependencies REQUIRED)
endif()

####
#### END DEPENDENCIES
####

install_git_dependency( Json-cpp
        https://github.com/cellworld-neuro/dependency_json-cpp
        PRIVATE
        CMAKE_PROJECT
        IMPORT_TARGETS
        json-cpp )


####
#### MAIN LIBRARY SETUP
####

set(cellworld_files
        src/frame.cpp
        src/agent.cpp
        src/cell_group.cpp
        src/connection.cpp
        src/coordinates.cpp
        src/location.cpp
        src/geometry.cpp
        src/transformation.cpp
        src/core.cpp
        src/cell.cpp
        src/shape.cpp
        src/model.cpp
        src/chance.cpp
        src/visibility.cpp
        src/world.cpp
        src/graph.cpp
        src/paths.cpp
        src/state.cpp
        src/map.cpp
        src/capture.cpp
        src/peeking.cpp
        src/pose.cpp
        src/experiment.cpp
        )

if (NOT "${EXCLUDE_RESOURCES}" MATCHES "TRUE")

    set(cellworld_files_c ${cellworld_files} src/resources.cpp )

    add_library(cellworld STATIC
            ${cellworld_files_c})

    target_include_directories(cellworld
            SYSTEM INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>)

    target_link_libraries(cellworld PRIVATE json-cpp)


    install_git_dependency( Requests
            https://github.com/cellworld-neuro/dependency_requests
            PRIVATE
            CMAKE_PROJECT
            IMPORT_TARGETS
            requests
            TARGET
                cellworld )

    set_target_properties(cellworld
            PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED On
            CXX_EXTENSIONS Off
            VERSION ${PROJECT_VERSION})


    include_directories(include)

    target_compile_options(cellworld PRIVATE -pthread  -Wextra -pedantic -Wall)

else()
    set(cellworld_files_c ${cellworld_files} )
endif()

####
#### TESTS
####

if (NOT "${BUILD_AS_DEPENDENCY}" MATCHES "TRUE")

    install_git_dependency( Catch
            https://github.com/cellworld-neuro/dependency_catch
            ADD_SUBDIRECTORY)

    add_catch_test( chance_test
            TEST_FILES
            catchtests/chance.cpp
            LINK_LIBRARIES cellworld
            INCLUDE_DIRECTORIES
            include)

endif()
