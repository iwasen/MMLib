// MMLibDB.h: CMMLibDB クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define ERROR_PROGRAM		-1
#define ERROR_VERSION		-2
#define ERROR_ALREADY_OPEN	-3
#define ERROR_PERMISSION	-4
#define ERROR_PASSWORD		-5

class CMMPcserve : public CObject
{
public:
	CObList m_oPcsDBList;
	CPcserve m_oPcserve;
	int m_nChType;
	CString m_sServerName;

	int Open(int nChType, LPCSTR pServerName);
};

class CMMPcsDB : public CObject
{
public:
	CMMPcsDB();

	CMMPcserve *m_pPcserve;
	CMMPcserve *m_pCachePcserve;
	CPSDB m_oFolderDB;
	CPSDB m_oDataDB;
	CPSDB m_oFolderDB2;
	CPSDB m_oDataDB2;
	CPSDB m_oFindPictDB;
	CString m_sDirName;
	int m_nCacheMode;
	CString m_sServerName;
	int m_nChType;
	CString m_sTitle;
	CString m_sLibraryID;
	CString m_sPassword;
	DWORD m_dwPermission;

	BOOL OpenEx(CMMPcserve *pPcserve, CMMPcserve *pCachePcserve, LPCSTR pDirName, int nOpenMode, LPCSTR pLibraryID);
	void ReOpenEx();
	BOOL CreateEx(CMMPcserve *pPcserve, CMMPcserve *pCachePcserve, LPCSTR pDirName, int nOpenMode, FOLDERREC *pLibraryRec);
	void CloseEx();
	BOOL ReadLibraryEx(FOLDERREC *pLibraryRec) const;
	BOOL ReadFolderEx(LPCSTR pFID, FOLDERREC *pFolderRec);
	BOOL ReadChildFolderEx(LPCSTR pFID, FOLDERREC *pFolderRec, BOOL bFirst);
	BOOL AddFolderEx(FOLDERREC *pFolderRec);
	BOOL UpdateFolderEx(FOLDERREC *pFolderRec);
	BOOL DeleteFolderEx(LPCSTR pFID);
	BOOL ReadTextFolderEx(LPCSTR pFID, CString *pText);
	BOOL WriteTextFolderEx(LPCSTR pFID, CString *pText);
	BOOL MoveFolderEx(LPCSTR pFidSrc, LPCSTR pFidDst, FOLDERREC *pFolderRec);
	static BOOL CopyFolderEx(CMMPcsDB *pPcsDBSrc, LPCSTR pFidSrc, CMMPcsDB *pPcsDBDst, LPCSTR pFidDst, FOLDERREC *pFolderRec);
	BOOL RotateFolderEx(LPCSTR pFID, int nRotate);
	BOOL ReadDataEx(LPCSTR pDID, DATAREC *pDataRec);
	BOOL RotateDataEx(LPCSTR pDID, int nRotate);
	BOOL ReadChildDataEx(LPCSTR pFID, DATAREC *pDataRec, BOOL bFirst);
	BOOL ReadBinaryDataEx(LPCSTR pDID, LPCSTR pFieldName, LPSTR *pPict, long *pPictSize);
	BOOL ReadPictureDataEx(LPCSTR pDID, PHDIB phDIB1, PHDIB phDIB2, DATAREC *pDataRec = NULL);
	BOOL WritePictureDataEx(LPCSTR pDID, HDIB hDIB1, HDIB hDIB2, short nMMType);
	BOOL ReadTextDataEx(LPCSTR pDID, CString *pText);
	BOOL WriteTextDataEx(LPCSTR pDID, const CString &oText);
	BOOL AddDataEx(DATAREC *pDataRec);
	BOOL UpdateDataEx(DATAREC *pDataRec);
	BOOL DeleteDataEx(LPCSTR pDID);
	BOOL MoveDataEx(LPCSTR pDidSrc, LPCSTR pFidDst, DATAREC *pDataRec);
	static BOOL CopyDataEx(CMMPcsDB *pPcsDBSrc, LPCSTR pDidSrc, CMMPcsDB *pPcsDBDst, LPCSTR pFidDst, DATAREC *pDataRec);
	BOOL FillCacheEx(LPCSTR pFID);
	BOOL ClearCacheEx();
	BOOL FilterFolderEx(LPCSTR pFilter);
	BOOL GetFolderCountEx(long *pRecNum);
	BOOL ReadFolderAllEx(FOLDERREC *pFolderRec, BOOL *pRead);
	BOOL FilterDataEx(LPCSTR pFilter);
	BOOL GetDataCountEx(long *pRecNum);
	BOOL ReadDataAllEx(DATAREC *pDataRec, BOOL *pRead);
	BOOL PackFileEx();
	BOOL BackupEx(LPCSTR pBackupDir, CProgressCtrl *pProgress);
	BOOL RestoreEx(LPCSTR pBackupDir, CProgressCtrl *pProgress);
	BOOL CheckPassword(CString sPassword) const;
	void SetPassword(CString sPassword);
	BOOL CheckPermission(DWORD dwPermission) const;
	void SetPermission(DWORD dwPermission);
	int SetFolderIndex(int nIndexNo, int nIndexNo2 = -1);
	int SetDataIndex(int nIndexNo, int nIndexNo2 = -1);
	BOOL CreateFindPictDB();
	BOOL OpenFindPictDB();
	void UpdateFindPictDB(LPCSTR pDID, HDIB hDIB);
	void DeleteFindPictDB();
	void DeleteFindPictItem(LPCSTR pDID);
	BOOL CheckFolderID(LPCSTR pFolderID, int nDepth = 0);

protected:
	int m_nCurrentFolderIndex;
	int m_nCurrentFolderIndex2;
	int m_nCurrentDataIndex;
	int m_nCurrentDataIndex2;
	FOLDERREC m_oLibraryRec;

