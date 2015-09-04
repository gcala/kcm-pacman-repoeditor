add_subdirectory(helper)

set(kcmpacmanrepoeditor_SRCS 
    kcmpacmanrepoeditor.cpp 
    addrepo.cpp 
    checkboxdelegate.cpp 
    optionsdelegate.cpp 
    repoconf.cpp 
    repoentry.cpp
    )

ki18n_wrap_ui(kcmpacmanrepoeditor_SRCS ../ui/kcmpacmanrepoeditor.ui)
ki18n_wrap_ui(kcmpacmanrepoeditor_SRCS ../ui/addrepo.ui)

#include_directories(${kcmpacmanrepoeditor_INCLUDE_DIRS})
add_library(kcm_pacmanrepoeditor MODULE ${kcmpacmanrepoeditor_SRCS})
target_link_libraries(kcm_pacmanrepoeditor 
    KF5::CoreAddons
    KF5::WidgetsAddons
    KF5::KCMUtils
    KF5::I18n
    )

install(TARGETS kcm_pacmanrepoeditor DESTINATION ${CMAKE_INSTALL_PLUGINDIR})
