cmake_minimum_required(VERSION 3.16)

project(triangulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_library(triangulation
    triangulation.h
    triangulation_with_eigen_1.h
    optimized_eigein_triangulation.h
    triangulation_with_eigen_1.cpp)
target_include_directories(triangulation PUBLIC .)
target_link_libraries (triangulation Eigen3::Eigen #[[logger]])
include(GNUInstallDirs)
install(TARGETS triangulation
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
