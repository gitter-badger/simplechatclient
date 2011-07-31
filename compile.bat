cmake . -G "MinGW Makefiles" -DWIN32_QCA2_DIR=C:\qca2
mingw32-make

@ECHO OFF 

set DESTINATION="C:\scc\release"
set QT_DIR="C:\Qt\4.7.3\bin"
set QT_PLUGINS_DIR="C:\Qt\4.7.3\plugins"
set OPENSSL_DIR="C:\OpenSSL-Win32"
set ZLIB_DIR="C:\zlib-1.2.5"
set QCA2_DIR="C:\qca2\bin"

echo Removing destination folder
rd %DESTINATION% /S /Q

echo Copying main files
xcopy AUTHORS %DESTINATION%\ /R /Y /Q
xcopy ChangeLog %DESTINATION%\ /R /Y /Q
xcopy LICENSE %DESTINATION%\ /R /Y /Q
xcopy README %DESTINATION%\ /R /Y /Q
xcopy TODO %DESTINATION%\ /R /Y /Q
xcopy VERSION %DESTINATION%\ /R /Y /Q
xcopy scc.exe %DESTINATION%\ /R /Y /Q

echo Copying Qt files
xcopy %QT_DIR%\libgcc_s_dw2-1.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\mingwm10.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\phonon4.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\QtCore4.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\QtGui4.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\QtNetwork4.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\QtXml4.dll %DESTINATION%\ /C /H /R /Y /Q
xcopy %QT_DIR%\QtXmlPatterns4.dll %DESTINATION%\ /C /H /R /Y /Q

echo Copying translations
xcopy translations\*.qm %DESTINATION%\translations\ /C /H /R /Y /Q 

echo Copying images
xcopy images\wallpaper\*.jpg %DESTINATION%\images\wallpaper\ /C /H /R /Y /Q 

echo Copying 3rdparty
xcopy 3rdparty\LICENSE %DESTINATION%\3rdparty\ /C /H /R /Y /Q 
xcopy 3rdparty\emoticons\*.gif %DESTINATION%\3rdparty\emoticons\ /C /H /R /Y /Q 
xcopy 3rdparty\emoticons_other\*.gif %DESTINATION%\3rdparty\emoticons_other\ /C /H /R /Y /Q 
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
xcopy qca-ossl2.dll  %DESTINATION%\plugins\crypto\ /C /H /R /Y /Q
xcopy libcrypto-8.dll  %DESTINATION%\ /C /H /R /Y /Q
xcopy libssl-8.dll  %DESTINATION%\ /C /H /R /Y /Q

echo Done!