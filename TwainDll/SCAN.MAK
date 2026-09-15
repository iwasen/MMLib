# Microsoft Visual C++ generated build script - Do not modify

PROJ = SCAN
DEBUG = 0
PROGTYPE = 1
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = B:\HAMA\WIN\TWAINDLL\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = DCA_ACQ.C   
FIRSTCPP =             
RC = rc
CFLAGS_D_WDLL = /nologo /W3 /FR /G2 /Zi /D_DEBUG /Od /GD /ALw
CFLAGS_R_WDLL = /nologo /W3 /FR /O1 /DNDEBUG /GD /ALw
LFLAGS_D_WDLL = /NOLOGO /NOD /NOE /PACKC:61440 /ALIGN:16 /ONERROR:NOEXE /CO /MAP:FULL
LFLAGS_R_WDLL = /NOLOGO /NOD /NOE /PACKC:61440 /ALIGN:16 /ONERROR:NOEXE /MAP:FULL
LIBS_D_WDLL = oldnames libw ldllcew 
LIBS_R_WDLL = oldnames libw ldllcew 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
DEFFILE = SCAN.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WDLL)
LFLAGS = $(LFLAGS_D_WDLL)
LIBS = $(LIBS_D_WDLL)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WDLL)
LFLAGS = $(LFLAGS_R_WDLL)
LIBS = $(LIBS_R_WDLL)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = DCA_ACQ.SBR \
		DCA_GLUE.SBR \
		SCAN.SBR \
		TOOL.SBR \
		MAIN.SBR \
		DATA.SBR


DCA_ACQ_DEP = b:\hama\win\twaindll\twain.h \
	b:\hama\win\twaindll\code.h \
	b:\hama\win\twaindll\extern.h \
	b:\hama\win\twaindll\data.h


DCA_GLUE_DEP = b:\hama\win\twaindll\twain.h \
	b:\hama\win\twaindll\code.h \
	b:\hama\win\twaindll\extern.h \
	b:\hama\win\twaindll\data.h


SCAN_DEP = b:\hama\win\twaindll\twain.h \
	b:\hama\win\twaindll\code.h \
	b:\hama\win\twaindll\extern.h \
	b:\hama\win\twaindll\data.h \
	b:\hama\win\twaindll\scan.h


TOOL_DEP = b:\hama\win\twaindll\twain.h \
	b:\hama\win\twaindll\code.h \
	b:\hama\win\twaindll\extern.h \
	b:\hama\win\twaindll\data.h


SCAN_RCDEP = b:\hama\win\twaindll\code.h \
	b:\hama\win\twaindll\twacker.ico


MAIN_DEP = b:\hama\win\twaindll\twain.h \
	b:\hama\win\twaindll\code.h \
	b:\hama\win\twaindll\extern.h \
	b:\hama\win\twaindll\data.h


DATA_DEP = b:\hama\win\twaindll\twain.h


all:	$(PROJ).DLL $(PROJ).BSC

DCA_ACQ.OBJ:	DCA_ACQ.C $(DCA_ACQ_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c DCA_ACQ.C

DCA_GLUE.OBJ:	DCA_GLUE.C $(DCA_GLUE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c DCA_GLUE.C

SCAN.OBJ:	SCAN.C $(SCAN_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c SCAN.C

TOOL.OBJ:	TOOL.C $(TOOL_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c TOOL.C

SCAN.RES:	SCAN.RC $(SCAN_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r SCAN.RC

MAIN.OBJ:	MAIN.C $(MAIN_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c MAIN.C

DATA.OBJ:	DATA.C $(DATA_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c DATA.C


$(PROJ).DLL::	SCAN.RES

$(PROJ).DLL::	DCA_ACQ.OBJ DCA_GLUE.OBJ SCAN.OBJ TOOL.OBJ MAIN.OBJ DATA.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
DCA_ACQ.OBJ +
DCA_GLUE.OBJ +
SCAN.OBJ +
TOOL.OBJ +
MAIN.OBJ +
DATA.OBJ +
$(OBJS_EXT)
$(PROJ).DLL
$(MAPFILE)
a:\msvc\lib\+
a:\msvc\mfc\lib\+
a:\vfwdk\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) SCAN.RES $@
	@copy $(PROJ).CRF MSVC.BND
	implib /nowep $(PROJ).LIB $(PROJ).DLL

$(PROJ).DLL::	SCAN.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) SCAN.RES $@

run: $(PROJ).DLL
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
