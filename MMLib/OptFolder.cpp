// OptFolder.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "OptFolder.h"

/////////////////////////////////////////////////////////////////////////////
// COptFolder プロパティ ページ

COptFolder::COptFolder() : CPropertyPage(COptFolder::IDD)
{
}

void COptFolder::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BACKUP_FOLDER, m_sBackupFolder);
	DDX_Text(pDX, IDC_CACHE_FOLDER, m_sCacheFolder);
	DDX_Text(pDX, IDC_LIBRARY_FOLDER, m_sLibraryFolder);
	DDX_Text(pDX, IDC_INBOX_FOLDER, m_sInboxFolder);
	DDX_Text(pDX, IDC_KEYWORD_FOLDER, m_sKeywordFolder);
	DDX_Text(pDX, IDC_ACCESS_LOG_FOLDER, m_sAccessLogFolder);
}

BEGIN_MESSAGE_MAP(COptFolder, CPropertyPage)
	ON_BN_CLICKED(IDC_LIBRARY_REFERENCE, OnLibraryReference)
	ON_BN_CLICKED(IDC_CACHE_REFERENCE, OnCacheReference)
	ON_BN_CLICKED(IDC_BACKUP_REFERENCE, OnBackupReference)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_INBOX_REFERENCE, OnInboxReference)
	ON_BN_CLICKED(IDC_KEYWORD_REFERENCE, OnKeywordReference)
	ON_BN_CLICKED(IDC_ACCESS_LOG_REFERENCE, OnAccessLogReference)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptFolder メッセージ ハンドラ

BOOL COptFolder::OnInitDialog()
{
	::GetLibraryDir(m_sLibraryFolder);
	::GetCacheDir(m_sCacheFolder);
	::GetBackupDir(m_sBackupFolder);
	::GetInboxDir(m_sInboxFolder);
	::GetKeywordDir(m_sKeywordFolder);
	::GetAccessLogDir(m_sAccessLogFolder);

	CPropertyPage::OnInitDialog();

	return TRUE;
}

void COptFolder::OnOK()
{
	CWinApp *pWinApp = ::AfxGetApp();

	UpdateData(TRUE);

	pWinApp->WriteProfileString(g_sSectionOptions, "LibraryFolder", m_sLibraryFolder);
	pWinApp->WriteProfileString(g_sSectionOptions, "CacheFolder", m_sCacheFolder);
	pWinApp->WriteProfileString(g_sSectionOptions, "BackupFolder", m_sBackupFolder);
	pWinApp->WriteProfileString(g_sSectionOptions, "InboxFolder", m_sInboxFolder);
	pWinApp->WriteProfileString(g_sSectionOptions, "KeywordFolder", m_sKeywordFolder);
	pWinApp->WriteProfileString(g_sSectionOptions, "AccessLogFolder", m_sAccessLogFolder);

	CPropertyPage::OnOK();
}

void COptFolder::OnLibraryReference()
{
	UpdateData(TRUE);
	SelectFolder(m_sLibraryFolder, IDS_SELECT_LIBRARY_FOLDER);
	UpdateData(FALSE);
}

void COptFolder::OnCacheReference()
{
	UpdateData(TRUE);
	SelectFolder(m_sCacheFolder, IDS_SELECT_CACHE_FOLDER);
	UpdateData(FALSE);
}

void COptFolder::OnBackupReference()
{
	UpdateData(TRUE);
	SelectFolder(m_sBackupFolder, IDS_SELECT_BACKUP_FOLDER);
	UpdateData(FALSE);
}

void COptFolder::OnDefault()
{
	::GetDefaultLibraryDir(m_sLibraryFolder);
	::GetDefaultCacheDir(m_sCacheFolder);
	::GetDefaultBackupDir(m_sBackupFolder);
	::GetDefaultInboxDir(m_sInboxFolder);
	::GetDefaultKeywordDir(m_sKeywordFolder);
	::GetDefaultAccessLogDir(m_sAccessLogFolder);

	UpdateData(FALSE);
}

void COptFolder::OnInboxReference()
{
	UpdateData(TRUE);
	SelectFolder(m_sInboxFolder, IDS_SELECT_INBOX_FOLDER);
	UpdateData(FALSE);
}

void COptFolder::OnKeywordReference()
{
	UpdateData(TRUE);
	SelectFolder(m_sKeywordFolder, IDS_SELECT_KEYWORD_FOLDER);
	UpdateData(FALSE);
}

void COptFolder::OnAccessLogReference()
{
	UpdateData(TRUE);
	SelectFolder(m_sAccessLogFolder, IDS_SELECT_ACCESS_LOG_FOLDER);
	UpdateData(FALSE);
}

void COptFolder::SelectFolder(CString &sFolder, int nTitleId)
{
	char dst_file[MAX_PATH];
	LPITEMIDLIST idlist;
	CString sTitle;

	sTitle.LoadString(nTitleId);
	strcpy_s(dst_file, sFolder);
	BROWSEINFO binfo{
		.hwndOwner = m_hWnd,
		.pidlRoot = NULL,
		.pszDisplayName = dst_file,
		.lpszTitle = sTitle,
		.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI,
		.lpfn = &BrowseCallbackProc,
		.lParam = (LPARAM)dst_file,
		.iImage = (int)NULL
	};
	idlist = ::SHBrowseForFolder(&binfo);
	if (idlist) {
		::SHGetPathFromIDList(idlist, dst_file);
		::CoTaskMemFree(idlist);
		sFolder = dst_file;
	}
}
