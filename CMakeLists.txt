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
        https://github.com/germanespinosa/json-cpp
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

if (NOT "${BUILD_PYTHON_MODULE_ONLY}" MATCHES "TRUE")

    set(cellworld_files_c ${cellworld_files} src/resources.cpp )

    add_library(cellworld STATIC
            ${cellworld_files_c})

    target_include_directories(cellworld
            SYSTEM INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>)

    target_link_libraries(cellworld PRIVATE json-cpp)


    install_git_dependency( Requests
            https://github.com/germanespinosa/requests
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

    ####
    #### TESTS
    ####

#    install_git_dependency( Catch
#            https://github.com/germanespinosa/catch
#            ADD_SUBDIRECTORY)
#
#    test_library(cellworld)

    #    test_library(cellworld catchtests/entropy.cpp)

    #test_files(TESTS catchtests/graph.cpp LIBRARIES cellworld)

    ####
    #### LIBRARY INSTALLATION
    ####

    include(GNUInstallDirs)

    install(TARGETS cellworld EXPORT CellworldConfig
            ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})

    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    install(EXPORT CellworldConfig
            DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/Cellworld
            EXPORT_LINK_INTERFACE_LIBRARIES)

    export(TARGETS cellworld FILE CellworldConfig.cmake)
else()
    set(cellworld_files_c ${cellworld_files} )
endif()

####
#### Python module
####

if (NOT "${BUILD_AS_DEPENDENCY}" MATCHES "TRUE")

    install_git_dependency( Pybind11
            https://github.com/pybind/pybind11
            ADD_SUBDIRECTORY
            INCLUDE_DIRECTORIES
            include )

    pybind11_add_module(core src/python.cpp ${cellworld_files})

    target_compile_options(core PRIVATE -Wextra -pedantic -Wall -static)

    target_include_directories(core PRIVATE include )

    target_compile_options(core PRIVATE)

    target_include_directories(core
            PUBLIC
            ${PYTHON_INCLUDE_DIRS})

    target_link_libraries(core
            PRIVATE
            json-cpp
            ${PYTHON_LIBRARIES})

    string(TOLOWER ${CMAKE_BUILD_TYPE} LBT)

    add_custom_command(TARGET core POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy
            "$<TARGET_FILE:core>"
            "${CMAKE_CURRENT_SOURCE_DIR}/python/cellworld2/build-${LBT}/$<TARGET_FILE_NAME:core>"
            COMMENT "core output copied to ${CMAKE_CURRENT_SOURCE_DIR}/python/cellworld2/build-${LBT}")
endif()