cmake_minimum_required(VERSION 3.16)

project(triangulation LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(triangulation #[[main.cpp]]
    include/triangulation.h
    include/triangulation.cpp)

target_include_directories(triangulation PUBLIC include)

target_link_libraries(triangulation
    logger  Qt6::Core Eigen3::Eigen
)

include(GNUInstallDirs)
install(TARGETS triangulation
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
