cmake_minimum_required(VERSION 3.19)

project(QtQuickProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# <<<<<<< HEAD
# find_package(Qt6 REQUIRED COMPONENTS Quick Quick3D Gui)
# =======
find_package(Qt6 REQUIRED COMPONENTS
    Quick
    Quick3D
    Qml
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
        SOURCES model.h model.cpp
        SOURCES objectentry.h  objectentry.cpp
        SOURCES strings_to_screen_version1.h strings_to_screen_version1.cpp
        RESOURCES test_ndeditor_expressions
        SOURCES log.h
        QML_FILES SaveDialog.qml
        QML_FILES demo_variables.json
        SOURCES chmhelp.h chmhelp.cpp
        RESOURCES help.chm
        QML_FILES demo.json
# >>>>>>> origin/loggerr
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
        NdEditorLib
        Parser_ver_2
        triangulation
        Qt6::Quick
        Qt6::Quick3D
        Qt6::Qml
        # logger
# >>>>>>> origin/logger
)

include(GNUInstallDirs)
install(TARGETS appQtQuickProject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# qt6_generate_deploy_qml_app_script(
#     TARGET appQtQuickProject
#     OUTPUT_SCRIPT deploy_script
# )
# install(SCRIPT ${deploy_script})
