[Setup]
AppName=マルチメディアライブラリビューワ
AppVerName=マルチメディアライブラリビューワ Version 1.2
AppVersion=1.2
AppCopyright=Copyright (C) 2000-2026 Yoshimasa Electronic Inc.
DefaultDirName={commonpf}\YMEC\MMLibJ
DefaultGroupName=マルチメディアライブラリ
OutputBaseFilename=MMViewJ32
LicenseFile=J:\ymec\MyDev\RTF\LicenseAgreementJ.rtf
UninstallDisplayIcon={app}\MMView.exe
Compression=lzma
SolidCompression=yes
OutputDir=Installer
ChangesAssociations=yes
AppPublisher=Yoshimasa Electronic Inc.
SetupIconFile=compiler:SetupClassicIcon.ico

[Languages]
Name: japanese; MessagesFile: compiler:Languages\Japanese.isl

[Files]
Source: "..\win32\JpnRel\MMView.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\win32\JpnRel\MMDB.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\win32\JpnRel\PcsWin.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\win32\JpnRel\YmUpdate.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\win32\JpnRel\YmUpdate.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Readme\MMLibJ\Readme.txt"; DestDir: "{app}"; Flags: isreadme ignoreversion

[Icons]
Name: "{group}\MMLIBビューワ"; Filename: "{app}\MMView.exe"; WorkingDir: "{app}"

[Registry]
Root: HKCR; Subkey: ".mml"; ValueType: string; ValueName: ""; ValueData: "MMLIB.File"; Flags: uninsdeletevalue deletevalue
Root: HKCR; Subkey: "MMLIB.File"; ValueType: string; ValueName: ""; ValueData: "Multimedia Library File"; Flags: uninsdeletekey deletevalue
Root: HKCR; Subkey: "MMLIB.File\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\MMView.exe,0"; Flags: uninsdeletekey deletevalue
Root: HKCR; Subkey: "MMLIB.File\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\MMView.exe"" ""%1"""; Flags: uninsdeletekey deletevalue

[Code]
// ***インストール画面を最前面に表示*** ↓↓↓ここから↓↓↓
const
  SWP_NOMOVE       = $0002;
  SWP_NOSIZE       = $0001;
  TOPMOST_FLAGS    = SWP_NOMOVE or SWP_NOSIZE;
  HWND_TOPMOST     = -1;
  HWND_NOTOPMOST   = -2;

procedure SetWindowPos(
  hWnd: HWND;
  hWndInsertAfter: HWND;
  X, Y, cx, cy: Integer;
  uFlags: UINT
); external 'SetWindowPos@user32.dll stdcall';

procedure SetForegroundWindow(hWnd: HWND); external 'SetForegroundWindow@user32.dll stdcall';
procedure ShowWindow(hWnd: HWND; nCmdShow: Integer); external 'ShowWindow@user32.dll stdcall';
procedure BringWindowToTop(hWnd: HWND); external 'BringWindowToTop@user32.dll stdcall';

// より強力な前面復帰処理
procedure BringWizardToFront();
begin
  // 複数の方法を組み合わせて確実に前面に
  ShowWindow(WizardForm.Handle, 9);  // SW_RESTORE
  BringWindowToTop(WizardForm.Handle);
  SetWindowPos(WizardForm.Handle, HWND_TOPMOST, 0, 0, 0, 0, TOPMOST_FLAGS);
  SetForegroundWindow(WizardForm.Handle);
  SetWindowPos(WizardForm.Handle, HWND_NOTOPMOST, 0, 0, 0, 0, TOPMOST_FLAGS);
  
  // 少し待ってから再度実行
  Sleep(100);
  SetForegroundWindow(WizardForm.Handle);
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssInstall then
  begin
    BringWizardToFront();
  end;
end;

// UAC後の復帰をより確実にする
procedure CurPageChanged(CurPageID: Integer);
begin
  // どのページでも前面に保持
  BringWizardToFront();
end;
// ***インストール画面を最前面に表示*** ↑↑↑ここまで↑↑↑
