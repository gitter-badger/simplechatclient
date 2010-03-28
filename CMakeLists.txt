
# init

project (scc)
cmake_minimum_required (VERSION 2.6.4 FATAL_ERROR)
set (QT_MIN_VERSION "4.5.2")
find_package (Qt4 REQUIRED)

# include

include (FindPkgConfig)

# sub dir

add_subdirectory (src)

# install

if (UNIX AND NOT APPLE)
	install (FILES scc DESTINATION usr/local/bin)
	install (FILES scc.conf DESTINATION usr/local/etc)
endif (UNIX AND NOT APPLE)
