# Set Sources
set(kcmpacmanrepoeditorhelper_SRCS 
    helper.cpp
    )

# Set Link Libraries
#set(kcmpacmanrepoeditorhelper_LINK_LIBS ${KDE4_KDECORE_LIBS})

# Build & Link
add_executable(kcmpacmanrepoeditorhelper ${kcmpacmanrepoeditorhelper_SRCS})
target_link_libraries(kcmpacmanrepoeditorhelper KF5::Auth KF5::ConfigCore)

# Install
install(TARGETS kcmpacmanrepoeditorhelper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
kauth_install_helper_files(kcmpacmanrepoeditorhelper org.kde.kcontrol.kcmpacmanrepoeditor root)
kauth_install_actions(org.kde.kcontrol.kcmpacmanrepoeditor kcmpacmanrepoeditor.actions)
