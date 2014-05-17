add_subdirectory(helper)

set(kcmpacmanrepoeditor_INCLUDE_DIRS ${KDE4_INCLUDES})

set(kcmpacmanrepoeditor_SRCS kcmpacmanrepoeditor.cpp addrepo.cpp checkboxdelegate.cpp optionsdelegate.cpp repoconf.cpp repoentry.cpp)

kde4_add_ui_files(kcmpacmanrepoeditor_SRCS ../ui/kcmpacmanrepoeditor.ui)
kde4_add_ui_files(kcmpacmanrepoeditor_SRCS ../ui/addrepo.ui)

include_directories(${kcmpacmanrepoeditor_INCLUDE_DIRS})
kde4_add_plugin(kcm_pacmanrepoeditor ${kcmpacmanrepoeditor_SRCS})
target_link_libraries(kcm_pacmanrepoeditor ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
install(TARGETS kcm_pacmanrepoeditor DESTINATION ${PLUGIN_INSTALL_DIR})
