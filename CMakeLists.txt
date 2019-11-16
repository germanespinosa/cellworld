cmake_minimum_required(VERSION 3.13)
project(cellworld 
            VERSION 2019.1.0
            DESCRIPTION "Cross platform object communication framework"
            LANGUAGES CXX)

include_directories(src)
include_directories(include)

add_subdirectory(lib/ge211/)
find_package(Ge211 CONFIG QUIET)

add_library( cellworld
    include/cellworld.h
    include/core.h
    include/world.h
    include/agent.h
    include/model.h
    include/view.h
    include/controller.h
    include/visibility.h
    src/core.cpp
    src/world.cpp
    src/agent.cpp
    src/model.cpp 
    src/view.cpp 
    src/controller.cpp
    src/visibility.cpp)
target_link_libraries(cellworld ge211)

set_target_properties( cellworld
    PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED On
        CXX_EXTENSIONS Off
        VERSION ${PROJECT_VERSION} 
        RUNTIME_OUTPUT_DIRECTORY "bin/")


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
        
target_compile_definitions(cellworld PRIVATE
    -DGE211_RESOURCES="$<JOIN:${GE211_RESOURCE_PATH},\"$<COMMA>\">")
    
include(GNUInstallDirs)

install(TARGETS cellworld  EXPORT CellworldConfig
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT CellworldConfig
        DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/Cellworld)
export(TARGETS cellworld FILE CellworldConfig.cmake)

include(CMakeTests.txt)