cmake_minimum_required(VERSION 3.16)

project(QtQuickProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# <<<<<<< HEAD
# find_package(Qt6 REQUIRED COMPONENTS Quick Quick3D Gui)
# =======
find_package(Qt6 REQUIRED COMPONENTS
    Quick
    Quick3D
)
# >>>>>>> origin/logger

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appQtQuickProject
    main.cpp
)

qt_add_qml_module(appQtQuickProject
    URI QtQuickProject
    VERSION 1.0
    QML_FILES
# <<<<<<< HEAD
# =======
        Main.qml
        SOURCES triangle_geometry.h
        SOURCES triangles.h
        SOURCES triangle_geometry.cpp
        SOURCES
# >>>>>>> origin/logger
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appQtQuickProject PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appQtQuickProject
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appQtQuickProject
# <<<<<<< HEAD
#     PRIVATE Qt6::Quick
#     Qt6::Quick3D
#     Qt6::Gui
# =======
    PRIVATE
        Qt6::Quick
        Qt6::Quick3D
# >>>>>>> origin/logger
)

include(GNUInstallDirs)
install(TARGETS appQtQuickProject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
