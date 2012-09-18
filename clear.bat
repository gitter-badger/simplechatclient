call make clean

rd /S /Q CMakeFiles
del /S /Q /F /A CMakeCache.txt cmake_install.cmake Makefile scc.pro.user scc-config.h

cd translations
rd /S /Q CMakeFiles
rd /S /Q translations.dir
del /S /Q /F /A *.qm
cd ..

cd src
rd /S /Q CMakeFiles scc.dir __
del /S /Q /F /A cmake_install.cmake Makefile moc_* ui_* qrc_* scc.*
cd ..

cls