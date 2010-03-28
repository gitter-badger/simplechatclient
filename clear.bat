call make clean
rd /S /Q CMakeFiles
del /S /Q /F CMakeCache.txt cmake_install.cmake Makefile scc.pro.user
cd src
rd /S /Q CMakeFiles scc.dir
del /S /Q /F cmake_install.cmake Makefile
cd ..
cls