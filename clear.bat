call make clean

rd /S /Q CMakeFiles
rd /S /Q ZERO_CHECK.dir
del /S /Q /F /A CMakeCache.txt cmake_install.cmake Makefile scc.pro.user ALL_BUILD* ZERO_CHECK* scc.ncb scc.sln scc.suo scc.vcproj*

cd i18n
rd /S /Q CMakeFiles
rd /S /Q i18n.dir
del /S /Q /F /A i18n.vcproj* *.qm generate*
cd ..

cd src
rd /S /Q CMakeFiles scc.dir
del /S /Q /F /A cmake_install.cmake Makefile moc_* ui_* qrc_* scc.*
cd ..
cls