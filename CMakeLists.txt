
# init

project (scc)
cmake_minimum_required (VERSION 2.8.0 FATAL_ERROR)
set (QT_MIN_VERSION "4.6.1")
find_package (Qt4 REQUIRED)

# include

include (FindPkgConfig)

# sub dir

add_subdirectory (src)

# install

if (UNIX AND NOT APPLE)
	install (FILES scc DESTINATION usr/local/bin)
endif (UNIX AND NOT APPLE)
