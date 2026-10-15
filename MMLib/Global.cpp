#include "stdafx.h"
#include "MMLib.h"
#include "Mml.h"
#include "PictFile.h"
#include <mapi.h>
#include <sys/stat.h>
#include <shlwapi.h>

CImageList g_oImageList;
CImageList g_oImageList2;
CTreeCtrl *g_pTreeCtrl;
CString g_sStatusInfo;
constexpr char g_sSectionSettings[] = "Settings";
constexpr char g_sSectionMainWindow[] = "MainWindow";
constexpr char g_sSectionLibrary[] = "Library";
constexpr char g_sSectionDetails[] = "Details";
constexpr char g_sSectionOptions[] = "Options";
constexpr char g_sSectionRecent[] = "Recent";
constexpr char g_sSectionTransfer[] = "Transfer";
constexpr char g_sSectionFindWindow[] = "FindWindow";
constexpr char g_sSectionMailAtt[] = "MailAtt";
constexpr char g_sSectionSearchHistory[] = "SearchHistory";
constexpr char g_sProgName[] = "Multimedia Library Manager";
constexpr char g_sRegistryName[] = "Multimedia Library";

HICON g_hIcon;

constexpr int g_tColumnOrder[] = {
	DETAIL_NAME,
	DETAIL_FIRSTDATA,
	DETAIL_CODE,
	DETAIL_CDATE,
	DETAIL_UDATE,
	DETAIL_CREATOR,
	DETAIL_REMARK,
	DETAIL_KEYWORD,
	DETAIL_SERVER,
	DETAIL_DIRECTORY
};

UINT g_nCFMMLibrary;
int g_nDetailMode;
BOOL g_bParamOffline;
CFont g_oDefaultFont;
int g_nFontSize;
CString g_sViewFontName;
int g_nViewFontSize;
CString g_sTempFile;
CItemInbox *g_pItemInbox;
CString g_sImageEditor;
DEVMODE *g_pPrintDevMode;
CRect g_rcPrintMargin;
BOOL g_bPrintMargin;

static void MakeFilterSub(CString &sStr, LPCSTR pOption, LPCSTR pFieldName, LPCSTR pOperation, LPCSTR pFind, BOOL bAnd = TRUE);
static CString MakeDateBefore(const CString &sDays);
static void GetMmlibDir(CString &sDir, LPCSTR pSubDir);
static LONG GetLineSize(const int Width, const int BitCount);
static COLORREF GetDIBPixel(const BYTE *buff, const int x, const int y);

void FormatDate(const CString &sStrIn, CString &sStrOut)
{
	LPCSTR pDate = sStrIn;

	sStrOut.Format("%.4s/%.2s/%.2s %.2s:%.2s",
			pDate, pDate + 4, pDate + 6,
			pDate + 8, pDate + 10);
}

HDIB MakeSmallPicture(HDIB hDIB1, int nWidth, int nHeight)
{
	short nWidth1, nHeight1, nColor;

	if (MMPictureGetInfo(hDIB1, &nWidth1, &nHeight1, &nColor) != 0)
		return NULL;

	if (nHeight1 * nWidth / nWidth1 < nHeight)
		nHeight = nHeight1 * nWidth / nWidth1;
	else
		nWidth = nWidth1 * nHeight / nHeight1;

	HDIB hDIB2;
	if (MMPictureCopy(hDIB1, &hDIB2, (SHORT)nWidth, (SHORT)nHeight) != 0)
		return NULL;

	return hDIB2;
}

BOOL CreateDirectoryAll(LPCSTR pDirPath)
{
	DWORD err;

	if (!::CreateDirectory(pDirPath, NULL)) {
		err = GetLastError();
		if (err == ERROR_ALREADY_EXISTS)
			return TRUE;
		else if (err == ERROR_PATH_NOT_FOUND) {
			char buf[_MAX_PATH];
			strcpy(buf, pDirPath);
			char *p;
			if ((p = strrchr(buf, '\\')) != NULL) {
				*p = '\0';
				if (CreateDirectoryAll(buf))
					return ::CreateDirectory(pDirPath, NULL);
				else
					return FALSE;
			} else
				return FALSE;
		} else
			return FALSE;
	}

	return TRUE;
}

HANDLE CopyHandle(HANDLE hHandle)
{
	if (hHandle == NULL)
		return NULL;

	SIZE_T dwLen = ::GlobalSize((HGLOBAL)hHandle);

	HANDLE hCopy;
	if ((hCopy = (HANDLE)::GlobalAlloc(GHND, dwLen)) != NULL)
	{
		BYTE *lpCopy = (BYTE *)::GlobalLock((HGLOBAL)hCopy);
		BYTE *lp = (BYTE *)::GlobalLock((HGLOBAL)hHandle);

		if (lpCopy != NULL && lp != NULL) {
			while (dwLen--)
				*lpCopy++ = *lp++;
		}

		::GlobalUnlock((HGLOBAL)hCopy);
		::GlobalUnlock((HGLOBAL)hHandle);
	}

	return hCopy;
}

