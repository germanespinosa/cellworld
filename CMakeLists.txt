cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

project(cellworld
        VERSION 2019.1.0
        DESCRIPTION "Agent cell world simulation framework"
        LANGUAGES CXX)

find_package( Json-cpp REQUIRED )
find_package( Easy-tcp REQUIRED )

string(APPEND CMAKE_CXX_FLAGS "-fno-strict-aliasing ")
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
    src/map.cpp
    src/resources.cpp
    src/message.cpp
    src/message_router.cpp
    src/message_client.cpp
    src/message_server.cpp)

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
    add_compile_options(-Wall -Wextra -pedantic)
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
    json-cpp
    easy-tcp)

###
### TESTS
###
find_package(CatchTests CONFIG QUIET)


###
### LIBRARY INSTALLATION
###

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
