#! /bin/bash
cmake . && make -j$(echo $(($(lscpu | awk '/^CPU\(s\):/ {print $2}')+1))) && sudo make install
