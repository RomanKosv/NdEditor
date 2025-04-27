cmake_minimum_required(VERSION 3.16)

project(Parser_ver_2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Parser_ver_2
    parsing_funs_ver_2.h parsing_funs_ver_2.cpp
    context_ver_2.h context_ver_2.cpp
    parse_errors_ver_2.h
    parse_errors_ver_2.cpp
    some_more_parsing_utilites.h)

target_include_directories(Parser_ver_2 PUBLIC .)

target_link_libraries(Parser_ver_2
    PRIVATE
        NdEditorLib
)

include(GNUInstallDirs)
install(TARGETS Parser_ver_2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
