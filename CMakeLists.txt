cmake_minimum_required(VERSION 3.16)

project(NdEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(NdEditor main.cpp
    linear_algebra/linear_algebra.h linear_algebra/linear_algebra_utilites.h nd_geometry/example_architect_linear_geometry.h nd_geometry/example_linear_geometry.h nd_geometry/geom_sys.h nd_geometry/group_geom_sys.h nd_geometry/linear_geometry_std_structures.h nd_geometry/naive_geometry_objects_factory.h nd_geometry/polyhedron_geom_sys.h nd_geometry/std_space_factory.h nd_geometry/test_group_gs.cpp nd_geometry/transformation.h parser/debug.hh parser/function_parse_architect.hh parser/function_parse_instructions.hh parser/main_test.cpp parser/operator_parse_instructions.hh parser/outparse.hh parser/parse_instructions.hh parser/parse_terminals.hh parser/parse_tree.hh parser/parsing_errors.h parser/std_context.h parser/wrappers.hh array_utilites.h
    nd_geometry/example_architect_linear_geometry.h nd_geometry/example_linear_geometry.h nd_geometry/geom_sys.h nd_geometry/group_geom_sys.h nd_geometry/linear_geometry_std_structures.h nd_geometry/naive_geometry_objects_factory.h nd_geometry/polyhedron_geom_sys.h nd_geometry/std_space_factory.h nd_geometry/test_group_gs.cpp nd_geometry/transformation.h
    parser/std_parsing_funs.h)

target_link_libraries (NdEditor Eigen3::Eigen)

include(GNUInstallDirs)
install(TARGETS NdEditor
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
add_subdirectory(testWindow)
add_subdirectory(QtQuickProject)
add_subdirectory(logger)
