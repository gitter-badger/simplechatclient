[Setup]
AppName=Simple Chat Client
AppVerName=Simple Chat Client 1.7
AppVersion=1.7
AppPublisher=Simple Chat Client
AppPublisherURL=http://simplechatclien.sourceforge.net/
AppSupportURL=http://czat.onet.pl/scc,chat.html
AppUpdatesURL=http://simplechatclien.sourceforge.net/download/
VersionInfoVersion=1.7
DefaultDirName={pf32}\SimpleChatClient
DefaultGroupName=Simple Chat Client
UninstallDisplayIcon={app}\scc.ico
Compression=lzma2
SolidCompression=yes

[Languages]
Name: en; MessagesFile: "compiler:Default.isl"
Name: de; MessagesFile: "compiler:Languages\German.isl"
Name: pl; MessagesFile: "compiler:Languages\Polish.isl"

[Files]
Source: "release\*"; DestDir: "{app}"
Source: "license.txt"; DestDir: "{app}"
Source: "scc.ico"; DestDir: "{app}"
Source: "vcredist_x86.exe"; DestDir: {tmp};  Flags: deleteafterinstall

[Run]
Filename: "{tmp}\vcredist_x86.exe"; Parameters: "/q /norestart"; StatusMsg: Installing Visual C++ 2010 Redistributable Package...

[Code]
function InitializeSetup(): Boolean;
var
ErrorCode: Integer;
begin
ShellExec('open','taskkill.exe','/f /im scc.exe','',SW_HIDE,ewNoWait,ErrorCode);
ShellExec('open','tskill.exe',' scc','',SW_HIDE,ewNoWait,ErrorCode);
result := True;
end;
