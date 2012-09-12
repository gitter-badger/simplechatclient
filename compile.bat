@echo off 

set SCC_DIR=%CD%
set DESTINATION="C:\scc\release"
set QT_DIR="C:\Qt\4.8.2"
set QCA2_DIR="C:\qca2"
set OPENSSL_DIR="C:\OpenSSL-Win32"
set QCA_OSSL_DIR=%SCC_DIR%\3rdparty\qca-ossl-2.0.0-beta3
set MINGW_DIR="C:\MinGW"
set CMAKE_DIR="C:\Program Files\CMake 2.8"
set CMAKE32_DIR="C:\Program Files (x86)\CMake 2.8"

rem ----------------------- END OF CONFIG ---------------------------

rem settings for QCA2 and QCA-OSSL
set QTDIR=%QT_DIR%

if not exist %CMAKE_DIR%\bin\cmake.exe (
if not exist %CMAKE32_DIR%\bin\cmake.exe (
echo Download CMake from http://www.cmake.org/files/v2.8/cmake-2.8.9-win32-x86.exe
pause
exit
)
)

if not exist %QT_DIR%\bin\qtvars.bat (
echo Download Qt 4.8.2 for MinGW from http://qt-project.org/downloads#qt-lib
pause
exit
)

if not exist %MINGW_DIR%\bin\mingw32-make.exe (
echo Download MinGW from http://sourceforge.net/projects/mingw/files/
pause
exit
)

if not exist %QCA2_DIR%\include\QtCrypto\qca.h (
echo Download QCA2 from http://delta.affinix.com/download/qca/2.0/qca-2.0.3.tar.bz2
pause
exit
)

if not exist %QCA2_DIR%\lib\qca2.dll (goto compile_qca2)
if not exist %QT_DIR%\mkspecs\features\crypto.prf (goto compile_qca2)
goto end_compile_qca2
:compile_qca2
echo Compiling qca2
cd /D %QCA2_DIR%
configure.exe --release --disable-tests --qtdir=%QT_DIR%
mingw32-make
call installwin
cd /D %SCC_DIR%
:end_compile_qca2

if not exist %OPENSSL_DIR%\include\openssl\ssl.h (
echo Download OpenSSL from http://slproweb.com/download/Win32OpenSSL-1_0_1c.exe
pause
exit
)

if not exist %QT_DIR%\plugins\crypto\qca-ossl2.dll (
echo Compiling qca-ossl plugin
cd /D %QCA_OSSL_DIR%
echo WINLOCAL_PREFIX = %OPENSSL_DIR% > %QT_DIR%\mkspecs\features\winlocal.prf 
call configwin r
qmake
mingw32-make 
xcopy lib\* %QT_DIR%\plugins\crypto\ /C /H /R /Y /Q 
cd /D %SCC_DIR%
)

if exist src\scc.exe (
del /Q /F /A src\scc.exe
)
cmake . -G "MinGW Makefiles" -DWIN32_QCA2_DIR=%QCA2_DIR%
mingw32-make

if not exist src\scc.exe (
echo SCC is not compiled. Please compile program.
pause
exit
)

echo Killing scc
taskkill /F /IM scc.exe

echo Removing destination folder
if exist %DESTINATION% (
rd %DESTINATION% /S /Q
)

echo Copying main files
xcopy AUTHORS      %DESTINATION%\ /R /Y /Q
xcopy ChangeLog    %DESTINATION%\ /R /Y /Q
xcopy LICENSE      %DESTINATION%\ /R /Y /Q
xcopy README.rst   %DESTINATION%\ /R /Y /Q
xcopy TODO         %DESTINATION%\ /R /Y /Q
xcopy VERSION      %DESTINATION%\ /R /Y /Q
xcopy src\scc.exe  %DESTINATION%\ /R /Y /Q

echo Copying Qt files
xcopy %QT_DIR%\bin\libgcc_s_dw2-1.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\mingwm10.dll       %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\phonon4.dll        %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\QtCore4.dll        %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\QtGui4.dll         %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\QtNetwork4.dll     %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\QtWebKit4.dll      %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\QtXml4.dll         %DESTINATION%\ /C /H /R /Y /Q

echo Copying MinGW files 
xcopy %MINGW_DIR%\bin\libstdc++-6.dll %DESTINATION%\ /C /H /R /Y /Q

echo Copying Qt plugins
xcopy %QT_DIR%\plugins\imageformats\qgif4.dll         %DESTINATION%\plugins\imageformats\   /C /H /R /Y /Q
xcopy %QT_DIR%\plugins\imageformats\qjpeg4.dll        %DESTINATION%\plugins\imageformats\   /C /H /R /Y /Q
xcopy %QT_DIR%\plugins\phonon_backend\phonon_ds94.dll %DESTINATION%\plugins\phonon_backend\ /C /H /R /Y /Q

echo Creating Qt conf
echo [Paths] > %DESTINATION%\qt.conf
echo Plugins = plugins >> %DESTINATION%\qt.conf
echo Translations = translations >> %DESTINATION%\qt.conf

echo Copying OpenSSL libs
xcopy %OPENSSL_DIR%\libeay32.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %OPENSSL_DIR%\ssleay32.dll %DESTINATION%\ /C /H /R /Y /Q 

echo Copying qca2 libs
xcopy %QCA2_DIR%\lib\qca2.dll %DESTINATION%\ /C /H /R /Y /Q

echo Copying crypto plugin
xcopy %QT_DIR%\plugins\crypto\qca-ossl2.dll           %DESTINATION%\plugins\crypto\         /C /H /R /Y /Q

echo Copying scripts
xcopy scripts\*.js %DESTINATION%\scripts\ /C /H /R /Y /Q 

echo Copying translations
xcopy translations\*.qm %DESTINATION%\translations\ /C /H /R /Y /Q 

echo Copying images
xcopy images\user_avatar.png %DESTINATION%\images\           /C /H /R /Y /Q 
xcopy images\wallpaper\*.jpg %DESTINATION%\images\wallpaper\ /C /H /R /Y /Q 

echo Copying 3rdparty
xcopy 3rdparty\LICENSE         %DESTINATION%\3rdparty\           /C /H /R /Y /Q 
xcopy 3rdparty\emoticons\*.gif %DESTINATION%\3rdparty\emoticons\ /C /H /R /Y /Q /S
xcopy 3rdparty\sounds\*.wav    %DESTINATION%\3rdparty\sounds\    /C /H /R /Y /Q 

echo Done!
pause
