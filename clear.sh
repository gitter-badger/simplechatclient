#! /bin/bash

make_uninstall()
{
	if [ -e install_manifest.txt ] && [ -e Makefile ]; then
		if [ "$(lsb_release -i | awk '{print $3}')" = "Ubuntu" ]; then
			sudo make uninstall
		else
			su -c 'make uninstall'
		fi
	fi
}

clean_all()
{
	make clean
	rm -rdf CMakeFiles CMakeCache.txt cmake_install.cmake cmake_uninstall.cmake Makefile scc.pro.user scc-config.h install_manifest.txt
	cd translations && rm -rdf CMakeFiles cmake_install.cmake Makefile
	cd ..
	cd src && rm -rdf CMakeFiles scc.dir cmake_install.cmake Makefile
	cd ..
}

make_uninstall && clean_all && clear
