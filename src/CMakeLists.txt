
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
	../ui/find_text.ui
	../ui/friends.ui
	../ui/ignore.ui
	../ui/invite.ui
	../ui/moderation.ui
	../ui/my_avatar.ui
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
	../ui/webbrowser.ui
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
	gui/find_text.cpp
	gui/friends.cpp
	gui/ignore.cpp
	gui/invite.cpp
	gui/moderation.cpp
	gui/my_avatar.cpp
	gui/my_profile.cpp
	gui/my_stats.cpp
	gui/notes.cpp
	gui/offlinemsg.cpp
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

	singleton/awaylog_model.cpp
	singleton/message.cpp
	singleton/nicklist.cpp
	singleton/notes_model.cpp
	singleton/notify.cpp
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
	common/convert.h
	common/crypt.h
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
	gui/find_text.h
	gui/friends.h
	gui/ignore.h
	gui/invite.h
	gui/moderation.h
	gui/my_avatar.h
	gui/my_profile.h
	gui/my_stats.h
	gui/notes.h
	gui/offlinemsg.h
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

	singleton/awaylog_model.h
	singleton/message.h
	singleton/nicklist.h
	singleton/notes_model.h
	singleton/notify.h
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
endif (WIN32)

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

find_package (Phonon)
include_directories (${PHONON_INCLUDE_DIR})

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

target_link_libraries (scc ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARIES} ${QCA2_LIBRARIES} ${PHONON_LIBRARY})

# install
if (UNIX AND NOT APPLE)
	install (TARGETS scc DESTINATION ${SCC_BIN_DIR})
endif (UNIX AND NOT APPLE)
