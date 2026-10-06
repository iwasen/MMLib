// LibOpenDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "LibOpenDlg.h"
#include "RefNetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CLibOpenDlg ダイアログ

CLibOpenDlg::CLibOpenDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CLibOpenDlg::IDD, pParent)
{
	m_iChType = -1;
	m_bCache = FALSE;
}

void CLibOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIBRARY_LIST, m_cLibraryList);
	DDX_Control(pDX, IDC_EXTERNAL, m_cExternal);
	DDX_Control(pDX, IDC_SEPARATOR, m_cSeparator);
	DDX_Control(pDX, IDC_DIRECTORY, m_cDirectory);
	DDX_Control(pDX, IDC_SERVER_NAME, m_cServerName);
	DDX_Control(pDX, IDC_CACHE, m_cCache);
	DDX_Text(pDX, IDC_DIRECTORY, m_sDirectory);
	DDX_Radio(pDX, IDC_LOCAL, m_iChType);
	DDX_Text(pDX, IDC_SERVER_NAME, m_sServerName);
	DDX_Check(pDX, IDC_CACHE, m_bCache);
}

BEGIN_MESSAGE_MAP(CLibOpenDlg, CDialogAF)
	ON_BN_CLICKED(IDC_TCPIP, OnTcpip)
	ON_BN_CLICKED(IDC_NETBIOS, OnNetbios)
	ON_BN_CLICKED(IDC_LOCAL, OnLocal)
	ON_BN_CLICKED(IDC_REFERENCE, OnReference)
	ON_BN_CLICKED(IDC_EXTERNAL, OnExternal)
	ON_LBN_SELCHANGE(IDC_LIBRARY_LIST, OnSelchangeLibraryList)
	ON_WM_DESTROY()
	ON_LBN_DBLCLK(IDC_LIBRARY_LIST, OnDblclkLibraryList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLibOpenDlg メッセージ ハンドラ

BOOL CLibOpenDlg::OnInitDialog()
{
	CWinApp *pWinApp = ::AfxGetApp();
	CString sEntry;
	CString sValue;
	int i;

	m_iChType = 0;

	CDialogAF::OnInitDialog();

	CRect rectSeparator;
	GetWindowRect(m_rectDlg);
	m_cSeparator.GetWindowRect(rectSeparator);
	SetWindowPos(NULL, 0, 0, m_rectDlg.Width(), rectSeparator.top - m_rectDlg.top, SWP_NOZORDER | SWP_NOMOVE);

	SetLibraryList();

	for (i = 0; i < MAX_RECENT; i++) {
		sEntry.Format("Server%d", i);
		sValue = pWinApp->GetProfileString(g_sSectionRecent, sEntry);
		if (sValue.IsEmpty())
			break;
		m_cServerName.AddString(sValue);
	}

	for (i = 0; i < MAX_RECENT; i++) {
		sEntry.Format("Directory%d", i);
		sValue = pWinApp->GetProfileString(g_sSectionRecent, sEntry);
		if (sValue.IsEmpty())
			break;
		m_cDirectory.AddString(sValue);
	}

	OnLocal();

	return TRUE;
}

void CLibOpenDlg::OnLocal()
{
	m_cServerName.EnableWindow(FALSE);
	m_cCache.SetCheck(0);
	m_cCache.EnableWindow(FALSE);
}

void CLibOpenDlg::OnTcpip()
{
	m_cServerName.EnableWindow(TRUE);
	m_cCache.EnableWindow(TRUE);
}

void CLibOpenDlg::OnNetbios()
{
	m_cServerName.EnableWindow(TRUE);
	m_cCache.EnableWindow(TRUE);
}

void CLibOpenDlg::OnOK()
{
	CWinApp *pWinApp = ::AfxGetApp();
	int nCount;
	CString sEntry;
	CString sValue;
	int i, n;

	UpdateData(TRUE);

	n = 0;
	if (!m_sServerName.IsEmpty()) {
		pWinApp->WriteProfileString(g_sSectionRecent, "Server0", m_sServerName);
		n++;
	}
	nCount = m_cServerName.GetCount();
	for (i = 0; i < nCount && n < MAX_RECENT; i++) {
		m_cServerName.GetLBText(i, sValue);
		if (sValue.CompareNoCase(m_sServerName) != 0) {
			sEntry.Format("Server%d", n++);
			pWinApp->WriteProfileString(g_sSectionRecent, sEntry, sValue);
		}
	}

	n = 0;
	if (!m_sDirectory.IsEmpty()) {
		pWinApp->WriteProfileString(g_sSectionRecent, "Directory0", m_sDirectory);
		n++;
	}
	nCount = m_cDirectory.GetCount();
	for (i = 0; i < nCount && n < MAX_RECENT; i++) {
		m_cDirectory.GetLBText(i, sValue);
		if (sValue.CompareNoCase(m_sDirectory) != 0) {
			sEntry.Format("Directory%d", n++);
			pWinApp->WriteProfileString(g_sSectionRecent, sEntry, sValue);
		}
	}

	CDialogAF::OnOK();
}

void CLibOpenDlg::OnReference()
{
	UpdateData(TRUE);

	switch (m_iChType) {
	case CHT_LOCAL:
		ReferenceLocal();
		break;
	case CHT_TCPIP:
	case CHT_NETBIOS:
		ReferenceNetwork();
		break;
	}
}

void CLibOpenDlg::ReferenceLocal()
{
    LPITEMIDLIST pIDList;
	char buf[_MAX_PATH];
	CString sTitle;

	sTitle.LoadString(IDS_MMLIB_FOLDER);
	buf[0] = '\0';
    // BROWSEINFO構造体に値を設定
	BROWSEINFO bInfo{
		.hwndOwner = m_hWnd,				// ダイアログの親ウインドウのハンドル
		.pidlRoot = NULL,					// ルートフォルダを示すITEMIDLISTのポインタ (NULLの場合デスクトップフォルダが使われます）
		.pszDisplayName = buf,					// 選択されたフォルダ名を受け取るバッファのポインタ
		.lpszTitle = sTitle,				// ツリービューの上部に表示される文字列 
		.ulFlags = BIF_RETURNONLYFSDIRS,	// 表示されるフォルダの種類を示すフラグ
		.lpfn = BrowseCallbackProc,	// BrowseCallbackProc関数のポインタ
		.lParam = (LPARAM)buf				// コールバック関数に渡す値
	};

    // フォルダ選択ダイアログを表示
    pIDList = ::SHBrowseForFolder(&bInfo);
    if(pIDList != NULL){
        // ItemIDListをパス名に変換します
        ::SHGetPathFromIDList(pIDList, buf);

        // 最後にpIDListのポイントしているメモリを解放します
        ::CoTaskMemFree( pIDList );
		m_sDirectory = buf;

		UpdateData(FALSE);
    }
/*
	CFileDialog fileDlg(TRUE, NULL, "*", OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_NOVALIDATE, NULL, this);

	fileDlg.m_ofn.lpstrTitle = GetString(IDS_MMLIB_FOLDER);
	if (fileDlg.DoModal() == IDOK) {
		m_sDirectory = fileDlg.GetPathName();
		int pos = m_sDirectory.ReverseFind('\\');
		if (pos != -1)
			m_sDirectory = m_sDirectory.Mid(0, pos);

		UpdateData(FALSE);
	}
*/
}

void CLibOpenDlg::ReferenceNetwork()
{
	CRefNetDlg dlg;

	UpdateData(TRUE);

	if (m_sServerName.IsEmpty()) {
		::AfxMessageBox(IDS_INPUT_SERVER_NAME);
		return;
	}

	dlg.m_sServerName = m_sServerName;
	dlg.m_nChType = m_iChType;
	if (dlg.DoModal() == IDOK) {
		m_sDirectory = dlg.m_sDirectory;
		UpdateData(FALSE);
	}
}

void CLibOpenDlg::OnExternal()
{
	SetWindowPos(NULL, 0, 0, m_rectDlg.Width(), m_rectDlg.Height(), SWP_NOZORDER | SWP_NOMOVE);
	m_cExternal.EnableWindow(FALSE);
}

void CLibOpenDlg::SetLibraryList()
{
	CString sDir;
	WIN32_FIND_DATA	oFindBuf;
	CString sLibraryName;
	CString sLibraryPath;
	CString *pLibraryPath;

	::GetLibraryDir(sDir);

	HANDLE hDir = FindFirstFile(sDir + "\\*", &oFindBuf);
	if (hDir != INVALID_HANDLE_VALUE) {
		do {
			if (oFindBuf.dwFileAttributes && FILE_ATTRIBUTE_DIRECTORY) {
				 sLibraryPath = sDir + "\\" + oFindBuf.cFileName;
				if (g_pMMLibDB->GetLibraryName(sLibraryPath, sLibraryName)) {
					const int nIndex = m_cLibraryList.AddString(sLibraryName);
					pLibraryPath = new CString;
					*pLibraryPath = sLibraryPath;
					m_cLibraryList.SetItemDataPtr(nIndex, pLibraryPath);
				}
			}
		} while (FindNextFile(hDir, &oFindBuf));

		FindClose(hDir);
	}
}

void CLibOpenDlg::OnSelchangeLibraryList()
{
	int nIndex;

	if ((nIndex = m_cLibraryList.GetCurSel()) == LB_ERR)
		return;

	m_iChType = 0;
	m_sServerName.Empty();
	CString *pLibraryPath = (CString *)m_cLibraryList.GetItemDataPtr(nIndex);
	m_sDirectory = *pLibraryPath;
	m_bCache = FALSE;

	UpdateData(FALSE);

	OnLocal();
}

void CLibOpenDlg::OnDestroy()
{
	CDialogAF::OnDestroy();

	CString *pLibraryPath;

	while (m_cLibraryList.GetCount() > 0) {
		pLibraryPath = (CString *)m_cLibraryList.GetItemDataPtr(0);
		delete pLibraryPath;
		m_cLibraryList.DeleteString(0);
	}
}

void CLibOpenDlg::OnDblclkLibraryList()
{
	OnOK();
}
