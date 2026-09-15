[Setup]
AppName=Multimedia Library Manager
AppVerName=Multimedia Library Manager Version 1.2
AppVersion=1.2
AppCopyright=Copyright (C) 2000-2026 Yoshimasa Electronic Inc.
DefaultDirName={commonpf}\YMEC\MMLibE
DefaultGroupName=Multimedia Library Manager
OutputBaseFilename=MMLibE32
LicenseFile=J:\ymec\MyDev\RTF\LicenseAgreementE.rtf
UninstallDisplayIcon={app}\MMLib.exe
Compression=lzma
SolidCompression=yes
OutputDir=Installer
AppPublisher=Yoshimasa Electronic Inc.
SetupIconFile=compiler:SetupClassicIcon.ico

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "..\win32\EngRel\MMLIB.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\win32\EngRel\MMDB.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\win32\EngRel\PcsWin.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\win32\EngRel\RegistE.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\win32\EngRel\YmUpdate.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\win32\EngRel\YmUpdate.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\win32\EngRel\CapDll.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\win32\EngRel\TWainDll.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\win32\EngRel\xd2txlib.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Readme\MMLibE\Readme.txt"; DestDir: "{app}"; Flags: isreadme ignoreversionSource: "J:\YMEC\MyDev\DLL32\HardKey.dll"; DestDir: "{app}"; Flags: dontcopy

[Icons]
Name: "{group}\MMLIB Manager"; Filename: "{app}\MMLIB.exe"; WorkingDir: "{app}"

[Code]
procedure GetHardKey(nAddSeed: Integer; sHardKey: AnsiString);
external 'GetHardKey@files:HardKey.dll stdcall setuponly';

function GetHardKey2(param: String): String;
var
  asHardKey: AnsiString;
  sHardKey: String;
  rc: Boolean;
begin
  rc := RegQueryStringValue(HKLM, 'SoftWare\Yoshimasa Electronic Inc.\Multimedia Library', 'HardKey', sHardKey);
  if rc = false then begin
    asHardKey := '00000000';
    GetHardKey(0, asHardKey);
    sHardKey := String(asHardKey);
  end;
  Result := sHardKey;
end;

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

[Registry]
Root: HKLM; Subkey: SoftWare\\Yoshimasa Electronic Inc.\\Multimedia Library; ValueType: string; ValueName: HardKey; ValueData: {code:GetHardKey2};
Root: HKLM; Subkey: SoftWare\\Yoshimasa Electronic Inc.\\Multimedia Library; ValueType: dword; ValueName: Version; ValueData: 1;
