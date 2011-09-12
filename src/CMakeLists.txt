
set (scc_UIS
	../ui/about.ui
	../ui/awaylog.ui
	../ui/captcha.ui
	../ui/channel_favourites.ui
	../ui/channel_homes.ui
	../ui/channel_key.ui
	../ui/channel_list.ui
	../ui/channel_settings.ui
	../ui/emoticons.ui
	../ui/friends.ui
	../ui/ignore.ui
	../ui/invite.ui
	../ui/moderation.ui
	../ui/my_profile.ui
	../ui/my_stats.ui
	../ui/notes.ui
	../ui/offlinemsg.ui
	../ui/options.ui
	../ui/profile_add.ui
	../ui/profile_edit.ui
	../ui/profile_manager.ui
	../ui/register_nick.ui
	../ui/update.ui
	../ui/user_avatar.ui
)

set (scc_SRCS
	avatars/avatar.cpp
	common/commands.cpp
	common/config.cpp
	common/convert.cpp
	common/crypt.cpp
	common/html_messages_renderer.cpp
	common/log.cpp
	common/replace.cpp
	core/core.cpp
	core/debug_block.cpp
	core/main.cpp
	core/mainwindow.cpp
	core/network.cpp
	core/update.cpp
	gui/dlg_about.cpp
	gui/dlg_awaylog.cpp
	gui/dlg_channel_favourites.cpp
	gui/dlg_channel_homes.cpp
	gui/dlg_channel_key.cpp
	gui/dlg_channel_list.cpp
	gui/dlg_channel_settings.cpp
	gui/dlg_emoticons.cpp
	gui/dlg_friends.cpp
	gui/dlg_ignore.cpp
	gui/dlg_invite.cpp
	gui/dlg_moderation.cpp
	gui/dlg_my_profile.cpp
	gui/dlg_my_stats.cpp
	gui/dlg_notes.cpp
	gui/dlg_offlinemsg.cpp
	gui/dlg_options.cpp
	gui/dlg_profile_add.cpp
	gui/dlg_profile_edit.cpp
	gui/dlg_profile_manager.cpp
	gui/dlg_register_nick.cpp
	gui/dlg_update.cpp
	gui/dlg_user_avatar.cpp
	gui/dlg_user_profile.cpp
	notify/notify.cpp
	onet/dlg_captcha.cpp
	onet/onet_auth.cpp
	onet/onet_kernel.cpp
	onet/onet_utils.cpp
	tab/tab_container.cpp
	tab/tab_manager.cpp
	tab/tab_widget.cpp
	widgets/chat_view.cpp
	widgets/inputline_widget.cpp
	widgets/inputline_dock_widget.cpp
	widgets/input_widget.cpp
	widgets/nicklist_delegate.cpp
	widgets/nicklist_widget.cpp
	widgets/simple_rank_widget.cpp
	widgets/simple_stats_widget.cpp
	widgets/tool_widget.cpp
)

set (scc_MOC_HDRS
	avatars/avatar.h
	common/commands.h
	common/config.h
	common/convert.h
	common/crypt.h
	common/html_messages_renderer.h
	common/log.h
	common/replace.h
	core/core.h
	core/mainwindow.h
	core/network.h
	core/update.h
	gui/dlg_about.h
	gui/dlg_awaylog.h
	gui/dlg_channel_favourites.h
	gui/dlg_channel_homes.h
	gui/dlg_channel_key.h
	gui/dlg_channel_list.h
	gui/dlg_channel_settings.h
	gui/dlg_emoticons.h
	gui/dlg_friends.h
	gui/dlg_ignore.h
	gui/dlg_invite.h
	gui/dlg_moderation.h
	gui/dlg_my_profile.h
	gui/dlg_my_stats.h
	gui/dlg_notes.h
	gui/dlg_offlinemsg.h
	gui/dlg_options.h
	gui/dlg_profile_add.h
	gui/dlg_profile_edit.h
	gui/dlg_profile_manager.h
	gui/dlg_register_nick.h
	gui/dlg_update.h
	gui/dlg_user_avatar.h
	gui/dlg_user_profile.h
	notify/notify.h
	onet/dlg_captcha.h
	onet/onet_auth.h
	onet/onet_kernel.h
	onet/onet_utils.h
	tab/tab_container.h
	tab/tab_manager.h
	tab/tab_widget.h
	widgets/chat_view.h
	widgets/inputline_widget.h
	widgets/inputline_dock_widget.h
	widgets/input_widget.h
	widgets/nicklist_delegate.h
	widgets/nicklist_widget.h
	widgets/simple_rank_widget.h
	widgets/simple_stats_widget.h
	widgets/tool_widget.h
)

