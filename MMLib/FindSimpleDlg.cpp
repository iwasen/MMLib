// FindSimpleDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "FindSimpleDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CFindSimpleDlg ダイアログ

CFindSimpleDlg::CFindSimpleDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CFindSimpleDlg::IDD, pParent)
{
	m_bSave = FALSE;
	m_pItemParent = NULL;
	m_bUpdateMode = FALSE;
}

void CFindSimpleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FIND_EXPERT, m_cFindExpert);
	DDX_Control(pDX, IDC_FINDNUM, m_cFindNum);
	DDX_Control(pDX, IDC_SEARCH_TEXT, m_cSearchText);
	DDX_Control(pDX, IDC_LIBRARY, m_cLibrary);
	DDX_Text(pDX, IDC_FIND_FOLDER, m_sFindFolder);
	DDX_Check(pDX, IDC_SAVE, m_bSave);
	DDX_Text(pDX, IDC_SEARCH_TEXT, m_sSearchText);
}

BEGIN_MESSAGE_MAP(CFindSimpleDlg, CDialogAF)
	ON_BN_CLICKED(IDC_FIND_EXPERT, OnFindExpert)
	ON_BN_CLICKED(IDC_GETFINDNUM, OnGetfindnum)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindSimpleDlg メッセージ ハンドラ

BOOL CFindSimpleDlg::OnInitDialog()
{
	CDialogAF::OnInitDialog();

	BOOL bSelectedLibrary = FALSE;

	SetLibraryCombo();
	const int n = m_cLibrary.GetCount();
	if (n == 1) {
		m_cLibrary.SetCurSel(0);
		bSelectedLibrary = TRUE;
	} else if (n != 0) {
		for (int i = 0; i < n; i++) {
			if (m_cLibrary.GetItemDataPtr(i) == m_pItemParent) {
				m_cLibrary.SetCurSel(i);
				bSelectedLibrary = TRUE;
				break;
			}
		}
	}

	if (m_bUpdateMode) {
		m_cLibrary.EnableWindow(FALSE);
		m_cFindExpert.EnableWindow(FALSE);
	} else
		m_sFindFolder.LoadString(IDS_FIND_FOLDER);

	UpdateData(FALSE);

	if (bSelectedLibrary) {
		m_cSearchText.SetFocus();
		return FALSE;
	} else
		return TRUE;
}

void CFindSimpleDlg::OnOK()
{
	int nIndex;

	if ((nIndex = m_cLibrary.GetCurSel()) == LB_ERR) {
		::AfxMessageBox(IDS_SELECT_LIBRARY);
		return;
	}

	m_pItemParent = (CItem *)m_cLibrary.GetItemDataPtr(nIndex);

	UpdateData(TRUE);

	EndDialog(IDOK);
}

void CFindSimpleDlg::OnFindExpert()
{
	EndDialog(IDC_FIND_EXPERT);
}

void CFindSimpleDlg::OnGetfindnum()
{
	int nSel;
	if ((nSel = m_cLibrary.GetCurSel()) == LB_ERR) {
		::AfxMessageBox(IDS_SELECT_LIBRARY);
		return;
	}

	CWaitCursor wait;

	UpdateData(TRUE);

	long nFindNum = 0;

	CItem *pItem = (CItem *)m_cLibrary.GetItemDataPtr(nSel);
	switch (pItem->m_nItemType) {
	case ITEM_ROOT:
		m_pDocument->UpdateAllViews(NULL, VIEWHINT_OPENALLLIBRARY);
		SetLibraryCombo();
		m_cLibrary.SetCurSel(0);

		for (int i = 0; i < m_cLibrary.GetCount(); i++) {
			CItem *pItem2 = (CItem *)m_cLibrary.GetItemDataPtr(i);

			if (pItem2->m_nItemType == ITEM_FOLDER) {
				nFindNum += GetFindCount((CItemFolder *)pItem2);
			}
		}
		break;
	case ITEM_FOLDER:
		nFindNum = GetFindCount((CItemFolder *)pItem);
		break;
	}

	m_cFindNum = nFindNum;
}

void CFindSimpleDlg::SetLibraryCombo()
{
	CString sStr;

	sStr.LoadString(IDS_ALL_LIBRARIES);
	m_cLibrary.ResetContent();
	const int nIndex = m_cLibrary.AddString(sStr);
	m_cLibrary.SetItemData(nIndex, g_pTreeCtrl->GetItemData(g_pTreeCtrl->GetRootItem()));
	SetLibraryComboBox(m_cLibrary);
}

long CFindSimpleDlg::GetFindCount(const CItemFolder *pItemFolder)
{
	CMMPcsDB *pPcsDB = pItemFolder->m_pPcsDB;
	long nCount, nFindCount = 0;
	CString sFolderFilter;
	CString sDataFilter;

	MakeFilterString2(m_sSearchText, sFolderFilter, sDataFilter);

	if (pPcsDB->FilterFolderEx(sFolderFilter)) {
		if (pPcsDB->GetFolderCountEx(&nCount))
			nFindCount += nCount;
		pPcsDB->FilterFolderEx(NULL);
	}

	if (pPcsDB->FilterDataEx(sDataFilter)) {
		if (pPcsDB->GetDataCountEx(&nCount))
			nFindCount += nCount;
		pPcsDB->FilterDataEx(NULL);
	}

	return nFindCount;
}
