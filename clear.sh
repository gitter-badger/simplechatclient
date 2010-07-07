#! /bin/bash
make clean
rm -rd log CMakeFiles CMakeCache.txt cmake_install.cmake Makefile scc.pro.user scc.conf
cd i18n && rm -rd CMakeFiles cmake_install.cmake Makefile
cd ..
cd src && rm -rd CMakeFiles scc.dir cmake_install.cmake Makefile
cd ..
clear