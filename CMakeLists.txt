cmake_minimum_required(VERSION 3.13)
project(cellworld
        VERSION 2019.1.0
        DESCRIPTION "Agent cell world simulation framework"
        LANGUAGES CXX)

###
### DEPENDENCIES
###
find_package(Ge211 CONFIG)

###
### MAIN LIBRARY SETUP
###

add_library(cellworld
        include/cell_world.h
        src/core.cpp
        src/world.cpp
        src/model.cpp
        src/view.cpp
        src/simulation.cpp
        src/visibility.cpp
        src/connection.cpp
        src/map_editor.cpp
        src/sub_world.cpp include/cell_group.h src/cell_group.cpp catchtests/visibility.cpp include/probabilities.h src/probabilities.cpp catchtests/location.cpp src/agent.cpp)

target_link_libraries(cellworld
        PUBLIC
        ge211 )

set_target_properties( cellworld
        PROPERTIES
        CXX_STANDARD 14
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
        $<INSTALL_INTERFACE:include>
        ${SDL2_INCLUDE_DIRS})

target_include_directories(cellworld
        PRIVATE
        include )

target_link_libraries(cellworld
        PUBLIC
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        utf8-cpp)

SET(GE211_RESOURCE_PATH
        "${CMAKE_INSTALL_FULL_DATADIR}/cellworld/")

target_compile_definitions(cellworld PRIVATE
        -DCELLWORLD_RESOURCES="$<JOIN:${CELLWORLD_RESOURCE_PATH},\"$<COMMA>\">")

###
### LIBRARY INSTALLATION
###

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
install(CODE "FILE(APPEND ${CMAKE_INSTALL_FULL_DATADIR}/cmake/Cellworld/CellworldConfig.cmake \"find_package(Ge211   REQUIRED)\n\") ")

###
### TESTS CONFIGURATION
###

find_package(CatchTests CONFIG QUIET)
