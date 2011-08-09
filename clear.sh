#! /bin/bash
make clean
rm -rdf CMakeFiles CMakeCache.txt cmake_install.cmake cmake_uninstall.cmake install_manifest.txt Makefile scc.pro.user scc-config.h
cd translations && rm -rdf CMakeFiles cmake_install.cmake Makefile
cd ..
cd src && rm -rdf CMakeFiles scc.dir cmake_install.cmake Makefile
cd ..
clear