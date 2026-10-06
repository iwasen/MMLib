// MMLibDB.cpp: CMMLibDB クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MMLib.h"
#include "CheckPwdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDX_NULL			0
#define IDX_FOLDER_FID		1
#define IDX_FOLDER_PFID		2
#define IDX_DATA_FID		1
#define IDX_DATA_DID		2

#define MAX_LOG_COUNT		300

CMMLibDB *g_pMMLibDB;
int g_nLastPcserveError;
BOOL g_bNoDispError;

static int CopyDB(CPSDB *pSrcFolderDB, CPSDB *pSrcDataDB, CPSDB *pDstFolderDB, CPSDB *pDstDataDB, CProgressCtrl *pProgress);
static int CopyFolderDB(CPSDB *pSrcFolderDB, CPSDB *pDstFolderDB, CProgressCtrl *pProgress, int nCount);
static int CopyDataDB(CPSDB *pSrcDataDB, CPSDB *pDstDataDB, CProgressCtrl *pProgress, int nCount);
static int CopyBinaryDB(CPSDB *pSrcDB, CPSDB *pDstDB, LPCSTR pFieldName);
static void PcserveError(int nErrorCode);
//static void VersionError(int nChType, LPCSTR pServer, LPCSTR pDirectory);

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CMMLibDB::CMMLibDB()
{
	::GetCacheDir(m_sCacheDir);

	if (!CreateDirectoryAll(m_sCacheDir))
		::AfxMessageBox(IDS_CREATE_CACHE_FOLDER_FAIL);

	if (m_sCacheDir.Right(1) != "\\")
		m_sCacheDir += "\\";

	m_oPcserveLocal.Open(CHT_LOCAL, "");

	OpenAccessLog();
}

CMMLibDB::~CMMLibDB()
{
	m_oAccessLog.DBClose();
}

CMMPcsDB *CMMLibDB::OpenMMLibrary(int nChType, LPCSTR pServerName, LPCSTR pDirName, int nOpenMode, LPCSTR pLibraryID)
{
	CMMPcserve *pPcserve;
	CMMPcsDB *pPcsDB;

	CString oDirectory = pDirName;

	if (oDirectory.IsEmpty())
		oDirectory = ".";

	if (oDirectory.Right(1) != '\\')
		oDirectory += "\\";

	if (nOpenMode != CACHEMODE_OFFLINE) {
		if ((pPcserve = ConnectPcserve(nChType, pServerName)) == NULL)
			return NULL;

		POSITION pos = pPcserve->m_oPcsDBList.GetHeadPosition();
		while (pos != NULL) {
			pPcsDB = (CMMPcsDB *)pPcserve->m_oPcsDBList.GetNext(pos);
			if (pPcsDB->m_sDirName.CompareNoCase(oDirectory) == 0) {
				PcserveError(ERROR_ALREADY_OPEN);
				return NULL;
			}
		}
	} else
		pPcserve = NULL;

	pPcsDB = new CMMPcsDB;
	if (!pPcsDB->OpenEx(pPcserve, &m_oPcserveLocal, oDirectory, nOpenMode, pLibraryID)) {
		delete pPcsDB;
		return NULL;
	}

	if (nOpenMode != CACHEMODE_OFFLINE)
		pPcserve->m_oPcsDBList.AddTail(pPcsDB);

	pPcsDB->m_sServerName = pServerName;
	pPcsDB->m_nChType = nChType;

	return pPcsDB;
}

void CMMLibDB::ReOpenMMLibrary()
{
	for (POSITION pos = m_oPcserveList.GetHeadPosition(); pos != NULL; ) {
		CMMPcserve *pPcserve = (CMMPcserve *)m_oPcserveList.GetNext(pos);
		for (POSITION pos2 = pPcserve->m_oPcsDBList.GetHeadPosition(); pos2 != NULL; ) {
			CMMPcsDB *pPcsDB = (CMMPcsDB *)pPcserve->m_oPcsDBList.GetNext(pos2);
			pPcsDB->ReOpenEx();
		}
	}
}

CMMPcsDB *CMMLibDB::CreateMMLibrary(int nChType, LPCSTR pServerName, LPCSTR pDirName, int nOpenMode, FOLDERREC *pLibraryRec)
{
	CMMPcserve *pPcserve;
	CMMPcsDB *pPcsDB;
	CString oDirectory;

	oDirectory = pDirName;

	if (oDirectory.IsEmpty())
		oDirectory = ".";

	if (oDirectory.Right(1) != '\\')
		oDirectory += "\\";

	if (nOpenMode != CACHEMODE_OFFLINE) {
		if ((pPcserve = ConnectPcserve(nChType, pServerName)) == NULL)
			return NULL;
	} else
		pPcserve = NULL;

	pPcsDB = new CMMPcsDB;
	if (!pPcsDB->CreateEx(pPcserve, &m_oPcserveLocal, oDirectory, nOpenMode, pLibraryRec)) {
		delete pPcsDB;
		return NULL;
	}

	if (nOpenMode != CACHEMODE_OFFLINE)
		pPcserve->m_oPcsDBList.AddTail(pPcsDB);

	pPcsDB->m_sServerName = pServerName;
	pPcsDB->m_nChType = nChType;

	return pPcsDB;
}

void CMMLibDB::CloseMMLibrary(CMMPcsDB *pPcsDB)
{
	if (pPcsDB->m_nCacheMode == CACHEMODE_OFFLINE)
		return;

	CMMPcserve *pPcserve = pPcsDB->m_pPcserve;

	POSITION pos;
	if ((pos = pPcserve->m_oPcsDBList.Find(pPcsDB)) != NULL)
		pPcserve->m_oPcsDBList.RemoveAt(pos);

	delete pPcsDB;

	if (pPcserve->m_oPcsDBList.IsEmpty()) {
		if ((pos = m_oPcserveList.Find(pPcserve)) != NULL)
			m_oPcserveList.RemoveAt(pos);

		delete pPcserve;
	}
}

BOOL CMMLibDB::DeleteMMLibrary(CMMPcsDB *pPcsDB)
{
	int err;

	if (pPcsDB->m_nCacheMode == CACHEMODE_OFFLINE)
		return TRUE;

	if (!pPcsDB->CheckPermission(PERMISSION_DELETE)) {
		PcserveError(ERROR_PERMISSION);
		return FALSE;
	}

	CMMPcserve *pPcserve = pPcsDB->m_pPcserve;

	pPcsDB->CloseEx();

	if ((err = DeleteAllFiles(pPcserve, pPcsDB->m_sDirName)) != 0) {
		PcserveError(err);
		pPcsDB->OpenEx(pPcserve, &m_oPcserveLocal, pPcsDB->m_sDirName, pPcsDB->m_nCacheMode, pPcsDB->m_sLibraryID);
		return FALSE;
	}

	POSITION pos;
	if ((pos = pPcserve->m_oPcsDBList.Find(pPcsDB)) != NULL)
		pPcserve->m_oPcsDBList.RemoveAt(pos);

	delete pPcsDB;

	if (pPcserve->m_oPcsDBList.IsEmpty()) {
		if ((pos = m_oPcserveList.Find(pPcserve)) != NULL)
			m_oPcserveList.RemoveAt(pos);

		delete pPcserve;
	}

	return TRUE;
}

void CMMLibDB::ExitMMLibrary()
{
	while (!m_oPcserveList.IsEmpty()) {
		CMMPcserve *pPcserve = (CMMPcserve *)m_oPcserveList.RemoveHead();
		while (!pPcserve->m_oPcsDBList.IsEmpty()) {
			CMMPcsDB *pPcsDB = (CMMPcsDB *)pPcserve->m_oPcsDBList.RemoveHead();
			delete pPcsDB;
		}
		delete pPcserve;
	}
}

BOOL CMMLibDB::GetLibraryName(LPCSTR pDirName, CString &sLibraryName)
{
	CPSDB oFolderDB;
	CString sPathName;
	int nIdxFolderFID;
	int nFind;

	sPathName.Format("%s\\%s", pDirName, FN_FOLDER_DBF);
	if (oFolderDB.DBOpen(m_oPcserveLocal.m_oPcserve, sPathName) != 0)
		return FALSE;

	sPathName.Format("%s\\%s", pDirName, FN_FOLDER_NDX1);
	if (oFolderDB.DBIndex(sPathName, &nIdxFolderFID) != 0) {
		if (oFolderDB.DBIdxCreate(sPathName, "FID", 1, &nIdxFolderFID) != 0)
			return FALSE;
	}

	FOLDERBUF oFolderBuf;
	if (oFolderDB.DBReadKey(FID_LIBRARY, LEN_ID, &oFolderBuf, 0, &nFind) != 0)
		return FALSE;

	if (nFind == 0)
		return FALSE;

	FOLDERREC oFolderRec;
	oFolderDB.DBGetFieldBuf(&oFolderBuf, &oFolderRec);

	sLibraryName = oFolderRec.sFolderName;

	return TRUE;
}

CMMPcserve *CMMLibDB::ConnectPcserve(int nChType, LPCSTR pServerName)
{
	CMMPcserve *pPcserve = NULL;
	BOOL bFind = FALSE;
	int err;

	POSITION pos = m_oPcserveList.GetHeadPosition();
	while (pos != NULL) {
		pPcserve = (CMMPcserve *)m_oPcserveList.GetNext(pos);
		if (pPcserve->m_nChType == nChType && (nChType == 0 || pPcserve->m_sServerName.CompareNoCase(pServerName) == 0)) {
			bFind = TRUE;
			break;
		}
	}

	if (!bFind) {
		pPcserve = new CMMPcserve;
		if ((err = pPcserve->Open(nChType, pServerName)) != 0) {
			delete pPcserve;
			return NULL;
		}

		m_oPcserveList.AddTail(pPcserve);
	}

	return pPcserve;
}

int CMMLibDB::DeleteAllFiles(CMMPcserve *pPcserve, LPCSTR pDirName)
{
	CString sPathName;
	int err;

	sPathName.Format("%s%s", pDirName, FN_FOLDER_DBF);
	if ((err = pPcserve->m_oPcserve.CFDelete(sPathName)) != 0)
		return err;

	sPathName.Format("%s%s", pDirName, FN_FOLDER_BIN);
	if ((err = pPcserve->m_oPcserve.CFDelete(sPathName)) != 0)
		return err;

	sPathName.Format("%s%s", pDirName, FN_FOLDER_NDX1);
	if ((err = pPcserve->m_oPcserve.CFDelete(sPathName)) != 0)
		return err;

	sPathName.Format("%s%s", pDirName, FN_FOLDER_NDX2);
	if ((err = pPcserve->m_oPcserve.CFDelete(sPathName)) != 0)
		return err;

	sPathName.Format("%s%s", pDirName, FN_DATA_DBF);
	if ((err = pPcserve->m_oPcserve.CFDelete(sPathName)) != 0)
		return err;

	sPathName.Format("%s%s", pDirName, FN_DATA_BIN);
	if ((err = pPcserve->m_oPcserve.CFDelete(sPathName)) != 0)
		return err;

	sPathName.Format("%s%s", pDirName, FN_DATA_NDX1);
	if ((err = pPcserve->m_oPcserve.CFDelete(sPathName)) != 0)
		return err;

	sPathName.Format("%s%s", pDirName, FN_DATA_NDX2);
	if ((err = pPcserve->m_oPcserve.CFDelete(sPathName)) != 0)
		return err;

	if (pPcserve->m_nChType == CHT_LOCAL) {
		CString sLibraryDir;
		::GetLibraryDir(sLibraryDir);
		if (_strnicmp(sLibraryDir, pDirName, sLibraryDir.GetLength()) == 0) {
			sLibraryDir = pDirName;
			sLibraryDir.TrimRight('\\');
			RemoveDirectory(sLibraryDir);
		}
	}

	return 0;
}

BOOL CMMLibDB::OpenAccessLog()
{
	int nIndexNo;

	CString sDir;
	::GetAccessLogDir(sDir);

	CString sPathName;
	sPathName.Format("%s\\%s", (LPCSTR)sDir, FN_ACCESS_LOG_DBF);
	if (m_oAccessLog.DBOpen(m_oPcserveLocal.m_oPcserve, sPathName) != 0)
		return CreateAccessLog();

	sPathName.Format("%s\\%s", (LPCSTR)sDir, FN_ACCESS_LOG_NDX1);
	if (m_oAccessLog.DBIndex(sPathName, &nIndexNo) != 0) {
		if (m_oAccessLog.DBIdxCreate(sPathName, "SEQNO", 1, &nIndexNo) != 0)
			return FALSE;
	}

	sPathName.Format("%s\\%s", (LPCSTR)sDir, FN_ACCESS_LOG_NDX2);
	if (m_oAccessLog.DBIndex(sPathName, &nIndexNo) != 0) {
		if (m_oAccessLog.DBIdxCreate(sPathName, "LID+DID", 1, &nIndexNo) != 0)
			return FALSE;
	}

	m_oAccessLog.DBReindex();

	return TRUE;
}

