cmake_minimum_required(VERSION 3.16)

project(logger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(logger main.cpp
# <<<<<<< HEAD
#     logger.h)
# =======
    logger.h
    basic_loggers.h)
# >>>>>>> origin/logger

include(GNUInstallDirs)
install(TARGETS logger
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