void PrintPicture(CDC *pDC, HDIB hDIB, CRect rcMargin)
{
	if (hDIB == NULL)
		return;

	short nSrcWidth, nSrcHeight, nColor;
	MMPictureGetInfo(hDIB, &nSrcWidth, &nSrcHeight, &nColor);

	const int nMapMode = pDC->SetMapMode(MM_HIMETRIC);
	pDC->LPtoDP(rcMargin);
	pDC->SetMapMode(nMapMode);

	const int nDevWidth = pDC->GetDeviceCaps(HORZRES) - (rcMargin.left + rcMargin.right);
	const int nDevHeight = pDC->GetDeviceCaps(VERTRES) + (rcMargin.top + rcMargin.bottom);

	int nDstWidth = nDevWidth;
	int nDstHeight = nSrcHeight * nDevWidth / nSrcWidth;
	if (nDstHeight > nDevHeight) {
		nDstWidth = nSrcWidth * nDevHeight / nSrcHeight;
		nDstHeight = nDevHeight;
	}

	MMPictureDraw(hDIB, pDC->m_hDC, (SHORT)rcMargin.left, (SHORT)-rcMargin.top, (SHORT)nDstWidth, (SHORT)nDstHeight, 0, 0, (SHORT)nSrcWidth, (SHORT)nSrcHeight);
}

BOOL SendDocuments(LPCSTR pFileName)
{
	HINSTANCE hModule;
	if ((hModule = LoadLibrary("MAPI32.DLL")) == NULL)
		return FALSE;

	FARPROC  pProc;
	if ((pProc = GetProcAddress(hModule, "MAPISendMail")) != NULL) {
		CStringArray files;
		StrSplit(pFileName, ";", files);
		const int nFileCount = (int)files.GetCount();

		MapiFileDesc *pMfd = new MapiFileDesc[nFileCount]();
		for (int i = 0; i < nFileCount; i++) {
			pMfd[i].lpszPathName = (LPSTR)(LPCSTR)files[i];
			pMfd[i].nPosition = i;
		}

		CString spc(' ', nFileCount);

		MapiMessage mms{
			.lpszNoteText = (LPSTR)(LPCSTR)spc,
			.nFileCount = (ULONG)nFileCount,
			.lpFiles = pMfd
		};
		(*(LPMAPISENDMAIL)pProc)(0, 0, &mms, MAPI_LOGON_UI | MAPI_DIALOG, 0);

		delete [] pMfd;
	}

	FreeLibrary(hModule);

	return TRUE;
}

void MakeFilterString(const FIND_CONDITION *pFindConditions, int nFindType, CString &sFilter)
{
	CString sStr;
	CString sOption;

	sFilter.Empty();
	for (int i = 0; i < N_FIND_CONDITIONS; i++) {
		sOption.Empty();
		for (int j = 0; j < pFindConditions[i].sMatching.GetLength(); j++) {
			sOption += "*";
			sOption += pFindConditions[i].sMatching[j];
		}

		sStr.Empty();
		MakeFilterSub(sStr, NULL, "CDATE", ">=", pFindConditions[i].sCreateDateFrom);
		MakeFilterSub(sStr, NULL, "CDATE", "<=", pFindConditions[i].sCreateDateTo);
		MakeFilterSub(sStr, NULL, "CDATE", ">=", MakeDateBefore(pFindConditions[i].sCreateDateBefore));
		MakeFilterSub(sStr, NULL, "UDATE", ">=", pFindConditions[i].sUpdateDateFrom);
		MakeFilterSub(sStr, NULL, "UDATE", "<=", pFindConditions[i].sUpdateDateTo);
		MakeFilterSub(sStr, NULL, "UDATE", ">=", MakeDateBefore(pFindConditions[i].sUpdateDateBefore));
		MakeFilterSub(sStr, sOption, "CREATOR", "=", pFindConditions[i].sCreator);
		if (nFindType == FIND_TYPE_FOLDER) {
			MakeFilterSub(sStr, sOption, "FNAME", "=", pFindConditions[i].sTitle);
		} else {
			MakeFilterSub(sStr, sOption, "CODE", "=", pFindConditions[i].sCode);
			MakeFilterSub(sStr, sOption, "TITLE", "=", pFindConditions[i].sTitle);
		}
		MakeFilterSub(sStr, sOption, "REMARK1", "=", pFindConditions[i].sRemark);
		MakeFilterSub(sStr, sOption, "REMARK2", "=", pFindConditions[i].sKeyword);
		MakeFilterSub(sStr, sOption, nFindType == FIND_TYPE_FOLDER ? FORM_FIELD : TEXT_FIELD, "=", pFindConditions[i].sText);

		if (!sStr.IsEmpty()) {
			if (!sFilter.IsEmpty())
				sFilter += "|";
			sFilter += "(" + sStr + ")";
		}
	}
}

