#pragma once

class CDbArchive
{
public:
	BOOL Export(const CString &sFilePath, CMMPcsDB *pPcsDB, const FOLDERREC &oFolderRec);
	BOOL Import(const CString &sFilePath, CMMPcsDB *pPcsDB, const FOLDERREC &oFolderRec);

protected:
	long m_nVersion;
	CFile m_oFile;
	CMMPcsDB *m_pPcsDB;
	CMapStringToString m_oFolderIDMap;

	void ExportFolder(const FOLDERREC &oFolderRec);
	void ExportData(const FOLDERREC &oFolderRec);
	void WriteFolder(const FOLDERREC &oFolderRec);
	void WriteData(const DATAREC &oDataRec);
	void WriteString(LPCSTR pData);
	void WriteLong(UINT32 nData);
	void WriteBinary(CPSDB &oPsDb, LPCSTR pFieldName);
	void ReadFolder(const CString *pFolderID);
	void ReadData();
	BOOL ReadString(CString &sData);
	BOOL ReadLong(long &nData);
	BOOL ReadBinary(CPSDB &oPsDb, LPCSTR pFieldName);
};
