// RestoreDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "RestoreDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	int nIndex;
	CMMPcsDB *pPcsDB;
	CString *pBackupPath;

	if ((pos = m_cBackupList.GetFirstSelectedItemPosition()) == NULL) {
		::AfxMessageBox(IDS_SELECT_RECTORE_LIBRARY);
		return;
	}

	if (::AfxMessageBox(IDS_RESTORE_LIBRARY, MB_OKCANCEL | MB_ICONQUESTION) != IDOK)
		return;

	if ((pPcsDB = CreateRestoreLibrary()) == NULL) {
		::AfxMessageBox(IDS_RESTORE_FAIL);
		return;
	}

	nIndex = m_cBackupList.GetNextSelectedItem(pos);
	pBackupPath = (CString *)m_cBackupList.GetItemData(nIndex);

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
	int index1, index2;

	LV_FINDINFO findInfo1{
		.flags = LVFI_PARAM,
		.lParam = lParam1
	};
	index1 = dataList.FindItem(&findInfo1);

	LV_FINDINFO findInfo2{
		.flags = LVFI_PARAM,
		.lParam = lParam2
	};
	index2 = dataList.FindItem(&findInfo2);

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

	int nIndex = m_cBackupList.GetNextSelectedItem(pos);

	CString *pBackupPath = (CString *)m_cBackupList.GetItemData(nIndex);
	::DeleteDirectory(*pBackupPath);

	SetBackupList();
}

void CRestoreDlg::SetBackupList()
{
	CString dir;
	WIN32_FIND_DATA	oFindBuf;
	int	nIndex = 0;
	CString sLibraryName;
	CString sBackupPath;
	CString *pBackupPath;

	::GetBackupDir(dir);

	RemoveBackupItem();

	HANDLE hdir = FindFirstFile(dir + "\\*", &oFindBuf);
	if (hdir != INVALID_HANDLE_VALUE) {
		do {
			if (oFindBuf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				 sBackupPath = dir + "\\" + oFindBuf.cFileName;
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
		} while (FindNextFile(hdir, &oFindBuf));

		FindClose(hdir);
	}

	m_cBackupList.SortItems(CompareItems, (LPARAM)this);

	m_cDelete.EnableWindow(nIndex != 0);
}

CMMPcsDB *CRestoreDlg::CreateRestoreLibrary()
{
	CMMPcsDB *pPcsDB;
	FOLDERREC oFolderRec;
	CString sLibraryDir;
	CString sLibraryPath;

	::GetLibraryDir(sLibraryDir);
	::GetNewFolder(sLibraryDir, "MMLIB", sLibraryPath);
	CreateDirectoryAll(sLibraryPath);

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
	int nCount = m_cBackupList.GetItemCount();

	for (int i = 0; i < nCount; i++)
		delete (CString *)m_cBackupList.GetItemData(i);

	m_cBackupList.DeleteAllItems();
}
