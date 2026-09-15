# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=TWainDll - Win32 Debug
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの TWainDll - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "TWainDll - Win32 Release" && "$(CFG)" !=\
 "TWainDll - Win32 Debug"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛ 'CFG' を定義することによって
!MESSAGE NMAKE 実行時にﾋﾞﾙﾄﾞ ﾓｰﾄﾞを指定できます。例えば:
!MESSAGE 
!MESSAGE NMAKE /f "TWainDll.mak" CFG="TWainDll - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "TWainDll - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "TWainDll - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
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
# PROP Target_Last_Scanned "TWainDll - Win32 Release"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "TWainDll - Win32 Release"

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

ALL : "$(OUTDIR)\Scan32.dll" "..\Scan32.dll"

CLEAN : 
	-@erase "$(INTDIR)\DATA.OBJ"
	-@erase "$(INTDIR)\DCA_ACQ.OBJ"
	-@erase "$(INTDIR)\DCA_GLUE.OBJ"
	-@erase "$(INTDIR)\MAIN.OBJ"
	-@erase "$(INTDIR)\SCAN.OBJ"
	-@erase "$(INTDIR)\TOOL.OBJ"
	-@erase "$(OUTDIR)\Scan32.dll"
	-@erase "$(OUTDIR)\Scan32.exp"
	-@erase "$(OUTDIR)\Scan32.lib"
	-@erase "..\Scan32.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /WX /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MT /W3 /WX /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/TWainDll.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TWainDll.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Release/Scan32.dll"
LINK32_FLAGS=kernel32.lib user32.lib /nologo /subsystem:windows /dll\
 /incremental:no /pdb:"$(OUTDIR)/Scan32.pdb" /machine:I386 /def:".\SCAN.DEF"\
 /out:"$(OUTDIR)/Scan32.dll" /implib:"$(OUTDIR)/Scan32.lib" 
DEF_FILE= \
	".\SCAN.DEF"
LINK32_OBJS= \
	"$(INTDIR)\DATA.OBJ" \
	"$(INTDIR)\DCA_ACQ.OBJ" \
	"$(INTDIR)\DCA_GLUE.OBJ" \
	"$(INTDIR)\MAIN.OBJ" \
	"$(INTDIR)\SCAN.OBJ" \
	"$(INTDIR)\TOOL.OBJ"

"$(OUTDIR)\Scan32.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build
TargetDir=.\Release
InputPath=.\Release\Scan32.dll
SOURCE=$(InputPath)

"..\Scan32.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(TargetDir)\Scan32.dll ..

# End Custom Build

!ELSEIF  "$(CFG)" == "TWainDll - Win32 Debug"

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

ALL : "$(OUTDIR)\Scan32.dll" "..\Scan32.dll"

CLEAN : 
	-@erase "$(INTDIR)\DATA.OBJ"
	-@erase "$(INTDIR)\DCA_ACQ.OBJ"
	-@erase "$(INTDIR)\DCA_GLUE.OBJ"
	-@erase "$(INTDIR)\MAIN.OBJ"
	-@erase "$(INTDIR)\SCAN.OBJ"
	-@erase "$(INTDIR)\TOOL.OBJ"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Scan32.dll"
	-@erase "$(OUTDIR)\Scan32.exp"
	-@erase "$(OUTDIR)\Scan32.ilk"
	-@erase "$(OUTDIR)\Scan32.lib"
	-@erase "$(OUTDIR)\Scan32.pdb"
	-@erase "..\Scan32.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MTd /W3 /WX /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/TWainDll.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TWainDll.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/Scan32.dll"
LINK32_FLAGS=kernel32.lib user32.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)/Scan32.pdb" /debug /machine:I386\
 /def:".\SCAN.DEF" /out:"$(OUTDIR)/Scan32.dll" /implib:"$(OUTDIR)/Scan32.lib" 
DEF_FILE= \
	".\SCAN.DEF"
LINK32_OBJS= \
	"$(INTDIR)\DATA.OBJ" \
	"$(INTDIR)\DCA_ACQ.OBJ" \
	"$(INTDIR)\DCA_GLUE.OBJ" \
	"$(INTDIR)\MAIN.OBJ" \
	"$(INTDIR)\SCAN.OBJ" \
	"$(INTDIR)\TOOL.OBJ"

"$(OUTDIR)\Scan32.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build
TargetDir=.\Debug
InputPath=.\Debug\Scan32.dll
SOURCE=$(InputPath)

"..\Scan32.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(TargetDir)\Scan32.dll ..

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

# Name "TWainDll - Win32 Release"
# Name "TWainDll - Win32 Debug"

!IF  "$(CFG)" == "TWainDll - Win32 Release"

!ELSEIF  "$(CFG)" == "TWainDll - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\DATA.C
DEP_CPP_DATA_=\
	".\twain.h"\
	

"$(INTDIR)\DATA.OBJ" : $(SOURCE) $(DEP_CPP_DATA_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TOOL.C
DEP_CPP_TOOL_=\
	".\code.h"\
	".\data.h"\
	".\extern.h"\
	".\twain.h"\
	

"$(INTDIR)\TOOL.OBJ" : $(SOURCE) $(DEP_CPP_TOOL_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DCA_GLUE.C
DEP_CPP_DCA_G=\
	".\code.h"\
	".\data.h"\
	".\extern.h"\
	".\twain.h"\
	

"$(INTDIR)\DCA_GLUE.OBJ" : $(SOURCE) $(DEP_CPP_DCA_G) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SCAN.C
DEP_CPP_SCAN_=\
	".\code.h"\
	".\data.h"\
	".\extern.h"\
	".\scan.h"\
	".\twain.h"\
	

"$(INTDIR)\SCAN.OBJ" : $(SOURCE) $(DEP_CPP_SCAN_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DCA_ACQ.C
DEP_CPP_DCA_A=\
	".\code.h"\
	".\data.h"\
	".\extern.h"\
	".\twain.h"\
	

"$(INTDIR)\DCA_ACQ.OBJ" : $(SOURCE) $(DEP_CPP_DCA_A) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MAIN.C
DEP_CPP_MAIN_=\
	".\code.h"\
	".\data.h"\
	".\extern.h"\
	".\twain.h"\
	

"$(INTDIR)\MAIN.OBJ" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SCAN.DEF

!IF  "$(CFG)" == "TWainDll - Win32 Release"

!ELSEIF  "$(CFG)" == "TWainDll - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
