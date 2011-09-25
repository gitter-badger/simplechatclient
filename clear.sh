#! /bin/bash
if [ "$(lsb_release -i | awk '{print $3}')" = "Ubuntu" ]; then
	sudo make uninstall && make clean && rm -f install_manifest.txt
else
	su -c 'make uninstall' && make clean && rm -f install_manifest.txt
fi
rm -rdf CMakeFiles CMakeCache.txt cmake_install.cmake cmake_uninstall.cmake Makefile scc.pro.user scc-config.h
cd translations && rm -rdf CMakeFiles cmake_install.cmake Makefile
cd ..
cd src && rm -rdf CMakeFiles scc.dir cmake_install.cmake Makefile
cd ..
clear
