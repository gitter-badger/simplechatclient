
; Simple Chat Client NSIS installer script

; general

!define PROGRAM_NAME "SimpleChatClient"
!define PUBLISHER "Simple Chat Client Project"
!define CONTACT "piotr.luczko@gmail.com"
!define VERSION "1.7.1"
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
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Polish"

!insertmacro MUI_RESERVEFILE_LANGDLL

; ----------------------------------------------------------------------------------
; *************************** SECTION FOR INSTALLING *******************************
; ----------------------------------------------------------------------------------

Section "${PROGRAM_NAME}" ro
SetOverwrite on

; kill process
KillProcDLL::KillProc "scc.exe"

; delete desktop link
Delete "$DESKTOP\${PROGRAM_NAME}.lnk"

; delete Start Menu Shortcuts
Delete "$SMPROGRAMS\${PROGRAM_NAME}\*.*"
RMDir  "$SMPROGRAMS\${PROGRAM_NAME}"

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

File vcredist_x86.exe
File scc.ico

; all files
File /r release\*.*

; vc redist
SetDetailsPrint textonly
DetailPrint "Installing Microsoft Visual C++ 2008 Redistributable..."
SetDetailsPrint listonly

ExecWait '"$INSTDIR\vcredist_x86.exe" /q /norestart'  
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
