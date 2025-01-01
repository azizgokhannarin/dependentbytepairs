cmake_minimum_required(VERSION 3.5)

project(DependentByte LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    dependentbyte.cpp
    main.cpp
)

set(HEADERS
    dependentbyte.h
)

add_executable(DependentByte ${SOURCES} ${HEADERS})

install(TARGETS DependentByte
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
