// BackupDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "BackupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBackupDlg ダイアログ

CBackupDlg::CBackupDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CBackupDlg::IDD, pParent)
{
	m_pItemFolder = NULL;
	m_nSortItem = 0;
}

void CBackupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DELETE, m_cDelete);
	DDX_Control(pDX, IDCANCEL, m_cCancel);
	DDX_Control(pDX, IDC_BACKUP_LIST, m_cBackupList);
	DDX_Control(pDX, IDC_LIBRARY, m_cLibrary);
	DDX_Control(pDX, IDC_PROGRESS, m_cProgress);
}

BEGIN_MESSAGE_MAP(CBackupDlg, CDialogAF)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_BACKUP_LIST, OnColumnclickBackupList)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackupDlg メッセージ ハンドラ

BOOL CBackupDlg::OnInitDialog()
{
	CDialogAF::OnInitDialog();

	int n = SetLibraryComboBox(m_cLibrary);
	if (n == 1)
		m_cLibrary.SetCurSel(0);
	else if (n != 0) {
		for (int i = 0; i < n; i++) {
			if (m_cLibrary.GetItemDataPtr(i) == m_pItemFolder) {
				m_cLibrary.SetCurSel(i);
				break;
			}
		}
	}

	m_cBackupList.InsertColumn(0, GetString(IDS_LIBRARY_NAME), LVCFMT_LEFT, CDpi::AdjustDpi(225, 95));
	m_cBackupList.InsertColumn(1, GetString(IDS_BACKUP_TIME), LVCFMT_LEFT, CDpi::AdjustDpi(125));

	SetBackupList();

	return TRUE;
}

void CBackupDlg::OnOK()
{
	int nIndex;
	if ((nIndex = m_cLibrary.GetCurSel()) == LB_ERR) {
		::AfxMessageBox(IDS_SELECT_LIBRARY);
		return;
	}

	CString sBackupDir;
	CString sBackupPath;

	::GetBackupDir(sBackupDir);
	::GetNewFolder(sBackupDir, "BAKUP", sBackupPath);

	CItemFolder *pItemFolder = (CItemFolder *)m_cLibrary.GetItemDataPtr(nIndex);

	if (pItemFolder->m_pPcsDB->BackupEx(sBackupPath, &m_cProgress)) {
		SetBackupList();
		m_cCancel.SetWindowText(GetString(IDS_CLOSE));
		::AfxMessageBox(IDS_BACKUP_NORMAL);
	} else {
		::DeleteDirectory(sBackupPath);
		::AfxMessageBox(IDS_BACKUP_FAIL);
	}

	m_cProgress.SetPos(0);
}

void CBackupDlg::SetBackupList()
{
	WIN32_FIND_DATA	oFindBuf;
	int	nIndex = 0;
	CString sLibraryName;
	CString sBackupPath;
	CString *pBackupPath;

	CString sDir;
	::GetBackupDir(sDir);

	RemoveBackupItem();

	HANDLE hDir = FindFirstFile(sDir + "\\*", &oFindBuf);
	if (hDir != INVALID_HANDLE_VALUE) {
		do {
			if (oFindBuf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				 sBackupPath = sDir + "\\" + oFindBuf.cFileName;
				if (g_pMMLibDB->GetLibraryName(sBackupPath, sLibraryName)) {
					m_cBackupList.InsertItem(nIndex, sLibraryName);

					CTime time(oFindBuf.ftCreationTime);
					m_cBackupList.SetItemText(nIndex, 1, time.Format("%Y/%m/%d %X"));

					pBackupPath = new CString;
					*pBackupPath = sBackupPath;
					m_cBackupList.SetItemData(nIndex, (DWORD_PTR)pBackupPath);
					nIndex++;
				}
			}
		} while (FindNextFile(hDir, &oFindBuf));

		FindClose(hDir);
	}

	m_cBackupList.SortItems(CompareItems, (LPARAM)this);

	m_cDelete.EnableWindow(nIndex != 0);
}

void CBackupDlg::OnDelete()
{
	POSITION pos;
	if ((pos = m_cBackupList.GetFirstSelectedItemPosition()) == NULL) {
		::AfxMessageBox(IDS_SELECT_DELETE_LIBRARY);
		return;
	}

	if (::AfxMessageBox(IDS_DELETE_SELECTED_LIBRARY, MB_OKCANCEL | MB_ICONQUESTION) != IDOK)
		return;

	int nIndex = m_cBackupList.GetNextSelectedItem(pos);
	CString *pBackupPath = (CString *)m_cBackupList.GetItemData(nIndex);
	::DeleteDirectory(*pBackupPath);

	SetBackupList();
}

void CBackupDlg::OnColumnclickBackupList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (m_nSortItem != pNMListView->iSubItem)
		m_nSortDir = 1;
	else
		m_nSortDir *= -1;

	m_nSortItem = pNMListView->iSubItem;
	m_cBackupList.SortItems(CompareItems, (LPARAM)this);

	*pResult = 0;
}

int CALLBACK CBackupDlg::CompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CBackupDlg *pDlg = (CBackupDlg *)lParamSort;
	CListCtrl &oDataList = pDlg->m_cBackupList;
	LV_FINDINFO findInfo;

	findInfo.flags = LVFI_PARAM;
	findInfo.lParam = lParam1;
	int nIndex1 = oDataList.FindItem(&findInfo);

	findInfo.flags = LVFI_PARAM;
	findInfo.lParam = lParam2;
	int nIndex2 = oDataList.FindItem(&findInfo);

	return oDataList.GetItemText(nIndex1, pDlg->m_nSortItem).Compare(oDataList.GetItemText(nIndex2, pDlg->m_nSortItem)) * pDlg->m_nSortDir;
}

void CBackupDlg::OnDestroy()
{
	CDialogAF::OnDestroy();

	RemoveBackupItem();
}

void CBackupDlg::RemoveBackupItem()
{
	int nCount = m_cBackupList.GetItemCount();
	for (int i = 0; i < nCount; i++)
		delete (CString *)m_cBackupList.GetItemData(i);

	m_cBackupList.DeleteAllItems();
}
