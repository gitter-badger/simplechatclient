
set (scc_UIS
	../ui/about.ui
	../ui/cam.ui
	../ui/channel_favourites.ui
	../ui/channel_favourites_ad.ui
	../ui/channel_homes.ui
	../ui/channel_homes_ad.ui
	../ui/channel_key.ui
	../ui/channel_list.ui
	../ui/channel_settings.ui
	../ui/email.ui
	../ui/friends.ui
	../ui/friends_ad.ui
	../ui/ignore.ui
	../ui/ignore_ad.ui
	../ui/invite.ui
	../ui/kick.ui
	../ui/moderation.ui
	../ui/options.ui
	../ui/privilege.ui
	../ui/register_nick.ui
	../ui/update.ui
)

set (scc_SRCS
	avatars/channel_avatar.cpp
	common/commands.cpp
	common/config.cpp
	common/crypt.cpp
	core/debug.cpp
	gui/dlg_about.cpp
	gui/dlg_cam.cpp
	gui/dlg_channel_favourites.cpp
	gui/dlg_channel_favourites_ad.cpp
	gui/dlg_channel_homes.cpp
	gui/dlg_channel_homes_ad.cpp
	gui/dlg_channel_key.cpp
	gui/dlg_channel_list.cpp
	gui/dlg_channel_settings.cpp
	gui/dlg_email.cpp
	gui/dlg_friends.cpp
	gui/dlg_friends_ad.cpp
	gui/dlg_ignore.cpp
	gui/dlg_ignore_ad.cpp
	gui/dlg_invite.cpp
	gui/dlg_kick.cpp
	gui/dlg_moderation.cpp
	gui/dlg_options.cpp
	gui/dlg_privilege.cpp
	gui/dlg_register_nick.cpp
	gui/dlg_update.cpp
	onet/onet_auth.cpp
	onet/onet_kernel.cpp
	common/log.cpp
	core/main.cpp
	core/mainwindow.cpp
	core/network.cpp
	core/network_thread.cpp
	avatars/nick_avatar.cpp
	widgets/qinputline.cpp
	widgets/qnicklist.cpp
	widgets/qnicklistdelegate.cpp
	notify/qnotify.cpp
	common/replace.cpp
	tab/tab_container.cpp
	tab/tab_manager.cpp
	tab/tab_widget.cpp
	core/update.cpp
	core/update_thread.cpp
)

set (scc_MOC_HDRS
	avatars/channel_avatar.h
	common/commands.h
	common/config.h
	common/crypt.h
	gui/dlg_about.h
	gui/dlg_cam.h
	gui/dlg_channel_favourites.h
	gui/dlg_channel_favourites_ad.h
	gui/dlg_channel_homes.h
	gui/dlg_channel_homes_ad.h
	gui/dlg_channel_key.h
	gui/dlg_channel_list.h
	gui/dlg_channel_settings.h
	gui/dlg_email.h
	gui/dlg_friends.h
	gui/dlg_friends_ad.h
	gui/dlg_ignore.h
	gui/dlg_ignore_ad.h
	gui/dlg_invite.h
	gui/dlg_kick.h
	gui/dlg_moderation.h
	gui/dlg_options.h
	gui/dlg_privilege.h
	gui/dlg_register_nick.h
	gui/dlg_update.h
	onet/onet_auth.h
	onet/onet_kernel.h
	common/log.h
	core/mainwindow.h
	core/network.h
	core/network_thread.h
	avatars/nick_avatar.h
	widgets/qinputline.h
	widgets/qnicklist.h
	widgets/qnicklistdelegate.h
	notify/qnotify.h
	common/replace.h
	tab/tab_container.h
	tab/tab_manager.h
	tab/tab_widget.h
	core/update.h
	core/update_thread.h
)

set (scc_HDRS
	core/debug.h
)

# debug
if (UNIX)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
endif (UNIX)

# qt qtnetwork
include_directories (
	${QT_INCLUDE_DIR}
	${QT_QTNETWORK_INCLUDE_DIR}
	avatars
	common
	core
	gui
	notify
	onet
	tab
	widgets
	.
)

# qca2
if (WIN32)
	set (QCA2_INCLUDE_DIRS ${WIN32_QCA2_DIR}/include/QtCrypto)
	set (QCA2_LIBRARY_DIRS ${WIN32_QCA2_DIR}/lib)
	if (MINGW)
		set (QCA2_LIBRARIES -lqca2)
	else (MINGW)
		set (QCA2_LIBRARIES ${WIN32_QCA2_DIR}/lib/qca2.lib)
	endif (MINGW)
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
	if (MINGW)
		set (PHONON_LIBRARIES -lphonon4)
	else (MINGW)
		set (PHONON_LIBRARIES ${QT_LIBRARY_DIR}/phonon4.lib)
	endif (MINGW)
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

if (MSVC)
	add_executable (../scc WIN32 ${scc_SRCS} ${MOC_FILES} ${scc_HDRS} ${scc_QRC_SRCS} ${scc_UIS_H})
else (MSVC)
	add_executable (../scc ${scc_SRCS} ${MOC_FILES} ${scc_HDRS} ${scc_QRC_SRCS} ${scc_UIS_H})
endif (MSVC)

target_link_libraries (../scc ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARIES} ${QCA2_LIBRARIES} ${PHONON_LIBRARIES})

add_dependencies (../scc i18n)