BOOL CMMLibDB::CreateAccessLog()
{
	static constexpr DBF_I field[] = {
		{"SEQNO",      'I', LEN_SEQ_NO,      0},	// シーケンス番号
		{"LID",        'C', LEN_ID,          0},	// ライブラリＩＤ
		{"DID",        'C', LEN_ID,          0},	// データＩＤ
		{"ACCESSTIME", 'C', LEN_TIME,        0},	// アクセス日時
		{"TITLE",	   'C', LEN_TITLE,       0},	// タイトル
		{"FOLDERPATH", 'C', LEN_FOLDER_PATH, 0},	// フォルダーパス
		{"CDATE",      'C', LEN_DATE,        0}		// 作成日付
	};
	CString sDir, sPathName;
	int nIndexNo;

	::GetAccessLogDir(sDir);
	CreateDirectoryAll(sDir);

	sPathName.Format("%s\\%s", (LPCSTR)sDir, FN_ACCESS_LOG_DBF);
	if (m_oAccessLog.DBCreate(m_oPcserveLocal.m_oPcserve, sPathName, field, ARRAY_SIZE(field)) != 0)
		return FALSE;

	sPathName.Format("%s\\%s", (LPCSTR)sDir, FN_ACCESS_LOG_NDX1);
	if (m_oAccessLog.DBIdxCreate(sPathName, "SEQNO", 1, &nIndexNo) != 0)
		return FALSE;

	sPathName.Format("%s\\%s", (LPCSTR)sDir, FN_ACCESS_LOG_NDX2);
	if (m_oAccessLog.DBIdxCreate(sPathName, "LID+DID", 1, &nIndexNo) != 0)
		return FALSE;

	return TRUE;
}

void CMMLibDB::WriteAccessLog(CItemData *pItemData)
{
	long nRecCount = 0;
	int nSeqNo;
	BOOL bEof;

	if (m_oAccessLog.GetFCB() == NULL)
		return;

	DeleteAccessLog(pItemData->m_pPcsDB->m_sLibraryID, pItemData->m_oDataRec.sDataID);

	m_oAccessLog.DBChgIdx(1);

	while (true) {
		m_oAccessLog.DBRecCount(&nRecCount);
		if (nRecCount < MAX_LOG_COUNT)
			break;

		m_oAccessLog.DBTop();
		m_oAccessLog.DBDelete2();
	}

	ACCESSLOGBUF oAccessLogBuf;
	ACCESSLOGREC oAccessLogRec;

	m_oAccessLog.DBBottom();
	m_oAccessLog.DBEof(&bEof);
	if (!bEof) {
		m_oAccessLog.DBRead(&oAccessLogBuf);
		m_oAccessLog.DBGetFieldBuf(&oAccessLogBuf, &oAccessLogRec);
		nSeqNo = oAccessLogRec.nSeqNo + 1;
	} else
		nSeqNo = 1;

	CStringArray saFolderPath;
	GetFolderPath(pItemData, saFolderPath);

	CTime oTime = CTime::GetCurrentTime();
	oAccessLogRec.nSeqNo = nSeqNo;
	oAccessLogRec.sLibraryID = pItemData->m_pPcsDB->m_sLibraryID;
	oAccessLogRec.sDataID = pItemData->m_oDataRec.sDataID;
	oAccessLogRec.sAccessTime.Format("%04d/%02d/%02d %02d:%02d:%02d", oTime.GetYear(), oTime.GetMonth(), oTime.GetDay(), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());
	oAccessLogRec.sTitle = pItemData->m_oDataRec.sTitle;
	StrJoin(saFolderPath, "/", oAccessLogRec.sFolderPath);
	oAccessLogRec.sCreateDate = pItemData->m_oDataRec.sCreateDate;
	m_oAccessLog.DBSetFieldBuf(&oAccessLogBuf, &oAccessLogRec);
	m_oAccessLog.DBStore(&oAccessLogBuf);
}

void CMMLibDB::DeleteAccessLog(LPCSTR pLID, LPCSTR pDID)
{
	CString sKey;

	sKey.Format("%s%s", pLID, pDID);
	m_oAccessLog.DBChgIdx(2);
	m_oAccessLog.DBDeleteKey(sKey, LEN_ID + LEN_ID, 1);
}

void CMMLibDB::GetFolderPath(CItem *pItem, CStringArray &saFolderPath)
{
	switch (pItem->m_nItemType) {
	case ITEM_INBOX:
	case ITEM_FOLDER:
	case ITEM_FIND:
		GetFolderPath(pItem->GetParent(), saFolderPath);
		saFolderPath.Add(pItem->GetTitle());
		break;
	case ITEM_DATA:
		GetFolderPath(pItem->GetParent(), saFolderPath);
		break;
	default:
		return;
	}
}

//////////////////////////////////////////////////////////////////////
// CMMPcserve クラス
//////////////////////////////////////////////////////////////////////

int CMMPcserve::Open(int nChType, LPCSTR pServerName)
{
	int err;

	if ((err = m_oPcserve.Connect(nChType, pServerName, "MMLIB")) != 0) {
		PcserveError(err);
		return err;
	}

	m_nChType = nChType;

	if (nChType == CHT_LOCAL)
		m_sServerName = "(Local)";
	else
		m_sServerName = pServerName;

	return 0;
}

//////////////////////////////////////////////////////////////////////
// CMMPcsDB クラス
//////////////////////////////////////////////////////////////////////

CMMPcsDB::CMMPcsDB()
{
	m_nCurrentFolderIndex = 1;
	m_nCurrentFolderIndex2 = 1;
	m_nCurrentDataIndex = 1;
	m_nCurrentDataIndex2 = 1;
}

//////////////////////////////////////////////////////////////////////
// CMMPcsDB クラス
//////////////////////////////////////////////////////////////////////

BOOL CMMPcsDB::OpenEx(CMMPcserve *pPcserve, CMMPcserve *pCachePcserve, LPCSTR pDirName, int nOpenMode, LPCSTR pLibraryID)
{
	int err;

open:
	if ((err = Open(pPcserve, pCachePcserve, pDirName, nOpenMode, pLibraryID)) != 0) {
		if (nOpenMode != CACHEMODE_OFFLINE) {
			if (err == ERROR_DB_NOFILE) {
				if ((err = ConvertVersion(pPcserve, pDirName)) == 0)
					goto open;
			}
			PcserveError(err);
		}
		return FALSE;
	}

	if (!m_sPassword.IsEmpty()) {
		CCheckPwdDlg dlg;

		dlg.m_sLibrary = m_oLibraryRec.sFolderName;
		for (;;) {
			if (dlg.DoModal() == IDOK) {
				if (dlg.m_sPassword == m_sPassword)
					break;
				else
					PcserveError(ERROR_PASSWORD);
			} else {
				CloseEx();
				return FALSE;
			}
		}
	}

	OpenFindPictDB();

	return TRUE;
}

void CMMPcsDB::ReOpenEx()
{
	CloseEx();
	OpenEx(m_pPcserve, m_pCachePcserve, m_sDirName, m_nCacheMode, m_sLibraryID);
}

int CMMPcsDB::ConvertVersion(CMMPcserve *pPcserve, LPCSTR pDirName)
{
	int err;

	if ((err = ConvertVersionSub(pPcserve, pDirName)) != 0) {
		CString sPathName;
		sPathName.Format("%s%s", pDirName, FN_FOLDER_DBF);
		pPcserve->m_oPcserve.CFDelete(sPathName);
		sPathName.Format("%s%s", pDirName, FN_FOLDER_BIN);
		pPcserve->m_oPcserve.CFDelete(sPathName);
		sPathName.Format("%s%s", pDirName, FN_DATA_DBF);
		pPcserve->m_oPcserve.CFDelete(sPathName);
		sPathName.Format("%s%s", pDirName, FN_DATA_BIN);
		pPcserve->m_oPcserve.CFDelete(sPathName);
	}

	return err;
}

int CMMPcsDB::ConvertVersionSub(CMMPcserve *pPcserve, LPCSTR pDirName)
{
	CString sPathName;
	CPSDB oOldDB;
	int err;

	sPathName.Format("%s%s", pDirName, "FOLDER2.DBF");
	if ((err = oOldDB.DBOpen(pPcserve->m_oPcserve, sPathName)) == 0) {
		CString sMsg;
		sMsg.FormatMessage(IDS_OLD_DATABASE_VERSION, (LPCSTR)sPathName);
		::MessageBox(NULL, sMsg, "MMLib", MB_OK | MB_ICONEXCLAMATION);
		if ((err = CreateDB(pPcserve, m_oFolderDB, m_oDataDB, pDirName)) != 0)
			return err;
		{
			CWaitCursor wait;
			if ((err = m_oFolderDB.DBCopy(oOldDB)) != 0)
				return err;
		}
		oOldDB.DBClose();

		sPathName.Format("%s%s", pDirName, "MMDATA1.DBF");
		if ((err = oOldDB.DBOpen(pPcserve->m_oPcserve, sPathName)) == 0) {
			CWaitCursor wait;
			if ((err = m_oDataDB.DBCopy(oOldDB)) != 0)
				return err;
		}
		oOldDB.DBClose();

		sPathName.Format("%s%s", pDirName, "FOLDER2.DBF");
		pPcserve->m_oPcserve.CFDelete(sPathName);
		sPathName.Format("%s%s", pDirName, "FOLDER2.BIN");
		pPcserve->m_oPcserve.CFDelete(sPathName);
		sPathName.Format("%s%s", pDirName, "MMDATA1.DBF");
		pPcserve->m_oPcserve.CFDelete(sPathName);
		sPathName.Format("%s%s", pDirName, "MMDATA1.BIN");
		pPcserve->m_oPcserve.CFDelete(sPathName);
	}

	return err;
}

