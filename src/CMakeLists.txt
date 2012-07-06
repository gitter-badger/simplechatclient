if (WIN32)
	option (WITH_KAMERZYSTA "Compile with Kamerzysta" ON)
else (WIN32)
	option (WITH_KAMERZYSTA "Compile with Kamerzysta" OFF)
endif (WIN32)

set (scc_UIS
	../ui/about.ui
	../ui/avatar_edit.ui
	../ui/avatar_list_widget.ui
	../ui/awaylog.ui
	../ui/captcha.ui
	../ui/channel_favourites.ui
	../ui/channel_homes.ui
	../ui/channel_key.ui
	../ui/channel_list.ui
	../ui/channel_settings.ui
	../ui/emoticons.ui
	../ui/find_nick.ui
	../ui/find_text.ui
	../ui/friends.ui
	../ui/ignore.ui
	../ui/invite.ui
	../ui/invite_list.ui
	../ui/moderation.ui
	../ui/my_avatar.ui
	../ui/my_profile.ui
	../ui/my_stats.ui
	../ui/notes.ui
	../ui/offline_messages.ui
	../ui/options.ui
	../ui/profile_add.ui
	../ui/profile_edit.ui
	../ui/profile_manager.ui
	../ui/register_nick.ui
	../ui/update.ui
	../ui/user_avatar.ui
	../ui/webbrowser.ui
)

set (scc_SRCS
	avatars/avatar.cpp

	common/commands.cpp
	common/config.cpp
	common/convert.cpp
	common/html_messages_renderer.cpp
	common/log.cpp
	common/replace.cpp
	common/simple_crypt.cpp
	common/xml.cpp

	core/core.cpp
	core/debug_block.cpp
	core/main.cpp
	core/mainwindow.cpp
	core/network.cpp

	gui/about.cpp
	gui/avatar_edit.cpp
	gui/awaylog.cpp
	gui/channel_favourites.cpp
	gui/channel_homes.cpp
	gui/channel_key.cpp
	gui/channel_list.cpp
	gui/channel_settings.cpp
	gui/emoticons.cpp
	gui/find_nick.cpp
	gui/find_text.cpp
	gui/friends.cpp
	gui/ignore.cpp
	gui/invite.cpp
	gui/invite_list.cpp
	gui/moderation.cpp
	gui/my_avatar.cpp
	gui/my_profile.cpp
	gui/my_stats.cpp
	gui/notes.cpp
	gui/offline_messages.cpp
	gui/options.cpp
	gui/profile_add.cpp
	gui/profile_edit.cpp
	gui/profile_manager.cpp
	gui/register_nick.cpp
	gui/update.cpp
	gui/user_avatar.cpp
	gui/user_profile.cpp
	gui/webbrowser.cpp

	onet/avatar_client.cpp
	onet/captcha.cpp
	onet/my_avatar_model.cpp
	onet/onet_auth.cpp
	onet/onet_kernel.cpp
	onet/onet_utils.cpp

	singleton/autoaway.cpp
	singleton/away.cpp
	singleton/awaylog_model.cpp
	singleton/busy.cpp
	singleton/channel.cpp
	singleton/channel_homes_model.cpp
	singleton/channel_list_model.cpp
	singleton/find_nick_model.cpp
	singleton/highlight.cpp
	singleton/invite_model.cpp
	singleton/lag.cpp
	singleton/message.cpp
	singleton/nicklist.cpp
	singleton/notes_model.cpp
	singleton/notification.cpp
	singleton/offline.cpp
	singleton/punish_reason.cpp
	singleton/sound_notify.cpp
	singleton/tray.cpp
	singleton/updates.cpp

	tab/tab_container.cpp
	tab/tab_manager.cpp
	tab/tab_widget.cpp

	widgets/avatar_edit_scene.cpp
	widgets/avatar_list_widget.cpp
	widgets/chat_view.cpp
	widgets/inputline_widget.cpp
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
	common/simple_crypt.h
	common/xml.h

	core/core.h
	core/mainwindow.h
	core/network.h

	gui/about.h
	gui/avatar_edit.h
	gui/awaylog.h
	gui/channel_favourites.h
	gui/channel_homes.h
	gui/channel_key.h
	gui/channel_list.h
	gui/channel_settings.h
	gui/emoticons.h
	gui/find_nick.h
	gui/find_text.h
	gui/friends.h
	gui/ignore.h
	gui/invite.h
	gui/invite_list.h
	gui/moderation.h
	gui/my_avatar.h
	gui/my_profile.h
	gui/my_stats.h
	gui/notes.h
	gui/offline_messages.h
	gui/options.h
	gui/profile_add.h
	gui/profile_edit.h
	gui/profile_manager.h
	gui/register_nick.h
	gui/update.h
	gui/user_avatar.h
	gui/user_profile.h
	gui/webbrowser.h

	onet/avatar_client.h
	onet/captcha.h
	onet/onet_auth.h
	onet/onet_kernel.h

	singleton/autoaway.h
	singleton/away.h
	singleton/awaylog_model.h
	singleton/busy.h
	singleton/channel.h
	singleton/channel_homes_model.h
	singleton/channel_list_model.h
	singleton/find_nick_model.h
	singleton/highlight.h
	singleton/invite_model.h
	singleton/lag.h
	singleton/message.h
	singleton/nicklist.h
	singleton/notes_model.h
	singleton/notification.h
	singleton/offline.h
	singleton/punish_reason.h
	singleton/sound_notify.h
	singleton/tray.h
	singleton/updates.h

	tab/tab_container.h
	tab/tab_manager.h
	tab/tab_widget.h

	widgets/avatar_edit_scene.h
	widgets/avatar_list_widget.h
	widgets/chat_view.h
	widgets/inputline_widget.h
	widgets/nicklist_delegate.h
	widgets/nicklist_widget.h
	widgets/simple_rank_widget.h
	widgets/simple_stats_widget.h
	widgets/tool_widget.h
)

