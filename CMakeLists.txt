
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
	install (FILES scc DESTINATION opt/scc)
endif (UNIX AND NOT APPLE)
