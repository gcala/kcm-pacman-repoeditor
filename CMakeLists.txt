project(kcmpacmanrepoeditor)
set(KCM_PACMANREPOEDITOR_VERSION "0.2.0")

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

set(QT_MIN_VERSION "5.3.0")
set(KF5_MIN_VERSION "5.1.0")

find_package(ECM 1.0.0 REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Widgets
    DBus
)

find_package(KF5 REQUIRED COMPONENTS
    CoreAddons
    WidgetsAddons
    KCMUtils
    I18n
    Auth
    WidgetsAddons
)

find_package( Gettext REQUIRED )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")

configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/src/config.h)

add_subdirectory(other)
add_subdirectory(resources)
add_subdirectory(src)
add_subdirectory(po)