void MakeFilterString2(LPCSTR pKeyword, CString &sFolderFilter, CString &sDataFilter)
{
	CString sStr;
	char buf[256];

	constexpr char sOption[] = "*S*N*Z";

	sFolderFilter.Empty();
	sDataFilter.Empty();

	strcpy(buf, pKeyword);
	char *token = strtok(buf, " ");
	while(token != NULL) {
		sStr.Empty();
		MakeFilterSub(sStr, sOption, "FNAME", "=", token, FALSE);
		MakeFilterSub(sStr, sOption, "CREATOR", "=", token, FALSE);
		MakeFilterSub(sStr, sOption, "REMARK1", "=", token, FALSE);
		MakeFilterSub(sStr, sOption, "REMARK2", "=", token, FALSE);
		MakeFilterSub(sStr, sOption, FORM_FIELD, "=", token, FALSE);

		if (!sStr.IsEmpty()) {
			if (!sFolderFilter.IsEmpty())
				sFolderFilter += "&";
			sFolderFilter += "(" + sStr + ")";
		}

		sStr.Empty();
		MakeFilterSub(sStr, sOption, "TITLE", "=", token, FALSE);
		MakeFilterSub(sStr, sOption, "CREATOR", "=", token, FALSE);
		MakeFilterSub(sStr, sOption, "CODE", "=", token, FALSE);
		MakeFilterSub(sStr, sOption, "REMARK1", "=", token, FALSE);
		MakeFilterSub(sStr, sOption, "REMARK2", "=", token, FALSE);
		MakeFilterSub(sStr, sOption, TEXT_FIELD, "=", token, FALSE);

		if (!sStr.IsEmpty()) {
			if (!sDataFilter.IsEmpty())
				sDataFilter += "&";
			sDataFilter += "(" + sStr + ")";
		}

		token = strtok(NULL, " ");
	}
}

static void MakeFilterSub(CString &sStr, LPCSTR pOption, LPCSTR pFieldName, LPCSTR pOperation, LPCSTR pFind, BOOL bAnd)
{

	if (*pFind != '\0') {
		if (!sStr.IsEmpty())
			sStr += bAnd ? "&" : "|";
		if (pOption == NULL)
			pOption = "";

		CString sTemp;
		sTemp.Format("%s%s%s'%s'", pOption, pFieldName, pOperation, pFind);
		sStr += sTemp;
	}
}

static CString MakeDateBefore(const CString &sDays)
{
	CString sStr;

	if (!sDays.IsEmpty()) {
		SYSTEMTIME st;
		GetLocalTime(&st);
		CTime time(st);
		CTimeSpan span(atoi(sDays), 0, 0, 0);
		time -= span;
		sStr.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	}

	return sStr;
}

void MakeID(char prefix, DWORD nNumber, CString &sID)
{
	char buf[8];

	for (int i = 6; i >= 0; i--) {
		const int n = (nNumber & 0x1f);
		if (n < 10)
			buf[i] = (char)(n + 0x30);
		else
			buf[i] = (char)(n - 10 + 0x41);

		nNumber >>= 5;
	}

	buf[7] = '\0';

	sID.Format("%c%s", prefix, buf);
}

int GetPictureBitCount(HDIB hDIB)
{
	LPBITMAPINFOHEADER lpBmi;

	if ((lpBmi = (LPBITMAPINFOHEADER)::GlobalLock(hDIB)) == NULL) {
		::GlobalFree(hDIB);
		return 0;
	}

	const int nBitCount = lpBmi->biBitCount;

	::GlobalUnlock(hDIB);

	return nBitCount;
}

short GetMMType(HDIB hDIB)
{
	long nSizePng = 0, nSizeJpg = 0;

	MMPictureWriteNull(hDIB, MMTYPE_PNG, &nSizePng);
	MMPictureWriteNull(hDIB, MMTYPE_JPG, &nSizeJpg);

	if (nSizePng != 0 && nSizePng < nSizeJpg)
		return MMTYPE_PNG;
	else if (nSizeJpg != 0)
		return MMTYPE_JPG;
	else
		return MMTYPE_BMP;
}

BOOL MakeTempFile(CItem *pItem, CString &sFileName, int nSupportFile, BOOL bReadIniFile, BOOL bDeleteTemp)
{
	CWaitCursor wait;
	CItemFolder *pItemFolder;
	CItemData *pItemData;
	CItemFind *pItemFind;
	char tempPath[MAX_PATH];
	CString sTitle;
	CString sTempFile;

	DeleteTempFile(g_sTempFile);
	g_sTempFile.Empty();

	::GetTempPath(sizeof(tempPath), tempPath);
	size_t len = strlen(tempPath);
	if (tempPath[len - 1] != '\\')
		strcat(tempPath, "\\");

	for ( ; pItem != NULL; pItem = pItem->m_pNextSelected) {
		switch (pItem->m_nItemType) {
		case ITEM_FOLDER:
			pItemFolder = (CItemFolder *)pItem;
			if (pItemFolder->m_nFolderType != FTYPE_DFOLDER)
				goto error;
			sTitle = pItemFolder->m_oFolderRec.sFolderName;
			CheckFileName(sTitle);
			sTempFile.Format("%s%s.mml", tempPath, (LPCSTR)sTitle);
			if (!MakeMmlFile(pItemFolder, sTempFile, bReadIniFile))
				continue;
			sFileName += sTempFile + ";";
			break;
		case ITEM_DATA:
			pItemData = (CItemData *)pItem;
			sTitle = pItemData->m_oDataRec.sTitle;
			CheckFileName(sTitle);
			sTempFile.Format("%s%s", tempPath, (LPCSTR)sTitle);
			if (!MakePictFile(pItemData, sTempFile, nSupportFile, bReadIniFile))
				continue;
			sFileName += sTempFile + ";";
			break;
		case ITEM_FIND:
			pItemFind = (CItemFind *)pItem;
			if (pItemFind->m_nFindType == FIND_TYPE_FOLDER)
				goto error;
			sTitle = pItemFind->m_oFindRec.sFolderName;
			CheckFileName(sTitle);
			sTempFile.Format("%s%s.mml", tempPath, (LPCSTR)sTitle);
			if (!MakeMmlFile(pItemFind, sTempFile, bReadIniFile))
				continue;
			sFileName += sTempFile + ";";
			break;
		default:
		error:
			return FALSE;
		}
	}

	if (bDeleteTemp)
		g_sTempFile = sFileName;

	return TRUE;
}

