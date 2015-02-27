
.. |travisone| image:: https://travis-ci.org/simplechatclient/simplechatclient.svg?branch=1.0
    :target: https://travis-ci.org/simplechatclient/simplechatclient/

.. |travismaster| image:: https://travis-ci.org/simplechatclient/simplechatclient.svg?branch=master
    :target: https://travis-ci.org/simplechatclient/simplechatclient/

1.0 |travisone| master |travismaster|

##########################
What is Simple Chat Client
##########################

Simple Chat Client is a lightweight and simple program which allows talking in the czat.onet.pl
without using a browser or java.

The program of the assumptions is small, lightweight and stable client.

*******************
Release Information
*******************

This repo contains in development code for future releases.

To download the latest stable release please visit the `Simple Chat Client Downloads
<http://simplechatclien.sourceforge.net/download/>`_ page.

****************
SCC Requirements
****************

Windows
=======
`Microsoft Visual C++ 2008 SP1 Redistributable Package (x86)
<http://www.microsoft.com/download/en/details.aspx?displaylang=en&id=5582>`_

Example Environment variables for development:

PATH
::

	c:\Qt\5.4\mingw491_32\bin;c:\Qt\Tools\mingw491_32\bin;C:\Program Files (x86)\CMake\bin;

QT_DIR
::

	c:\Qt\5.4\mingw491_32\bin\

Linux Debian/Ubuntu packages
============================
::

	cmake make pkg-config g++ libssl-dev qtbase5-dev qttools5-dev qttools5-dev-tools qtmultimedia5-dev libqt5webkit5-dev
	
	Install qca plugin from https://projects.kde.org/projects/kdesupport/qca/repository
	We need qca build with Qt5 support. Current version from packages is with Qt4 support.
	   $ git clone git://anongit.kde.org/qca
	   $ cd qca
	   $ cmake .
	   $ make
	   $ sudo make install


*******************
Custom build
*******************

If you want to use "cmake" with custom options, type::

   cmake . [options]
   make
   make install

On Windows "make install" does not fully work. Edit cmake options in compile.bat instead.

Available options
=================

::

	-DWIN32_QCA2_DIR=DIR

compile with custom libQCA path i.e.: -DWIN32_QCA2_DIR=C:\QCA2

::

	-DCMAKE_BUILD_TYPE=Debug

compile with full debug
