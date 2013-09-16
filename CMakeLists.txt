# init
project (scc)
cmake_minimum_required (VERSION 2.8.9 FATAL_ERROR)
set (QT_MIN_VERSION "4.8.0")
find_package (Qt4 REQUIRED)

# optimization
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo." FORCE)
endif (NOT CMAKE_BUILD_TYPE)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

# warnings
#set (CMAKE_C_FLAGS "-W -Wall -Wextra ${CMAKE_C_FLAGS}")
#set (CMAKE_CXX_FLAGS "-W -Wall -Wextra ${CMAKE_CXX_FLAGS}")

# debug
#if (UNIX)
#	set (CMAKE_CXX_FLAGS "-ggdb -pg ${CMAKE_CXX_FLAGS}")
#endif (UNIX)

if (MINGW)
	execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)

	if (GCC_VERSION VERSION_GREATER 4.5 OR GCC_VERSION VERSION_EQUAL 4.5)
		# enable ASLR
		set (CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -Wl,--dynamicbase -Wl,--nxcompat")
	else (GCC_VERSION VERSION_GREATER 4.5 OR GCC_VERSION VERSION_EQUAL 4.5)
		message(STATUS "DEP and ASLR not supported. Use GCC 4.5 or greater.")
	endif (GCC_VERSION VERSION_GREATER 4.5 OR GCC_VERSION VERSION_EQUAL 4.5)

	# disable console on windows mingw
	if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
		set (CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -mwindows")
	endif (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
endif (MINGW)

# set
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX /usr)
endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set (SCC_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)
set (SCC_SHARE_DIR ${CMAKE_INSTALL_PREFIX}/share)
set (SCC_DATA_DIR ${CMAKE_INSTALL_PREFIX}/share/scc)

# include
include (FindPkgConfig)
include (CheckIncludeFiles)

# sub dir
add_subdirectory (src)
add_subdirectory (translations)

# configure
configure_file (${CMAKE_SOURCE_DIR}/scc-config.h.cmake ${CMAKE_BINARY_DIR}/scc-config.h)

if (UNIX AND NOT APPLE)
	message (STATUS " * install prefix: ${CMAKE_INSTALL_PREFIX}")
endif (UNIX AND NOT APPLE)
message (STATUS " * build type: ${CMAKE_BUILD_TYPE}")

# install
if (UNIX AND NOT APPLE)
	install (FILES AUTHORS ChangeLog LICENSE README.rst scc.png TODO DESTINATION ${SCC_DATA_DIR})
	install (FILES scc.desktop DESTINATION ${SCC_SHARE_DIR}/applications)
	install (FILES images/user_avatar.png DESTINATION ${SCC_DATA_DIR}/images)

	install (DIRECTORY images/wallpaper DESTINATION ${SCC_DATA_DIR}/images)
	install (DIRECTORY sounds DESTINATION ${SCC_DATA_DIR})
	install (DIRECTORY emoticons DESTINATION ${SCC_DATA_DIR})
endif (UNIX AND NOT APPLE)

if (MINGW)
	install (FILES src/scc.exe DESTINATION ${CMAKE_SOURCE_DIR}/release)
endif (MINGW)

# uninstall
if (UNIX AND NOT APPLE)
	configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
	add_custom_target (uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif (UNIX AND NOT APPLE)
