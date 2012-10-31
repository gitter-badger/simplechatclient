
; Simple Chat Client NSIS installer script

; general

!define PROGRAM_NAME "SimpleChatClient"
!define PUBLISHER "Simple Chat Client Project"
!define CONTACT "piotr.luczko@gmail.com"
!define VERSION "1.7.0"
!define PROGRAM_FILENAME "scc"
!define PROGRAM_SIZE "65000"
!define MUI_ICON "scc.ico"

; crc
CRCCheck force

; name
Name "SimpleChatClient"

; installer file
OutFile "scc-${VERSION}.exe"

; details
ShowInstDetails "nevershow"
ShowUninstDetails "nevershow"

; default Installation Directory
InstallDir "$PROGRAMFILES\${PROGRAM_NAME}"

; compressor
SetCompressor /SOLID lzma

; request application privileges for Windows Vista/7
RequestExecutionLevel admin

; branding text
BrandingText "${PROGRAM_NAME}"

; --------------------------------
; Modern UI Configuration

!include "MUI.nsh"

!define MUI_ABORTWARNING

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "license.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; language
!insertmacro MUI_LANGUAGE "English" ;first language is the default language
!insertmacro MUI_LANGUAGE "Afrikaans"
!insertmacro MUI_LANGUAGE "Albanian"
!insertmacro MUI_LANGUAGE "Arabic"
!insertmacro MUI_LANGUAGE "Basque"
!insertmacro MUI_LANGUAGE "Belarusian"
!insertmacro MUI_LANGUAGE "Bosnian"
!insertmacro MUI_LANGUAGE "Breton"
!insertmacro MUI_LANGUAGE "Bulgarian"
!insertmacro MUI_LANGUAGE "Catalan"
!insertmacro MUI_LANGUAGE "Croatian"
!insertmacro MUI_LANGUAGE "Czech"
!insertmacro MUI_LANGUAGE "Danish"
!insertmacro MUI_LANGUAGE "Dutch"
!insertmacro MUI_LANGUAGE "Esperanto"
!insertmacro MUI_LANGUAGE "Estonian"
!insertmacro MUI_LANGUAGE "Farsi"
!insertmacro MUI_LANGUAGE "Finnish"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "Galician"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Greek"
!insertmacro MUI_LANGUAGE "Hebrew"
!insertmacro MUI_LANGUAGE "Hungarian"
!insertmacro MUI_LANGUAGE "Icelandic"
!insertmacro MUI_LANGUAGE "Indonesian"
!insertmacro MUI_LANGUAGE "Irish"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "Korean"
!insertmacro MUI_LANGUAGE "Kurdish"
!insertmacro MUI_LANGUAGE "Latvian"
!insertmacro MUI_LANGUAGE "Lithuanian"
!insertmacro MUI_LANGUAGE "Luxembourgish"
!insertmacro MUI_LANGUAGE "Macedonian"
!insertmacro MUI_LANGUAGE "Malay"
!insertmacro MUI_LANGUAGE "Mongolian"
!insertmacro MUI_LANGUAGE "Norwegian"
!insertmacro MUI_LANGUAGE "NorwegianNynorsk"
!insertmacro MUI_LANGUAGE "Polish"
!insertmacro MUI_LANGUAGE "Portuguese"
!insertmacro MUI_LANGUAGE "PortugueseBR"
!insertmacro MUI_LANGUAGE "Romanian"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "Serbian"
!insertmacro MUI_LANGUAGE "SerbianLatin"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "Slovak"
!insertmacro MUI_LANGUAGE "Slovenian"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "SpanishInternational"
!insertmacro MUI_LANGUAGE "Swedish"
!insertmacro MUI_LANGUAGE "Thai"
!insertmacro MUI_LANGUAGE "TradChinese"
!insertmacro MUI_LANGUAGE "Turkish"
!insertmacro MUI_LANGUAGE "Ukrainian"
!insertmacro MUI_LANGUAGE "Uzbek"
!insertmacro MUI_LANGUAGE "Welsh"

!insertmacro MUI_RESERVEFILE_LANGDLL

; ----------------------------------------------------------------------------------
; *************************** SECTION FOR INSTALLING *******************************
; ----------------------------------------------------------------------------------

