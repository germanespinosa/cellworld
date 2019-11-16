cmake_minimum_required(VERSION 3.13)
project(catch
            VERSION 2019.1.0
            DESCRIPTION "Testing framework"
            LANGUAGES CXX)

add_library(catch
        src/catch.h
        src/catch-main.cpp)
set_property(TARGET catch PROPERTY CXX_STANDARD 14)
set_property(TARGET catch PROPERTY CXX_STANDARD_REQUIRED On)
target_include_directories(catch PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
