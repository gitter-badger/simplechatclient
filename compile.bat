@echo off 

if not exist "C:\Program Files\CMake 2.8" (
if not exist "C:\Program Files (x86)\CMake 2.8" (
echo Download CMake from http://www.cmake.org/files/v2.8/cmake-2.8.6-win32-x86.exe
pause
exit
)
)

if not exist C:\Qt\4.8.0 (
echo Download Qt 4.8.0 from http://qt.nokia.com/downloads/windows-cpp
pause
exit
)

if not exist C:\MinGW\bin\mingw32-make.exe (
echo Download MinGW from http://sourceforge.net/projects/mingw/files/
pause
exit
)

if not exist C:\qca2 (
echo Download QCA2 from http://delta.affinix.com/download/qca/2.0/qca-2.0.3.tar.bz2
pause
exit
)

if not exist C:\qca2\lib\qca2.dll (
echo Compiling qca2
cd C:\qca2
configure.exe --release --disable-tests
mingw32-make
cd C:\scc
)

if exist src\scc.exe (
del /Q /F /A src\scc.exe
)
cmake . -G "MinGW Makefiles" -DWIN32_QCA2_DIR=C:\qca2
mingw32-make

if not exist src\scc.exe (
echo SCC is not compiled. Please compile program.
pause
exit
)

if not exist C:\OpenSSL-Win32 (
echo Download OpenSSL from http://www.slproweb.com/download/Win32OpenSSL_Light-1_0_0e.exe
pause
exit
)

if not exist C:\zlib-1.2.5 (
echo Download ZLib from http://zlib.net/zlib125-dll.zip
pause
exit
)

set DESTINATION="C:\scc\release"
set QT_DIR="C:\Qt\4.8.0\bin"
set QT_PLUGINS_DIR="C:\Qt\4.8.0\plugins"
set OPENSSL_DIR="C:\OpenSSL-Win32"
set ZLIB_DIR="C:\zlib-1.2.5"
set QCA2_DIR="C:\qca2\lib"
set MINGW_BIN_DIR="C:\MinGW\bin"

echo Removing destination folder
if exist %DESTINATION% (
rd %DESTINATION% /S /Q
)

echo Copying main files
xcopy AUTHORS %DESTINATION%\ /R /Y /Q
xcopy ChangeLog %DESTINATION%\ /R /Y /Q
xcopy LICENSE %DESTINATION%\ /R /Y /Q
xcopy README %DESTINATION%\ /R /Y /Q
xcopy TODO %DESTINATION%\ /R /Y /Q
xcopy VERSION %DESTINATION%\ /R /Y /Q
xcopy src\scc.exe %DESTINATION%\ /R /Y /Q

echo Copying Qt files
xcopy %QT_DIR%\libgcc_s_dw2-1.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\mingwm10.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\phonon4.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\QtCore4.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\QtGui4.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\QtNetwork4.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\QtWebKit4.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\QtXml4.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\QtXmlPatterns4.dll %DESTINATION%\ /C /H /R /Y /Q

echo Copying MinGW files
xcopy %MINGW_BIN_DIR%\libstdc++-6.dll %DESTINATION%\ /C /H /R /Y /Q

echo Copying scripts
xcopy scripts\*.js %DESTINATION%\scripts\ /C /H /R /Y /Q 

echo Copying translations
xcopy translations\*.qm %DESTINATION%\translations\ /C /H /R /Y /Q 

echo Copying images
xcopy images\user_avatar.png %DESTINATION%\images\ /C /H /R /Y /Q 
xcopy images\wallpaper\*.jpg %DESTINATION%\images\wallpaper\ /C /H /R /Y /Q 

echo Copying 3rdparty
xcopy 3rdparty\LICENSE %DESTINATION%\3rdparty\ /C /H /R /Y /Q 
xcopy 3rdparty\emoticons\*.gif %DESTINATION%\3rdparty\emoticons\ /C /H /R /Y /Q /S /exclude:exclude.txt
xcopy 3rdparty\sounds\*.wav %DESTINATION%\3rdparty\sounds\ /C /H /R /Y /Q 

echo Copying Qt plugins
xcopy %QT_PLUGINS_DIR%\imageformats\qgif4.dll %DESTINATION%\plugins\imageformats\ /C /H /R /Y /Q
xcopy %QT_PLUGINS_DIR%\imageformats\qjpeg4.dll %DESTINATION%\plugins\imageformats\ /C /H /R /Y /Q
xcopy %QT_PLUGINS_DIR%\phonon_backend\phonon_ds94.dll %DESTINATION%\plugins\phonon_backend\ /C /H /R /Y /Q

echo Creating Qt conf
echo [Paths] > %DESTINATION%\qt.conf
echo Plugins = plugins >> %DESTINATION%\qt.conf
echo Translations = translations >> %DESTINATION%\qt.conf

echo Copying OpenSSL libs
xcopy %OPENSSL_DIR%\libeay32.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %OPENSSL_DIR%\ssleay32.dll %DESTINATION%\ /C /H /R /Y /Q 

echo Copying zlib libs
xcopy %ZLIB_DIR%\zlib1.dll %DESTINATION%\ /C /H /R /Y /Q 

echo Copying qca2 libs
xcopy %QCA2_DIR%\qca2.dll %DESTINATION%\ /C /H /R /Y /Q

echo Copying crypt libs
xcopy crypt\qca-ossl2.dll  %DESTINATION%\plugins\crypto\ /C /H /R /Y /Q
xcopy crypt\libcrypto-8.dll  %DESTINATION%\ /C /H /R /Y /Q
xcopy crypt\libssl-8.dll  %DESTINATION%\ /C /H /R /Y /Q

echo Done!
pause
