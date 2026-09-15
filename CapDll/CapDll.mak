# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=CapDll - Win32 Debug
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの CapDll - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "CapDll - Win32 Release" && "$(CFG)" != "CapDll - Win32 Debug"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛ 'CFG' を定義することによって
!MESSAGE NMAKE 実行時にﾋﾞﾙﾄﾞ ﾓｰﾄﾞを指定できます。例えば:
!MESSAGE 
!MESSAGE NMAKE /f "CapDll.mak" CFG="CapDll - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "CapDll - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "CapDll - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "CapDll - Win32 Release"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "CapDll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\CapDll32.dll" "..\CapDll32.dll"

CLEAN : 
	-@erase "$(INTDIR)\AUDPLAY.OBJ"
	-@erase "$(INTDIR)\CAPDLL.OBJ"
	-@erase "$(INTDIR)\Capdll.res"
	-@erase "$(INTDIR)\DIALOGS.OBJ"
	-@erase "$(INTDIR)\DIB.OBJ"
	-@erase "$(INTDIR)\MULDIVAV.OBJ"
	-@erase "$(OUTDIR)\CapDll32.dll"
	-@erase "$(OUTDIR)\CapDll32.exp"
	-@erase "$(OUTDIR)\CapDll32.lib"
	-@erase "..\CapDll32.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /WX /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MT /W3 /WX /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/CapDll.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/Capdll.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/CapDll.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib vfw32.lib largeint.lib libcmt.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib /out:"Release/CapDll32.dll"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winmm.lib vfw32.lib largeint.lib\
 libcmt.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/CapDll32.pdb" /machine:I386 /nodefaultlib /def:".\CAPDLL.DEF"\
 /out:"$(OUTDIR)/CapDll32.dll" /implib:"$(OUTDIR)/CapDll32.lib" 
DEF_FILE= \
	".\CAPDLL.DEF"
LINK32_OBJS= \
	"$(INTDIR)\AUDPLAY.OBJ" \
	"$(INTDIR)\CAPDLL.OBJ" \
	"$(INTDIR)\Capdll.res" \
	"$(INTDIR)\DIALOGS.OBJ" \
	"$(INTDIR)\DIB.OBJ" \
	"$(INTDIR)\MULDIVAV.OBJ"

"$(OUTDIR)\CapDll32.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build
TargetDir=.\Release
InputPath=.\Release\CapDll32.dll
SOURCE=$(InputPath)

"..\CapDll32.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(TargetDir)\CapDll32.dll ..

# End Custom Build

!ELSEIF  "$(CFG)" == "CapDll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\CapDll32.dll" "..\CapDll32.dll"

CLEAN : 
	-@erase "$(INTDIR)\AUDPLAY.OBJ"
	-@erase "$(INTDIR)\CAPDLL.OBJ"
	-@erase "$(INTDIR)\Capdll.res"
	-@erase "$(INTDIR)\DIALOGS.OBJ"
	-@erase "$(INTDIR)\DIB.OBJ"
	-@erase "$(INTDIR)\MULDIVAV.OBJ"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\CapDll32.dll"
	-@erase "$(OUTDIR)\CapDll32.exp"
	-@erase "$(OUTDIR)\CapDll32.ilk"
	-@erase "$(OUTDIR)\CapDll32.lib"
	-@erase "$(OUTDIR)\CapDll32.pdb"
	-@erase "..\CapDll32.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MTd /W3 /WX /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/CapDll.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/Capdll.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/CapDll.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib vfw32.lib largeint.lib libcmt.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib /out:"Debug/CapDll32.dll"
# SUBTRACT LINK32 /map
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winmm.lib vfw32.lib largeint.lib\
 libcmt.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/CapDll32.pdb" /debug /machine:I386 /nodefaultlib\
 /def:".\CAPDLL.DEF" /out:"$(OUTDIR)/CapDll32.dll"\
 /implib:"$(OUTDIR)/CapDll32.lib" 
DEF_FILE= \
	".\CAPDLL.DEF"
LINK32_OBJS= \
	"$(INTDIR)\AUDPLAY.OBJ" \
	"$(INTDIR)\CAPDLL.OBJ" \
	"$(INTDIR)\Capdll.res" \
	"$(INTDIR)\DIALOGS.OBJ" \
	"$(INTDIR)\DIB.OBJ" \
	"$(INTDIR)\MULDIVAV.OBJ"

"$(OUTDIR)\CapDll32.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build
TargetDir=.\Debug
InputPath=.\Debug\CapDll32.dll
SOURCE=$(InputPath)

"..\CapDll32.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(TargetDir)\CapDll32.dll ..

# End Custom Build

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "CapDll - Win32 Release"
# Name "CapDll - Win32 Debug"

!IF  "$(CFG)" == "CapDll - Win32 Release"

!ELSEIF  "$(CFG)" == "CapDll - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\CAPDLL.C
DEP_CPP_CAPDL=\
	".\Cap.h"\
	".\Capdll.h"\
	".\Dialogs.h"\
	".\dib.h"\
	".\Extern.h"\
	

"$(INTDIR)\CAPDLL.OBJ" : $(SOURCE) $(DEP_CPP_CAPDL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DIALOGS.C
DEP_CPP_DIALO=\
	".\Cap.h"\
	".\Dialogs.h"\
	

"$(INTDIR)\DIALOGS.OBJ" : $(SOURCE) $(DEP_CPP_DIALO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DIB.C
DEP_CPP_DIB_C=\
	".\dib.h"\
	

"$(INTDIR)\DIB.OBJ" : $(SOURCE) $(DEP_CPP_DIB_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\AUDPLAY.C
DEP_CPP_AUDPL=\
	".\audplay.h"\
	".\muldivav.h"\
	

"$(INTDIR)\AUDPLAY.OBJ" : $(SOURCE) $(DEP_CPP_AUDPL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MULDIVAV.C
DEP_CPP_MULDI=\
	".\muldivav.h"\
	

"$(INTDIR)\MULDIVAV.OBJ" : $(SOURCE) $(DEP_CPP_MULDI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CAPDLL.DEF

!IF  "$(CFG)" == "CapDll - Win32 Release"

!ELSEIF  "$(CFG)" == "CapDll - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Capdll.rc
DEP_RSC_CAPDLL=\
	".\Cap.h"\
	".\Dialogs.h"\
	".\PACHI1.ICO"\
	

"$(INTDIR)\Capdll.res" : $(SOURCE) $(DEP_RSC_CAPDLL) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
