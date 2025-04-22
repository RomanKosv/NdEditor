cmake_minimum_required(VERSION 3.16)

project(logger LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(logger #[[main.cpp]]
# <<<<<<< HEAD
#     logger.h)
# =======
    include/logger.h
    include/basic_loggers.h
    include/logger.cpp)

set_target_properties(logger PROPERTIES LINKER_LANGUAGE CXX)

# >>>>>>> origin/logger
target_include_directories(logger PUBLIC include)
include(GNUInstallDirs)
install(TARGETS logger
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
