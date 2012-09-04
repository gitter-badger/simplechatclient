#!/bin/bash

make_install()
{
	if [ -e /etc/lsb-release ]; then
		if [ "$(lsb_release -i | awk '{print $3}')" = "Ubuntu" ]; then
			sudo make install
		else
			su -c 'make install'
		fi
	else
		su -c 'make install'
	fi
}

cmake . && make -j$(echo $(($(grep -i ^processor /proc/cpuinfo | wc -l)+1))) && make_install
