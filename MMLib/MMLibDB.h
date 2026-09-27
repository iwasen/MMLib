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

	int Open(int nChType, LPCTSTR pServerName);
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

	BOOL OpenEx(CMMPcserve *pPcserve, CMMPcserve *pCachePcserve, LPCTSTR pDirName, int nOpenMode, LPCTSTR pLibraryID);
	void ReOpenEx();
	BOOL CreateEx(CMMPcserve *pPcserve, CMMPcserve *pCachePcserve, LPCTSTR pDirName, int nOpenMode, FOLDERREC *pLibraryRec);
	void CloseEx();
	BOOL ReadLibraryEx(FOLDERREC *pLibraryRec) const;
	BOOL ReadFolderEx(LPCTSTR pFID, FOLDERREC *pFolderRec);
	BOOL ReadChildFolderEx(LPCTSTR pFID, FOLDERREC *pFolderRec, BOOL bFirst);
	BOOL AddFolderEx(FOLDERREC *pFolderRec);
	BOOL UpdateFolderEx(FOLDERREC *pFolderRec);
	BOOL DeleteFolderEx(LPCTSTR pFID);
	BOOL ReadTextFolderEx(LPCTSTR pFID, CString *pText);
	BOOL WriteTextFolderEx(LPCTSTR pFID, CString *pText);
	BOOL MoveFolderEx(LPCTSTR pFidSrc, LPCTSTR pFidDst, FOLDERREC *pFolderRec);
	static BOOL CopyFolderEx(CMMPcsDB *pPcsDBSrc, LPCTSTR pFidSrc, CMMPcsDB *pPcsDBDst, LPCTSTR pFidDst, FOLDERREC *pFolderRec);
	BOOL RotateFolderEx(LPCTSTR pFID, int nRotate);
	BOOL ReadDataEx(LPCTSTR pDID, DATAREC *pDataRec);
	BOOL RotateDataEx(LPCTSTR pDID, int nRotate);
	BOOL ReadChildDataEx(LPCTSTR pFID, DATAREC *pDataRec, BOOL bFirst);
	BOOL ReadBinaryDataEx(LPCTSTR pDID, LPCTSTR pFieldName, LPSTR *pPict, long *pPictSize);
	BOOL ReadPictureDataEx(LPCTSTR pDID, PHDIB phDIB1, PHDIB phDIB2, DATAREC *pDataRec = NULL);
	BOOL WritePictureDataEx(LPCTSTR pDID, HDIB hDIB1, HDIB hDIB2, short nMMType);
	BOOL ReadTextDataEx(LPCTSTR pDID, CString *pText);
	BOOL WriteTextDataEx(LPCTSTR pDID, const CString &oText);
	BOOL AddDataEx(DATAREC *pDataRec);
	BOOL UpdateDataEx(DATAREC *pDataRec);
	BOOL DeleteDataEx(LPCTSTR pDID);
	BOOL MoveDataEx(LPCTSTR pDidSrc, LPCTSTR pFidDst, DATAREC *pDataRec);
	static BOOL CopyDataEx(CMMPcsDB *pPcsDBSrc, LPCTSTR pDidSrc, CMMPcsDB *pPcsDBDst, LPCTSTR pFidDst, DATAREC *pDataRec);
	BOOL FillCacheEx(LPCTSTR pFID);
	BOOL ClearCacheEx();
	BOOL FilterFolderEx(LPCTSTR pFilter);
	BOOL GetFolderCountEx(long *pRecNum);
	BOOL ReadFolderAllEx(FOLDERREC *pFolderRec, BOOL *pRead);
	BOOL FilterDataEx(LPCTSTR pFilter);
	BOOL GetDataCountEx(long *pRecNum);
	BOOL ReadDataAllEx(DATAREC *pDataRec, BOOL *pRead);
	BOOL PackFileEx();
	BOOL BackupEx(LPCTSTR pBackupDir, CProgressCtrl *pProgress);
	BOOL RestoreEx(LPCTSTR pBackupDir, CProgressCtrl *pProgress);
	BOOL CheckPassword(CString sPassword) const;
	void SetPassword(CString sPassword);
	BOOL CheckPermission(DWORD dwPermission) const;
	void SetPermission(DWORD dwPermission);
	int SetFolderIndex(int nIndexNo, int nIndexNo2 = -1);
	int SetDataIndex(int nIndexNo, int nIndexNo2 = -1);
	BOOL CreateFindPictDB();
	BOOL OpenFindPictDB();
	void UpdateFindPictDB(LPCTSTR pDID, HDIB hDIB);
	void DeleteFindPictDB();
	void DeleteFindPictItem(LPCTSTR pDID);
	BOOL CheckFolderID(LPCTSTR pFolderID, int nDepth = 0);

