cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

find_package( Json-cpp CONFIG QUIET )

project(cellworld
        VERSION 2019.1.0
        DESCRIPTION "Agent cell world simulation framework"
        LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fno-strict-aliasing")

###


###
### MAIN LIBRARY SETUP
###

set(cellworld_files
    src/agent.cpp
    src/cell_group.cpp
    src/connection.cpp
    src/core.cpp
    src/model.cpp
    src/chance.cpp
    src/visibility.cpp
    src/world.cpp
    src/graph.cpp
    src/paths.cpp
    src/state.cpp
    src/map.cpp)

add_library(cellworld ${cellworld_files})

set_target_properties(cellworld
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    VERSION ${PROJECT_VERSION})

if(MSVC)
    target_compile_options(cellworld PRIVATE /W4)
else(MSVC)
    target_compile_options(cellworld PRIVATE -Wall -Wextra -pedantic)
endif(MSVC)

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

###
### TESTS
###
find_package(CatchTests CONFIG QUIET)


###
### LIBRARY INSTALLATION
###

include(GNUInstallDirs)

SET(CELLWORLD_RESOURCES "${CMAKE_INSTALL_FULL_DATADIR}/cellworld/resources")

target_compile_definitions(cellworld PRIVATE
        -DCELLWORLD_RESOURCES="${CELLWORLD_RESOURCES}")

install(TARGETS cellworld EXPORT CellworldConfig
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY resources/ DESTINATION ${CELLWORLD_RESOURCES})
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
install(FILES resources.cmake DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/cmake/Cellworld/)
install(CODE "FILE(APPEND ${CMAKE_INSTALL_FULL_DATADIR}/cmake/Cellworld/CellworldConfig.cmake \"find_package(Ge211 REQUIRED)\n\") ")
install(CODE "FILE(APPEND ${CMAKE_INSTALL_FULL_DATADIR}/cmake/Cellworld/CellworldConfig.cmake \"SET(CELLWORLD_RESOURCES ${CELLWORLD_RESOURCES})\n\") ")
install(CODE "FILE(APPEND ${CMAKE_INSTALL_FULL_DATADIR}/cmake/Cellworld/CellworldConfig.cmake \"include(${CMAKE_INSTALL_FULL_DATADIR}/cmake/Cellworld/resources.cmake)\n\") ")
install(CODE "FILE(APPEND ${CMAKE_INSTALL_FULL_DATADIR}/cmake/Cellworld/CellworldConfig.cmake \"get_target_property( UTF8-CPP_INTERFACE_INCLUDE_DIRECTORIES utf8-cpp INTERFACE_INCLUDE_DIRECTORIES )\n\") ")