set (scc_HDRS
	core/defines.h
	core/debug_block.h
)

# debug
if (UNIX)
	set (CMAKE_CXX_FLAGS "-ggdb ${CMAKE_CXX_FLAGS}")
endif (UNIX)

if (UNIX AND NOT APPLE)
	set (CMAKE_C_FLAGS "-W -Wall ${CMAKE_C_FLAGS}")
	set (CMAKE_CXX_FLAGS "-W -Wall ${CMAKE_CXX_FLAGS}")
endif (UNIX AND NOT APPLE)

# debug
if (UNIX)
	LIST (APPEND scc_SRCS
		core/debug.cpp
	)
	LIST (APPEND scc_HDRS
		core/debug.h
	)
endif (UNIX)

# kamerzysta/cam
if (WIN32)
	LIST (APPEND scc_SRCS
		common/kamerzysta.cpp
	)
	LIST (APPEND scc_MOC_HDRS
		common/kamerzysta.h
	)
else (WIN32)
	LIST (APPEND scc_UIS
		../ui/webcam_standard.ui
		../ui/webcam_mini.ui
	)
	LIST (APPEND scc_SRCS
		common/webcam_engine.cpp
		common/webcam_network.cpp
		gui/dlg_webcam.cpp
		gui/dlg_webcam_standard.cpp
		gui/dlg_webcam_mini.cpp
		widgets/webcam_delegate.cpp
	)
	LIST (APPEND scc_MOC_HDRS
		common/webcam_engine.h
		common/webcam_network.h
		gui/dlg_webcam.h
		gui/dlg_webcam_standard.h
		gui/dlg_webcam_mini.h
		widgets/webcam_delegate.h
	)
endif (WIN32)

# winamp
if (WIN32)
	LIST (APPEND scc_SRCS
		common/winamp.cpp
	)
	LIST (APPEND scc_MOC_HDRS
		common/winamp.h
	)
endif (WIN32)

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
# check for qca2
	find_package (QCA2)
	if (QCA2_FOUND)
		set (QCA2_INCLUDE_DIRS ${QT_INCLUDE_DIR}/../QtCrypto)
		set (QCA2_LIBRARY_DIRS ${QT_LIBRARY_DIR})
		set (QCA2_LIBRARIES -lqca)
	endif (QCA2_FOUND)
# if not working try:
# pkg_search_module (QCA2 REQUIRED qca2)
endif (WIN32)

include_directories (${QCA2_INCLUDE_DIRS})
link_directories (${QCA2_LIBRARY_DIRS})
add_definitions (${QCA2_CFLAGS_OTHER})

# qt
find_package (Qt4 REQUIRED)

# xml
set (QT_USE_QTXML TRUE)

# phonon
set (QT_USE_PHONON TRUE)

# webkit
set(QT_USE_QTWEBKIT TRUE)

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

# resources
set (scc_QRC ../scc.qrc)
qt4_add_resources (scc_QRC_SRCS ${scc_QRC})

# ui
qt4_wrap_ui (scc_UIS_H ${scc_UIS})

include_directories (${CMAKE_SOURCE_DIR}/ui)

# disable console on windows mingw
if (MINGW)
	SET (CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -mwindows")
endif (MINGW)

# moc
qt_wrap_cpp (scc MOC_FILES ${scc_MOC_HDRS})

# link
if (MSVC) # windows
	add_executable (scc WIN32 ${scc_SRCS} ${MOC_FILES} ${scc_HDRS} ${scc_QRC_SRCS} ${scc_UIS_H})
	target_link_libraries (scc ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARIES} ${QCA2_LIBRARIES})
else (MSVC) # mingw or linux
	if (MINGW) # mingw
		add_executable (scc ${scc_SRCS} ${MOC_FILES} ${scc_HDRS} ${scc_QRC_SRCS} ${scc_UIS_H})
	else (MINGW) # linux
		add_executable (scc ${scc_SRCS} ${MOC_FILES} ${scc_HDRS} ${scc_QRC_SRCS} ${scc_UIS_H})
	endif (MINGW)

	target_link_libraries (scc ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARIES} ${QCA2_LIBRARIES})
endif (MSVC)

# install
if (UNIX AND NOT APPLE)
	install (TARGETS scc DESTINATION ${SCC_BIN_DIR})
endif (UNIX AND NOT APPLE)
