
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

	set (scc_INSTALL_DIR /opt)

	set (scc_INSTALL_MAIN_FILES
		LICENSE
		README
		TODO
		scc
		VERSION
	)

	file (GLOB scc_INSTALL_I18N_FILES i18n/*.qm)
	file (GLOB scc_INSTALL_IMAGES_FILES images/*.png)

	file (GLOB scc_INSTALL_EMOTICONS1_FILES 3rdparty/emoticons/*.gif)
	file (GLOB scc_INSTALL_EMOTICONS2_FILES 3rdparty/emoticons_other/*.gif)
	file (GLOB scc_INSTALL_IMAGES_FILES 3rdparty/images/*.png)
	file (GLOB scc_INSTALL_SOUNDS_FILES 3rdparty/sounds/*.wav)
	set (scc_INSTALL_3RDPARTY_LICENSE 3rdparty/LICENSE)

	install (FILES ${scc_INSTALL_MAIN_FILES} DESTINATION ${scc_INSTALL_DIR}/scc)
	install (FILES ${scc_INSTALL_I18N_FILES} DESTINATION ${scc_INSTALL_DIR}/scc/i18n)
	install (FILES ${scc_INSTALL_IMAGES_FILES} DESTINATION ${scc_INSTALL_DIR}/scc/images)

	install (FILES ${scc_INSTALL_EMOTICONS1_FILES} DESTINATION ${scc_INSTALL_DIR}/scc/3rdparty/emoticons)
	install (FILES ${scc_INSTALL_EMOTICONS2_FILES} DESTINATION ${scc_INSTALL_DIR}/scc/3rdparty/emoticons_other)
	install (FILES ${scc_INSTALL_IMAGES_FILES} DESTINATION ${scc_INSTALL_DIR}/scc/3rdparty/images)
	install (FILES ${scc_INSTALL_SOUNDS_FILES} DESTINATION ${scc_INSTALL_DIR}/scc/3rdparty/sounds)
	install (FILES ${scc_INSTALL_3RDPARTY_LICENSE} DESTINATION ${scc_INSTALL_DIR}/scc/3rdparty)


endif (UNIX AND NOT APPLE)
