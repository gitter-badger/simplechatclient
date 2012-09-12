###################
What is Simple Chat Client
###################

Simple Chat Client is a lightweight and simple program which allows talking in the czat.onet.pl
without using a browser or java.

The program of the assumptions is small, lightweight and stable client.

*******************
Release Information
*******************

This repo contains in development code for future releases. To download the
latest stable release please visit the `Simple Chat Client Downloads
<http://simplechatclien.sourceforge.net/download/>`_ page.

*******************
SCC Requirements
*******************

Windows
=========
`Microsoft Visual C++ 2008 SP1 Redistributable Package (x86)
<http://www.microsoft.com/download/en/details.aspx?displaylang=en&id=5582>`_

Linux Debian/Ubuntu packages
=========
::

	libssl-dev libqca2-dev libqca2-plugin-ossl libphonon-dev g++ cmake make pkg-config

*******************
Custom build
*******************

If you want to use "cmake" with custom options, type::

   cmake . [options]
   make
   make install

On Windows "make install" does not fully work. Edit cmake options in compile.bat instead.

Available options
=========

-   -DWITH_KAMERZYSTA=ON|OFF
      ON - compile with Kamerzysta connection (default=ON on Windows)
      OFF - compile with internal webcam window (default=OFF on Unix/Linux)

-   -DWITH_PHONON=ON|OFF
      compile with Phonon (default=ON)

-   -DWIN32_QCA2_DIR=DIR
      compile with custom libQCA path i.e.: -DWIN32_QCA2_DIR=C:\QCA2

-   -DCMAKE_BUILD_TYPE=Debug
      compile with full debug