BOOL MakeMmlFile(CItem *pItem, LPCSTR pFileName, BOOL bReadIniFile)
{
	FOLDERREC *pFolderRec;
	FINDREC *pFindRec;
	CItemFolder *pItemFolder;
	CItemFind *pItemFind;
	CItemData *pItemData;
	DATAREC *pDataRec;
	LPCSTR pPict;
	long nSize;
	CString sText;

	switch (pItem->m_nItemType) {
	case ITEM_FOLDER:
		if (((CItemFolder *)pItem)->m_nFolderType != FTYPE_DFOLDER)
			return FALSE;
		break;
	case ITEM_FIND:
		if (((CItemFind *)pItem)->m_nFindType == FIND_TYPE_FOLDER)
			return FALSE;
		break;
	default:
		return FALSE;
	}

	CMmlWrite mml;
	if (!mml.Create(pFileName))
		return FALSE;

	switch (pItem->m_nItemType) {
	case ITEM_FOLDER:
		pItemFolder = (CItemFolder *)pItem;
		pFolderRec = &pItemFolder->m_oFolderRec;
		mml.WriteItem("FNAME", (LPCSTR)pFolderRec->sFolderName, pFolderRec->sFolderName.GetLength());
		mml.WriteItem("CDATE", (LPCSTR)pFolderRec->sCreateDate, pFolderRec->sCreateDate.GetLength());
		mml.WriteItem("UDATE", (LPCSTR)pFolderRec->sUpdateDate, pFolderRec->sUpdateDate.GetLength());
		mml.WriteItem("CREATOR", (LPCSTR)pFolderRec->sCreator, pFolderRec->sCreator.GetLength());
		mml.WriteItem("REMARK1", (LPCSTR)pFolderRec->sRemark, pFolderRec->sRemark.GetLength());
		mml.WriteItem("REMARK2", (LPCSTR)pFolderRec->sKeyword, pFolderRec->sKeyword.GetLength());
		if (!pItemFolder->m_pPcsDB->ReadTextFolderEx(pItemFolder->m_oFolderRec.sFolderID, &sText))
			return FALSE;
		if (sText.GetLength() != 0)
			mml.WriteItem("TEXT", (LPCSTR)sText, sText.GetLength());
		break;
	case ITEM_FIND:
		pItemFind = (CItemFind *)pItem;
		pFindRec = &pItemFind->m_oFindRec;
		mml.WriteItem("FNAME", (LPCSTR)pFindRec->sFolderName, pFindRec->sFolderName.GetLength());
		break;
	default:
		return FALSE;
	}

	CPictFile *pPictFile = new CPictFile;
	if (bReadIniFile)
		pPictFile->ReadIniFile();

	for (HTREEITEM hItem = g_pTreeCtrl->GetChildItem(pItem->m_hItem); hItem != NULL;
				hItem = g_pTreeCtrl->GetNextSiblingItem(hItem)) {
		pItem = (CItem *)g_pTreeCtrl->GetItemData(hItem);
		if (pItem->m_nItemType != ITEM_DATA)
			continue;

		pItemData = (CItemData *)pItem;
		if ((pPict = pPictFile->ReadPict(pItemData, PICTFILE_ALL, &nSize)) == NULL)
			continue;

		mml.NextPage();

		pDataRec = &pItemData->m_oDataRec;
		mml.WriteItem("CDATE", (LPCSTR)pDataRec->sCreateDate, pDataRec->sCreateDate.GetLength());
		mml.WriteItem("UDATE", (LPCSTR)pDataRec->sUpdateDate, pDataRec->sUpdateDate.GetLength());
		mml.WriteItem("CREATOR", (LPCSTR)pDataRec->sCreator, pDataRec->sCreator.GetLength());
		mml.WriteItem("CODE", (LPCSTR)pDataRec->sCode, pDataRec->sCode.GetLength());
		mml.WriteItem("TITLE", (LPCSTR)pDataRec->sTitle, pDataRec->sTitle.GetLength());
		mml.WriteItem("REMARK1", (LPCSTR)pDataRec->sRemark, pDataRec->sRemark.GetLength());
		mml.WriteItem("REMARK2", (LPCSTR)pDataRec->sKeyword, pDataRec->sKeyword.GetLength());

		if (!pItemData->m_pPcsDB->ReadTextDataEx(pItemData->m_oDataRec.sDataID, &sText))
			return FALSE;
		if (sText.GetLength() != 0)
			mml.WriteItem("TEXT", (LPCSTR)sText, sText.GetLength());

		mml.WriteItem(PICT1_FIELD, pPict, nSize);
	}

	delete pPictFile;

	return TRUE;
}

