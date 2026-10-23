// LibNewDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "LibNewDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CLibNewDlg ダイアログ

CLibNewDlg::CLibNewDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CLibNewDlg::IDD, pParent)
{
	m_iChType = -1;
	m_bCache = FALSE;
}

void CLibNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEPARATOR, m_cSeparator);
	DDX_Control(pDX, IDC_EXTERNAL, m_cExternal);
	DDX_Control(pDX, IDC_REFERENCE, m_cReference);
	DDX_Control(pDX, IDC_SERVER_NAME, m_cServerName);
	DDX_Control(pDX, IDC_DIRECTORY, m_cDirectory);
	DDX_Control(pDX, IDC_CACHE, m_cCache);
	DDX_Text(pDX, IDC_CREATOR, m_sCreator);
	DDX_Text(pDX, IDC_DIRECTORY, m_sDirectory);
	DDX_Text(pDX, IDC_LIBNAME, m_sLibName);
	DDX_Radio(pDX, IDC_LOCAL, m_iChType);
	DDX_Text(pDX, IDC_SERVER_NAME, m_sServerName);
	DDX_Text(pDX, IDC_REMARK, m_sRemark);
	DDX_Text(pDX, IDC_KEYWORD, m_sKeyword);
	DDX_Check(pDX, IDC_CACHE, m_bCache);
}

BEGIN_MESSAGE_MAP(CLibNewDlg, CDialogAF)
	ON_BN_CLICKED(IDC_LOCAL, OnLocal)
	ON_BN_CLICKED(IDC_TCPIP, OnTcpip)
	ON_BN_CLICKED(IDC_NETBIOS, OnNetbios)
	ON_BN_CLICKED(IDC_REFERENCE, OnReference)
	ON_BN_CLICKED(IDC_EXTERNAL, OnExternal)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLibNewDlg メッセージ ハンドラ

BOOL CLibNewDlg::OnInitDialog()
{
	CWinApp *pWinApp = ::AfxGetApp();
	CString sEntry;
	CString sValue;
	int i;

	DWORD userNameSize = MAXUSERNAME;

	m_iChType = 0;
	GetUserName(m_sCreator.GetBuffer(MAXUSERNAME), &userNameSize);
	m_sCreator.ReleaseBuffer();

	CDialogAF::OnInitDialog();

	RECT rectSeparator;
	GetWindowRect(m_rectDlg);
	m_cSeparator.GetWindowRect(&rectSeparator);
	SetWindowPos(NULL, 0, 0, m_rectDlg.Width(), rectSeparator.top - m_rectDlg.top, SWP_NOZORDER | SWP_NOMOVE);

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

	CString sLibraryDir, defaultPath;
	::GetLibraryDir(sLibraryDir);
	::GetNewFolder(sLibraryDir, "MMLIB", defaultPath);
	m_cDirectory.SetWindowText(defaultPath);

	m_cServerName.EnableWindow(FALSE);
	m_cCache.EnableWindow(FALSE);

	return TRUE;
}

void CLibNewDlg::OnLocal()
{
	m_cServerName.EnableWindow(FALSE);
	m_cCache.EnableWindow(FALSE);
	m_cReference.EnableWindow(TRUE);
}

void CLibNewDlg::OnTcpip()
{
	m_cServerName.EnableWindow(TRUE);
	m_cCache.EnableWindow(TRUE);
	m_cReference.EnableWindow(FALSE);
}

void CLibNewDlg::OnNetbios()
{
	m_cServerName.EnableWindow(TRUE);
	m_cCache.EnableWindow(TRUE);
	m_cReference.EnableWindow(FALSE);
}

void CLibNewDlg::OnOK()
{
	CWinApp *pWinApp = ::AfxGetApp();
	int nCount;
	CString sEntry;
	CString sValue;
	int i, j;

	UpdateData(TRUE);

	pWinApp->WriteProfileString(g_sSectionRecent, "Server0", m_sServerName);
	nCount = m_cServerName.GetCount();
	for (i = 0, j = 1; i < nCount && j < MAX_RECENT; i++) {
		m_cServerName.GetLBText(i, sValue);
		if (sValue.CompareNoCase(m_sServerName) != 0) {
			sEntry.Format("Server%d", j++);
			pWinApp->WriteProfileString(g_sSectionRecent, sEntry, sValue);
		}
	}

	pWinApp->WriteProfileString(g_sSectionRecent, "Directory0", m_sDirectory);
	nCount = m_cDirectory.GetCount();
	for (i = 0, j = 1; i < nCount && j < MAX_RECENT; i++) {
		m_cDirectory.GetLBText(i, sValue);
		if (sValue.CompareNoCase(m_sDirectory) != 0) {
			sEntry.Format("Directory%d", j++);
			pWinApp->WriteProfileString(g_sSectionRecent, sEntry, sValue);
		}
	}

	CDialogAF::OnOK();
}

void CLibNewDlg::OnReference()
{
	CFileDialog fileDlg(TRUE, NULL, "*", OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_NOVALIDATE, NULL, this);

	fileDlg.m_ofn.lpstrTitle = GetString(IDS_MMLIB_FOLDER);
	if (fileDlg.DoModal() == IDCANCEL)
		return;

	UpdateData(TRUE);

	m_sDirectory = fileDlg.GetPathName();
	int pos = m_sDirectory.ReverseFind('\\');
	if (pos != -1)
		m_sDirectory = m_sDirectory.Mid(0, pos);

	UpdateData(FALSE);
}

void CLibNewDlg::OnExternal()
{
	SetWindowPos(NULL, 0, 0, m_rectDlg.Width(), m_rectDlg.Height(), SWP_NOZORDER | SWP_NOMOVE);
	m_cExternal.EnableWindow(FALSE);
}
