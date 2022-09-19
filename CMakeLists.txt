cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

project(cellworld
        VERSION 2019.1.0
        DESCRIPTION "Agent cell world simulation framework"
        LANGUAGES CXX)

find_package(Dependencies)

install_dependency(https://github.com/germanespinosa/json-cpp Json-cpp)

string(APPEND CMAKE_CXX_FLAGS " -fno-strict-aliasing -pthread  -fPIC")

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
    src/resources.cpp
    src/capture.cpp
    src/peeking.cpp
        )

add_library(cellworld ${cellworld_files})

set_target_properties(cellworld
    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    VERSION ${PROJECT_VERSION})

if(MSVC)
    add_compile_options(/W4)
else(MSVC)
    add_compile_options(-Wall -Wextra -pedantic )
endif(MSVC)

dependency_include(include)
dependency_include(${dependencies_folder}/easy-tcp/include)

target_include_directories(cellworld
    SYSTEM INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_include_directories(cellworld
    PRIVATE
    include )

target_link_libraries(cellworld
    PUBLIC
    json-cpp)


####
#### Python module
####

if (NOT "$ENV{BUILD_AS_DEPENDENCY}" MATCHES "TRUE")
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

    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})
    LINK_LIBRARIES(${Boost_LIBRARIES} ${PYTHON_LIBRARIES}) # Deprecated but so convenient!

    PYTHON_ADD_MODULE(cpp_cell_world ../src/python.cpp)

    target_link_libraries(cpp_cell_world  cellworld)

endif()

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