BOOL MakePictFile(CItemData *pItemData, CString &sTempFile, int nSupportFile, BOOL bReadIniFile)
{
	LPCSTR pPict;
	long nSize;

	nSupportFile &= PICTFILE_DEFAULT;

	CPictFile *pPictFile = new CPictFile;
	if (bReadIniFile)
		pPictFile->ReadIniFile();

	if ((pPict = pPictFile->ReadPict(pItemData, nSupportFile, &nSize)) == NULL) {
		delete pPictFile;
		return FALSE;
	}

	CString sExt;
	switch (*(SHORT *)pPict) {
	case MMTYPE_GIF:
		sExt = ".gif";
		break;
	case MMTYPE_JPG:
		sExt = ".jpg";
		break;
	case MMTYPE_BMP:
		sExt = ".bmp";
		break;
	case MMTYPE_PNG:
		sExt = ".png";
		break;
	default:
		delete pPictFile;
		return FALSE;
	}

	if (sExt.CompareNoCase(sTempFile.Right(4)) != 0)
		sTempFile += sExt;

	CFile file;
	if (!file.Open(sTempFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone)) {
		delete pPictFile;
		return FALSE;
	}
	file.Write(pPict + sizeof(SHORT), nSize - sizeof(SHORT));
	file.Close();
	::SetFileAttributes(sTempFile, FILE_ATTRIBUTE_NORMAL);

	delete pPictFile;

	return TRUE;
}

void DeleteTempFile(CString &sFileName)
{
	CString sTempFile;

	for (;;) {
		sTempFile = sFileName.SpanExcluding(";");
		if (sTempFile.IsEmpty())
			break;
		::DeleteFile(sTempFile);
		sFileName.Delete(0, sTempFile.GetLength());
		if (!sFileName.IsEmpty() && sFileName.GetAt(0) == ';')
			sFileName.Delete(0, 1);
	}
}

void CheckFileName(CString &sFileName)
{
	int n;

	while ((n = sFileName.FindOneOf(":<>\\/*?| ")) != -1)
		sFileName.SetAt(n, '_');
}

int CheckFileType(LPCSTR pFileName)
{
	LPCSTR p;

	if ((p = strrchr(pFileName, '.')) == NULL)
		return 0;

	if (_stricmp(p, ".mml") == 0)
		return SRCFILETYPE_MML;

	if (_stricmp(p, ".mmdb") == 0)
		return SRCFILETYPE_MMDB;

	if (_stricmp(p, ".bmp") == 0 || _stricmp(p, ".gif") == 0 || _stricmp(p, ".jpg") == 0 || _stricmp(p, ".png") == 0)
		return SRCFILETYPE_PICT;

	if (_stricmp(p, ".pdf") == 0)
		return SRCFILETYPE_PDF;

	return 0;
}

void GetLibraryDir(CString &sDir)
{
	CWinApp *pWinApp = ::AfxGetApp();

	sDir = pWinApp->GetProfileString(g_sSectionOptions, "LibraryFolder");
	if (sDir.IsEmpty()) {
		GetDefaultLibraryDir(sDir);
		pWinApp->WriteProfileString(g_sSectionOptions, "LibraryFolder", sDir);
	}
}

void GetDefaultLibraryDir(CString &sDir)
{
	GetMmlibDir(sDir, "LIBRARY");
}

void GetCacheDir(CString &sDir)
{
	CWinApp *pWinApp = ::AfxGetApp();

	sDir = pWinApp->GetProfileString(g_sSectionOptions, "CacheFolder");
	if (sDir.IsEmpty()) {
		GetDefaultCacheDir(sDir);
		pWinApp->WriteProfileString(g_sSectionOptions, "CacheFolder", sDir);
	}
}

void GetDefaultCacheDir(CString &sDir)
{
	GetMmlibDir(sDir, "CACHE");
}

void GetBackupDir(CString &sDir)
{
	CWinApp *pWinApp = ::AfxGetApp();

	sDir = pWinApp->GetProfileString(g_sSectionOptions, "BackupFolder");
	if (sDir.IsEmpty()) {
		GetDefaultBackupDir(sDir);
		pWinApp->WriteProfileString(g_sSectionOptions, "BackupFolder", sDir);
	}
}

void GetDefaultBackupDir(CString &sDir)
{
	GetMmlibDir(sDir, "BACKUP");
}

void GetInboxDir(CString &sDir)
{
	CWinApp *pWinApp = ::AfxGetApp();

	sDir = pWinApp->GetProfileString(g_sSectionOptions, "InboxFolder");
	if (sDir.IsEmpty()) {
		GetDefaultInboxDir(sDir);
		pWinApp->WriteProfileString(g_sSectionOptions, "InboxFolder", sDir);
	}
}

void GetDefaultInboxDir(CString &sDir)
{
	GetMmlibDir(sDir, "INBOX");
}

void GetKeywordDir(CString &sDir)
{
	CWinApp *pWinApp = ::AfxGetApp();

	sDir = pWinApp->GetProfileString(g_sSectionOptions, "KeywordFolder");
	if (sDir.IsEmpty()) {
		GetDefaultKeywordDir(sDir);
		pWinApp->WriteProfileString(g_sSectionOptions, "KeywordFolder", sDir);
	}
}

void GetDefaultKeywordDir(CString &sDir)
{
	GetMmlibDir(sDir, "KEYWORD");
}