protected:
	int m_nCurrentFolderIndex;
	int m_nCurrentFolderIndex2;
	int m_nCurrentDataIndex;
	int m_nCurrentDataIndex2;
	FOLDERREC m_oLibraryRec;

	int ConvertVersion(CMMPcserve *pPcserve, LPCTSTR pDirName);
	int ConvertVersionSub(CMMPcserve *pPcserve, LPCTSTR pDirName);
	int Open(CMMPcserve *pPcserve, CMMPcserve *pCachePcserve, LPCTSTR pDirName, int nOpenMode, LPCTSTR pLibraryID);
	int Create(CMMPcserve *pPcserve, CMMPcserve *pCachePcserve, LPCTSTR pDirName, int nOpenMode, FOLDERREC *pLibraryRec);
	int ReadFolder(LPCTSTR pFID, FOLDERREC *pFolderRec);
	int ReadChildFolder(LPCTSTR pFID, FOLDERREC *pFolderRec, BOOL bFirst, BOOL *pRead);
	int AddFolder(FOLDERREC *pFolderRec);
	int UpdateFolder(FOLDERREC *pFolderRec);
	int DeleteFolder(LPCTSTR pFID);
	int ReadTextFolder(LPCTSTR pFID, CString *pText);
	int WriteTextFolder(LPCTSTR pFID, CString *pText);
	int CacheBinFldFolder(LPCTSTR pFID, LPCTSTR pFieldName);
	int MoveFolder(LPCTSTR pFidSrc, LPCTSTR pFidDst, FOLDERREC *pFolderRec = NULL);
	static int CopyFolder(CMMPcsDB *pPcsDBSrc, LPCTSTR pFidSrc, CMMPcsDB *pPcsDBDst, LPCTSTR pFidDst, FOLDERREC *pFolderRec = NULL);
	int RotateFolder(LPCTSTR pFID, int nRotate);
	int RotateData(LPCTSTR pDID, int nRotate);
	int ReadData(LPCTSTR pDID, DATAREC *pDataRec);
	int ReadChildData(LPCTSTR pFID, DATAREC *pDataRec, BOOL bFirst, BOOL *pRead);
	int ReadBinaryData(LPCTSTR pDID, LPCTSTR pFieldName, LPSTR *pPict, long *pPictSize);
	int ReadPictureData(LPCTSTR pDID, PHDIB phDIB1, PHDIB phDIB2, DATAREC *pDataRec);
	int WritePictureData(LPCTSTR pDID, HDIB hDIB1, HDIB hDIB2, short nMMType);
	int ReadTextData(LPCTSTR pDID, CString *pText);
	int WriteTextData(LPCTSTR pDID, const CString *pText);
	void WriteCacheBinary(CPSDB &oDB, CPSDB &oDB2, LPCTSTR pFieldName);
	int AddData(DATAREC *pDataRec);
	int UpdateData(DATAREC *pDataRec);
	int DeleteData(LPCTSTR pDID);
	int MoveData(LPCTSTR pDidSrc, LPCTSTR pFidDst, DATAREC *pDataRec);
	static int CopyData(CMMPcsDB *pPcsDBSrc, LPCTSTR pDidSrc, CMMPcsDB *pPcsDBDst, LPCTSTR pFidDst, DATAREC *pDataRec = NULL);
	static int CopyChildData(CMMPcsDB *pPcsDBSrc, LPCTSTR pFidSrc, CMMPcsDB *pPcsDBDst, LPCTSTR pFidDst);
	static int OpenDB(CMMPcserve *pPcserve, CPSDB &oFolderDB, CPSDB &oDataDB, LPCTSTR pDirName, BOOL bIndex = TRUE);
	static int CreateDB(CMMPcserve *pPcserve, CPSDB &oFolderDB, CPSDB &oDataDB, LPCTSTR pDirName, BOOL bIndex = TRUE);
	int CacheFolder(LPCTSTR pFID, LPBOOL pCachedChildren, LPTSTR pFolderType);
	int CacheChildFolder(LPCTSTR pFID);
	int CacheChildData(LPCTSTR pFID);
	int CacheBinaryFolder(LPCTSTR pFieldName);
	int CacheBinaryData(LPCTSTR pFieldName);
	int CacheBinary(LPCTSTR pFieldName, CPSDB &oDB, CPSDB &oDB2);
	int CacheBinFldData(LPCTSTR pDID, LPCTSTR pFieldName);
	int WritePictureCacheData(LPCTSTR pDID);
	int WriteTextCacheData(LPCTSTR pDID);
	int WriteTextCacheFolder(LPCTSTR pFID);
	static int CopyBinary(CPSDB &oSrcDB, CPSDB &oDstDB, LPCTSTR pFieldName);
	void MakeLibraryID(CString &sID);
	int MakeFolderID(CString &sID);
	int MakeDataID(CString &sID);
	void SetCurrentTime(CString &sTime);
	int UpdateFolderUpdateCount(LPCTSTR pFID);
	int FillCache(LPCTSTR pFID);
	int FillDataCache(LPCTSTR pFID);
	int ClearCache();
	int FilterFolder(LPCTSTR pFilter);
	int GetFolderCount(long *pRecNum);
	int ReadFolderAll(FOLDERREC *pFolderRec, BOOL *pRead);
	int FilterData(LPCTSTR pFilter);
	int GetDataCount(long *pRecNum);
	int ReadDataAll(DATAREC *pDataRec, BOOL *pRead);
	int PackFile();
	int SelectFolder(LPCTSTR pFID, long *nRec);
	int SelectData(LPCTSTR pFID, long *nRec);
	int Backup(LPCTSTR pBackupDir, CProgressCtrl *pProgress);
	int Restore(LPCTSTR pBackupDir, CProgressCtrl *pProgress);
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

	CMMPcsDB *OpenMMLibrary(int nChType, LPCTSTR pServerName, LPCTSTR pDirName, int nOpenMode, LPCTSTR pLibraryID = NULL);
	void ReOpenMMLibrary();
	CMMPcsDB *CreateMMLibrary(int nChType, LPCTSTR pServerName, LPCTSTR pDirName, int nOpenMode, FOLDERREC *pLibraryRec);
	void CloseMMLibrary(CMMPcsDB *pPcsDB);
	BOOL DeleteMMLibrary(CMMPcsDB *pPcsDB);
	void ExitMMLibrary();
	BOOL GetLibraryName(LPCTSTR pDirName, CString &sLibraryName);
	void WriteAccessLog(class CItemData *pItemData);
	void DeleteAccessLog(LPCTSTR pLID, LPCTSTR pDID);

protected:
	CObList m_oPcserveList;
	CMMPcserve m_oPcserveLocal;

	CMMPcserve *ConnectPcserve(int nChType, LPCTSTR pServerName);
	int DeleteAllFiles(CMMPcserve *pPcserve, LPCTSTR pDirName);
	BOOL OpenAccessLog();
	BOOL CreateAccessLog();
	void GetFolderPath(class CItem *pItem, CStringArray &saFolderPath);
};

extern CMMLibDB *g_pMMLibDB;
extern int g_nLastPcserveError;
extern BOOL g_bNoDispError;