set (scc_HDRS
	common/convert.h
	common/html_messages_renderer.cpp
	common/log.cpp
	common/replace.h
	core/defines.h
	core/debug_block.h
)

# debug
if (UNIX)
	set (CMAKE_CXX_FLAGS "-ggdb ${CMAKE_CXX_FLAGS}")
endif (UNIX)

# wall
set (CMAKE_C_FLAGS "-W -Wall ${CMAKE_C_FLAGS}")
set (CMAKE_CXX_FLAGS "-W -Wall ${CMAKE_CXX_FLAGS}")

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
if (WITH_KAMERZYSTA)
	message (STATUS "Kamerzysta support enabled.")
	LIST (APPEND scc_SRCS
		common/kamerzysta.cpp
	)
	LIST (APPEND scc_MOC_HDRS
		common/kamerzysta.h
	)
else (WITH_KAMERZYSTA)
	message (STATUS "Kamerzysta support disabled. Using internal Webcam window")
	LIST (APPEND scc_UIS
		../ui/webcam_standard.ui
		../ui/webcam_mini.ui
	)
	LIST (APPEND scc_SRCS
		common/webcam_engine.cpp
		common/webcam_network.cpp
		gui/webcam.cpp
		gui/webcam_standard.cpp
		gui/webcam_mini.cpp
		widgets/webcam_delegate.cpp
	)
	LIST (APPEND scc_MOC_HDRS
		common/webcam_engine.h
		common/webcam_network.h
		gui/webcam.h
		gui/webcam_standard.h
		gui/webcam_mini.h
		widgets/webcam_delegate.h
	)
endif (WITH_KAMERZYSTA)

# winamp
if (WIN32)
	LIST (APPEND scc_SRCS
		common/winamp.cpp
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
	pkg_search_module (QCA2 REQUIRED qca2)
endif (WIN32)

include_directories (${QCA2_INCLUDE_DIRS})
link_directories (${QCA2_LIBRARY_DIRS})
add_definitions (${QCA2_CFLAGS_OTHER})

option (WITH_PHONON "Compile with Phonon support" ON)
if (WITH_PHONON)
	pkg_search_module (PHONON phonon)
	if (PHONON_FOUND)
		set (PHONON_LIBRARIES ${PHONON_LIBRARIES})
		link_directories (${PHONON_LIBRARY_DIRS})
		include_directories (${PHONON_INCLUDE_DIRS})
	else (PHONON_FOUND)
		if (QT_PHONON_FOUND)
			set (PHONON_LIBRARIES ${QT_PHONON_LIBRARY})
			include_directories (${QT_PHONON_INCLUDE_DIR})
		else (QT_PHONON_FOUND)
			message (FATAL_ERROR "Could not find Phonon library")
		endif (QT_PHONON_FOUND)
	endif (PHONON_FOUND)
	message (STATUS "Found Phonon library: ${PHONON_LIBRARIES}") 
else (WITH_PHONON)
	message (STATUS "Phonon disabled")
endif (WITH_PHONON)

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
	${CMAKE_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/avatars
	${CMAKE_CURRENT_SOURCE_DIR}/common
	${CMAKE_CURRENT_SOURCE_DIR}/core
	${CMAKE_CURRENT_SOURCE_DIR}/gui
	${CMAKE_CURRENT_SOURCE_DIR}/onet
	${CMAKE_CURRENT_SOURCE_DIR}/singleton
	${CMAKE_CURRENT_SOURCE_DIR}/tab
	${CMAKE_CURRENT_SOURCE_DIR}/widgets
)

# resources
set (scc_QRC ../scc.qrc)
qt4_add_resources (scc_QRC_SRCS ${scc_QRC})

# ui
qt4_wrap_ui (scc_UIS_H ${scc_UIS})

# moc
qt_wrap_cpp (scc MOC_FILES ${scc_MOC_HDRS})

# resource
set (scc_RESOURCE ../scc_mingw.rc)

include_directories (${CMAKE_SOURCE_DIR}/ui)

if (MINGW)
	execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)

	if (GCC_VERSION VERSION_GREATER 4.5 OR GCC_VERSION VERSION_EQUAL 4.5)
		# enable ASLR
		set (CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -Wl,--dynamicbase -Wl,--nxcompat")
	else (GCC_VERSION VERSION_GREATER 4.5 OR GCC_VERSION VERSION_EQUAL 4.5)
		message(STATUS "DEP and ASLR not supported. Use GCC 4.5 or greater.")
	endif (GCC_VERSION VERSION_GREATER 4.5 OR GCC_VERSION VERSION_EQUAL 4.5)

	# disable console on windows mingw
	set (CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -mwindows")
endif (MINGW)

# link
if (MINGW) # mingw
	add_executable (scc ${scc_SRCS} ${MOC_FILES} ${scc_HDRS} ${scc_QRC_SRCS} ${scc_UIS_H} ${scc_RESOURCE})
else (MINGW) # linux
	add_executable (scc ${scc_SRCS} ${MOC_FILES} ${scc_HDRS} ${scc_QRC_SRCS} ${scc_UIS_H})
endif (MINGW)

target_link_libraries (scc ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARIES} ${QCA2_LIBRARIES} ${PHONON_LIBRARIES})

# install
if (UNIX AND NOT APPLE)
	install (TARGETS scc DESTINATION ${SCC_BIN_DIR})
endif (UNIX AND NOT APPLE)
