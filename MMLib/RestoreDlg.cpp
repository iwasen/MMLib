// RestoreDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "RestoreDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CRestoreDlg ダイアログ

CRestoreDlg::CRestoreDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CRestoreDlg::IDD, pParent)
{
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。

	m_nSortItem = 0;
}

void CRestoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_cCancel);
	DDX_Control(pDX, IDC_PROGRESS1, m_cProgress);
	DDX_Control(pDX, IDC_DELETE, m_cDelete);
	DDX_Control(pDX, IDC_BACKUP_LIST, m_cBackupList);
}

BEGIN_MESSAGE_MAP(CRestoreDlg, CDialogAF)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_BACKUP_LIST, OnColumnclickBackupList)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRestoreDlg メッセージ ハンドラ

BOOL CRestoreDlg::OnInitDialog()
{
	CDialogAF::OnInitDialog();

	m_cBackupList.InsertColumn(0, GetString(IDS_LIBRARY_NAME), LVCFMT_LEFT, CDpi::AdjustDpi(220, 86));
	m_cBackupList.InsertColumn(1, GetString(IDS_BACKUP_TIME), LVCFMT_LEFT, CDpi::AdjustDpi(125));

	SetBackupList();

	return TRUE;
}

void CRestoreDlg::OnOK()
{
	POSITION pos;
	if ((pos = m_cBackupList.GetFirstSelectedItemPosition()) == NULL) {
		::AfxMessageBox(IDS_SELECT_RECTORE_LIBRARY);
		return;
	}

	if (::AfxMessageBox(IDS_RESTORE_LIBRARY, MB_OKCANCEL | MB_ICONQUESTION) != IDOK)
		return;

	CMMPcsDB *pPcsDB;
	if ((pPcsDB = CreateRestoreLibrary()) == NULL) {
		::AfxMessageBox(IDS_RESTORE_FAIL);
		return;
	}

	int nIndex = m_cBackupList.GetNextSelectedItem(pos);
	CString *pBackupPath = (CString *)m_cBackupList.GetItemData(nIndex);

	if (pPcsDB->RestoreEx(*pBackupPath, &m_cProgress)) {
		g_pMMLibDB->CloseMMLibrary(pPcsDB);
		m_cCancel.SetWindowText(GetString(IDS_CLOSE));
		::AfxMessageBox(IDS_RESTORE_NORMAL);
	} else {
		g_pMMLibDB->DeleteMMLibrary(pPcsDB);
		::AfxMessageBox(IDS_RESTORE_FAIL);
	}

	m_cProgress.SetPos(0);
}

void CRestoreDlg::OnColumnclickBackupList(NMHDR* pNMHDR, LRESULT* pResult)
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

int CALLBACK CRestoreDlg::CompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CRestoreDlg *pDlg = (CRestoreDlg *)lParamSort;
	CListCtrl &dataList = pDlg->m_cBackupList;

	LV_FINDINFO findInfo1{
		.flags = LVFI_PARAM,
		.lParam = lParam1
	};
	int index1 = dataList.FindItem(&findInfo1);

	LV_FINDINFO findInfo2{
		.flags = LVFI_PARAM,
		.lParam = lParam2
	};
	int index2 = dataList.FindItem(&findInfo2);

	return dataList.GetItemText(index1, pDlg->m_nSortItem).Compare(dataList.GetItemText(index2, pDlg->m_nSortItem)) * pDlg->m_nSortDir;
}

void CRestoreDlg::OnDelete()
{
	POSITION pos;
	if ((pos = m_cBackupList.GetFirstSelectedItemPosition()) == NULL) {
		::AfxMessageBox(IDS_SELECT_DELETE_LIBRARY);
		return;
	}

	if (::AfxMessageBox(IDS_DELETE_SELECTED_LIBRARY, MB_OKCANCEL | MB_ICONQUESTION) != IDOK)
		return;

	const int nIndex = m_cBackupList.GetNextSelectedItem(pos);

	CString *pBackupPath = (CString *)m_cBackupList.GetItemData(nIndex);
	::DeleteDirectory(*pBackupPath);

	SetBackupList();
}

void CRestoreDlg::SetBackupList()
{
	CString dir;
	::GetBackupDir(dir);

	RemoveBackupItem();

	int	nIndex = 0;
	WIN32_FIND_DATA	oFindBuf;
	HANDLE hdir = FindFirstFile(dir + "\\*", &oFindBuf);
	if (hdir != INVALID_HANDLE_VALUE) {
		CString sLibraryName;
		CString sBackupPath;
		do {
			if (oFindBuf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				 sBackupPath = dir + "\\" + oFindBuf.cFileName;
				if (g_pMMLibDB->GetLibraryName(sBackupPath, sLibraryName)) {
					m_cBackupList.InsertItem(nIndex, sLibraryName);

					CTime time(oFindBuf.ftCreationTime);
					m_cBackupList.SetItemText(nIndex, 1, time.Format("%Y/%m/%d %X"));

					CString *pBackupPath = new CString;
					*pBackupPath = sBackupPath;
					m_cBackupList.SetItemData(nIndex, (DWORD_PTR)pBackupPath);
					nIndex++;
				}
			}
		} while (FindNextFile(hdir, &oFindBuf));

		FindClose(hdir);
	}

	m_cBackupList.SortItems(CompareItems, (LPARAM)this);

	m_cDelete.EnableWindow(nIndex != 0);
}

CMMPcsDB *CRestoreDlg::CreateRestoreLibrary()
{
	CString sLibraryDir;
	::GetLibraryDir(sLibraryDir);
	CString sLibraryPath;
	::GetNewFolder(sLibraryDir, "MMLIB", sLibraryPath);
	CreateDirectoryAll(sLibraryPath);

	CMMPcsDB *pPcsDB;
	FOLDERREC oFolderRec;
	if ((pPcsDB = g_pMMLibDB->CreateMMLibrary(CHT_LOCAL, "", sLibraryPath, CACHEMODE_DIRECT, &oFolderRec)) == NULL)
		return NULL;

	return pPcsDB;
}

void CRestoreDlg::OnDestroy()
{
	CDialogAF::OnDestroy();

	RemoveBackupItem();
}

void CRestoreDlg::RemoveBackupItem()
{
	const int nCount = m_cBackupList.GetItemCount();

	for (int i = 0; i < nCount; i++)
		delete (CString *)m_cBackupList.GetItemData(i);

	m_cBackupList.DeleteAllItems();
}
