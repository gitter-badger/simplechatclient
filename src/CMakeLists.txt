
set (scc_UIS
	../ui/about.ui
	../ui/cam.ui
	../ui/channel_favourites.ui
	../ui/channel_homes.ui
	../ui/channel_key.ui
	../ui/channel_list.ui
	../ui/channel_settings.ui
	../ui/email.ui
	../ui/emoticons.ui
	../ui/friends.ui
	../ui/ignore.ui
	../ui/invite.ui
	../ui/moderation.ui
	../ui/options.ui
	../ui/register_nick.ui
	../ui/update.ui
	../ui/user_profile.ui
)

set (scc_SRCS
	avatars/channel_avatar.cpp
	avatars/nick_avatar.cpp
	common/commands.cpp
	common/config.cpp
	common/convert.cpp
	common/crypt.cpp
	common/kamerzysta.cpp
	common/log.cpp
	common/replace.cpp
	core/core.cpp
	core/debug.cpp
	core/main.cpp
	core/mainwindow.cpp
	core/network.cpp
	core/network_thread.cpp
	core/update.cpp
	gui/dlg_about.cpp
	gui/dlg_cam.cpp
	gui/dlg_channel_favourites.cpp
	gui/dlg_channel_homes.cpp
	gui/dlg_channel_key.cpp
	gui/dlg_channel_list.cpp
	gui/dlg_channel_settings.cpp
	gui/dlg_email.cpp
	gui/dlg_emoticons.cpp
	gui/dlg_friends.cpp
	gui/dlg_ignore.cpp
	gui/dlg_invite.cpp
	gui/dlg_moderation.cpp
	gui/dlg_options.cpp
	gui/dlg_register_nick.cpp
	gui/dlg_update.cpp
	gui/dlg_user_profile.cpp
	notify/notify.cpp
	onet/onet_auth.cpp
	onet/onet_kernel.cpp
	tab/tab_container.cpp
	tab/tab_manager.cpp
	tab/tab_widget.cpp
	widgets/inputlinewidget.cpp
	widgets/inputlinedockwidget.cpp
	widgets/mainwebview.cpp
	widgets/nicklistdelegate.cpp
	widgets/nicklistwidget.cpp
	widgets/nicklistdockwidget.cpp
)

set (scc_MOC_HDRS
	avatars/channel_avatar.h
	avatars/nick_avatar.h
	common/commands.h
	common/config.h
	common/convert.h
	common/crypt.h
	common/kamerzysta.h
	common/log.h
	common/replace.h
	core/core.h
	core/mainwindow.h
	core/network.h
	core/network_thread.h
	core/update.h
	gui/dlg_about.h
	gui/dlg_cam.h
	gui/dlg_channel_favourites.h
	gui/dlg_channel_homes.h
	gui/dlg_channel_key.h
	gui/dlg_channel_list.h
	gui/dlg_channel_settings.h
	gui/dlg_email.h
	gui/dlg_emoticons.h
	gui/dlg_friends.h
	gui/dlg_ignore.h
	gui/dlg_invite.h
	gui/dlg_moderation.h
	gui/dlg_options.h
	gui/dlg_register_nick.h
	gui/dlg_update.h
	gui/dlg_user_profile.h
	notify/notify.h
	onet/onet_auth.h
	onet/onet_kernel.h
	tab/tab_container.h
	tab/tab_manager.h
	tab/tab_widget.h
	widgets/inputlinewidget.h
	widgets/inputlinedockwidget.h
	widgets/mainwebview.h
	widgets/nicklistdelegate.h
	widgets/nicklistwidget.h
	widgets/nicklistdockwidget.h
)

set (scc_HDRS
	core/debug.h
)

# debug
if (UNIX)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
endif (UNIX)

if (UNIX AND NOT APPLE)
	set (CMAKE_C_FLAGS "-W -Wall -fno-strict-aliasing ${CMAKE_C_FLAGS}")
	set (CMAKE_CXX_FLAGS "-W -Wall -fno-strict-aliasing ${CMAKE_CXX_FLAGS}")
endif (UNIX AND NOT APPLE)

# qt
find_package (Qt4 REQUIRED)

# xml
set (QT_USE_QTXML TRUE)

# webkit
set (QT_USE_QTWEBKIT TRUE)

# rest
include (${QT_USE_FILE})

# include qt qtnetwork
include_directories (
	${QT_INCLUDE_DIR}
	${QT_QTNETWORK_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/avatars
	${CMAKE_CURRENT_SOURCE_DIR}/common
	${CMAKE_CURRENT_SOURCE_DIR}/core
	${CMAKE_CURRENT_SOURCE_DIR}/gui
	${CMAKE_CURRENT_SOURCE_DIR}/notify
	${CMAKE_CURRENT_SOURCE_DIR}/onet
	${CMAKE_CURRENT_SOURCE_DIR}/tab
	${CMAKE_CURRENT_SOURCE_DIR}/widgets
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

# resources
set (scc_QRC ../scc.qrc)
qt4_add_resources (scc_QRC_SRCS ${scc_QRC})

# ui
qt4_wrap_ui (scc_UIS_H ${scc_UIS})

include_directories (${CMAKE_SOURCE_DIR}/ui)

# disable console on windows
if (MINGW)
	SET (CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -mwindows")
endif (MINGW)

# moc
qt_wrap_cpp (scc MOC_FILES ${scc_MOC_HDRS})

# link
if (MSVC) # windows
	add_executable (../scc WIN32 ${scc_SRCS} ${MOC_FILES} ${scc_HDRS} ${scc_QRC_SRCS} ${scc_UIS_H})
else (MSVC) # mingw or linux
	if (MINGW) # mingw
		add_executable (../scc ${scc_SRCS} ${MOC_FILES} ${scc_HDRS} ${scc_QRC_SRCS} ${scc_UIS_H})
	else (MINGW) # linux
		add_executable (../scc ${scc_SRCS} ${MOC_FILES} ${scc_HDRS} ${scc_QRC_SRCS} ${scc_UIS_H})
	endif (MINGW)
endif (MSVC)

target_link_libraries (../scc ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARIES} ${QCA2_LIBRARIES} ${PHONON_LIBRARIES})

# i18n
add_dependencies (../scc i18n)
