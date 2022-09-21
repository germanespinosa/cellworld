cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

project(cellworld
        VERSION 2019.1.0
        DESCRIPTION "Agent cell world simulation framework"
        LANGUAGES CXX)

find_package(Dependencies)

install_dependency(https://github.com/germanespinosa/json-cpp Json-cpp)

dependency_include(include)

####
#### MAIN LIBRARY SETUP
####

set(cellworld_files
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
        )


if (NOT "$ENV{BUILD_PYTHON_MODULE_ONLY}" MATCHES "TRUE")
    install_dependency(https://github.com/germanespinosa/requests Requests)
    set(cellworld_files_c ${cellworld_files} src/resources.cpp )
    add_library(cellworld STATIC
            ${cellworld_files_c})

    set_target_properties(cellworld
            PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED On
            CXX_EXTENSIONS Off
            VERSION ${PROJECT_VERSION})


    target_include_directories(cellworld
            SYSTEM INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>)

    target_include_directories(cellworld
            PRIVATE
            include )

    target_compile_options(cellworld PRIVATE -pthread  -Wextra -pedantic -Wall)

    target_link_libraries(cellworld
            PUBLIC
            json-cpp
            requests)

    ####
    #### TESTS
    ####

    install_dependency(https://github.com/germanespinosa/catch CatchTests)

    test_library(cellworld)

    test_library(cellworld catchtests/entropy.cpp)

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
    install(DIRECTORY scripts/ DESTINATION /bin)
    install(CODE "execute_process(COMMAND chmod +777 /bin/worker)")
    install(CODE "execute_process(COMMAND chmod +777 /bin/params)")
    install(CODE "execute_process(COMMAND chmod +777 /bin/gauge)")
    install(CODE "execute_process(COMMAND chmod +777 /bin/json)")
    install(CODE "execute_process(COMMAND chmod +777 /bin/unique)")

    install(EXPORT CellworldConfig
            DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/Cellworld
            EXPORT_LINK_INTERFACE_LIBRARIES)

    export(TARGETS cellworld FILE CellworldConfig.cmake)
    install(CODE "FILE(APPEND ${CMAKE_INSTALL_FULL_DATADIR}/cmake/Cellworld/CellworldConfig.cmake \"find_package(Easy-tcp REQUIRED)\n\") ")
    install(CODE "FILE(APPEND ${CMAKE_INSTALL_FULL_DATADIR}/cmake/Cellworld/CellworldConfig.cmake \"find_package(Json-cpp REQUIRED)\n\") ")
    install(CODE "FILE(APPEND ${CMAKE_INSTALL_FULL_DATADIR}/cmake/Cellworld/CellworldConfig.cmake \"set(CELLWORLD_LIBRARIES cellworld json-cpp easy-tcp)\n\") ")

else()
    set(cellworld_files_c ${cellworld_files} )
endif()

####
#### Python module
####

if (NOT "$ENV{BUILD_AS_DEPENDENCY}" MATCHES "TRUE")

    install_dependency(https://github.com/pybind/pybind11)
    include_directories(cmake-dependencies/pybind11/include)

    FIND_PACKAGE(PythonInterp)
    if (PYTHONINTERP_FOUND)
        if (UNIX AND NOT APPLE)
            if (PYTHON_VERSION_MAJOR EQUAL 3)
                FIND_PACKAGE(Boost COMPONENTS python${PYTHON_VERSION_SUFFIX})
                FIND_PACKAGE(PythonInterp 3)
                FIND_PACKAGE(PythonLibs 3 REQUIRED)
            else()
                FIND_PACKAGE(Boost COMPONENTS python)
                FIND_PACKAGE(PythonInterp)
                FIND_PACKAGE(PythonLibs REQUIRED)
            endif()
        else()
            if (PYTHON_VERSION_MAJOR EQUAL 3)
                FIND_PACKAGE(Boost COMPONENTS python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR})
                FIND_PACKAGE(PythonInterp 3)
                FIND_PACKAGE(PythonLibs 3 REQUIRED)
            else()
                FIND_PACKAGE(Boost COMPONENTS python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR})
                FIND_PACKAGE(PythonInterp)
                FIND_PACKAGE(PythonLibs REQUIRED)
            endif()
        endif()
    else()
        message("Python not found")
    endif()
    message(STATUS "PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")
    message(STATUS "PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")
    message(STATUS "PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")


    PYTHON_ADD_MODULE(core ../src/python.cpp ${cellworld_files})

    target_compile_options(core PRIVATE -static -fPIC)

    target_include_directories(core
            PUBLIC
            ${Boost_INCLUDE_DIRS}
            ${PYTHON_INCLUDE_DIRS})

    target_link_libraries(core
            PUBLIC
            json-cpp
            ${Boost_LIBRARIES}
            ${PYTHON_LIBRARIES})

    string(TOLOWER ${CMAKE_BUILD_TYPE} LBT)

    add_custom_command(TARGET core POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy
            "$<TARGET_FILE:core>"
            "${CMAKE_CURRENT_SOURCE_DIR}/python/cellworld2/build-${LBT}/$<TARGET_FILE_NAME:core>"
            COMMENT "core output copied to ${CMAKE_CURRENT_SOURCE_DIR}/python/cellworld2/build-${LBT}")
endif()