BOOL CMMPcsDB::CreateEx(CMMPcserve *pPcserve, CMMPcserve *pCachePcserve, LPCSTR pDirName, int nOpenMode, FOLDERREC *pLibraryRec)
{
	int err;

	if ((err = Create(pPcserve, pCachePcserve, pDirName, nOpenMode, pLibraryRec)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	FlushFolder();
	FlushData();

	return TRUE;
}

void CMMPcsDB::CloseEx()
{
	m_oFolderDB.DBClose();
	m_oDataDB.DBClose();
	m_oFolderDB2.DBClose();
	m_oDataDB2.DBClose();
}

BOOL CMMPcsDB::ReadLibraryEx(FOLDERREC *pLibraryRec) const
{
	*pLibraryRec = m_oLibraryRec;

	return TRUE;
}

BOOL CMMPcsDB::ReadFolderEx(LPCSTR pFID, FOLDERREC *pFolderRec)
{
	int err;

	if ((err = ReadFolder(pFID, pFolderRec)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::ReadChildFolderEx(LPCSTR pFID, FOLDERREC *pFolderRec, BOOL bFirst)
{
	BOOL bRead;
	int err;

	if ((err = ReadChildFolder(pFID, pFolderRec, bFirst, &bRead)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return bRead;
}

BOOL CMMPcsDB::AddFolderEx(FOLDERREC *pFolderRec)
{
	int err;

	if (!CheckPermission(PERMISSION_ADD)) {
		PcserveError(ERROR_PERMISSION);
		return FALSE;
	}

	if ((err = AddFolder(pFolderRec)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	FlushFolder();

	return TRUE;
}

BOOL CMMPcsDB::UpdateFolderEx(FOLDERREC *pFolderRec)
{
	int err;

	if (!CheckPermission(PERMISSION_UPDATE)) {
		PcserveError(ERROR_PERMISSION);
		return FALSE;
	}

	if ((err = UpdateFolder(pFolderRec)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	FlushFolder();

	return TRUE;
}

BOOL CMMPcsDB::DeleteFolderEx(LPCSTR pFID)
{
	int err;

	if (!CheckPermission(PERMISSION_DELETE)) {
		PcserveError(ERROR_PERMISSION);
		return FALSE;
	}

	if ((err = DeleteFolder(pFID)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	FlushFolder();
	FlushData();

	return TRUE;
}

BOOL CMMPcsDB::ReadTextFolderEx(LPCSTR pFID, CString *pText)
{
	int err;

	if ((err = ReadTextFolder(pFID, pText)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::WriteTextFolderEx(LPCSTR pFID, CString *pText)
{
	int err;

	if ((err = WriteTextFolder(pFID, pText)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::MoveFolderEx(LPCSTR pFidSrc, LPCSTR pFidDst, FOLDERREC *pFolderRec)
{
	int err;

	if (!CheckPermission(PERMISSION_UPDATE)) {
		PcserveError(ERROR_PERMISSION);
		return FALSE;
	}

	if ((err = MoveFolder(pFidSrc, pFidDst, pFolderRec)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	FlushFolder();

	return TRUE;
}

BOOL CMMPcsDB::CopyFolderEx(CMMPcsDB *pPcsDBSrc, LPCSTR pFidSrc, CMMPcsDB *pPcsDBDst, LPCSTR pFidDst, FOLDERREC *pFolderRec)
{
	int err;

	if (!pPcsDBDst->CheckPermission(PERMISSION_ADD)) {
		PcserveError(ERROR_PERMISSION);
		return FALSE;
	}

	if ((err = CopyFolder(pPcsDBSrc, pFidSrc, pPcsDBDst, pFidDst, pFolderRec)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	pPcsDBDst->FlushFolder();
	pPcsDBDst->FlushData();

	return TRUE;
}

BOOL CMMPcsDB::RotateFolderEx(LPCSTR pFID, int nRotate)
{
	int err;

	if (!CheckPermission(PERMISSION_UPDATE)) {
		PcserveError(ERROR_PERMISSION);
		return FALSE;
	}

	if ((err = RotateFolder(pFID, nRotate)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	FlushData();

	return TRUE;
}

BOOL CMMPcsDB::ReadDataEx(LPCSTR pDID, DATAREC *pDataRec)
{
	int err;

	if ((err = ReadData(pDID, pDataRec)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::RotateDataEx(LPCSTR pDID, int nRotate)
{
	int err;

	if (!CheckPermission(PERMISSION_UPDATE)) {
		PcserveError(ERROR_PERMISSION);
		return FALSE;
	}

	if ((err = RotateData(pDID, nRotate)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	FlushData();

	return TRUE;
}

BOOL CMMPcsDB::ReadChildDataEx(LPCSTR pFID, DATAREC *pDataRec, BOOL bFirst)
{
	BOOL bRead;
	int err;

	if ((err = ReadChildData(pFID, pDataRec, bFirst, &bRead)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return bRead;
}

BOOL CMMPcsDB::ReadBinaryDataEx(LPCSTR pDID, LPCSTR pFieldName, LPSTR *pPict, long *pPictSize)
{
	int err;

	if ((err = ReadBinaryData(pDID, pFieldName, pPict, pPictSize)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::ReadPictureDataEx(LPCSTR pDID, PHDIB phDIB1, PHDIB phDIB2, DATAREC *pDataRec)
{
	int err;

	if ((err = ReadPictureData(pDID, phDIB1, phDIB2, pDataRec)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::WritePictureDataEx(LPCSTR pDID, HDIB hDIB1, HDIB hDIB2, short nMMType)
{
	int err;

	if ((err = WritePictureData(pDID, hDIB1, hDIB2, nMMType)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	FlushData();

	return TRUE;
}

BOOL CMMPcsDB::ReadTextDataEx(LPCSTR pDID, CString *pText)
{
	int err;

	if ((err = ReadTextData(pDID, pText)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::WriteTextDataEx(LPCSTR pDID, const CString &oText)
{
	int err;

	if ((err = WriteTextData(pDID, &oText)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::AddDataEx(DATAREC *pDataRec)
{
	int err;

	if (!CheckPermission(PERMISSION_ADD)) {
		PcserveError(ERROR_PERMISSION);
		return FALSE;
	}

	if ((err = AddData(pDataRec)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	FlushData();

	return TRUE;
}

BOOL CMMPcsDB::UpdateDataEx(DATAREC *pDataRec)
{
	int err;

	if (!CheckPermission(PERMISSION_UPDATE)) {
		PcserveError(ERROR_PERMISSION);
		return FALSE;
	}

	if ((err = UpdateData(pDataRec)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	FlushData();

	return TRUE;
}

BOOL CMMPcsDB::DeleteDataEx(LPCSTR pDID)
{
	int err;

	if (!CheckPermission(PERMISSION_DELETE)) {
		PcserveError(ERROR_PERMISSION);
		return FALSE;
	}

	if ((err = DeleteData(pDID)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	FlushData();

	return TRUE;
}

BOOL CMMPcsDB::MoveDataEx(LPCSTR pDidSrc, LPCSTR pFidDst, DATAREC *pDataRec)
{
	int err;

	if (!CheckPermission(PERMISSION_UPDATE)) {
		PcserveError(ERROR_PERMISSION);
		return FALSE;
	}

	if ((err = MoveData(pDidSrc, pFidDst, pDataRec)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	FlushData();

	return TRUE;
}

BOOL CMMPcsDB::CopyDataEx(CMMPcsDB *pPcsDBSrc, LPCSTR pDidSrc, CMMPcsDB *pPcsDBDst, LPCSTR pFidDst, DATAREC *pDataRec)
{
	int err;

	if (!pPcsDBDst->CheckPermission(PERMISSION_ADD)) {
		PcserveError(ERROR_PERMISSION);
		return FALSE;
	}

	if ((err = CopyData(pPcsDBSrc, pDidSrc, pPcsDBDst, pFidDst, pDataRec)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	pPcsDBDst->FlushData();

	return TRUE;
}

BOOL CMMPcsDB::FillCacheEx(LPCSTR pFID)
{
	int err;

	if ((err = FillCache(pFID)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::ClearCacheEx()
{
	int err;

	if ((err = ClearCache()) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::FilterFolderEx(LPCSTR pFilter)
{
	int err;

	if ((err = FilterFolder(pFilter)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::GetFolderCountEx(long *pRecNum)
{
	int err;

	if ((err = GetFolderCount(pRecNum)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::ReadFolderAllEx(FOLDERREC *pFolderRec, BOOL *pRead)
{
	int err;

	if ((err = ReadFolderAll(pFolderRec, pRead)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::FilterDataEx(LPCSTR pFilter)
{
	int err;

	if ((err = FilterData(pFilter)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::GetDataCountEx(long *pRecNum)
{
	int err;

	if ((err = GetDataCount(pRecNum)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::ReadDataAllEx(DATAREC *pDataRec, BOOL *pRead)
{
	int err;

	if ((err = ReadDataAll(pDataRec, pRead)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::PackFileEx()
{
	int err;

	if (!CheckPermission(PERMISSION_UPDATE)) {
		PcserveError(ERROR_PERMISSION);
		return FALSE;
	}

	if ((err = PackFile()) != 0) {
		PcserveError(err);
		return FALSE;
	}

	FlushFolder();
	FlushData();

	return TRUE;
}

BOOL CMMPcsDB::BackupEx(LPCSTR pBackupDir, CProgressCtrl *pProgress)
{
	int err;

	if ((err = Backup(pBackupDir, pProgress)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CMMPcsDB::RestoreEx(LPCSTR pBackupDir, CProgressCtrl *pProgress)
{
	int err;

	if (!CheckPermission(PERMISSION_UPDATE)) {
		PcserveError(ERROR_PERMISSION);
		return FALSE;
	}

	if ((err = Restore(pBackupDir, pProgress)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	return TRUE;
}

int CMMPcsDB::Open(CMMPcserve *pPcserve, CMMPcserve *pCachePcserve, LPCSTR pDirName, int nOpenMode, LPCSTR pLibraryID)
{
	CString sCacheDirName;
	int err;

	m_sDirName = pDirName;
	m_nCacheMode = nOpenMode;

	switch (nOpenMode) {
	case CACHEMODE_DIRECT:
		m_pPcserve = pPcserve;
		if ((err = OpenDB(pPcserve, m_oFolderDB, m_oDataDB, pDirName)) != 0)
			return err;

		::AfxGetApp()->DoWaitCursor(0);

		if ((err = ReadFolder(FID_LIBRARY, &m_oLibraryRec)) != 0)
			return err;
		break;
	case CACHEMODE_CACHE:
		m_pPcserve = pPcserve;
		if ((err = OpenDB(pPcserve, m_oFolderDB, m_oDataDB, pDirName)) != 0)
			return err;

		if ((err = ReadFolder(FID_LIBRARY, &m_oLibraryRec)) != 0)
			return err;

		sCacheDirName = g_pMMLibDB->m_sCacheDir + m_sLibraryID + "\\";
		if ((err = OpenDB(pCachePcserve, m_oFolderDB2, m_oDataDB2, sCacheDirName)) != 0) {
			::CreateDirectory(sCacheDirName, NULL);
			if ((err = CreateDB(pCachePcserve, m_oFolderDB2, m_oDataDB2, sCacheDirName)) != 0)
				return err;
		}
		break;
	case CACHEMODE_OFFLINE:
		m_pPcserve = pCachePcserve;
		m_sLibraryID = pLibraryID;
		sCacheDirName = g_pMMLibDB->m_sCacheDir + m_sLibraryID + "\\";
		if ((err = OpenDB(pCachePcserve, m_oFolderDB, m_oDataDB, sCacheDirName)) != 0)
			return err;

		if ((err = ReadFolder(FID_LIBRARY, &m_oLibraryRec)) != 0)
			return err;
		break;
	}

	m_sPassword = m_oLibraryRec.sCodeDef;
	m_dwPermission = m_oLibraryRec.nCodeStep;
	if (m_dwPermission > PERMISSION_FULL || m_dwPermission == 0)
		m_dwPermission = PERMISSION_FULL;

	return 0;
}

int CMMPcsDB::OpenDB(CMMPcserve *pPcserve, CPSDB &oFolderDB, CPSDB &oDataDB, LPCSTR pDirName, BOOL bIndex)
{
	CString sPathName;
	int nRecSize;
	int nIndexNo;
	BOOL bCreateIndex;
	int err;

	if (::GetKeyState(VK_CONTROL) & 0x8000)
		bCreateIndex = TRUE;
	else
		bCreateIndex = FALSE;

	// フォルダ管理
	sPathName.Format("%s%s", pDirName, FN_FOLDER_DBF);
	if ((err = oFolderDB.DBOpen(pPcserve->m_oPcserve, sPathName)) != 0)
		return err;

	if ((err = oFolderDB.DBRecSize(&nRecSize)) != 0)
		return err;

	if (nRecSize - 1 != sizeof(FOLDERBUF)) {
//		VersionError(pPcserve->m_nChType, pPcserve->m_sServerName, pDirName);
		return ERROR_VERSION;
	}

	if (bIndex) {
		sPathName.Format("%s%s", pDirName, FN_FOLDER_NDX1);
		if (bCreateIndex || (err = oFolderDB.DBIndex(sPathName, &nIndexNo)) != 0) {
			if ((err = oFolderDB.DBIdxCreate(sPathName, "FID", 1, &nIndexNo)) != 0)
				return err;
		}

		sPathName.Format("%s%s", pDirName, FN_FOLDER_NDX2);
		if (bCreateIndex || (err = oFolderDB.DBIndex(sPathName, &nIndexNo)) != 0) {
			if ((err = oFolderDB.DBIdxCreate(sPathName, "PFID", 0, &nIndexNo)) != 0)
				return err;
		}
	}

	// データファイル管理
	sPathName.Format("%s%s", pDirName, FN_DATA_DBF);
	if ((err = oDataDB.DBOpen(pPcserve->m_oPcserve, sPathName)) != 0)
			return err;

	if ((err = oDataDB.DBRecSize(&nRecSize)) != 0)
		return err;

	if (nRecSize - 1 != sizeof(DATABUF)) {
//		VersionError(pPcserve->m_nChType, pPcserve->m_sServerName, pDirName);
		return ERROR_VERSION;
	}

	if (bIndex) {
		sPathName.Format("%s%s", pDirName, FN_DATA_NDX1);
		if (bCreateIndex || (err = oDataDB.DBIndex(sPathName, &nIndexNo)) != 0) {
			if ((err = oDataDB.DBIdxCreate(sPathName, "FID", 0, &nIndexNo)) != 0)
				return err;
		}

		sPathName.Format("%s%s", pDirName, FN_DATA_NDX2);
		if (bCreateIndex || (err = oDataDB.DBIndex(sPathName, &nIndexNo)) != 0) {
			if ((err = oDataDB.DBIdxCreate(sPathName, "DID", 1, &nIndexNo)) != 0)
				return err;
		}
	}

	return 0;
}

int CMMPcsDB::Create(CMMPcserve *pPcserve, CMMPcserve *pCachePcserve, LPCSTR pDirName, int nOpenMode, FOLDERREC *pLibraryRec)
{
	CString sCacheDirName;
	int err;

	m_pPcserve = pPcserve;
	m_sDirName = pDirName;
	m_nCacheMode = nOpenMode;

	MakeLibraryID(m_sLibraryID);

	switch (nOpenMode) {
	case CACHEMODE_DIRECT:
		if ((err = CreateDB(pPcserve, m_oFolderDB, m_oDataDB, pDirName)) != 0)
			return err;
		break;
	case CACHEMODE_CACHE:
		if ((err = CreateDB(pPcserve, m_oFolderDB, m_oDataDB, pDirName)) != 0)
			return err;

		sCacheDirName = g_pMMLibDB->m_sCacheDir + m_sLibraryID + "\\";
		::CreateDirectory(sCacheDirName, NULL);
		if ((err = CreateDB(pCachePcserve, m_oFolderDB2, m_oDataDB2, sCacheDirName)) != 0)
			return err;
		break;
	case CACHEMODE_OFFLINE:
		return ERROR_PROGRAM;
	}

	pLibraryRec->sParentFolderID = m_sLibraryID;
	pLibraryRec->nCodeStep = PERMISSION_FULL;

	if ((err = AddFolder(pLibraryRec)) != 0) {
		PcserveError(err);
		return FALSE;
	}

	m_oLibraryRec = *pLibraryRec;
	m_dwPermission = m_oLibraryRec.nCodeStep;

	return 0;
}

int CMMPcsDB::CreateDB(CMMPcserve *pPcserve, CPSDB &oFolderDB, CPSDB &oDataDB, LPCSTR pDirName, BOOL bIndex)
{
	static DBF_I   folderField[] = {
		{"FID",       'C', LEN_ID,         0},	// フォルダＩＤ
		{"PFID",      'C', LEN_ID,         0},	// 親フォルダＩＤ
		{"FTYPE",     'C', LEN_FOLDERTYPE, 0},	// フォルダタイプ
		{"FNAME",     'C', LEN_FOLDERNAME, 0},	// フォルダ名
		{"CDATE",     'C', LEN_DATE,       0},	// 作成日付
		{"UDATE",     'C', LEN_DATE,       0},	// 更新日付
		{"CREATOR",   'C', LEN_CREATOR,    0},	// 作成者
		{"REMARK1",   'C', LEN_REMARK,     0},	// 備考
		{"REMARK2",   'C', LEN_KEYWORD,    0},	// キーワード
		{"UCOUNT",    'I', LEN_UCOUNT,     0},	// 更新カウンタ
		{"CODEDEF",   'C', LEN_CODE,       0},	// デフォルトデータコード
		{"CODESTEP",  'I', LEN_CODESTEP,   0},	// コード更新ステップ
		{FORM_FIELD,  'B', 0, 0}				// フォームデータ
	};

	static DBF_I   dataField[] = {
		{"FID",       'C', LEN_ID,      0},		// フォルダＩＤ
		{"DID",       'C', LEN_ID,      0},		// 画像ＩＤ
		{"CDATE",     'C', LEN_DATE,    0},		// 撮影年月日時分
		{"UDATE",     'C', LEN_DATE,    0},		// 更新年月日時分
		{"CREATOR",   'C', LEN_CREATOR, 0},		// 作成者
		{"CODE",      'C', LEN_CODE,    0},		// コード
		{"TITLE",     'C', LEN_TITLE,   0},		// 画像タイトル
		{"REMARK1",   'C', LEN_REMARK,  0},		// 備考
		{"REMARK2",   'C', LEN_KEYWORD, 0},		// キーワード
		{"DPI",       'I', LEN_DPI,     0},		// DPI
		{"WIDTH",     'I', LEN_WIDTH,   0},		// 画像の幅
		{"HEIGHT",    'I', LEN_HEIGHT,  0},		// 画像の高さ
		{"BIT",       'I', LEN_BIT,     0},		// 画像のビット数
		{"UCOUNT",    'I', LEN_UCOUNT,  0},		// 更新カウンタ
		{PICT1_FIELD, 'B', 0, 0},				// 画像データ１
		{PICT2_FIELD, 'B', 0, 0},				// 画像データ２（縮小）
		{TEXT_FIELD,  'B', 0, 0},				// テキストデータ
		{SOUND_FIELD, 'B', 0, 0}				// 音声データ
	};
	CString sPathName;
	int nIndexNo;
	int err;

	// フォルダ管理
	sPathName.Format("%s%s", pDirName, FN_FOLDER_DBF);
	if ((err = oFolderDB.DBCreate(pPcserve->m_oPcserve, sPathName, folderField, ARRAY_SIZE(folderField))) != 0)
			return err;

	if (bIndex) {
		sPathName.Format("%s%s", pDirName, FN_FOLDER_NDX1);
		if ((err = oFolderDB.DBIdxCreate(sPathName, "FID", 1, &nIndexNo)) != 0)
			return err;

		sPathName.Format("%s%s", pDirName, FN_FOLDER_NDX2);
		if ((err = oFolderDB.DBIdxCreate(sPathName, "PFID", 0, &nIndexNo)) != 0)
			return err;
	}

	// データファイル管理
	sPathName.Format("%s%s", pDirName, FN_DATA_DBF);
	if ((err = oDataDB.DBCreate(pPcserve->m_oPcserve, sPathName, dataField, ARRAY_SIZE(dataField))) != 0)
			return err;

	if (bIndex) {
		sPathName.Format("%s%s", pDirName, FN_DATA_NDX1);
		if ((err = oDataDB.DBIdxCreate(sPathName, "FID", 0, &nIndexNo)) != 0)
			return err;

		sPathName.Format("%s%s", pDirName, FN_DATA_NDX2);
		if ((err = oDataDB.DBIdxCreate(sPathName, "DID", 1, &nIndexNo)) != 0)
			return err;
	}

	return 0;
}

int CMMPcsDB::ReadFolder(LPCSTR pFID, FOLDERREC *pFolderRec)
{
	int err;

	if ((err = SetFolderIndex(IDX_FOLDER_FID)) != 0)
		return err;

	FOLDERBUF oFolderBuf;
	int nFind;
	if ((err = m_oFolderDB.DBReadKey(pFID, LEN_ID, &oFolderBuf, 0, &nFind)) != 0)
		return err;

	if (nFind == 0)
		return ERROR_PROGRAM;

	m_oFolderDB.DBGetFieldBuf(&oFolderBuf, pFolderRec);

	if (pFolderRec->sFolderID == FID_LIBRARY) {
		if (pFolderRec->sParentFolderID.IsEmpty()) {
			MakeLibraryID(pFolderRec->sParentFolderID);
			m_oFolderDB.DBSetFieldBuf(&oFolderBuf, pFolderRec);
			m_oFolderDB.DBUpdateKey(pFID, LEN_ID, &oFolderBuf);
		}

		m_sTitle = pFolderRec->sFolderName;
		m_sLibraryID = pFolderRec->sParentFolderID;
	}

	return 0;
}

int CMMPcsDB::ReadChildFolder(LPCSTR pFID, FOLDERREC *pFolderRec, BOOL bFirst, BOOL *pRead)
{
	CPSDB *pFolderDB;
	int nRead;
	int err;

	if (m_nCacheMode == CACHEMODE_CACHE)
		pFolderDB = &m_oFolderDB2;
	else
		pFolderDB = &m_oFolderDB;

	if (bFirst) {
		if (m_nCacheMode == CACHEMODE_CACHE)
			CacheChildFolder(pFID);

		if ((err = SetFolderIndex(IDX_FOLDER_PFID)) != 0)
			return err;

		if ((err = pFolderDB->DBSearch(pFID, LEN_ID)) != 0)
			return err;
	}

	FOLDERBUF oFolderBuf;
	if ((err = pFolderDB->DBReadNext(1, &oFolderBuf, &nRead)) != 0)
		return err;

	if (nRead == 0 || memcmp(pFID, oFolderBuf.parentFolderID, LEN_ID) != 0)
		*pRead = FALSE;
	else {
		oFolderBuf.folderType[0] = (char)toupper(oFolderBuf.folderType[0]);
		pFolderDB->DBGetFieldBuf(&oFolderBuf, pFolderRec);
		*pRead = TRUE;
	}

	return 0;
}

int CMMPcsDB::CacheFolder(LPCSTR pFID, LPBOOL pCachedChildren, LPSTR pFolderType)
{
	int nFind;
	int err;

	if ((err = SetFolderIndex(IDX_FOLDER_FID)) != 0)
		return err;

	FOLDERBUF folderBuf1;
	if ((err = m_oFolderDB.DBReadKey(pFID, LEN_ID, &folderBuf1, 0, &nFind)) != 0)
		return err;

	if (nFind == 0)
		return ERROR_PROGRAM;

	BOOL bCachedChildren = FALSE;
	FOLDERBUF folderBuf2;
	if (m_oFolderDB2.DBReadKey(pFID, LEN_ID, &folderBuf2, 0, &nFind) == 0 && nFind == 1) {
		if (isupper(folderBuf2.folderType[0]) && memcmp(folderBuf1.updateCount, folderBuf2.updateCount, LEN_UCOUNT) == 0)
			bCachedChildren = TRUE;
		else {
			if ((err = m_oFolderDB2.DBUpdate(&folderBuf1)) != 0)
				return err;
		}
	} else {
		if ((err = m_oFolderDB2.DBStoreUniq(&folderBuf1)) != 0)
			return err;
	}

	if (pCachedChildren != NULL)
		*pCachedChildren = bCachedChildren;

	if (pFolderType != NULL)
		*pFolderType = folderBuf1.folderType[0];

	return 0;
}

int CMMPcsDB::CacheChildFolder(LPCSTR pFID)
{
	BOOL bCachedChildren;
	int nFind;
	BOOL bReadFlag;
	long nRec;
	int err;

	if ((err = CacheFolder(pFID, &bCachedChildren, NULL)) != 0)
		return err;

	if (bCachedChildren)
		return 0;

	if ((err = SetFolderIndex(IDX_FOLDER_FID)) != 0)
		return err;

	if ((err = SelectFolder(pFID, &nRec)) != 0)
		return err;

	FOLDERBUF folderBuf1, folderBuf2;
	while (--nRec >= 0) {
		if ((err = m_oFolderDB.DBReadAll(&folderBuf1, &bReadFlag)) != 0)
			break;

		if (!bReadFlag)
			break;

		folderBuf1.folderType[0] = (char)tolower(folderBuf1.folderType[0]);

		if (m_oFolderDB2.DBReadKey(folderBuf1.folderID, LEN_ID, &folderBuf2, 0, &nFind) == 0 && nFind == 1) {
			if (memcmp(folderBuf1.updateCount, folderBuf2.updateCount, LEN_UCOUNT) != 0) {
				if ((err = m_oFolderDB2.DBUpdate(&folderBuf1)) != 0)
					return err;
			}
		} else {
			if ((err = m_oFolderDB2.DBStoreUniq(&folderBuf1)) != 0)
				return err;
		}
	}

	if ((err = SelectFolder(NULL, &nRec)) != 0)
		return err;

	return 0;
}

int CMMPcsDB::AddFolder(FOLDERREC *pFolderRec)
{
	int err;

	if ((err = SetFolderIndex(IDX_FOLDER_FID)) != 0)
		return err;

	if (pFolderRec->sFolderType == FTYPE_LIBRARY) {
		pFolderRec->sParentFolderID = m_sLibraryID;

		m_sTitle = pFolderRec->sFolderName;
		m_sLibraryID = pFolderRec->sParentFolderID;
		pFolderRec->sFolderID = FID_LIBRARY;
	} else {
		if ((err = MakeFolderID(pFolderRec->sFolderID)) != 0)
			return err;
	}

	if (pFolderRec->sCreateDate.IsEmpty())
		SetCurrentTime(pFolderRec->sCreateDate);
	if (pFolderRec->sUpdateDate.IsEmpty())
		pFolderRec->sUpdateDate = pFolderRec->sCreateDate;
	pFolderRec->nUpdateCount = 0;

	FOLDERBUF oFolderBuf;
	m_oFolderDB.DBSetFieldBuf(&oFolderBuf, pFolderRec);

	if ((err = m_oFolderDB.DBStoreUniq(&oFolderBuf)) != 0)
		return err;

	if (m_nCacheMode == CACHEMODE_CACHE) {
		m_oFolderDB2.DBDeleteKey(oFolderBuf.folderID, LEN_ID, 1);
		if ((err = m_oFolderDB2.DBStoreUniq(&oFolderBuf)) != 0)
			return err;
	}

	return 0;
}

int CMMPcsDB::UpdateFolder(FOLDERREC *pFolderRec)
{
	int err;

	if ((err = SetFolderIndex(IDX_FOLDER_FID)) != 0)
		return err;

	if (pFolderRec->sFolderType == FTYPE_LIBRARY) {
		m_sTitle = pFolderRec->sFolderName;
		m_sLibraryID = pFolderRec->sParentFolderID;
	}

	SetCurrentTime(pFolderRec->sUpdateDate);
	(pFolderRec->nUpdateCount)++;

	FOLDERBUF oFolderBuf;
	m_oFolderDB.DBSetFieldBuf(&oFolderBuf, pFolderRec);

	if ((err = m_oFolderDB.DBUpdateKey(pFolderRec->sFolderID, LEN_ID, &oFolderBuf)) != 0)
		return err;

	if (m_nCacheMode == CACHEMODE_CACHE) {
		m_oFolderDB2.DBDeleteKey(oFolderBuf.folderID, LEN_ID, 1);
		if ((err = m_oFolderDB2.DBStoreUniq(&oFolderBuf)) != 0)
			return err;
	}

//	UpdateFolderUpdateCount(oFolderBuf.parentFolderID);

	return 0;
}

int CMMPcsDB::DeleteFolder(LPCSTR pFID)
{
	FOLDERBUF oFolderBuf;
	int nFind;
	int err;

	for (;;) {
		if ((err = SetFolderIndex(IDX_FOLDER_PFID)) != 0)
			return err;

		if ((err = m_oFolderDB.DBSearch2(pFID, LEN_ID, &nFind)) != 0)
			return err;

		if (nFind != 1)
			break;

		if ((err = m_oFolderDB.DBRead(&oFolderBuf)) != 0)
			return err;

		if ((err = DeleteFolder(oFolderBuf.folderID)) != 0)
			return err;
	}

	if ((err = SetFolderIndex(IDX_FOLDER_FID)) != 0)
		return err;

	if ((err = m_oFolderDB.DBDeleteKey(pFID, LEN_ID, 1)) != 0)
		return err;

	if (m_nCacheMode == CACHEMODE_CACHE) {
		m_oFolderDB2.DBDeleteKey(pFID, LEN_ID, 1);
	}

	if ((err = SetDataIndex(IDX_DATA_FID)) != 0)
		return err;

	if ((err = m_oDataDB.DBDeleteKey(pFID, LEN_ID, 1)) != 0)
		return err;

	if (m_nCacheMode == CACHEMODE_CACHE) {
		m_oFolderDB2.DBDeleteKey(pFID, LEN_ID, 1);
	}

	return 0;
}

int CMMPcsDB::ReadTextFolder(LPCSTR pFID, CString *pText)
{
	CPSDB *pFolderDB;
	int nFind;
	long nSize;
	int err;

	pText->Empty();

	if ((err = SetFolderIndex(IDX_FOLDER_FID)) != 0)
		return err;

	if (m_nCacheMode == CACHEMODE_CACHE)
		pFolderDB = &m_oFolderDB2;
	else
		pFolderDB = &m_oFolderDB;

	if ((err = pFolderDB->DBSearch2(pFID, LEN_ID, &nFind)) != 0)
		return err;

	if (nFind != 1) {
		if (m_nCacheMode == CACHEMODE_CACHE) {
			FOLDERBUF oFolderBuf;
			if ((err = m_oFolderDB.DBReadKey(pFID, LEN_ID, &oFolderBuf, 0, &nFind)) != 0)
				return err;
			if (nFind == 1)
				m_oFolderDB2.DBStoreUniq(&oFolderBuf);
			else
				return ERROR_PROGRAM;
		} else
			return ERROR_PROGRAM;
	}

	if (m_nCacheMode == CACHEMODE_CACHE) {
		if (pText != NULL)
			CacheBinFldFolder(pFID, FORM_FIELD);
	}

	if (pText != NULL) {
		if (pFolderDB->DBGetBinarySize(FORM_FIELD, &nSize) == 0 && nSize != 0) {
			pFolderDB->DBReadBinary(FORM_FIELD, pText->GetBuffer(nSize), nSize, &nSize);
			pText->ReleaseBuffer(nSize);
		}
	}

	return 0;
}

int CMMPcsDB::WriteTextFolder(LPCSTR pFID, CString *pText)
{
	CPSDB *pFolderDB;
	int nFind;
	int err;

	if (m_nCacheMode == CACHEMODE_CACHE)
		pFolderDB = &m_oFolderDB2;
	else
		pFolderDB = &m_oFolderDB;

	if ((err = SetFolderIndex(IDX_FOLDER_FID)) != 0)
		return err;

	if ((err = pFolderDB->DBSearch2(pFID, LEN_ID, &nFind)) != 0)
		return err;

	if (nFind != 1)
		return ERROR_PROGRAM;

	if (pText != NULL && pText->GetLength() != 0) {
		if ((err = pFolderDB->DBWriteBinary(FORM_FIELD, (LPVOID)(LPCSTR)*pText, pText->GetLength())) != 0)
			return err;
	} else {
		if ((err = pFolderDB->DBWriteBinary(FORM_FIELD, NULL, 0)) != 0)
			return err;
	}

	if (m_nCacheMode == CACHEMODE_CACHE)
		WriteTextCacheFolder(pFID);

	return 0;
}

int CMMPcsDB::CacheBinFldFolder(LPCSTR pFID, LPCSTR pFieldName)
{
	long nSize;
	int err;

	if (m_oFolderDB2.DBGetBinarySize(pFieldName, &nSize) == 0 && nSize == 0) {
		int nFind;
		if (m_oFolderDB.DBSearch2(pFID, LEN_ID, &nFind) == 0 && nFind == 1) {
			if ((err = CacheBinaryFolder(pFieldName)) != 0)
				return err;
		}
	}

	return 0;
}

int CMMPcsDB::MoveFolder(LPCSTR pFidSrc, LPCSTR pFidDst, FOLDERREC *pFolderRec)
{
	FOLDERREC oFolderRec;
	int err;

	if ((err = ReadFolder(pFidSrc, &oFolderRec)) != 0)
		return err;

	oFolderRec.sParentFolderID = pFidDst;

	if ((err = UpdateFolder(&oFolderRec)) != 0)
		return err;

	if (pFolderRec != NULL)
		*pFolderRec = oFolderRec;

	return 0;
}

int CMMPcsDB::CopyFolder(CMMPcsDB *pPcsDBSrc, LPCSTR pFidSrc, CMMPcsDB *pPcsDBDst, LPCSTR pFidDst, FOLDERREC *pFolderRec)
{
	FOLDERREC oFolderRec;
	char (*pFID)[LEN_ID];
	long nRec;
	int nRead;
	int nLen;
	int i;
	int err;

	if ((err = pPcsDBSrc->ReadFolder(pFidSrc, &oFolderRec)) != 0)
		return err;

	oFolderRec.sParentFolderID = pFidDst;

	if ((err = pPcsDBDst->AddFolder(&oFolderRec)) != 0)
		return err;

	if ((err = CopyBinary(pPcsDBSrc->m_oFolderDB, pPcsDBDst->m_oFolderDB, FORM_FIELD)) != 0)
		return err;

	switch (oFolderRec.sFolderType[0]) {
	case FTYPE_GFOLDER:
		if ((err = pPcsDBSrc->SelectFolder(pFidSrc, &nRec)) != 0)
			return err;

		if ((err = pPcsDBSrc->m_oFolderDB.DBSetReadField("FID", &nLen)) != 0)
			return err;

		pFID = new char[nRec][LEN_ID];
		if ((err = pPcsDBSrc->m_oFolderDB.DBReadNext(nRec, pFID, &nRead)) != 0) {
			delete [] pFID;
			return err;
		}

		if ((err = pPcsDBSrc->SelectFolder(NULL, &nRec)) != 0) {
			delete [] pFID;
			return err;
		}

		if ((err = pPcsDBSrc->m_oFolderDB.DBSetReadField(NULL, &nLen)) != 0) {
			delete [] pFID;
			return err;
		}

		for (i = 0; i < nRead; i++) {
			if ((err = CopyFolder(pPcsDBSrc, pFID[i], pPcsDBDst, oFolderRec.sFolderID)) != 0) {
				delete [] pFID;
				return err;
			}
		}

		delete [] pFID;
		break;
	case FTYPE_DFOLDER:
		if ((err = CopyChildData(pPcsDBSrc, pFidSrc, pPcsDBDst, oFolderRec.sFolderID)) != 0)
			return err;
		break;
	}

	if (pFolderRec != NULL)
		*pFolderRec = oFolderRec;

	return 0;
}

int CMMPcsDB::RotateFolder(LPCSTR pFID, int nRotate)
{
	FOLDERREC oFolderRec;
	char (*pID)[LEN_ID];
	long nRec;
	int nRead;
	int nLen;
	int i;
	int err;

	if ((err = ReadFolder(pFID, &oFolderRec)) != 0)
		return err;

	switch (oFolderRec.sFolderType[0]) {
	case FTYPE_LIBRARY:
	case FTYPE_GFOLDER:
		if ((err = SelectFolder(pFID, &nRec)) != 0)
			return err;

		if ((err = m_oFolderDB.DBSetReadField("FID", &nLen)) != 0)
			return err;

		pID = new char[nRec][LEN_ID];
		if ((err = m_oFolderDB.DBReadNext(nRec, pID, &nRead)) != 0) {
			delete [] pID;
			return err;
		}

		if ((err = SelectFolder(NULL, &nRec)) != 0) {
			delete [] pID;
			return err;
		}

		if ((err = m_oFolderDB.DBSetReadField(NULL, &nLen)) != 0) {
			delete [] pID;
			return err;
		}

		for (i = 0; i < nRead; i++) {
			if ((err = RotateFolder(pID[i], nRotate)) != 0) {
				delete [] pID;
				return err;
			}
		}

		delete [] pID;
		break;
	case FTYPE_DFOLDER:
		if ((err = SelectData(pFID, &nRec)) != 0)
			return err;

		if ((err = m_oDataDB.DBSetReadField("DID", &nLen)) != 0)
			return err;

		pID = new char[nRec][LEN_ID];
		if ((err = m_oDataDB.DBReadNext(nRec, pID, &nRead)) != 0) {
			delete [] pID;
			return err;
		}

		if ((err = SelectData(NULL, &nRec)) != 0) {
			delete [] pID;
			return err;
		}

		if ((err = m_oDataDB.DBSetReadField(NULL, &nLen)) != 0) {
			delete [] pID;
			return err;
		}

		for (i = 0; i < nRead; i++) {
			if ((err = RotateData(pID[i], nRotate)) != 0) {
				delete [] pID;
				return err;
			}
		}

		delete [] pID;
		break;
	}

	return 0;
}

int CMMPcsDB::RotateData(LPCSTR pDID, int nRotate)
{
	HDIB hDIB;
	DATAREC oDataRec;
	short nWidth, nHeight, nColor;
	int err;

	if ((err = ReadPictureData(pDID, &hDIB, NULL, &oDataRec)) != 0)
		return err;

	if ((err = MMPictureRotate(hDIB, (SHORT)nRotate)) != 0) {
		MMPictureFree(hDIB);
		return err;
	}

	MMPictureGetInfo(hDIB, &nWidth, &nHeight, &nColor);
	oDataRec.nWidth = nWidth;
	oDataRec.nHeight = nHeight;

	if ((err = UpdateData(&oDataRec)) != 0) {
		MMPictureFree(hDIB);
		return err;
	}

	HDIB hDIB2 = MakeSmallPicture(hDIB, PICT_WIDTH, PICT_HEIGHT);

	WritePictureData(pDID, hDIB, hDIB2, GetMMType(hDIB));

	MMPictureFree(hDIB);
	MMPictureFree(hDIB2);

	return 0;
}

int CMMPcsDB::ReadData(LPCSTR pDID, DATAREC *pDataRec)
{
	int nFind;
	int err;

	if ((err = SetDataIndex(IDX_DATA_DID)) != 0)
		return err;

	DATABUF oDataBuf;
	if ((err = m_oDataDB.DBReadKey(pDID, LEN_ID, &oDataBuf, 0, &nFind)) != 0)
		return err;

	m_oDataDB.DBGetFieldBuf(&oDataBuf, pDataRec);

	return 0;
}

int CMMPcsDB::ReadChildData(LPCSTR pFID, DATAREC *pDataRec, BOOL bFirst, BOOL *pRead)
{
	CPSDB *pDataDB;
	int nRead;
	int err;

	if (m_nCacheMode == CACHEMODE_CACHE)
		pDataDB = &m_oDataDB2;
	else
		pDataDB = &m_oDataDB;

	if (bFirst) {
		if (m_nCacheMode == CACHEMODE_CACHE)
			CacheChildData(pFID);

		if ((err = SetDataIndex(IDX_DATA_FID)) != 0)
			return err;

		if ((err = pDataDB->DBSearch(pFID, LEN_ID)) != 0)
			return err;
	}

	DATABUF oDataBuf;
	if ((err = pDataDB->DBReadNext(1, &oDataBuf, &nRead)) != 0)
		return err;

	if (nRead == 0 || strncmp(pFID, oDataBuf.folderID, LEN_ID) != 0)
		*pRead = FALSE;
	else {
		pDataDB->DBGetFieldBuf(&oDataBuf, pDataRec);
		*pRead = TRUE;
	}

	return 0;
}

int CMMPcsDB::ReadBinaryData(LPCSTR pDID, LPCSTR pFieldName, LPSTR *pPict, long *pPictSize)
{
	CPSDB *pDataDB;
	int nFind;
	long nSize;
	int err;

	if ((err = SetDataIndex(IDX_DATA_DID)) != 0)
		return err;

	if (m_nCacheMode == CACHEMODE_CACHE)
		pDataDB = &m_oDataDB2;
	else
		pDataDB = &m_oDataDB;

	if ((err = pDataDB->DBSearch2(pDID, LEN_ID, &nFind)) != 0)
		return err;

	if (nFind != 1)
		return ERROR_PROGRAM;

	if (m_nCacheMode == CACHEMODE_CACHE)
		CacheBinFldData(pDID, pFieldName);

	if ((err = pDataDB->DBGetBinarySize(pFieldName, &nSize)) != 0)
		return err;

	if (nSize != 0) {
		*pPict = new char[nSize];
		if ((err = pDataDB->DBReadBinary(pFieldName, *pPict, nSize, pPictSize)) != 0) {
			delete [] pPict;
			return err;
		}
	} else {
		*pPict = NULL;
		*pPictSize = 0;
	}

	return 0;
}

int CMMPcsDB::CacheChildData(LPCSTR pFID)
{
	BOOL bCachedChildren;
	BOOL bReadFlag;
	int nFind;
	long nRec;
	int err;

	if ((err = CacheFolder(pFID, &bCachedChildren, NULL)) != 0)
		return err;

	if (bCachedChildren)
		return 0;

	if ((err = SetDataIndex(IDX_DATA_DID)) != 0)
		return err;

	if ((err = SelectData(pFID, &nRec)) != 0)
		return err;

	DATABUF oDataBuf1, oDataBuf2;
	while (--nRec >= 0) {
		if ((err = m_oDataDB.DBReadAll(&oDataBuf1, &bReadFlag)) != 0)
			break;

		if (!bReadFlag)
			break;

		if (m_oDataDB2.DBReadKey(oDataBuf1.dataID, LEN_ID, &oDataBuf2, 0, &nFind) == 0 && nFind == 1) {
			if (memcmp(oDataBuf1.updateCount, oDataBuf2.updateCount, LEN_UCOUNT) != 0) {
				if ((err = m_oDataDB2.DBUpdate(&oDataBuf1)) != 0)
					return err;

				m_oDataDB2.DBWriteBinary(PICT1_FIELD, NULL, 0);
				m_oDataDB2.DBWriteBinary(PICT2_FIELD, NULL, 0);
			}
		} else {
			if ((err = m_oDataDB2.DBStoreUniq(&oDataBuf1)) != 0)
				return err;
		}
	}

	if ((err = SelectData(NULL, &nRec)) != 0)
		return err;

	return 0;
}

int CMMPcsDB::CacheBinaryFolder(LPCSTR pFieldName)
{
	return CacheBinary(pFieldName, m_oFolderDB, m_oFolderDB2);
}

int CMMPcsDB::CacheBinaryData(LPCSTR pFieldName)
{
	return CacheBinary(pFieldName, m_oDataDB, m_oDataDB2);
}

int CMMPcsDB::CacheBinary(LPCSTR pFieldName, CPSDB &oDB, CPSDB &oDB2)
{
	long nSize;
	long nReadNum;
	int err;

	if ((err = oDB.DBGetBinarySize(pFieldName, &nSize)) != 0)
		return err;

	char *pBuf = new char[nSize];

	if ((err = oDB.DBReadBinary(pFieldName, pBuf, nSize, &nReadNum)) != 0) {
		delete [] pBuf;
		return err;
	}

	if ((err = oDB2.DBWriteBinary(pFieldName, pBuf, nReadNum)) != 0) {
		delete [] pBuf;
		return err;
	}

	delete [] pBuf;

	return 0;
}

int CMMPcsDB::ReadPictureData(LPCSTR pDID, PHDIB phDIB1, PHDIB phDIB2, DATAREC *pDataRec)
{
	CPSDB *pDataDB;
	DATABUF oDataBuf;
	DATAREC oDataRec;
	int nFind;
	int err;

	if ((err = SetDataIndex(IDX_DATA_DID)) != 0)
		return err;

	if (m_nCacheMode == CACHEMODE_CACHE)
		pDataDB = &m_oDataDB2;
	else
		pDataDB = &m_oDataDB;

	if ((err = pDataDB->DBReadKey(pDID, LEN_ID, &oDataBuf, 0, &nFind)) != 0)
		return err;

	if (nFind != 1) {
		if (m_nCacheMode == CACHEMODE_CACHE) {
			if ((err = m_oDataDB.DBReadKey(pDID, LEN_ID, &oDataBuf, 0, &nFind)) != 0)
				return err;
			if (nFind == 1)
				m_oDataDB2.DBStoreUniq(&oDataBuf);
			else
				return ERROR_PROGRAM;
		} else
			return ERROR_PROGRAM;
	}

	if (m_nCacheMode == CACHEMODE_CACHE) {
		if (phDIB1 != NULL)
			CacheBinFldData(pDID, PICT1_FIELD);
		if (phDIB2 != NULL)
			CacheBinFldData(pDID, PICT2_FIELD);
	}

	if (pDataRec == NULL)
		pDataRec = &oDataRec;

	pDataDB->DBGetFieldBuf(&oDataBuf, pDataRec);

	if (phDIB1 != NULL) {
		if (MMPictureReadDB(phDIB1, pDataDB->GetFCB(), PICT1_FIELD) != 0)
			*phDIB1 = NULL;
		else {
			if (pDataRec->nDpi > 0 && pDataRec->nDpi < 10000) {
				LPBITMAPINFO lpBmi;
				lpBmi = (LPBITMAPINFO)::GlobalLock(*phDIB1);
				if (lpBmi != NULL && lpBmi->bmiHeader.biXPelsPerMeter == 0)
					lpBmi->bmiHeader.biYPelsPerMeter = lpBmi->bmiHeader.biXPelsPerMeter = (long)(pDataRec->nDpi / 0.0254 + 0.5);
				::GlobalUnlock(*phDIB1);
			}
		}
	}

	if (phDIB2 != NULL) {
		if (MMPictureReadDB(phDIB2, pDataDB->GetFCB(), PICT2_FIELD) != 0)
			*phDIB2 = NULL;
	}

	return 0;
}

int CMMPcsDB::CacheBinFldData(LPCSTR pDID, LPCSTR pFieldName)
{
	long nSize;
	int nFind;
	int err;

	if (m_oDataDB2.DBGetBinarySize(pFieldName, &nSize) == 0 && nSize == 0) {
		if (m_oDataDB.DBSearch2(pDID, LEN_ID, &nFind) == 0 && nFind == 1) {
			if ((err = CacheBinaryData(pFieldName)) != 0)
				return err;
		}
	}

	return 0;
}

int CMMPcsDB::WritePictureData(LPCSTR pDID, HDIB hDIB1, HDIB hDIB2, short nMMType)
{
	CPSDB *pDataDB;
	int nFind;
	int err;

	if (m_nCacheMode == CACHEMODE_CACHE)
		pDataDB = &m_oDataDB2;
	else
		pDataDB = &m_oDataDB;

	if ((err = SetDataIndex(IDX_DATA_DID)) != 0)
		return err;

	if ((err = pDataDB->DBSearch2(pDID, LEN_ID, &nFind)) != 0)
		return err;

	if (nFind != 1)
		return ERROR_PROGRAM;

	if (hDIB1 != NULL) {
		if ((err = MMPictureWriteDB(hDIB1, pDataDB->GetFCB(), PICT1_FIELD, nMMType)) != 0)
			return err;
	} else {
		if ((err = pDataDB->DBWriteBinary(PICT1_FIELD, NULL, 0)) != 0)
			return err;
	}

	if (hDIB2 != NULL) {
		if ((err = MMPictureWriteDB(hDIB2, pDataDB->GetFCB(), PICT2_FIELD, nMMType)) != 0)
			return err;
	} else {
		if ((err = pDataDB->DBWriteBinary(PICT2_FIELD, NULL, 0)) != 0)
			return err;
	}

	if (m_nCacheMode == CACHEMODE_CACHE)
		WritePictureCacheData(pDID);

	UpdateFindPictDB(pDID, hDIB1);

	return 0;
}

int CMMPcsDB::ReadTextData(LPCSTR pDID, CString *pText)
{
	CPSDB *pDataDB;
	int nFind;
	long nSize;
	int err;

	pText->Empty();

	if ((err = SetDataIndex(IDX_DATA_DID)) != 0)
		return err;

	if (m_nCacheMode == CACHEMODE_CACHE)
		pDataDB = &m_oDataDB2;
	else
		pDataDB = &m_oDataDB;

	if ((err = pDataDB->DBSearch2(pDID, LEN_ID, &nFind)) != 0)
		return err;

	if (nFind != 1) {
		if (m_nCacheMode == CACHEMODE_CACHE) {
			DATABUF oDataBuf;
			if ((err = m_oDataDB.DBReadKey(pDID, LEN_ID, &oDataBuf, 0, &nFind)) != 0)
				return err;
			if (nFind == 1)
				m_oDataDB2.DBStoreUniq(&oDataBuf);
			else
				return ERROR_PROGRAM;
		} else
			return ERROR_PROGRAM;
	}

	if (m_nCacheMode == CACHEMODE_CACHE) {
		if (pText != NULL)
			CacheBinFldData(pDID, TEXT_FIELD);
	}

	if (pText != NULL) {
		if (pDataDB->DBGetBinarySize(TEXT_FIELD, &nSize) == 0 && nSize != 0) {
			pDataDB->DBReadBinary(TEXT_FIELD, pText->GetBuffer(nSize), nSize, &nSize);
			pText->ReleaseBuffer(nSize);
		}
	}

	return 0;
}

int CMMPcsDB::WriteTextData(LPCSTR pDID, const CString *pText)
{
	CPSDB *pDataDB;
	int nFind;
	int err;

	if (m_nCacheMode == CACHEMODE_CACHE)
		pDataDB = &m_oDataDB2;
	else
		pDataDB = &m_oDataDB;

	if ((err = SetDataIndex(IDX_DATA_DID)) != 0)
		return err;

	if ((err = pDataDB->DBSearch2(pDID, LEN_ID, &nFind)) != 0)
		return err;

	if (nFind != 1)
		return ERROR_PROGRAM;

	if (pText != NULL && pText->GetLength() != 0) {
		if ((err = pDataDB->DBWriteBinary(TEXT_FIELD, (LPVOID)(LPCSTR)*pText, pText->GetLength())) != 0)
			return err;
	} else {
		if ((err = pDataDB->DBWriteBinary(TEXT_FIELD, NULL, 0)) != 0)
			return err;
	}

	if (m_nCacheMode == CACHEMODE_CACHE)
		WriteTextCacheData(pDID);

	return 0;
}

int CMMPcsDB::WritePictureCacheData(LPCSTR pDID)
{
	int nFind;

	if (m_oDataDB2.DBSearch2(pDID, LEN_ID, &nFind) == 0 && nFind == 1) {
		if (m_oDataDB.DBSearch2(pDID, LEN_ID, &nFind) == 0 && nFind == 1) {
			WriteCacheBinary(m_oDataDB, m_oDataDB2, PICT1_FIELD);
			WriteCacheBinary(m_oDataDB, m_oDataDB2, PICT2_FIELD);
		}
	}

	return 0;
}

int CMMPcsDB::WriteTextCacheData(LPCSTR pDID)
{
	int nFind;

	if (m_oDataDB2.DBSearch2(pDID, LEN_ID, &nFind) == 0 && nFind == 1) {
		if (m_oDataDB.DBSearch2(pDID, LEN_ID, &nFind) == 0 && nFind == 1) {
			WriteCacheBinary(m_oDataDB, m_oDataDB2, TEXT_FIELD);
		}
	}

	return 0;
}

int CMMPcsDB::WriteTextCacheFolder(LPCSTR pFID)
{
	int nFind;

	if (m_oDataDB2.DBSearch2(pFID, LEN_ID, &nFind) == 0 && nFind == 1) {
		if (m_oDataDB.DBSearch2(pFID, LEN_ID, &nFind) == 0 && nFind == 1) {
			WriteCacheBinary(m_oDataDB, m_oDataDB2, FORM_FIELD);
		}
	}

	return 0;
}

void CMMPcsDB::WriteCacheBinary(CPSDB &oDB, CPSDB &oDB2, LPCSTR pFieldName)
{
	long nSize;

	if (oDB2.DBGetBinarySize(pFieldName, &nSize) == 0) {
		if (nSize != 0) {
			long nReadNum;
			char *pBuf = new char [nSize];
			if (oDB2.DBReadBinary(pFieldName, pBuf, nSize, &nReadNum) == 0)
				oDB.DBWriteBinary(pFieldName, pBuf, nReadNum);
			delete [] pBuf;
		} else
			oDB.DBWriteBinary(pFieldName, NULL, 0);
	}
}

int CMMPcsDB::AddData(DATAREC *pDataRec)
{
	int err;

	if ((err = MakeDataID(pDataRec->sDataID)) != 0)
		return err;

	if (pDataRec->sCreateDate.IsEmpty())
		SetCurrentTime(pDataRec->sCreateDate);
	if (pDataRec->sUpdateDate.IsEmpty())
		pDataRec->sUpdateDate = pDataRec->sCreateDate;
	pDataRec->nUpdateCount = 0;

	DATABUF oDataBuf;
	m_oDataDB.DBSetFieldBuf(&oDataBuf, pDataRec);

	if ((err = m_oDataDB.DBStoreUniq(&oDataBuf)) != 0)
		return err;

	if (m_nCacheMode == CACHEMODE_CACHE) {
		m_oDataDB2.DBDeleteKey(oDataBuf.dataID, LEN_ID, 1);
		if ((err = m_oDataDB2.DBStoreUniq(&oDataBuf)) != 0)
			return err;
	}

//	UpdateFolderUpdateCount(oDataBuf.folderID);

	return 0;
}

int CMMPcsDB::UpdateData(DATAREC *pDataRec)
{
	int err;

	if ((err = SetDataIndex(IDX_DATA_DID)) != 0)
		return err;

	SetCurrentTime(pDataRec->sUpdateDate);
	(pDataRec->nUpdateCount)++;

	DATABUF oDataBuf;
	m_oDataDB.DBSetFieldBuf(&oDataBuf, pDataRec);

	if ((err = m_oDataDB.DBUpdateKey(pDataRec->sDataID, LEN_ID, &oDataBuf)) != 0)
		return err;

	if (m_nCacheMode == CACHEMODE_CACHE) {
		m_oDataDB2.DBDeleteKey(oDataBuf.dataID, LEN_ID, 1);
		if ((err = m_oDataDB2.DBStoreUniq(&oDataBuf)) != 0)
			return err;
	}

	return 0;
}

int CMMPcsDB::DeleteData(LPCSTR pDID)
{
	int err;

	if ((err = SetDataIndex(IDX_DATA_DID)) != 0)
		return err;

	if ((err = m_oDataDB.DBDeleteKey(pDID, LEN_ID, 1)) != 0)
		return err;

	if (m_nCacheMode == CACHEMODE_CACHE) {
		if ((err = m_oDataDB2.DBDeleteKey(pDID, LEN_ID, 1)) != 0)
			return err;
	}

	DeleteFindPictItem(pDID);

	return 0;
}

int CMMPcsDB::MoveData(LPCSTR pDidSrc, LPCSTR pFidDst, DATAREC *pDataRec)
{
	DATAREC oDataRec;
	int err;

	if ((err = ReadData(pDidSrc, &oDataRec)) != 0)
		return err;

	oDataRec.sFolderID = pFidDst;

	if ((err = UpdateData(&oDataRec)) != 0)
		return err;

	if (pDataRec != NULL)
		*pDataRec = oDataRec;

	return 0;
}

int CMMPcsDB::CopyData(CMMPcsDB *pPcsDBSrc, LPCSTR pDidSrc, CMMPcsDB *pPcsDBDst, LPCSTR pFidDst, DATAREC *pDataRec)
{
	DATAREC oDataRec;
	int err;

	if ((err = pPcsDBSrc->ReadData(pDidSrc, &oDataRec)) != 0)
		return err;

	oDataRec.sFolderID = pFidDst;

	if ((err = pPcsDBDst->AddData(&oDataRec)) != 0)
		return err;

	if ((err = CopyBinary(pPcsDBSrc->m_oDataDB, pPcsDBDst->m_oDataDB, PICT1_FIELD)) != 0)
		return err;

	if ((err = CopyBinary(pPcsDBSrc->m_oDataDB, pPcsDBDst->m_oDataDB, PICT2_FIELD)) != 0)
		return err;

	if ((err = CopyBinary(pPcsDBSrc->m_oDataDB, pPcsDBDst->m_oDataDB, TEXT_FIELD)) != 0)
		return err;

	HDIB hDIB;
	if (MMPictureReadDB(&hDIB, pPcsDBSrc->m_oDataDB.GetFCB(), PICT1_FIELD) == 0) {
		pPcsDBDst->UpdateFindPictDB(oDataRec.sDataID, hDIB);
		MMPictureFree(hDIB);
	}

	if (pDataRec != NULL)
		*pDataRec = oDataRec;

	return 0;
}

int CMMPcsDB::CopyChildData(CMMPcsDB *pPcsDBSrc, LPCSTR pFidSrc, CMMPcsDB *pPcsDBDst, LPCSTR pFidDst)
{
	int err;

	if ((err = pPcsDBSrc->SetDataIndex(IDX_DATA_FID)) != 0)
		return err;

	if ((err = pPcsDBSrc->m_oDataDB.DBSearch(pFidSrc, LEN_ID)) != 0)
		return err;

	DATAREC oDataRec;
	DATABUF oDataBuf;
	BOOL bEof;
	for (;;) {
		if ((err = pPcsDBSrc->m_oDataDB.DBEof(&bEof)) != 0)
			return err;

		if (bEof)
			break;

		if ((err = pPcsDBSrc->m_oDataDB.DBRead(&oDataBuf)) != 0)
			return err;

		if (strncmp(pFidSrc, oDataBuf.folderID, LEN_ID) != 0)
			break;

		pPcsDBSrc->m_oDataDB.DBGetFieldBuf(&oDataBuf, &oDataRec);
		oDataRec.sFolderID = pFidDst;

		if ((err = pPcsDBDst->AddData(&oDataRec)) != 0)
			return err;

		if ((err = CopyBinary(pPcsDBSrc->m_oDataDB, pPcsDBDst->m_oDataDB, PICT1_FIELD)) != 0)
			return err;

		if ((err = CopyBinary(pPcsDBSrc->m_oDataDB, pPcsDBDst->m_oDataDB, PICT2_FIELD)) != 0)
			return err;

		if ((err = CopyBinary(pPcsDBSrc->m_oDataDB, pPcsDBDst->m_oDataDB, TEXT_FIELD)) != 0)
			return err;

		HDIB hDIB;
		if (MMPictureReadDB(&hDIB, pPcsDBSrc->m_oDataDB.GetFCB(), PICT1_FIELD) == 0) {
			pPcsDBDst->UpdateFindPictDB(oDataRec.sDataID, hDIB);
			MMPictureFree(hDIB);
		}

		if ((err = pPcsDBSrc->m_oDataDB.DBSkip(1)) != 0)
			return err;
	}

	return 0;
}

int CMMPcsDB::CopyBinary(CPSDB &oSrcDB, CPSDB &oDstDB, LPCSTR pFieldName)
{
	long nSize;
	int err;

	if ((err = oSrcDB.DBGetBinarySize(pFieldName, &nSize)) != 0)
		return err;

	char *pBuf = new char [nSize];
	long readNum;

	if ((err = oSrcDB.DBReadBinary(pFieldName, pBuf, nSize, &readNum)) != 0) {
		delete [] pBuf;
		return err;
	}

	if ((err = oDstDB.DBWriteBinary(pFieldName, pBuf, readNum)) != 0) {
		delete [] pBuf;
		return err;
	}

	delete [] pBuf;

	return 0;
}

int CMMPcsDB::SetFolderIndex(int nIndexNo, int nIndexNo2)
{
	int err;

	if (nIndexNo2 == -1)
		nIndexNo2 = nIndexNo;

	if (nIndexNo != m_nCurrentFolderIndex) {
		if ((err = m_oFolderDB.DBChgIdx(nIndexNo)) != 0)
			return err;

			m_nCurrentFolderIndex = nIndexNo;
	}

	if (m_nCacheMode == CACHEMODE_CACHE) {
		if (nIndexNo2 != m_nCurrentFolderIndex2) {
			if ((err = m_oFolderDB2.DBChgIdx(nIndexNo2)) != 0)
				return err;

			m_nCurrentFolderIndex2 = nIndexNo2;
		}
	}

	return 0;
}

int CMMPcsDB::SetDataIndex(int nIndexNo, int nIndexNo2)
{
	int err;

	if (nIndexNo2 == -1)
		nIndexNo2 = nIndexNo;

	if (nIndexNo != m_nCurrentDataIndex) {
		if ((err = m_oDataDB.DBChgIdx(nIndexNo)) != 0)
			return err;

			m_nCurrentDataIndex = nIndexNo;
	}

	if (m_nCacheMode == CACHEMODE_CACHE) {
		if (nIndexNo2 != m_nCurrentDataIndex2) {
			if ((err = m_oDataDB2.DBChgIdx(nIndexNo2)) != 0)
				return err;

			m_nCurrentDataIndex2 = nIndexNo2;
		}
	}

	return 0;
}

void CMMPcsDB::MakeLibraryID(CString &sID)
{
	time_t lid;
	static long offset;

	time(&lid);
	lid += offset++;
	MakeID('L', (DWORD)lid, sID);
}

int CMMPcsDB::MakeFolderID(CString &sID)
{
	time_t ltime;
	int nFind;
	int err;

	time(&ltime);

	for (;;) {
		MakeID('F', (DWORD)ltime, sID);

		if ((err = SetFolderIndex(IDX_FOLDER_FID)) != 0)
			return err;

		if ((err = m_oFolderDB.DBSearch2(sID, LEN_ID, &nFind)) != 0)
			return err;

		if (nFind != 1)
			break;

		ltime++;
	}

	return 0;
}

int CMMPcsDB::MakeDataID(CString &sID)
{
	int nFind;
	int err;

	time_t ltime;
	time(&ltime);

	for (;;) {
		MakeID('D', (DWORD)ltime, sID);

		if ((err = SetDataIndex(IDX_DATA_DID)) != 0)
			return err;

		if ((err = m_oDataDB.DBSearch2(sID, LEN_ID, &nFind)) != 0)
			return err;

		if (nFind != 1)
			break;

		ltime++;
	}

	return 0;
}

void CMMPcsDB::SetCurrentTime(CString &sTime)
{
	STIME stime;

	m_pPcserve->m_oPcserve.GetTime(&stime);
	sTime.Format("%04d%02d%02d%02d%02d", stime.year, stime.month, stime.day, stime.hour, stime.minute);
}

int CMMPcsDB::UpdateFolderUpdateCount(LPCSTR pFID)
{
	int nFind;
	int err;

	if ((err = SetFolderIndex(IDX_FOLDER_FID)) != 0)
		return err;

	FOLDERBUF oFolderBuf;
	if ((err = m_oFolderDB.DBReadKey(pFID, LEN_ID, &oFolderBuf, 0, &nFind)) != 0)
		return err;

	if (nFind == 0)
		return ERROR_PROGRAM;

	FOLDERREC oFolderRec;
	m_oFolderDB.DBGetFieldBuf(&oFolderBuf, &oFolderRec);

	(oFolderRec.nUpdateCount)++;

	m_oFolderDB.DBSetFieldBuf(&oFolderBuf, &oFolderRec);

	if ((err = m_oFolderDB.DBUpdateKey(pFID, LEN_ID, &oFolderBuf)) != 0)
		return err;

	return 0;
}

int CMMPcsDB::FillCache(LPCSTR pFID)
{
	if (m_nCacheMode != CACHEMODE_CACHE)
		return ERROR_PROGRAM;

	char folderType;
	char (*pID)[LEN_ID];
	long nRec;
	int nRead;
	int nLen;
	int i;
	int err;

	if ((err = CacheFolder(pFID, NULL, &folderType)) != 0)
		return err;

	switch (folderType) {
	case FTYPE_LIBRARY:
	case FTYPE_GFOLDER:
		if ((err = SelectFolder(pFID, &nRec)) != NULL)
			return err;

		if ((err = m_oFolderDB.DBSetReadField("FID", &nLen)) != 0)
			return err;

		pID = new char[nRec][LEN_ID];
		if ((err = m_oFolderDB.DBReadNext(nRec, pID, &nRead)) != 0) {
			delete [] pID;
			return err;
		}

		if ((err = SelectFolder(NULL, &nRec)) != 0) {
			delete [] pID;
			return err;
		}

		if ((err = m_oFolderDB.DBSetReadField(NULL, &nLen)) != 0) {
			delete [] pID;
			return err;
		}

		for (i = 0; i < nRead; i++) {
			if ((err = FillCache(pID[i])) != 0) {
				delete [] pID;
				return err;
			}
		}

		delete [] pID;
		break;
	case FTYPE_DFOLDER:
		if ((err = FillDataCache(pFID)) != 0)
			return err;
		break;
	}

	return 0;
}

int CMMPcsDB::FillDataCache(LPCSTR pFID)
{
	int err;

	if ((err = SetDataIndex(IDX_DATA_FID, IDX_DATA_DID)) != 0)
		return err;

	if ((err = m_oDataDB.DBSearch(pFID, LEN_ID)) != 0)
		return err;

	DATABUF oDataBuf1, oDataBuf2;
	int nFind;
	BOOL bEof;
	BOOL bBinaryCopy;
	long nSize;
	for (;;) {
		if ((err = m_oDataDB.DBEof(&bEof)) != 0)
			return err;

		if (bEof)
			break;

		if ((err = m_oDataDB.DBRead(&oDataBuf1)) != 0)
			return err;

		if (strncmp(pFID, oDataBuf1.folderID, LEN_ID) != 0)
			break;

		if (m_oDataDB2.DBReadKey(oDataBuf1.dataID, LEN_ID, &oDataBuf2, 0, &nFind) == 0 && nFind == 1) {
			if (memcmp(oDataBuf1.updateCount, oDataBuf2.updateCount, LEN_UCOUNT) != 0) {
				if ((err = m_oDataDB2.DBUpdate(&oDataBuf1)) != 0)
					return err;

				bBinaryCopy = TRUE;
			} else
				bBinaryCopy = FALSE;
		} else {
			if ((err = m_oDataDB2.DBStoreUniq(&oDataBuf1)) != 0)
				return err;

			bBinaryCopy = TRUE;
		}

		if ((err = m_oDataDB2.DBGetBinarySize(PICT1_FIELD, &nSize)) != 0)
				return err;

		if (nSize == 0) {
			if ((err = CacheBinaryData(PICT1_FIELD)) != 0)
				return err;
		}

		if ((err = m_oDataDB2.DBGetBinarySize(PICT2_FIELD, &nSize)) != 0)
				return err;

		if (nSize == 0) {
			if ((err = CacheBinaryData(PICT2_FIELD)) != 0)
				return err;
		}

		if ((err = m_oDataDB2.DBGetBinarySize(TEXT_FIELD, &nSize)) != 0)
				return err;

		if (nSize == 0) {
			if ((err = CacheBinaryData(TEXT_FIELD)) != 0)
				return err;
		}

		if ((err = m_oDataDB.DBSkip(1)) != 0)
			return err;
	}

	return 0;
}

int CMMPcsDB::ClearCache()
{
	int err;

	switch (m_nCacheMode) {
	case CACHEMODE_CACHE:
		if ((err = m_oFolderDB2.DBZip()) != 0)
			return err;

		if ((err = m_oDataDB2.DBZip()) != 0)
			return err;
		break;
	case CACHEMODE_OFFLINE:
		if ((err = m_oFolderDB.DBZip()) != 0)
			return err;

		if ((err = m_oDataDB.DBZip()) != 0)
			return err;
		break;
	default:
		return ERROR_PROGRAM;
	}

	return 0;
}

int CMMPcsDB::FilterFolder(LPCSTR pFilter)
{
	int err;

	if ((err = SetFolderIndex(IDX_FOLDER_FID)) != 0)
		return err;

	if ((err = m_oFolderDB.DBSetFilter(pFilter)) != 0)
		return err;

	return 0;
}

int CMMPcsDB::GetFolderCount(long *pRecNum)
{
	int err;

	if ((err = m_oFolderDB.DBLRecCount(pRecNum)) != 0)
		return err;

	return 0;
}

int CMMPcsDB::ReadFolderAll(FOLDERREC *pFolderRec, BOOL *pRead)
{
	FOLDERBUF oFolderBuf;
	int err;

	if ((err = m_oFolderDB.DBReadAll(&oFolderBuf, pRead)) != 0)
		return err;

	m_oFolderDB.DBGetFieldBuf(&oFolderBuf, pFolderRec);

	return 0;
}

int CMMPcsDB::FilterData(LPCSTR pFilter)
{
	int err;

	if ((err = SetDataIndex(IDX_DATA_DID)) != 0)
		return err;

	if ((err = m_oDataDB.DBSetFilter(pFilter)) != 0)
		return err;

	return 0;
}

int CMMPcsDB::GetDataCount(long *pRecNum)
{
	int err;

	if ((err = m_oDataDB.DBLRecCount(pRecNum)) != 0)
		return err;

	return 0;
}

int CMMPcsDB::ReadDataAll(DATAREC *pDataRec, BOOL *pRead)
{
	DATABUF oDataBuf;
	int err;

	if ((err = m_oDataDB.DBReadAll(&oDataBuf, pRead)) != 0)
		return err;

	if (m_nCacheMode == CACHEMODE_CACHE)
		m_oDataDB2.DBStoreUniq(&oDataBuf);

	m_oDataDB.DBGetFieldBuf(&oDataBuf, pDataRec);

	return 0;
}

int CMMPcsDB::PackFile()
{
	int err;

	if ((err = m_oFolderDB.DBPack()) != 0)
		return err;

	if ((err = m_oDataDB.DBPack()) != 0)
		return err;

	if (m_nCacheMode == CACHEMODE_CACHE) {
		if ((err = m_oFolderDB2.DBPack()) != 0)
			return err;

		if ((err = m_oDataDB2.DBPack()) != 0)
			return err;
	}

	return 0;
}

int CMMPcsDB::SelectFolder(LPCSTR pFID, long *nRec)
{
	int err;

	if (pFID != NULL) {
		char fidbuf[LEN_ID + 1];
		CString select;

		memcpy(fidbuf, pFID, LEN_ID);
		fidbuf[LEN_ID] = '\0';
		select.Format("#%d=%8s", IDX_FOLDER_PFID, fidbuf);
		if ((err = m_oFolderDB.DBSelect(select, nRec)) != 0)
			return err;
	} else {
		if ((err = m_oFolderDB.DBSelect(NULL, nRec)) != 0)
			return err;
	}

	return 0;
}

int CMMPcsDB::SelectData(LPCSTR pFID, long *nRec)
{
	int err;

	if (pFID != NULL) {
		char fidbuf[LEN_ID + 1];
		CString select;

		memcpy(fidbuf, pFID, LEN_ID);
		fidbuf[LEN_ID] = '\0';
		select.Format("#%d=%8s", IDX_DATA_FID, fidbuf);
		if ((err = m_oDataDB.DBSelect(select, nRec)) != 0)
			return err;
	} else {
		if ((err = m_oDataDB.DBSelect(NULL, nRec)) != 0)
			return err;
	}

	return 0;
}

int CMMPcsDB::Backup(LPCSTR pBackupDir, CProgressCtrl *pProgress)
{
	CMMPcserve oPcserveLocal;
	CPSDB oFolderDB, oDataDB;
	CString sDir;
	int err;

	CreateDirectoryAll(pBackupDir);

	oPcserveLocal.Open(CHT_LOCAL, NULL);

	sDir = pBackupDir;
	if (sDir.Right(1) != '\\')
		sDir += "\\";

	if ((err = CreateDB(&oPcserveLocal, oFolderDB, oDataDB, sDir, FALSE)) != 0)
		return err;

	if ((err = CopyDB(&m_oFolderDB, &m_oDataDB, &oFolderDB, &oDataDB, pProgress)) != 0)
		return err;

	return 0;
}

int CMMPcsDB::Restore(LPCSTR pBackupDir, CProgressCtrl *pProgress)
{
	CMMPcserve oPcserveLocal;
	CPSDB oFolderDB, oDataDB;
	CString sDir;
	int err;

	CreateDirectoryAll(pBackupDir);

	oPcserveLocal.Open(CHT_LOCAL, NULL);

	sDir = pBackupDir;
	if (sDir.Right(1) != '\\')
		sDir += "\\";

	if ((err = OpenDB(&oPcserveLocal, oFolderDB, oDataDB, sDir, FALSE)) != 0)
		return err;

	if ((err = CopyDB(&oFolderDB, &oDataDB, &m_oFolderDB, &m_oDataDB, pProgress)) != 0)
		return err;

	return 0;
}

void CMMPcsDB::FlushFolder()
{
	if (m_pPcserve == NULL)
		return;

	if (m_pPcserve->m_nChType != CHT_LOCAL)
		return;

	m_oFolderDB.DBFlush();
}

void CMMPcsDB::FlushData()
{
	if (m_pPcserve == NULL)
		return;

	if (m_pPcserve->m_nChType != CHT_LOCAL)
		return;

	m_oDataDB.DBFlush();
}

BOOL CMMPcsDB::CheckPassword(CString sPassword) const
{
	return (m_sPassword == sPassword);
}

void CMMPcsDB::SetPassword(CString sPassword)
{
	m_sPassword = sPassword;
	m_oLibraryRec.sCodeDef = sPassword;
	UpdateFolder(&m_oLibraryRec);
}

BOOL CMMPcsDB::CheckPermission(DWORD dwPermission) const
{
	return (m_dwPermission & dwPermission) ? TRUE : FALSE;
}

void CMMPcsDB::SetPermission(DWORD dwPermission)
{
	m_dwPermission = dwPermission;
	m_oLibraryRec.nCodeStep = dwPermission;
	UpdateFolder(&m_oLibraryRec);
}

BOOL CMMPcsDB::CreateFindPictDB()
{
	static constexpr DBF_I field[] = {
		{"DID",			'C', LEN_ID,         0},	// データＩＤ
		{"FACTOR_R",	'C', LEN_PFACTOR,    0},	// 画像ファクタ
		{"FACTOR_G",	'C', LEN_PFACTOR,    0},	// 画像ファクタ
		{"FACTOR_B",	'C', LEN_PFACTOR,    0}		// 画像ファクタ
	};
	CString sPath;
	int nIndexNo;

	if (m_oFindPictDB.GetFCB() != NULL)
		m_oFindPictDB.DBClose();

	sPath.Format("%s%s", (LPCSTR)m_sDirName, FN_PICTURE_DBF);
	if (m_oFindPictDB.DBCreate(m_pPcserve->m_oPcserve, sPath, field, ARRAY_SIZE(field)) != 0)
		return FALSE;

	sPath.Format("%s%s", (LPCSTR)m_sDirName, FN_PICTURE_NDX1);
	if (m_oFindPictDB.DBIdxCreate(sPath, "DID", 1, &nIndexNo) != 0)
		return FALSE;

	sPath.Format("%s%s", (LPCSTR)m_sDirName, FN_PICTURE_NDX2);
	if (m_oFindPictDB.DBIdxCreate(sPath, "FACTOR_R+DID", 1, &nIndexNo) != 0)
		return FALSE;

	sPath.Format("%s%s", (LPCSTR)m_sDirName, FN_PICTURE_NDX3);
	if (m_oFindPictDB.DBIdxCreate(sPath, "FACTOR_G+DID", 1, &nIndexNo) != 0)
		return FALSE;

	sPath.Format("%s%s", (LPCSTR)m_sDirName, FN_PICTURE_NDX4);
	if (m_oFindPictDB.DBIdxCreate(sPath, "FACTOR_B+DID", 1, &nIndexNo) != 0)
		return FALSE;

	return TRUE;
}

BOOL CMMPcsDB::OpenFindPictDB()
{
	CString sPath;
	int nIndexNo;
	int nRecSize;

	sPath.Format("%s%s", (LPCSTR)m_sDirName, FN_PICTURE_DBF);
	if (m_oFindPictDB.DBOpen(m_pPcserve->m_oPcserve, sPath) != 0)
		return FALSE;

	m_oFindPictDB.DBRecSize(&nRecSize);
	if (nRecSize != sizeof(FINDPICTBUF) + 1) {
		m_oFindPictDB.DBClose();
		return FALSE;
	}

	sPath.Format("%s%s", (LPCSTR)m_sDirName, FN_PICTURE_NDX1);
	if (m_oFindPictDB.DBIndex(sPath, &nIndexNo) != 0)
		return FALSE;

	sPath.Format("%s%s", (LPCSTR)m_sDirName, FN_PICTURE_NDX2);
	if (m_oFindPictDB.DBIndex(sPath, &nIndexNo) != 0)
		return FALSE;

	sPath.Format("%s%s", (LPCSTR)m_sDirName, FN_PICTURE_NDX3);
	if (m_oFindPictDB.DBIndex(sPath, &nIndexNo) != 0)
		return FALSE;

	sPath.Format("%s%s", (LPCSTR)m_sDirName, FN_PICTURE_NDX4);
	if (m_oFindPictDB.DBIndex(sPath, &nIndexNo) != 0)
		return FALSE;

	return TRUE;
}

void CMMPcsDB::UpdateFindPictDB(LPCSTR pDID, HDIB hDIB)
{
	if (hDIB != NULL && m_oFindPictDB.GetFCB() != NULL) {
		BOOL bEof;
		FINDPICTBUF findPictBuf;
		FINDPICTREC findPictRec;
		double factor_r, factor_g, factor_b;

		GetPictFactor(hDIB, &factor_r, &factor_g, &factor_b);
		findPictRec.sDataID = pDID;
		findPictRec.sFactor_r.Format("%8.6f", factor_r);
		findPictRec.sFactor_g.Format("%8.6f", factor_g);
		findPictRec.sFactor_b.Format("%8.6f", factor_b);
		m_oFindPictDB.DBSetFieldBuf(&findPictBuf, &findPictRec);

		m_oFindPictDB.DBChgIdx(1);
		m_oFindPictDB.DBSearch(pDID, LEN_ID);
		m_oFindPictDB.DBEof(&bEof);
		if (bEof)
			m_oFindPictDB.DBStoreUniq(&findPictBuf);
		else
			m_oFindPictDB.DBUpdate(&findPictBuf);
	}
}

void CMMPcsDB::DeleteFindPictDB()
{
	CString sPath;

	m_oFindPictDB.DBClose();

	sPath.Format("%s%s", (LPCSTR)m_sDirName, FN_PICTURE_DBF);
	::DeleteFile(sPath);

	sPath.Format("%s%s", (LPCSTR)m_sDirName, FN_PICTURE_NDX1);
	::DeleteFile(sPath);

	sPath.Format("%s%s", (LPCSTR)m_sDirName, FN_PICTURE_NDX2);
	::DeleteFile(sPath);

	sPath.Format("%s%s", (LPCSTR)m_sDirName, FN_PICTURE_NDX3);
	::DeleteFile(sPath);

	sPath.Format("%s%s", (LPCSTR)m_sDirName, FN_PICTURE_NDX4);
	::DeleteFile(sPath);
}

void CMMPcsDB::DeleteFindPictItem(LPCSTR pDID)
{
	if (m_oFindPictDB.GetFCB() != NULL) {
		m_oFindPictDB.DBChgIdx(1);
		m_oFindPictDB.DBDeleteKey(pDID, LEN_ID, 1);
	}
}

BOOL CMMPcsDB::CheckFolderID(LPCSTR pFolderID, int nDepth)
{
	if (nDepth > 100)
		return FALSE;

	FOLDERREC oFolderRec;
	if (ReadFolder(pFolderID, &oFolderRec) != 0)
		return FALSE;

	if (oFolderRec.sFolderType == FTYPE_LIBRARY)
		return TRUE;

	return CheckFolderID(oFolderRec.sParentFolderID, nDepth + 1);
}

static int CopyDB(CPSDB *pSrcFolderDB, CPSDB *pSrcDataDB, CPSDB *pDstFolderDB, CPSDB *pDstDataDB, CProgressCtrl *pProgress)
{
	long nFolderCount, nDataCount;
	int err;

	if ((err = pSrcFolderDB->DBRecCount(&nFolderCount)) != 0)
		return err;

	if ((err = pSrcDataDB->DBRecCount(&nDataCount)) != 0)
		return err;

	pProgress->SetRange32(0, nFolderCount + nDataCount);

	if ((err = CopyFolderDB(pSrcFolderDB, pDstFolderDB, pProgress, 0)) != 0)
		return err;

	if ((err = CopyDataDB(pSrcDataDB, pDstDataDB, pProgress, nFolderCount)) != 0)
		return err;

	return 0;
}

static int CopyFolderDB(CPSDB *pSrcFolderDB, CPSDB *pDstFolderDB, CProgressCtrl *pProgress, int nCount)
{
	int err;

	if ((err = pSrcFolderDB->DBTop()) != 0)
		return err;

	if ((err = pDstFolderDB->DBZip()) != 0)
		return err;

	FOLDERBUF oFolderBuf;
	BOOL bEof;
	for (;;) {
		if ((err = pSrcFolderDB->DBEof(&bEof)) != 0)
			return err;

		if (bEof)
			break;

		if ((err = pSrcFolderDB->DBRead(&oFolderBuf)) != 0)
			return err;

		if ((err = pDstFolderDB->DBStore(&oFolderBuf)) != 0)
			return err;

		if ((err = CopyBinaryDB(pSrcFolderDB, pDstFolderDB, FORM_FIELD)) != 0)
			return err;

		if ((err = pSrcFolderDB->DBSkip(1)) != 0)
			return err;

		pProgress->SetPos(++nCount);
	}

	return 0;
}

static int CopyDataDB(CPSDB *pSrcDataDB, CPSDB *pDstDataDB, CProgressCtrl *pProgress, int nCount)
{
	int err;

	if ((err = pSrcDataDB->DBTop()) != 0)
		return err;

	if ((err = pDstDataDB->DBZip()) != 0)
		return err;

	DATABUF oDataBuf;
	BOOL bEof;
	for (;;) {
		if ((err = pSrcDataDB->DBEof(&bEof)) != 0)
			return err;

		if (bEof)
			break;

		if ((err = pSrcDataDB->DBRead(&oDataBuf)) != 0)
			return err;

		if ((err = pDstDataDB->DBStore(&oDataBuf)) != 0)
			return err;

		if ((err = CopyBinaryDB(pSrcDataDB, pDstDataDB, PICT1_FIELD)) != 0)
			return err;

		if ((err = CopyBinaryDB(pSrcDataDB, pDstDataDB, PICT2_FIELD)) != 0)
			return err;

		if ((err = CopyBinaryDB(pSrcDataDB, pDstDataDB, TEXT_FIELD)) != 0)
			return err;

		if ((err = CopyBinaryDB(pSrcDataDB, pDstDataDB, SOUND_FIELD)) != 0)
			return err;

		if ((err = pSrcDataDB->DBSkip(1)) != 0)
			return err;

		pProgress->SetPos(++nCount);
	}

	return 0;
}

static int CopyBinaryDB(CPSDB *pSrcDB, CPSDB *pDstDB, LPCSTR pFieldName)
{
	long nSize;
	int err;

	if ((err = pSrcDB->DBGetBinarySize(pFieldName, &nSize)) != 0)
		return err;

	if (nSize == 0)
		return 0;

	char *pBuf = new char[nSize];
	long readNum;

	if ((err = pSrcDB->DBReadBinary(pFieldName, pBuf, nSize, &readNum)) != 0) {
		delete [] pBuf;
		return err;
	}

	if ((err = pDstDB->DBWriteBinary(pFieldName, pBuf, readNum)) != 0) {
		delete [] pBuf;
		return err;
	}

	delete [] pBuf;

	return 0;
}

static void PcserveError(int nErrorCode)
{
	if (!g_bNoDispError) {
		CString sMsg;

		switch (nErrorCode) {
		case ERROR_PROGRAM:
			sMsg = GetString(IDS_PROGRAM_ERROR);
			break;
		case ERROR_VERSION:
			return;
		case ERROR_ALREADY_OPEN:
			sMsg = GetString(IDS_LIBRARY_ALREADY_OPEN);
			break;
		case ERROR_PERMISSION:
			sMsg = GetString(IDS_ACCESS_DENY);
			break;
		case ERROR_PASSWORD:
			sMsg = GetString(IDS_PASSWORD_INCORRECT);
			break;
		default:
			sMsg.Format("Pcserve Error %d %s", nErrorCode, CPcserve::GetErrorText(nErrorCode));
			break;
		}

		::AfxMessageBox(sMsg);
	} else
		g_bNoDispError = FALSE;

	g_nLastPcserveError = nErrorCode;
}
