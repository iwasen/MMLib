// RefNetDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "RefNetDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CRefNetDlg ダイアログ

CRefNetDlg::CRefNetDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CRefNetDlg::IDD, pParent)
{
	m_nItem = 0;
}

void CRefNetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIBLIST, m_cLibList);
}

BEGIN_MESSAGE_MAP(CRefNetDlg, CDialogAF)
	ON_LBN_DBLCLK(IDC_LIBLIST, OnDblclkLiblist)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRefNetDlg メッセージ ハンドラ

BOOL CRefNetDlg::OnInitDialog()
{
	CDialogAF::OnInitDialog();

	if (!SetLibList())
		EndDialog(IDCANCEL);

	if (m_nItem == 0) {
		::AfxMessageBox(IDS_NOT_FOUND_LIBRARY);
		EndDialog(IDCANCEL);
	}

	return TRUE;
}

void CRefNetDlg::OnOK()
{
	int nIndex;

	if ((nIndex = m_cLibList.GetCurSel()) == LB_ERR) {
		::AfxMessageBox(IDS_SELECT_LIBRARY);
		return;
	}

	m_sDirectory = "%" + m_sPath[nIndex];

	CDialogAF::OnOK();
}

BOOL CRefNetDlg::SetLibList()
{
	CWaitCursor wait;

	CPcserve oPcserve;
	if (oPcserve.Connect(m_nChType, m_sServerName, ".MMLIB") != 0) {
		::AfxMessageBox(IDS_CONNECT_SERVER_FAIL);
		return FALSE;
	}

	CPSDB dir;
	if (dir.DBOpen(oPcserve, "_DIR.DBF") != 0)
		return TRUE;

	int nIndexNo;
	if (dir.DBIndex("_DIR.NDX", &nIndexNo) != 0)
		return TRUE;

	int nFind;
	if (dir.DBSearch2("MMLIB", 5, &nFind) != 0)
		return TRUE;

	if (nFind == 0)
		return TRUE;

	struct {
		char dirName[8];
		char directory[128];
	} dirBuf;
	struct dirRec {
		CString dirName;
		CString directory;
	} dirRec;
	for (;;) {
		int nRead;
		if (dir.DBReadNext(1, &dirBuf, &nRead) != 0)
			break;

		if (nRead == 0)
			break;

		if (memcmp(dirBuf.dirName, "MMLIB" , 5) != 0)
			break;

		if (dir.DBGetFieldBuf(&dirBuf, &dirRec) != 0)
			break;

		AddRefList(oPcserve, dirRec.dirName);
	}

	return TRUE;
}

void CRefNetDlg::AddRefList(CPcserve &oPcserve, CString &sDirName)
{
	CWaitCursor wait;

	CString pathName;
	pathName.Format("%%%s\\%s", (LPCSTR)sDirName, FN_FOLDER_DBF);
	CPSDB oFolderDB;
	if (oFolderDB.DBOpen(oPcserve, pathName) != 0)
		return;

	::AfxGetApp()->DoWaitCursor(0);

	int nIndexNo;
	pathName.Format("%%%s\\%s", (LPCSTR)sDirName, FN_FOLDER_NDX1);
	if (oFolderDB.DBIndex(pathName, &nIndexNo) != 0)
		return;

	int nLength;
	if (oFolderDB.DBSetReadField("FNAME", &nLength) != 0)
		return;

	int find;
	char folderName[LEN_FOLDERNAME + 1];
	if (oFolderDB.DBReadKey(FID_LIBRARY, LEN_ID, folderName, 0, &find) != 0)
		return;

	folderName[LEN_FOLDERNAME] = '\0';
	CString libName = folderName;
	libName.TrimRight();

	m_cLibList.InsertString(m_nItem, libName);
	m_sPath[m_nItem++] = sDirName;
}

void CRefNetDlg::OnDblclkLiblist()
{
	OnOK();
}