Section "${PROGRAM_NAME}" ro
SetOverwrite on

; delete desktop link
Delete "$DESKTOP\${PROGRAM_NAME}.lnk"

; delete Start Menu Shortcuts
Delete "$SMPROGRAMS\${PROGRAM_NAME}\*.*"
RMDir  "$SMPROGRAMS\${PROGRAM_NAME}"

; remove old folders
RMDir /r "$INSTDIR\3rdparty\emoticons_other"
Delete "$INSTDIR\3rdparty\emoticons\*.gif"

; remove all the files and folders
RMDir /r "$INSTDIR\translations"
RMDir /r "$INSTDIR\plugins"
RMDir /r "$INSTDIR\scripts"
Delete "$INSTDIR\*.*"

; delete Uninstaller And Unistall Registry Entries
DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\${PROGRAM_NAME}"
DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}"

; install
SetOutPath $INSTDIR\

File vcredist_x86_sp1.exe
File scc.ico

; all files
File /r scc\*.*

; vc redist
SetDetailsPrint textonly
DetailPrint "Installing Microsoft Visual C++ 2008 Redistributable..."
SetDetailsPrint listonly

ExecWait '"$INSTDIR\vcredist_x86_sp1.exe" /q:a /c:"VCREDI~1.EXE /q:a /c:""msiexec /i vcredist.msi /qn!"" "'  
SetDetailsPrint both

; desktop
CreateShortCut "$DESKTOP\${PROGRAM_NAME}.lnk" "$INSTDIR\${PROGRAM_FILENAME}.exe" "${PROGRAM_NAME}" "$INSTDIR\${MUI_ICON}"

; menu start
CreateDirectory "$SMPROGRAMS\${PROGRAM_NAME}"
CreateShortCut "$SMPROGRAMS\${PROGRAM_NAME}\${PROGRAM_NAME}.lnk" "$INSTDIR\${PROGRAM_FILENAME}.exe" "${PROGRAM_NAME}" "$INSTDIR\${MUI_ICON}"
CreateShortCut "$SMPROGRAMS\${PROGRAM_NAME}\Uninstall ${PROGRAM_NAME}.lnk" "$INSTDIR\Uninstall.exe"

; write uninstall
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "DisplayName" "${PROGRAM_NAME}"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "Publisher" "${PUBLISHER}"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "Contact" "${CONTACT}"
WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "EstimatedSize" "${PROGRAM_SIZE}"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "Size" ""
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "DisplayIcon" "$INSTDIR\${MUI_ICON}"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "DisplayVersion" "${VERSION}"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "URLInfoAbout" "http://simplechatclien.sourceforge.net/"
WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "NoModify" "1"
WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "NoRepair" "1"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "UninstallString" "$INSTDIR\Uninstall.exe"
WriteUninstaller $INSTDIR\Uninstall.exe

; add firewall rule
SimpleFC::AddApplication "${PROGRAM_NAME}" "$INSTDIR\${PROGRAM_FILENAME}.exe" 0 2 "" 1
SectionEnd

; ----------------------------------------------------------------------------------
; ************************** SECTION FOR UNINSTALLING ******************************
; ----------------------------------------------------------------------------------

Section "Uninstall"
; remove all the files and folders
RMDir /r "$INSTDIR\*.*"
RMDir $INSTDIR

; delete Start Menu Shortcuts
Delete "$DESKTOP\${PROGRAM_NAME}.lnk"
Delete "$SMPROGRAMS\${PROGRAM_NAME}\*.*"
RMDir  "$SMPROGRAMS\${PROGRAM_NAME}"

; delete Uninstaller And Unistall Registry Entries
DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\${PROGRAM_NAME}"
DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}"

; delete firewall rule
SimpleFC::RemoveApplication "$INSTDIR\${PROGRAM_FILENAME}.exe"
SectionEnd

; ----------------------------------------------------------------------------------
; ******************************* SECTION ON INIT **********************************
; ----------------------------------------------------------------------------------

Function ".onInit"
; read-only
IntOp $0 ${SF_SELECTED} | ${SF_RO}
SectionSetFlags ${ro} $0
; language
!insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

; eof
