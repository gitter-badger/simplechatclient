@echo off 

set SCC_DIR=%CD%
set DESTINATION="C:\scc\release"
set QT_DIR="c:\Qt\5.4\mingw491_32"
set QCA2_DIR="C:\qca-2.0.3"
set OPENSSL_DIR="C:\OpenSSL-Win32"
set MINGW_DIR="c:\Qt\Tools\mingw491_32"
set CMAKE_DIR="C:\Program Files\CMake"
set CMAKE32_DIR="C:\Program Files (x86)\CMake"

rem ----------------------- END OF CONFIG ---------------------------

rem settings for QCA2 and QCA-OSSL
set QTDIR=%QT_DIR%

if not exist %CMAKE_DIR%\bin\cmake.exe (
if not exist %CMAKE32_DIR%\bin\cmake.exe (
echo Download CMake from http://www.cmake.org/files/v3.0/cmake-3.0.2-win32-x86.exe
pause
exit
)
)

if not exist %QT_DIR%\bin\qmake.exe (
echo Download Qt 5.4.0 for MinGW from http://www.qt.io/download/
pause
exit
)

if not exist %MINGW_DIR%\bin\mingw32-make.exe (
echo Download MinGW from http://sourceforge.net/projects/mingw/files/
pause
exit
)

if not exist %QCA2_DIR%\include\QtCrypto\qca.h (
echo Download QCA2 from https://projects.kde.org/projects/kdesupport/qca/repository
pause
exit
)

if not exist %QT_DIR%\bin\libqca.dll (goto compile_qca)
if not exist %QT_DIR%\mkspecs\features\crypto.prf (goto compile_qca)
goto end_compile_qca
:compile_qca
echo Compiling qca
cd /D %QCA2_DIR%
cmake . -G "MinGW Makefiles"
mingw32-make
mingw32-make install
cd /D %SCC_DIR%
:end_compile_qca

if not exist %OPENSSL_DIR%\include\openssl\ssl.h (
echo Download OpenSSL from http://slproweb.com/download/Win32OpenSSL-1_0_1g.exe
pause
exit
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
xcopy %QT_DIR%\bin\Qt5Core.dll          %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\Qt5Gui.dll           %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\Qt5Multimedia.dll    %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\Qt5Network.dll       %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\Qt5WebKit.dll        %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\Qt5WebKitWidgets.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\Qt5Widgets.dll       %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\Qt5Xml.dll           %DESTINATION%\ /C /H /R /Y /Q

echo Copying Qt dependency
xcopy %QT_DIR%\bin\icuin53.dll           %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\icuuc53.dll           %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\icudt53.dll           %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\Qt5Sql.dll           %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\Qt5Qml.dll           %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\Qt5Quick.dll           %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\Qt5Sensors.dll           %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\Qt5Positioning.dll           %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\Qt5OpenGL.dll           %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\Qt5PrintSupport.dll           %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\Qt5MultimediaWidgets.dll           %DESTINATION%\ /C /H /R /Y /Q

echo Copying MinGW files
xcopy %QT_DIR%\bin\libgcc_s_dw2-1.dll   %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\libstdc++-6.dll      %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\bin\libwinpthread-1.dll  %DESTINATION%\ /C /H /R /Y /Q

echo Copying Qt plugins
xcopy %QT_DIR%\plugins\imageformats\qgif.dll         %DESTINATION%\plugins\imageformats\   /C /H /R /Y /Q
xcopy %QT_DIR%\plugins\imageformats\qjpeg.dll        %DESTINATION%\plugins\imageformats\   /C /H /R /Y /Q
xcopy %QT_DIR%\plugins\mediaservice\dsengine.dll     %DESTINATION%\plugins\mediaservice\ /C /H /R /Y /Q
xcopy %QT_DIR%\plugins\platforms\qwindows.dll        %DESTINATION%\plugins\platforms\ /C /H /R /Y /Q

echo Creating Qt conf
echo [Paths] > %DESTINATION%\qt.conf
echo Plugins = plugins >> %DESTINATION%\qt.conf
echo Translations = translations >> %DESTINATION%\qt.conf

echo Copying OpenSSL libs
xcopy %OPENSSL_DIR%\libeay32.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %OPENSSL_DIR%\ssleay32.dll %DESTINATION%\ /C /H /R /Y /Q 

echo Copying QCA libs
xcopy %QT_DIR%\bin\libqca.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\plugins\crypto\libqca-ossl.dll %DESTINATION%\plugins\crypto\ /C /H /R /Y /Q

echo Copying translations
xcopy translations\*.qm %DESTINATION%\translations\ /C /H /R /Y /Q 

echo Copying images
xcopy images\registered_user_avatar.png %DESTINATION%\images\ /C /H /R /Y /Q 
xcopy images\registered_user_avatar_big.png %DESTINATION%\images\ /C /H /R /Y /Q 
xcopy images\unregistered_user_avatar.png %DESTINATION%\images\ /C /H /R /Y /Q 
xcopy images\unregistered_user_avatar_big.png %DESTINATION%\images\ /C /H /R /Y /Q 
xcopy images\wallpaper\*.jpg %DESTINATION%\images\wallpaper\ /C /H /R /Y /Q 

echo Copying sounds
xcopy sounds\LICENSE %DESTINATION%\sounds\ /C /H /R /Y /Q 
xcopy sounds\*.wav %DESTINATION%\sounds\ /C /H /R /Y /Q 

echo Copying emoticons
xcopy emoticons\LICENSE %DESTINATION%\emoticons\ /C /H /R /Y /Q 
xcopy emoticons\*.gif %DESTINATION%\emoticons\ /C /H /R /Y /Q /S
xcopy emoticons\*.jpg %DESTINATION%\emoticons\ /C /H /R /Y /Q /S
xcopy emoticons\*.jpeg %DESTINATION%\emoticons\ /C /H /R /Y /Q /S
xcopy emoticons\*.png %DESTINATION%\emoticons\ /C /H /R /Y /Q /S
xcopy emoticons\*.bmp %DESTINATION%\emoticons\ /C /H /R /Y /Q /S

echo Done!
pause