	int ConvertVersion(CMMPcserve *pPcserve, LPCSTR pDirName);
	int ConvertVersionSub(CMMPcserve *pPcserve, LPCSTR pDirName);
	int Open(CMMPcserve *pPcserve, CMMPcserve *pCachePcserve, LPCSTR pDirName, int nOpenMode, LPCSTR pLibraryID);
	int Create(CMMPcserve *pPcserve, CMMPcserve *pCachePcserve, LPCSTR pDirName, int nOpenMode, FOLDERREC *pLibraryRec);
	int ReadFolder(LPCSTR pFID, FOLDERREC *pFolderRec);
	int ReadChildFolder(LPCSTR pFID, FOLDERREC *pFolderRec, BOOL bFirst, BOOL *pRead);
	int AddFolder(FOLDERREC *pFolderRec);
	int UpdateFolder(FOLDERREC *pFolderRec);
	int DeleteFolder(LPCSTR pFID);
	int ReadTextFolder(LPCSTR pFID, CString *pText);
	int WriteTextFolder(LPCSTR pFID, CString *pText);
	int CacheBinFldFolder(LPCSTR pFID, LPCSTR pFieldName);
	int MoveFolder(LPCSTR pFidSrc, LPCSTR pFidDst, FOLDERREC *pFolderRec = NULL);
	static int CopyFolder(CMMPcsDB *pPcsDBSrc, LPCSTR pFidSrc, CMMPcsDB *pPcsDBDst, LPCSTR pFidDst, FOLDERREC *pFolderRec = NULL);
	int RotateFolder(LPCSTR pFID, int nRotate);
	int RotateData(LPCSTR pDID, int nRotate);
	int ReadData(LPCSTR pDID, DATAREC *pDataRec);
	int ReadChildData(LPCSTR pFID, DATAREC *pDataRec, BOOL bFirst, BOOL *pRead);
	int ReadBinaryData(LPCSTR pDID, LPCSTR pFieldName, LPSTR *pPict, long *pPictSize);
	int ReadPictureData(LPCSTR pDID, PHDIB phDIB1, PHDIB phDIB2, DATAREC *pDataRec);
	int WritePictureData(LPCSTR pDID, HDIB hDIB1, HDIB hDIB2, short nMMType);
	int ReadTextData(LPCSTR pDID, CString *pText);
	int WriteTextData(LPCSTR pDID, const CString *pText);
	void WriteCacheBinary(CPSDB &oDB, CPSDB &oDB2, LPCSTR pFieldName);
	int AddData(DATAREC *pDataRec);
	int UpdateData(DATAREC *pDataRec);
	int DeleteData(LPCSTR pDID);
	int MoveData(LPCSTR pDidSrc, LPCSTR pFidDst, DATAREC *pDataRec);
	static int CopyData(CMMPcsDB *pPcsDBSrc, LPCSTR pDidSrc, CMMPcsDB *pPcsDBDst, LPCSTR pFidDst, DATAREC *pDataRec = NULL);
	static int CopyChildData(CMMPcsDB *pPcsDBSrc, LPCSTR pFidSrc, CMMPcsDB *pPcsDBDst, LPCSTR pFidDst);
	static int OpenDB(CMMPcserve *pPcserve, CPSDB &oFolderDB, CPSDB &oDataDB, LPCSTR pDirName, BOOL bIndex = TRUE);
	static int CreateDB(CMMPcserve *pPcserve, CPSDB &oFolderDB, CPSDB &oDataDB, LPCSTR pDirName, BOOL bIndex = TRUE);
	int CacheFolder(LPCSTR pFID, LPBOOL pCachedChildren, LPSTR pFolderType);
	int CacheChildFolder(LPCSTR pFID);
	int CacheChildData(LPCSTR pFID);
	int CacheBinaryFolder(LPCSTR pFieldName);
	int CacheBinaryData(LPCSTR pFieldName);
	int CacheBinary(LPCSTR pFieldName, CPSDB &oDB, CPSDB &oDB2);
	int CacheBinFldData(LPCSTR pDID, LPCSTR pFieldName);
	int WritePictureCacheData(LPCSTR pDID);
	int WriteTextCacheData(LPCSTR pDID);
	int WriteTextCacheFolder(LPCSTR pFID);
	static int CopyBinary(CPSDB &oSrcDB, CPSDB &oDstDB, LPCSTR pFieldName);
	void MakeLibraryID(CString &sID);
	int MakeFolderID(CString &sID);
	int MakeDataID(CString &sID);
	void SetCurrentTime(CString &sTime);
	int UpdateFolderUpdateCount(LPCSTR pFID);
	int FillCache(LPCSTR pFID);
	int FillDataCache(LPCSTR pFID);
	int ClearCache();
	int FilterFolder(LPCSTR pFilter);
	int GetFolderCount(long *pRecNum);
	int ReadFolderAll(FOLDERREC *pFolderRec, BOOL *pRead);
	int FilterData(LPCSTR pFilter);
	int GetDataCount(long *pRecNum);
	int ReadDataAll(DATAREC *pDataRec, BOOL *pRead);
	int PackFile();
	int SelectFolder(LPCSTR pFID, long *nRec);
	int SelectData(LPCSTR pFID, long *nRec);
	int Backup(LPCSTR pBackupDir, CProgressCtrl *pProgress);
	int Restore(LPCSTR pBackupDir, CProgressCtrl *pProgress);
	void FlushFolder();
	void FlushData();
};

