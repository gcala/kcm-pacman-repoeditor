# Set Include Directories
set(kcmpacmanrepoeditorhelper_INCLUDE_DIRS ${KDE4_INCLUDES})

# Set Sources
set(kcmpacmanrepoeditorhelper_SRCS helper.cpp)

# Set Link Libraries
set(kcmpacmanrepoeditorhelper_LINK_LIBS ${KDE4_KDECORE_LIBS})

# Build & Link
include_directories(${kcmpacmanrepoeditorhelper_INCLUDE_DIRS})
kde4_add_executable(kcmpacmanrepoeditorhelper ${kcmpacmanrepoeditorhelper_SRCS})
target_link_libraries(kcmpacmanrepoeditorhelper ${kcmpacmanrepoeditorhelper_LINK_LIBS})

# Install
install(TARGETS kcmpacmanrepoeditorhelper DESTINATION ${LIBEXEC_INSTALL_DIR})
kde4_install_auth_helper_files(kcmpacmanrepoeditorhelper org.kde.kcontrol.kcmpacmanrepoeditor root)
kde4_install_auth_actions(org.kde.kcontrol.kcmpacmanrepoeditor kcmpacmanrepoeditor.actions)