void GetAccessLogDir(CString &sDir)
{
	CWinApp *pWinApp = ::AfxGetApp();

	sDir = pWinApp->GetProfileString(g_sSectionOptions, "AccessLogFolder");
	if (sDir.IsEmpty()) {
		GetDefaultAccessLogDir(sDir);
		pWinApp->WriteProfileString(g_sSectionOptions, "AccessLogFolder", sDir);
	}
}

void GetDefaultAccessLogDir(CString &sDir)
{
	GetMmlibDir(sDir, "LOG");
}

static void GetFolder(int nFolder, TCHAR *pPath)
{
	HRESULT	(CALLBACK* pfnSHGetFolderPath)(HWND, int, HANDLE, DWORD, LPSTR) = NULL;

	HMODULE hDLL = ::LoadLibrary("shell32.dll");
	if (hDLL != NULL)
		*(FARPROC *)&pfnSHGetFolderPath = ::GetProcAddress(hDLL, "SHGetFolderPathA");

	if (pfnSHGetFolderPath == NULL) {
		if (hDLL != NULL)
			::FreeLibrary(hDLL);

		hDLL = ::LoadLibrary("shfolder.dll");
		if (hDLL != NULL)
			*(FARPROC *)&pfnSHGetFolderPath = ::GetProcAddress(hDLL, "SHGetFolderPathA");
	}

	if (pfnSHGetFolderPath != NULL)
		pfnSHGetFolderPath(NULL, nFolder, NULL, 0, pPath);
	else
		strcpy_s(pPath, MAX_PATH, "C:\\");

	if (hDLL != NULL)
		::FreeLibrary(hDLL);
}

void GetMmlibDir(CString &sDir, LPCSTR pSubDir)
{
	LPSTR pDir = sDir.GetBuffer(MAX_PATH);

	GetFolder(CSIDL_APPDATA, pDir);
	::PathAppend(pDir, "YMEC\\MMLIB");

	if (pSubDir != NULL)
		::PathAppend(pDir, pSubDir);

	sDir.ReleaseBuffer();
}

int SetLibraryComboBox(CComboBox &oComboBox)
{
	int nCount = 0;

	HTREEITEM hItem = g_pTreeCtrl->GetChildItem(g_pTreeCtrl->GetRootItem());
	for (int i = 0; hItem != NULL; i++) {
		CItemFolder *pItemFolder = (CItemFolder *)g_pTreeCtrl->GetItemData(hItem);
		if (pItemFolder->m_nItemType == ITEM_FOLDER) {
			const int nIndex = oComboBox.AddString(pItemFolder->m_oFolderRec.sFolderName);
			oComboBox.SetItemDataPtr(nIndex, pItemFolder);
			nCount++;
		}

		hItem = g_pTreeCtrl->GetNextSiblingItem(hItem);
	}

	return nCount;
}

void GetNewFolder(LPCSTR pDir, LPCSTR pPrefix, CString &sPath)
{
	struct _stat st;

	for (int i = 0; ; i++) {
		sPath.Format("%s\\%s%d", pDir, pPrefix,  i);
		if (_stat(sPath, &st) != 0)
			break;
	}
}

void DeleteDirectory(LPCSTR pDirName)
{
	WIN32_FIND_DATA	oFindBuf;

	CString sDir = pDirName;

	HANDLE hdir = FindFirstFile(sDir + "\\*", &oFindBuf);
	if (hdir != INVALID_HANDLE_VALUE) {
		CString sPath;
		do {
			sPath = sDir + "\\" + oFindBuf.cFileName;
			if (oFindBuf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (strcmp(oFindBuf.cFileName, ".") != 0 && strcmp(oFindBuf.cFileName, "..") != 0)
					DeleteDirectory(sPath);
			} else
				::DeleteFile(sPath);
		} while (FindNextFile(hdir, &oFindBuf));

		FindClose(hdir);
	}

	RemoveDirectory(pDirName);
}

int CompareTitle(const CString &sStr1, const CString &sStr2)
{
	const int n1 = FindLastNum(sStr1);
	const int n2 = FindLastNum(sStr2);

	if (n1 != -1 && n1 == n2) {
		int n = sStr1.Left(n1).CompareNoCase(sStr2.Left(n2));
		if (n == 0) {
			n = atoi(sStr1.Mid(n1)) - atoi(sStr2.Mid(n2));
			if (n == 0)
				n = sStr1.CompareNoCase(sStr2);
		}
		return n;
	} else
		return sStr1.CompareNoCase(sStr2);
}

int FindLastNum(const CString &sStr)
{
	int const len = sStr.GetLength();
	for (int i = len; i > 0; i--) {
		const int c = sStr[i - 1];
		if (c < '0' || c > '9') {
			return i == len ? -1 : i;
		}
	}
	return 0;
}

