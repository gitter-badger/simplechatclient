# - Try to find QCA2 (Qt Cryptography Architecture 2)
# Once done this will define
#
#  QCA2_FOUND - system has QCA2
#  QCA2_LIBRARY_DIR - the QCA2 top level directory
#  QCA2_INCLUDE_DIR - the QCA2 include directory
#  QCA2_LIBRARIES - the libraries needed to use QCA2
#  QCA2_DEFINITIONS - Compiler switches required for using QCA2
#
# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls

# Changed 2010, Joachim Langenbach, <joachim.langenbach@falaba.de>
#   - added QCA2_LIBRARY_DIR
#   - comment out mark_as_advanced
# Copyright (c) 2006, Michael Larouche, <michael.larouche@kdemail.net>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

IF (QCA2_INCLUDE_DIR AND QCA2_LIBRARIES)
	# in cache already
	SET(QCA2_FOUND TRUE)
ELSE (QCA2_INCLUDE_DIR AND QCA2_LIBRARIES)
	IF (WIN32)
		FIND_PATH(QCA2_LIBRARY_DIR WIN32_DEBUG_POSTFIX d NAMES qca HINTS ${PC_QCA2_LIBDIR} ${PC_QCA2_LIBRARY_DIRS})
		FIND_LIBRARY(QCA2_LIBRARIES NAMES qca HINTS ${QCA2_LIBRARY_DIR}/bin)
		FIND_PATH(QCA2_INCLUDE_DIR NAMES QtCrypto HINTS ${QCA2_LIBRARY_DIR}/include/QtCrypto)
	ELSE(WIN32)
		FIND_PACKAGE(PkgConfig)
#		pkg_check_modules(PC_QCA2 QUIET qca2)
		pkg_check_modules(PC_QCA2 qca2)
		SET(QCA2_DEFINITIONS ${PC_QCA2_CFLAGS_OTHER})
		FIND_LIBRARY(QCA2_LIBRARIES WIN32_DEBUG_POSTFIX d NAMES qca HINTS ${PC_QCA2_LIBDIR} ${PC_QCA2_LIBRARY_DIRS})
		FIND_PATH(QCA2_INCLUDE_DIR QtCrypto HINTS ${PC_QCA2_INCLUDEDIR} ${PC_QCA2_INCLUDE_DIRS} PATH_SUFFIXES QtCrypto)
	ENDIF (WIN32)

	INCLUDE(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(QCA2  DEFAULT_MSG  QCA2_LIBRARIES QCA2_INCLUDE_DIR)

#	mark_as_advanced(QCA2_INCLUDE_DIR QCA2_LIBRARIES)
ENDIF (QCA2_INCLUDE_DIR AND QCA2_LIBRARIES)
