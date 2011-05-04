#! /bin/bash
make clean
rm -rd CMakeFiles CMakeCache.txt cmake_install.cmake cmake_uninstall.cmake install_manifest.txt Makefile scc.pro.user
cd i18n && rm -rd CMakeFiles cmake_install.cmake Makefile
cd ..
cd src && rm -rd CMakeFiles scc.dir cmake_install.cmake Makefile scc-config.h
cd ..
clear