
set (scc_UIS
	../ui/about.ui
	../ui/cam.ui
	../ui/channel_favourites.ui
	../ui/channel_favourites_ad.ui
	../ui/channel_homes.ui
	../ui/channel_homes_ad.ui
	../ui/channel_list.ui
	../ui/channel_settings.ui
	../ui/email.ui
	../ui/friends.ui
	../ui/friends_ad.ui
	../ui/ignore.ui
	../ui/ignore_ad.ui
	../ui/kick.ui
	../ui/moderation.ui
	../ui/options.ui
	../ui/priv.ui
	../ui/privilege.ui
	../ui/update.ui
)

set (scc_SRCS
	channel_avatar.cpp
	commands.cpp
	config.cpp
	crypt.cpp
	debug.cpp
	dlg_about.cpp
	dlg_cam.cpp
	dlg_channel_favourites.cpp
	dlg_channel_favourites_ad.cpp
	dlg_channel_homes.cpp
	dlg_channel_homes_ad.cpp
	dlg_channel_list.cpp
	dlg_channel_settings.cpp
	dlg_email.cpp
	dlg_friends.cpp
	dlg_friends_ad.cpp
	dlg_ignore.cpp
	dlg_ignore_ad.cpp
	dlg_kick.cpp
	dlg_moderation.cpp
	dlg_options.cpp
	dlg_priv.cpp
	dlg_privilege.cpp
	dlg_update.cpp
	irc_auth.cpp
	irc_kernel.cpp
	log.cpp
	main.cpp
	mainwindow.cpp
	network.cpp
	network_thread.cpp
	nick_avatar.cpp
	qinputline.cpp
	qnicklist.cpp
	qnicklistdelegate.cpp
	qnotify.cpp
	replace.cpp
	tab_container.cpp
	tab_manager.cpp
	tab_widget.cpp
	update.cpp
	update_thread.cpp
)

set (scc_MOC_HDRS
	channel_avatar.h
	commands.h
	config.h
	crypt.h
	dlg_about.h
	dlg_cam.h
	dlg_channel_favourites.h
	dlg_channel_favourites_ad.h
	dlg_channel_homes.h
	dlg_channel_homes_ad.h
	dlg_channel_list.h
	dlg_channel_settings.h
	dlg_email.h
	dlg_friends.h
	dlg_friends_ad.h
	dlg_ignore.h
	dlg_ignore_ad.h
	dlg_kick.h
	dlg_moderation.h
	dlg_options.h
	dlg_priv.h
	dlg_privilege.h
	dlg_update.h
	irc_auth.h
	irc_kernel.h
	log.h
	mainwindow.h
	network.h
	network_thread.h
	nick_avatar.h
	qinputline.h
	qnicklist.h
	qnicklistdelegate.h
	qnotify.h
	replace.h
	tab_container.h
	tab_manager.h
	tab_widget.h
	update.h
	update_thread.h
)

set (scc_HDRS
	debug.h
)

# debug
if (UNIX)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
endif (UNIX)

# qt qtnetwork
include_directories (
	${QT_INCLUDE_DIR}
	${QT_QTNETWORK_INCLUDE_DIR}
)

# qca2
if (WIN32)
	set (QCA2_INCLUDE_DIRS ${WIN32_QCA2_DIR}/include/QtCrypto)
	set (QCA2_LIBRARY_DIRS ${WIN32_QCA2_DIR}/bin)
	set (QCA2_LIBRARIES -lqca2)
else (WIN32)
	pkg_search_module (QCA2 REQUIRED qca2)
endif (WIN32)

include_directories (${QCA2_INCLUDE_DIRS})
link_directories (${QCA2_LIBRARY_DIRS})
add_definitions (${QCA2_CFLAGS_OTHER})

# phonon
if (WIN32)
	set (PHONON_INCLUDE_DIRS ${QT_INCLUDE_DIR}/phonon)
	set (PHONON_LIBRARY_DIRS ${QT_LIBRARY_DIR})
	set (PHONON_LIBRARIES -lphonon4)
else (WIN32)
	pkg_search_module (PHONON REQUIRED phonon)
endif (WIN32)

include_directories (${PHONON_INCLUDE_DIRS})
link_directories (${PHONON_LIBRARY_DIRS})

# xml
set (QT_USE_QTXML TRUE)

# webkit
set(QT_USE_QTWEBKIT TRUE)

# resources
set (scc_QRC ../scc.qrc)
qt4_add_resources (scc_QRC_SRCS ${scc_QRC})

# ui
QT4_WRAP_UI (scc_UIS_H ${scc_UIS})
include_directories (${CMAKE_SOURCE_DIR}/ui)

# disable console on windows
if (MINGW)
	SET (CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -mwindows")
endif (MINGW)

# rest
include (${QT_USE_FILE})

qt_wrap_cpp (scc MOC_FILES ${scc_MOC_HDRS})
add_executable (../scc ${scc_SRCS} ${MOC_FILES} ${scc_HDRS} ${scc_QRC_SRCS} ${scc_UIS_H})
target_link_libraries (../scc ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARIES} ${QCA2_LIBRARIES} ${PHONON_LIBRARIES})

add_dependencies (../scc i18n)
