
# init

project (scc)
cmake_minimum_required (VERSION 2.8.1 FATAL_ERROR)
set (QT_MIN_VERSION "4.6.2")
find_package (Qt4 REQUIRED)

# set
set (CMAKE_INSTALL_PREFIX /usr)
set (SCC_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)
set (SCC_SHARE_DIR ${CMAKE_INSTALL_PREFIX}/share)
set (SCC_DATA_DIR ${CMAKE_INSTALL_PREFIX}/share/scc)

# modules

set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

# include

include (FindPkgConfig)
include (CheckIncludeFiles)

# configure
configure_file (${CMAKE_SOURCE_DIR}/scc-config.h.cmake ${CMAKE_BINARY_DIR}/scc-config.h)

# uninstall

if (UNIX AND NOT APPLE)
	CONFIGURE_FILE ("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
	ADD_CUSTOM_TARGET (uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif (UNIX AND NOT APPLE)

# sub dir

add_subdirectory (src)
add_subdirectory (translations)

# install

if (UNIX AND NOT APPLE)
	install (FILES AUTHORS ChangeLog LICENSE README scc.png TODO DESTINATION ${SCC_DATA_DIR})
	install (FILES scc.desktop DESTINATION ${SCC_SHARE_DIR}/applications)

	file (GLOB SCC_GLOB_FILES1 images/wallpaper/*.jpg)
	install (FILES ${SCC_GLOB_FILES1} DESTINATION ${SCC_DATA_DIR}/images/wallpaper)

	install (FILES 3rdparty/LICENSE DESTINATION ${SCC_DATA_DIR}/3rdparty)

	file (GLOB SCC_GLOB_FILES2 3rdparty/sounds/*.wav)
	install (FILES ${SCC_GLOB_FILES2} DESTINATION ${SCC_DATA_DIR}/3rdparty/sounds)

	file (GLOB SCC_GLOB_FILES3 3rdparty/emoticons/*.gif)
	install (FILES ${SCC_GLOB_FILES3} DESTINATION ${SCC_DATA_DIR}/3rdparty/emoticons)

	file (GLOB SCC_GLOB_FILES4 3rdparty/emoticons_other/*.gif)
	install (FILES ${SCC_GLOB_FILES4} DESTINATION ${SCC_DATA_DIR}/3rdparty/emoticons_other)
endif (UNIX AND NOT APPLE)
