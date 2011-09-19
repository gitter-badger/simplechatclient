#! /bin/bash

make_install()
{
	if [ "$(lsb_release -i | awk '{print $3}')" = "Ubuntu" ]; then
		sudo make install
	else
		su -c 'make install'
	fi
}

cmake . && make -j$(echo $(($(lscpu | awk '/^CPU\(s\):/ {print $2}')+1))) && make_install
