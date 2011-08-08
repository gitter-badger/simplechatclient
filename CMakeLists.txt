
# init

project (scc)
cmake_minimum_required (VERSION 2.8.1 FATAL_ERROR)
set (QT_MIN_VERSION "4.6.2")
find_package (Qt4 REQUIRED)

# modules

set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

# include

include (FindPkgConfig)
include (CheckIncludeFiles)

# sub dir

add_subdirectory (translations)
add_subdirectory (src)

# install

if (UNIX AND NOT APPLE)
	set (scc_BIN_DIR /usr/bin)
	set (scc_SHARE_DIR /usr/share/scc)

	file (GLOB scc_EMOTICONS_FILES 3rdparty/emoticons/*.gif)
	file (GLOB scc_EMOTICONS_OTHER_FILES 3rdparty/emoticons_other/*.gif)
	file (GLOB scc_SOUNDS_FILES 3rdparty/sounds/*.wav)

	file (GLOB scc_IMAGES_FILES 3rdparty/images/wallpaper/*.jpg)
	file (GLOB scc_TRANSLATIONS_FILES translations/*.qm)

	install (FILES AUTHORS ChangeLog LICENSE README scc.png TODO DESTINATION ${scc_SHARE_DIR})
	install (FILES scc DESTINATION ${scc_BIN_DIR})

	install (FILES ${scc_IMAGES_FILES} DESTINATION ${scc_SHARE_DIR}/images/wallpaper)
	install (FILES ${scc_TRANSLATIONS_FILES} DESTINATION ${scc_SHARE_DIR}/translations)

	install (FILES ${scc_EMOTICONS_FILES} DESTINATION ${scc_SHARE_DIR}/3rdparty/emoticons)
	install (FILES ${scc_EMOTICONS_OTHER_FILES} DESTINATION ${scc_SHARE_DIR}/3rdparty/emoticons_other)
	install (FILES ${scc_SOUNDS_FILES} DESTINATION ${scc_SHARE_DIR}/3rdparty/sounds)
	install (FILES 3rdparty/LICENSE DESTINATION ${scc_SHARE_DIR}/3rdparty)

	install (FILES scc.desktop DESTINATION /usr/share/applications)
endif (UNIX AND NOT APPLE)

# uninstall

if (UNIX AND NOT APPLE)
	CONFIGURE_FILE ("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
	ADD_CUSTOM_TARGET (uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif (UNIX AND NOT APPLE)
