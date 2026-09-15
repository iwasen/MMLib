# Microsoft Developer Studio Project File - Name="CapDll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CapDll - Win32 Debug_Eng
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "CapDll.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "CapDll.mak" CFG="CapDll - Win32 Debug_Eng"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "CapDll - Win32 Release_Jpn" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "CapDll - Win32 Debug_Jpn" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "CapDll - Win32 Release_Eng" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "CapDll - Win32 Debug_Eng" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CapDll - Win32 Release_Jpn"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CapDll___Win32_Release_Jpn"
# PROP BASE Intermediate_Dir "CapDll___Win32_Release_Jpn"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Jpn"
# PROP Intermediate_Dir "Release_Jpn"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /WX /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /WX /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_NEU" /d "AFX_TARG_JPN"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib vfw32.lib largeint.lib libcmt.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib /out:".\Release\CapDll32.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib vfw32.lib largeint.lib libcmt.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib /out:"Release_Jpn\CapDll32.dll"
# Begin Custom Build
TargetDir=.\Release_Jpn
InputPath=.\Release_Jpn\CapDll32.dll
SOURCE="$(InputPath)"

"..\CapDll32.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetDir)\CapDll32.dll ..

# End Custom Build

!ELSEIF  "$(CFG)" == "CapDll - Win32 Debug_Jpn"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CapDll___Win32_Debug_Jpn"
# PROP BASE Intermediate_Dir "CapDll___Win32_Debug_Jpn"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Jpn"
# PROP Intermediate_Dir "Debug_Jpn"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_NEU" /d "AFX_TARG_JPN"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib vfw32.lib largeint.lib libcmtd.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib /out:".\Debug\CapDll32.dll"
# SUBTRACT BASE LINK32 /map
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib vfw32.lib largeint.lib libcmtd.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib /out:"Debug_Jpn\CapDll32.dll"
# SUBTRACT LINK32 /map
# Begin Custom Build
TargetDir=.\Debug_Jpn
InputPath=.\Debug_Jpn\CapDll32.dll
SOURCE="$(InputPath)"

"..\CapDll32.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetDir)\CapDll32.dll ..

# End Custom Build

!ELSEIF  "$(CFG)" == "CapDll - Win32 Release_Eng"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CapDll___Win32_Release_Eng"
# PROP BASE Intermediate_Dir "CapDll___Win32_Release_Eng"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Eng"
# PROP Intermediate_Dir "Release_Eng"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /WX /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /WX /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_NEU" /d "AFX_TARG_ENU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib vfw32.lib largeint.lib libcmt.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib /out:".\Release\CapDll32.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib vfw32.lib largeint.lib libcmt.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib /out:"Release_Eng\CapDll32.dll"
# Begin Custom Build
TargetDir=.\Release_Eng
InputPath=.\Release_Eng\CapDll32.dll
SOURCE="$(InputPath)"

"..\CapDll32.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetDir)\CapDll32.dll ..

# End Custom Build

!ELSEIF  "$(CFG)" == "CapDll - Win32 Debug_Eng"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CapDll___Win32_Debug_Eng"
# PROP BASE Intermediate_Dir "CapDll___Win32_Debug_Eng"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Eng"
# PROP Intermediate_Dir "Debug_Eng"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_NEU" /d "AFX_TARG_ENU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib vfw32.lib largeint.lib libcmtd.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib /out:".\Debug\CapDll32.dll"
# SUBTRACT BASE LINK32 /map
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib vfw32.lib largeint.lib libcmtd.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib /out:"Debug_Eng\CapDll32.dll"
# SUBTRACT LINK32 /map
# Begin Custom Build
TargetDir=.\Debug_Eng
InputPath=.\Debug_Eng\CapDll32.dll
SOURCE="$(InputPath)"

"..\CapDll32.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetDir)\CapDll32.dll ..

# End Custom Build

!ENDIF 

# Begin Target

# Name "CapDll - Win32 Release_Jpn"
# Name "CapDll - Win32 Debug_Jpn"
# Name "CapDll - Win32 Release_Eng"
# Name "CapDll - Win32 Debug_Eng"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\AUDPLAY.C
# End Source File
# Begin Source File

SOURCE=.\CAPDLL.C
# End Source File
# Begin Source File

SOURCE=.\CAPDLL.DEF
# End Source File
# Begin Source File

SOURCE=.\Capdll.rc
# End Source File
# Begin Source File

SOURCE=.\DIALOGS.C
# End Source File
# Begin Source File

SOURCE=.\DIB.C
# End Source File
# Begin Source File

SOURCE=.\MULDIVAV.C
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\audplay.h
# End Source File
# Begin Source File

SOURCE=.\Cap.h
# End Source File
# Begin Source File

SOURCE=.\Capdll.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs.h
# End Source File
# Begin Source File

SOURCE=.\dib.h
# End Source File
# Begin Source File

SOURCE=.\Extern.h
# End Source File
# Begin Source File

SOURCE=.\muldivav.h
# End Source File
# Begin Source File

SOURCE=.\RESOURCE.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\PACHI1.ICO
# End Source File
# End Group
# End Target
# End Project
