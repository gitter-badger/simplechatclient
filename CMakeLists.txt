
# init

project (scc)
cmake_minimum_required (VERSION 2.8.0 FATAL_ERROR)
set (QT_MIN_VERSION "4.6.3")
find_package (Qt4 REQUIRED)

# include

include (FindPkgConfig)

# sub dir

add_subdirectory (i18n)
add_subdirectory (src)

# install

if (UNIX AND NOT APPLE)
	set (scc_BIN_DIR /usr/local/bin)
	set (scc_SHARE_DIR /usr/local/share)

	set (scc_MAIN_FILE scc)

	file (GLOB scc_I18N_FILES i18n/*.qm)
	file (GLOB scc_IMAGES_FILES images/*.png)

	file (GLOB scc_EMOTICONS1_FILES 3rdparty/emoticons/*.gif)
	file (GLOB scc_EMOTICONS2_FILES 3rdparty/emoticons_other/*.gif)
	file (GLOB scc_IMAGES_FILES 3rdparty/images/*.png)
	file (GLOB scc_SOUNDS_FILES 3rdparty/sounds/*.wav)
	set (scc_3RDPARTY_LICENSE 3rdparty/LICENSE)

	install (FILES ${scc_MAIN_FILE} DESTINATION ${scc_BIN_DIR})
	install (FILES ${scc_I18N_FILES} DESTINATION ${scc_SHARE_DIR}/scc/i18n)
	install (FILES ${scc_IMAGES_FILES} DESTINATION ${scc_SHARE_DIR}/scc/images)

	install (FILES ${scc_EMOTICONS1_FILES} DESTINATION ${scc_SHARE_DIR}/scc/3rdparty/emoticons)
	install (FILES ${scc_EMOTICONS2_FILES} DESTINATION ${scc_SHARE_DIR}/scc/3rdparty/emoticons_other)
	install (FILES ${scc_IMAGES_FILES} DESTINATION ${scc_SHARE_DIR}/scc/3rdparty/images)
	install (FILES ${scc_SOUNDS_FILES} DESTINATION ${scc_SHARE_DIR}/scc/3rdparty/sounds)
	install (FILES ${scc_3RDPARTY_LICENSE} DESTINATION ${scc_SHARE_DIR}/scc/3rdparty)
endif (UNIX AND NOT APPLE)

# uninstall

if (UNIX AND NOT APPLE)
	CONFIGURE_FILE ("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
	ADD_CUSTOM_TARGET (uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif (UNIX AND NOT APPLE)
