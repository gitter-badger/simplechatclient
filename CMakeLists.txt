# init
project (scc)
cmake_minimum_required (VERSION 2.8.7 FATAL_ERROR)
set (QT_MIN_VERSION "4.8.0")
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

# sub dir
add_subdirectory (src)
add_subdirectory (translations)

# configure
configure_file (${CMAKE_SOURCE_DIR}/scc-config.h.cmake ${CMAKE_BINARY_DIR}/scc-config.h)

# install
if (UNIX AND NOT APPLE)
	install (FILES AUTHORS ChangeLog LICENSE README.rst scc.png TODO DESTINATION ${SCC_DATA_DIR})
	install (FILES scc.desktop DESTINATION ${SCC_SHARE_DIR}/applications)
	install (FILES images/user_avatar.png DESTINATION ${SCC_DATA_DIR}/images)
	install (FILES 3rdparty/LICENSE DESTINATION ${SCC_DATA_DIR}/3rdparty)

	install (DIRECTORY scripts DESTINATION ${SCC_DATA_DIR})
	install (DIRECTORY images/wallpaper DESTINATION ${SCC_DATA_DIR}/images)
	install (DIRECTORY 3rdparty/sounds DESTINATION ${SCC_DATA_DIR}/3rdparty)
	install (DIRECTORY 3rdparty/emoticons DESTINATION ${SCC_DATA_DIR}/3rdparty)
endif (UNIX AND NOT APPLE)

if (MINGW)
	install (FILES src/scc.exe DESTINATION ${CMAKE_SOURCE_DIR}/release)
endif (MINGW)

# uninstall
if (UNIX AND NOT APPLE)
	configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
	add_custom_target (uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif (UNIX AND NOT APPLE)
