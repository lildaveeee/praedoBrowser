cmake_minimum_required(VERSION 3.16)

project(praedoBrowser VERSION 0.1 LANGUAGES CXX)

# Enable Qt's automatic processing features
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt Packages (Ensure static versions)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets WebEngineWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets WebEngineWidgets)


# Define source files
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    settings.h
    settings.cpp
)

# Add executable
add_executable(praedoBrowser ${PROJECT_SOURCES}
    media.h
    media.cpp




)

# Link static libraries
target_link_libraries(praedoBrowser PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::WebEngineWidgets
)


# Installation setup
include(GNUInstallDirs)
install(TARGETS praedoBrowser
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

