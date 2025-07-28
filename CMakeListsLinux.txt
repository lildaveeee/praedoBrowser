cmake_minimum_required(VERSION 3.16)

project(praedoBrowser VERSION 0.1 LANGUAGES CXX)

# Enable AUTOMOC, AUTOUIC, and AUTORCC.
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Use C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the required Qt packages.
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets WebEngineWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets WebEngineWidgets)

# Add the resource file. This resource file should include the font file.
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_resources(QT_RESOURCES resources.qrc)
else()
    qt5_add_resources(QT_RESOURCES resources.qrc)
endif()

# List project source files.
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    settings.h
    settings.cpp
)

# Create the executable target.
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(praedoBrowser
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(praedoBrowser ${PROJECT_SOURCES}
        settings.cpp
        settings.h)
endif()

# Optionally, explicitly add the generated resources.
target_sources(praedoBrowser PRIVATE ${QT_RESOURCES})

# Link against the Qt modules.
target_link_libraries(praedoBrowser PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::WebEngineWidgets
)

# Set bundle properties for macOS and Windows.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.praedoBrowser)
endif()

set_target_properties(praedoBrowser PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS praedoBrowser
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(praedoBrowser)
endif()