class CMMLibDB
{
public:
	CMMLibDB();
	virtual ~CMMLibDB();

	CString m_sCacheDir;
	CPSDB m_oAccessLog;

	CMMPcsDB *OpenMMLibrary(int nChType, LPCSTR pServerName, LPCSTR pDirName, int nOpenMode, LPCSTR pLibraryID = NULL);
	void ReOpenMMLibrary();
	CMMPcsDB *CreateMMLibrary(int nChType, LPCSTR pServerName, LPCSTR pDirName, int nOpenMode, FOLDERREC *pLibraryRec);
	void CloseMMLibrary(CMMPcsDB *pPcsDB);
	BOOL DeleteMMLibrary(CMMPcsDB *pPcsDB);
	void ExitMMLibrary();
	BOOL GetLibraryName(LPCSTR pDirName, CString &sLibraryName);
	void WriteAccessLog(class CItemData *pItemData);
	void DeleteAccessLog(LPCSTR pLID, LPCSTR pDID);

protected:
	CObList m_oPcserveList;
	CMMPcserve m_oPcserveLocal;

	CMMPcserve *ConnectPcserve(int nChType, LPCSTR pServerName);
	int DeleteAllFiles(CMMPcserve *pPcserve, LPCSTR pDirName);
	BOOL OpenAccessLog();
	BOOL CreateAccessLog();
	void GetFolderPath(class CItem *pItem, CStringArray &saFolderPath);
};

extern CMMLibDB *g_pMMLibDB;
extern int g_nLastPcserveError;
extern BOOL g_bNoDispError;
