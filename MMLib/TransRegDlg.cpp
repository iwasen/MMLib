// TransRegDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "TransRegDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CTransRegDlg ダイアログ

CTransRegDlg::CTransRegDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CTransRegDlg::IDD, pParent)
{
	m_iMenuIcon = -1;
	m_bPictBMP = TRUE;
	m_bPictGIF = FALSE;
	m_bPictJPG = TRUE;
	m_bPictPNG = TRUE;
}

void CTransRegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_APP_LIST, m_cAppList);
	DDX_Text(pDX, IDC_APP_FILE, m_sAppFile);
	DDX_Text(pDX, IDC_APP_NAME, m_sAppName);
	DDX_Radio(pDX, IDC_ICON_NONE, m_iMenuIcon);
	DDX_Check(pDX, IDC_PICT_BMP, m_bPictBMP);
	DDX_Check(pDX, IDC_PICT_GIF, m_bPictGIF);
	DDX_Check(pDX, IDC_PICT_JPG, m_bPictJPG);
	DDX_Check(pDX, IDC_PICT_PNG, m_bPictPNG);
}

BEGIN_MESSAGE_MAP(CTransRegDlg, CDialogAF)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_BN_CLICKED(IDC_REFERENCE, OnReference)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_APP_LIST, OnItemchangedAppList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransRegDlg メッセージ ハンドラ

BOOL CTransRegDlg::OnInitDialog()
{
	CWinApp *pWinApp = ::AfxGetApp();
	m_iMenuIcon = pWinApp->GetProfileInt(g_sSectionTransfer, "MenuIcon", 1);

	CDialogAF::OnInitDialog();

	m_cAppList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_cAppList.InsertColumn(0, GetString(IDS_APPLICATION_NAME), LVCFMT_LEFT, CDpi::AdjustDpi(110));
	m_cAppList.InsertColumn(1, GetString(IDS_EXECUTABLE_FILE), LVCFMT_LEFT, CDpi::AdjustDpi(300, 86));

	SetAppList();

	return TRUE;
}

void CTransRegDlg::OnOK()
{
	int i;

	UpdateData(TRUE);

	if (m_sAppName.IsEmpty()) {
		::AfxMessageBox(IDS_INPUT_APPLICATION_NAME);
		return;
	}

	if (m_sAppFile.IsEmpty()) {
		::AfxMessageBox(IDS_INPUT_EXECUTABLE_FILE);
		return;
	}

	const int n = m_cAppList.GetItemCount();
	for (i = 0; i < n; i++) {
		if (m_cAppList.GetItemText(i, 0) == m_sAppName)
			break;
	}
	if (i == n)
		m_cAppList.InsertItem(i, m_sAppName);
	m_cAppList.SetItemText(i, 1, m_sAppFile);

	int nSupportFile = 0;
	if (m_bPictBMP)
		nSupportFile |= PICTFILE_BMP;
	if (m_bPictGIF)
		nSupportFile |= PICTFILE_GIF;
	if (m_bPictJPG)
		nSupportFile |= PICTFILE_JPG;
	if (m_bPictPNG)
		nSupportFile |= PICTFILE_PNG;
	m_cAppList.SetItemData(i, nSupportFile);
}

void CTransRegDlg::OnDelete()
{
	if (m_cAppList.GetSelectedCount() == 0) {
		::AfxMessageBox(IDS_SELECT_DELETE_ITEM);
		return;
	}

	if (::AfxMessageBox(IDS_DELETE_SELECT_ITEM, MB_OKCANCEL + MB_ICONQUESTION) == IDCANCEL)
		return;

	const int nItem = m_cAppList.GetItemCount();
	for (int i = nItem - 1; i >= 0; i--) {
		if (m_cAppList.GetItemState(i, LVIS_SELECTED) != 0)
			m_cAppList.DeleteItem(i);
	}
}

void CTransRegDlg::OnClose()
{
	CWinApp *pWinApp = ::AfxGetApp();
	CString sEntry;

	UpdateData(TRUE);

	const int nItem = m_cAppList.GetItemCount();

	pWinApp->WriteProfileInt(g_sSectionTransfer, "Num", nItem);

	for (int i = 0; i < nItem; i++) {
		sEntry.Format("AppName%d", i);
		pWinApp->WriteProfileString(g_sSectionTransfer, sEntry, m_cAppList.GetItemText(i, 0));

		sEntry.Format("AppFile%d", i);
		pWinApp->WriteProfileString(g_sSectionTransfer, sEntry, m_cAppList.GetItemText(i, 1));

		sEntry.Format("SupportFile%d", i);
		pWinApp->WriteProfileInt(g_sSectionTransfer, sEntry, (int)m_cAppList.GetItemData(i));
	}

	pWinApp->WriteProfileInt(g_sSectionTransfer, "MenuIcon", m_iMenuIcon);

	EndDialog(IDOK);
}

void CTransRegDlg::OnReference()
{
	CFileDialog dlg(TRUE, "*.exe", NULL,
			OFN_HIDEREADONLY,
			GetString(IDS_PROGRAM) + " (*.exe)|*.exe|" + GetString(IDS_ALL_FILES, 1) + " (*.*)|*.*||", ::AfxGetMainWnd());

	dlg.m_ofn.lpstrTitle = GetString(IDS_SELECT_TRANSFER_APPL);
	if (dlg.DoModal() == IDOK) {
		UpdateData(TRUE);
		m_sAppFile = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CTransRegDlg::SetAppList()
{
	CWinApp *pWinApp = ::AfxGetApp();
	CString sEntry;
	CString sAppName, sAppFile;
	int nSupportFile;

	const int n = pWinApp->GetProfileInt(g_sSectionTransfer, "Num", 0);
	for (int i = 0; i < n; i++) {
		sEntry.Format("AppName%d", i);
		sAppName = pWinApp->GetProfileString(g_sSectionTransfer, sEntry);

		sEntry.Format("AppFile%d", i);
		sAppFile = pWinApp->GetProfileString(g_sSectionTransfer, sEntry);

		sEntry.Format("SupportFile%d", i);
		nSupportFile = pWinApp->GetProfileInt(g_sSectionTransfer, sEntry, PICTFILE_TRANSDEF);

		m_cAppList.InsertItem(i, sAppName);
		m_cAppList.SetItemText(i, 1, sAppFile);
		m_cAppList.SetItemData(i, nSupportFile);
	}
}

void CTransRegDlg::OnItemchangedAppList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->uNewState & LVIS_SELECTED) {
		m_sAppName = m_cAppList.GetItemText(pNMListView->iItem, 0);
		m_sAppFile = m_cAppList.GetItemText(pNMListView->iItem, 1);
		const int nSupportFile = (int)m_cAppList.GetItemData(pNMListView->iItem);
		m_bPictBMP = ((nSupportFile & PICTFILE_BMP) != 0);
//		m_bPictGIF = ((nSupportFile & PICTFILE_GIF) != 0);
		m_bPictJPG = ((nSupportFile & PICTFILE_JPG) != 0);
		m_bPictPNG = ((nSupportFile & PICTFILE_PNG) != 0);
		UpdateData(FALSE);
	}

	*pResult = 0;
}