CString ConvFileTime(const FILETIME *pFileTime)
{
	SYSTEMTIME st;
	CString sStr;

	FileTimeToSystemTime(pFileTime, &st);

	sStr.Format("%04d%02d%02d%02d%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
	return sStr;
}

CString RemoveDir(const CString &sPathName)
{
	CString sStr;

	const int n = sPathName.ReverseFind('\\');
	if (n != -1)
		sStr = sPathName.Mid(n + 1);
	else
		sStr = sPathName;

	return sStr;
}

CString RemoveExt(const CString &sFileName)
{
	int n;
	CString sStr;

	if ((n = sFileName.ReverseFind('.')) != -1)
		sStr = sFileName.Left(n);
	else
		sStr = sFileName;

	return sStr;
}

CItemInbox *OpenInbox()
{
	CMMPcsDB *pPcsDB;
	FOLDERREC oFolderRec;
	CString sDir;

	::GetInboxDir(sDir);

	g_bNoDispError = TRUE;
	if ((pPcsDB = g_pMMLibDB->OpenMMLibrary(CHT_LOCAL, "", sDir, FALSE)) != NULL) {
		pPcsDB->ReadLibraryEx(&oFolderRec);
	} else {
		CreateDirectoryAll(sDir);
		oFolderRec.sFolderType =  FTYPE_LIBRARY;
		oFolderRec.sFolderName.LoadString(IDS_INBOX);
		oFolderRec.sCreator = GetDefaultUserName();
		oFolderRec.sRemark = "1";
		oFolderRec.sKeyword.Empty();
		if ((pPcsDB = g_pMMLibDB->CreateMMLibrary(CHT_LOCAL, "", sDir, FALSE, &oFolderRec)) == NULL) {
			::AfxMessageBox(IDS_CREATE_INBOX_FAIL);
			return NULL;
		}
	}
	g_bNoDispError = FALSE;

	// 旧インボックスの処置
	if (oFolderRec.sRemark.IsEmpty()) {
		FOLDERREC oFolderRec2;
		CString sFolderID;
		if (pPcsDB->ReadChildFolderEx(FID_LIBRARY, &oFolderRec2, TRUE)) {
			if (oFolderRec2.sFolderType == FTYPE_GFOLDER) {
				oFolderRec.sFolderName = oFolderRec2.sFolderName;
				oFolderRec.sRemark = "1";
				pPcsDB->UpdateFolderEx(&oFolderRec);

				sFolderID = oFolderRec2.sFolderID;
				while (pPcsDB->ReadChildFolderEx(sFolderID, &oFolderRec2, TRUE)) {
					oFolderRec2.sParentFolderID = oFolderRec.sFolderID;
					pPcsDB->UpdateFolderEx(&oFolderRec2);
				}

				pPcsDB->DeleteFolderEx(sFolderID);
			}
		}
	}

	CItemInbox *pItemInbox = new CItemInbox(pPcsDB);
	pItemInbox->m_nFolderType = FTYPE_GFOLDER;
	pItemInbox->m_oFolderRec = oFolderRec;

	return pItemInbox;
}

void WritePicture(CMMPcsDB *pPcsDB, LPCSTR pDataID, HDIB hDIB)
{
	HDIB hDIB2;

	if (hDIB != NULL)
		hDIB2 = MakeSmallPicture(hDIB, PICT_WIDTH, PICT_HEIGHT);
	else
		hDIB2 = NULL;

	pPcsDB->WritePictureDataEx(pDataID, hDIB, hDIB2, GetMMType(hDIB));
}

CString GetDefaultUserName()
{
	CString userName;
	DWORD userNameSize = MAXUSERNAME;

	::GetUserName(userName.GetBuffer(MAXUSERNAME), &userNameSize);
	userName.ReleaseBuffer();

	return userName;
}

CString &GetString(UINT nID, int nIndex)
{
	static CString sStr[2];

	sStr[nIndex].LoadString(nID);
	return sStr[nIndex];
}

void AutoNextNumbering(CString &sCode, int nStep)
{
	static constexpr char number[] = "0123456789";
	int n;

	if ((n = sCode.FindOneOf(number)) == -1)
		return;

	CString format;
	long i, nMax;

	CString sStr = sCode.Mid(n).SpanIncluding(number);
	const int len = sStr.GetLength();
	for (i = 0, nMax = 1; i < len; i++)
		nMax *= 10;
	const long nNum = (atol(sStr) + nStep) % nMax;

	format.Format("%%0%du", len);
	sStr.Format(format, nNum);
	sCode.Delete(n, len);
	sCode.Insert(n, sStr);
}

void ExecProcess(LPCSTR pCommand)
{
	PROCESS_INFORMATION	piProcInfo;

	STARTUPINFO	siStartupInfo{
		.cb = sizeof(siStartupInfo)
	};
	CreateProcess(NULL, (LPSTR)pCommand, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, ".", &siStartupInfo, &piProcInfo);
	CloseHandle(piProcInfo.hThread);
	WaitForSingleObject(piProcInfo.hProcess, INFINITE);
	CloseHandle(piProcInfo.hProcess);
}

void DispTrialVersion(CWnd *pWnd, CDC *pDC)
{
	static constexpr char text[] = "Trial Version";
	CFont font;
	font.CreatePointFont(280, "Impact");
	CFont *pFontOrg = pDC->SelectObject(&font);
	const int nBkModeOrg = pDC->SetBkMode(TRANSPARENT);
	COLORREF nColorOrg = pDC->SetTextColor(RGB(128, 240, 128));
	SIZE size = pDC->GetTextExtent(text);
	RECT rect;
	pWnd->GetClientRect(&rect);
	pDC->TextOut((rect.right - size.cx) / 2, (rect.bottom - size.cy) / 2, text);
	pDC->SetTextColor(nColorOrg);
	pDC->SetBkMode(nBkModeOrg);
	pDC->SelectObject(pFontOrg);
}

void NotifyUpdate()
{
	CString sWindowText;

	CWnd *pWnd = CWnd::GetDesktopWindow()->GetWindow(GW_CHILD);
	while (pWnd != NULL) {
		pWnd->GetWindowText(sWindowText);
		if (sWindowText == "Multimedia Library Manager" && pWnd->m_hWnd != ::AfxGetMainWnd()->m_hWnd)
			pWnd->PostMessage(WM_NOTIFY_UPDATE);

		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
}

#define GetBMIHEAD(a) ((LPBITMAPINFOHEADER)(a))
#define GetRGBQUAD(a) ((RGBQUAD*)(a + sizeof(BITMAPINFOHEADER)))

void GetPictFactor(HDIB hDIB, double *factor_r, double *factor_g, double *factor_b)
{
	double fSumR = 0;
	double fSumG = 0;
	double fSumB = 0;

	BYTE *buff = (LPBYTE)::GlobalLock(hDIB);
	LPBITMAPINFOHEADER pbi = GetBMIHEAD(buff);
	for (int y = 0; y < pbi->biHeight; y++) {
		for (int x = 0; x < pbi->biWidth; x++) {
			COLORREF rgb = GetDIBPixel(buff, x, y);
			fSumR += GetRValue(rgb);
			fSumG += GetGValue(rgb);
			fSumB += GetBValue(rgb);
		}
	}

	::GlobalUnlock(hDIB);

	const double fTemp = (double)pbi->biWidth * pbi->biHeight * 255;

	*factor_r = fSumR / fTemp;
	*factor_g = fSumG / fTemp;
	*factor_b = fSumB / fTemp;
}

//---------------------------------------------------------------------------
// DIBの横の列のサイズを得る
//---------------------------------------------------------------------------
LONG GetLineSize(const int Width, const int BitCount)
{
	switch(BitCount) {
	case 1:
		return (((Width + 31) & 0xFFFFFFE0) / 8); // 2色
	case 8:
		return ((Width + 3) & 0xFFFFFFFc); // 256色
	case 24:
		return ((Width * 3 + 3) & 0xFFFFFFFc); // 24bit
	}
	return 0;
}

//---------------------------------------------------------------------------
//    ピクセルのCOLOR取得
//---------------------------------------------------------------------------
COLORREF GetDIBPixel(const BYTE *buff, const int x, const int y)
{
	int offset;
	LPBITMAPINFOHEADER pbi = GetBMIHEAD(buff);
	RGBQUAD *palette = GetRGBQUAD(buff);
	const BYTE *data = buff + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (pbi->biBitCount <= 8 ? 1 << pbi->biBitCount : 0);
	const BYTE *line = (const BYTE *)(data + GetLineSize(pbi->biWidth, pbi->biBitCount) * (pbi->biHeight - y - 1));
	switch(GetBMIHEAD(buff)->biBitCount) {
	case 1:
		offset = ((line[x / 8] & (1 << (7 - x % 8))) != 0) ? 1 : 0;
		return RGB(palette[offset].rgbRed, palette[offset].rgbGreen, palette[offset].rgbBlue);
	case 8:
		offset = line[x];
		return RGB(palette[offset].rgbRed, palette[offset].rgbGreen, palette[offset].rgbBlue);
	case 24:
		offset = x * 3;
		return RGB(line[offset + 2], line[offset + 1], line[offset]);
	}
	return 0;
}

void StrSplit(LPCSTR pText, LPCSTR pSeparator, CStringArray &saText)
{
	saText.RemoveAll();

	int nPos = 0;
	CString sText(pText);
	CString sToken = sText.Tokenize(pSeparator, nPos);

	while (sToken != "") {
		saText.Add(sToken);
		sToken = sText.Tokenize(pSeparator, nPos);
	}
}

void StrJoin(const CStringArray &saText, LPCSTR pSeparator, CString &sText)
{
	sText.Empty();

	for (int i = 0; i < saText.GetSize(); i++) {
		if (i != 0)
			sText += pSeparator;

		sText += saText[i];
	}
}

CString IntToCommaSeparateString(int n)
{
	int i = 0;		// 書き込んだ桁数
	CString sStr;
	char *p = sStr.GetBuffer(100);
	unsigned int  u = abs(n);

	do // while( 0 != u );
	{
		*p++ = "0123456789"[u%10];
		u = u / 10;
		i++;

		if ((0 != u) && (0 == (i % 3)))
			*p++ = ',';
	} while(0 != u);

	if (n < 0)
		*p++ = '-';
	*p = '\0';

	sStr.ReleaseBuffer();

	return sStr.MakeReverse();
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM /*lParam*/, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
		::SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);

	return 0;
}

void UpdateProgressBar(CProgressCtrl &oProgressCtrl, int nPos)
{
	int nLow, nUpper;

	oProgressCtrl.GetRange(nLow, nUpper);
	if (nPos < nUpper) {
		oProgressCtrl.SetPos(nPos + 1);
		oProgressCtrl.SetPos(nPos);
	} else {
		oProgressCtrl.SetRange32(nLow, nUpper + 1);
		oProgressCtrl.SetPos(nPos + 1);
		oProgressCtrl.SetPos(nPos);
		oProgressCtrl.SetRange32(nLow, nUpper);
	}
}
