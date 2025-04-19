cmake_minimum_required(VERSION 3.16)

project(triangulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(triangulation main.cpp
    triangulation.h)

include(GNUInstallDirs)
install(TARGETS triangulation
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
