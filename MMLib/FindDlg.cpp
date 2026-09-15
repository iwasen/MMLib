// FindDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "FindDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindDlg ダイアログ

CFindDlg::CFindDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CFindDlg::IDD, pParent)
{
	m_bNoCase = FALSE;
	m_iMatching = -1;
	m_iFindType = -1;
	m_bSave = FALSE;
	m_pItemParent = NULL;
	m_bUpdateMode = FALSE;
	m_nFindType = FIND_TYPE_DATA;
}

void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT, m_cText);
	DDX_Control(pDX, IDC_CODE, m_cCode);
	DDX_Control(pDX, IDC_FINDNUM, m_cFindNum);
	DDX_Control(pDX, IDC_UPDATE_DATE_TO, m_cUpdateDateTo);
	DDX_Control(pDX, IDC_UPDATE_DATE_FROM, m_cUpdateDateFrom);
	DDX_Control(pDX, IDC_CREATE_DATE_TO, m_cCreateDateTo);
	DDX_Control(pDX, IDC_CREATE_DATE_FROM, m_cCreateDateFrom);
	DDX_Control(pDX, IDC_CONDITION_TAB, m_cConditionTab);
	DDX_Control(pDX, IDC_LIBRARY, m_cLibrary);
	DDX_Text(pDX, IDC_CODE, m_sCode);
	DDX_Text(pDX, IDC_CREATE_DATE_BEFORE, m_sCreateDateBefore);
	DDX_Text(pDX, IDC_CREATOR, m_sCreator);
	DDX_Text(pDX, IDC_FIND_FOLDER, m_sFindFolder);
	DDX_Text(pDX, IDC_REMARK, m_sRemark);
	DDX_Text(pDX, IDC_KEYWORD, m_sKeyword);
	DDX_Text(pDX, IDC_TITLE, m_sTitle);
	DDX_Text(pDX, IDC_UPDATE_DATE_BEFORE, m_sUpdateDateBefore);
	DDX_Check(pDX, IDC_NOCASE, m_bNoCase);
	DDX_Radio(pDX, IDC_FRONT_MATCHING, m_iMatching);
	DDX_Radio(pDX, IDC_DATA, m_iFindType);
	DDX_Check(pDX, IDC_SAVE, m_bSave);
	DDX_Text(pDX, IDC_TEXT, m_sText);
}

BEGIN_MESSAGE_MAP(CFindDlg, CDialogAF)
	ON_NOTIFY(TCN_SELCHANGING, IDC_CONDITION_TAB, OnSelchangingConditionTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CONDITION_TAB, OnSelchangeConditionTab)
	ON_BN_CLICKED(IDC_GETFINDNUM, OnGetfindnum)
	ON_BN_CLICKED(IDC_FOLDER, OnFolder)
	ON_BN_CLICKED(IDC_DATA, OnData)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindDlg メッセージ ハンドラ

BOOL CFindDlg::OnInitDialog()
{
	CDialogAF::OnInitDialog();

	SetLibraryCombo();
	int n = m_cLibrary.GetCount();
	if (n == 1)
		m_cLibrary.SetCurSel(0);
	else if (n != 0) {
		for (int i = 0; i < n; i++) {
			if (m_cLibrary.GetItemDataPtr(i) == m_pItemParent) {
				m_cLibrary.SetCurSel(i);
				break;
			}
		}
	}

	CString sStr;
	for (int i = 0; i < N_FIND_CONDITIONS; i++) {
		sStr.Format(IDS_CONDITION, i + 1);
		m_cConditionTab.InsertItem(i, sStr);
	}

	m_iFindType = (m_nFindType == FIND_TYPE_DATA) ? 0 : 1;

	if (m_bUpdateMode)
		m_cLibrary.EnableWindow(FALSE);
	else
		m_sFindFolder.LoadString(IDS_FIND_FOLDER);

	m_cCode.EnableWindow(m_nFindType == FIND_TYPE_DATA);

	SetFindCondition();

	return TRUE;
}

void CFindDlg::OnOK()
{
	int nIndex;

	if ((nIndex = m_cLibrary.GetCurSel()) == LB_ERR) {
		::AfxMessageBox(IDS_SELECT_LIBRARY);
		return;
	}

	GetFindCondition();

	m_pItemParent = (CItem *)m_cLibrary.GetItemDataPtr(nIndex);

	EndDialog(IDOK);
}

void CFindDlg::OnSelchangingConditionTab(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	GetFindCondition();

	*pResult = 0;
}

void CFindDlg::OnSelchangeConditionTab(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	SetFindCondition();

	*pResult = 0;
}

void CFindDlg::GetFindCondition()
{
	int nIndex = m_cConditionTab.GetCurSel();

	UpdateData(TRUE);

	m_aFindConditions[nIndex].sCreateDateFrom = m_cCreateDateFrom;
	m_aFindConditions[nIndex].sCreateDateTo = m_cCreateDateTo;
	m_aFindConditions[nIndex].sCreateDateBefore = m_sCreateDateBefore;
	m_aFindConditions[nIndex].sUpdateDateFrom = m_cUpdateDateFrom;
	m_aFindConditions[nIndex].sUpdateDateTo = m_cUpdateDateTo;
	m_aFindConditions[nIndex].sUpdateDateBefore = m_sUpdateDateBefore;
	m_aFindConditions[nIndex].sCreator = m_sCreator;
	m_aFindConditions[nIndex].sCode = m_sCode;
	m_aFindConditions[nIndex].sTitle = m_sTitle;
	m_aFindConditions[nIndex].sRemark = m_sRemark;
	m_aFindConditions[nIndex].sKeyword = m_sKeyword;
	m_aFindConditions[nIndex].sText = m_sText;

	switch (m_iMatching) {
	default:
		m_aFindConditions[nIndex].sMatching.Empty();
		break;
	case 1:
		m_aFindConditions[nIndex].sMatching = "F";
		break;
	case 2:
//		m_aFindConditions[nIndex].sMatching = "W";
		m_aFindConditions[nIndex].sMatching = "S";
		break;
	}

	if (!m_bNoCase)
		m_aFindConditions[nIndex].sMatching += "N";
}

void CFindDlg::SetFindCondition()
{
	int nIndex = m_cConditionTab.GetCurSel();

	m_cCreateDateFrom = m_aFindConditions[nIndex].sCreateDateFrom;
	m_cCreateDateTo = m_aFindConditions[nIndex].sCreateDateTo;
	m_sCreateDateBefore = m_aFindConditions[nIndex].sCreateDateBefore;
	m_cUpdateDateFrom = m_aFindConditions[nIndex].sUpdateDateFrom;
	m_cUpdateDateTo = m_aFindConditions[nIndex].sUpdateDateTo;
	m_sUpdateDateBefore = m_aFindConditions[nIndex].sUpdateDateBefore;
	m_sCreator = m_aFindConditions[nIndex].sCreator;
	m_sCode = m_aFindConditions[nIndex].sCode;
	m_sTitle = m_aFindConditions[nIndex].sTitle;
	m_sRemark = m_aFindConditions[nIndex].sRemark;
	m_sKeyword = m_aFindConditions[nIndex].sKeyword;
	m_sText = m_aFindConditions[nIndex].sText;

	if (m_aFindConditions[nIndex].sMatching.Find('F') != -1)
		m_iMatching = 1;
	else if (m_aFindConditions[nIndex].sMatching.Find('S') != -1)
		m_iMatching = 2;
	else
		m_iMatching = 0;

	if (m_aFindConditions[nIndex].sMatching.Find('N') != -1)
		m_bNoCase = FALSE;
	else
		m_bNoCase = TRUE;

	UpdateData(FALSE);
}

void CFindDlg::OnGetfindnum()
{
	int nSel;
	if ((nSel = m_cLibrary.GetCurSel()) == LB_ERR) {
		::AfxMessageBox(IDS_SELECT_LIBRARY);
		return;
	}

	CWaitCursor wait;

	GetFindCondition();

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

long CFindDlg::GetFindCount(const CItemFolder *pItemFolder)
{
	CMMPcsDB *pPcsDB = pItemFolder->m_pPcsDB;
	long nCount, nFindCount = 0;
	CString sFilter;
	MakeFilterString(m_aFindConditions, m_nFindType, sFilter);

	switch (m_nFindType) {
	case FIND_TYPE_FOLDER:
		if (pPcsDB->FilterFolderEx(sFilter)) {
			if (pPcsDB->GetFolderCountEx(&nCount))
				nFindCount = nCount;
			pPcsDB->FilterFolderEx(NULL);
		}
		break;
	case FIND_TYPE_DATA:
		if (pPcsDB->FilterDataEx(sFilter)) {
			if (pPcsDB->GetDataCountEx(&nCount))
				nFindCount = nCount;
			pPcsDB->FilterDataEx(NULL);
		}
		break;
	}

	return nFindCount;
}

void CFindDlg::OnFolder()
{
	m_nFindType = FIND_TYPE_FOLDER;
	m_cCode.EnableWindow(FALSE);
}

void CFindDlg::OnData()
{
	m_nFindType = FIND_TYPE_DATA;
	m_cCode.EnableWindow(TRUE);
}

HBRUSH CFindDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogAF::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC) {
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	return hbr;
}

void CFindDlg::SetLibraryCombo()
{
	CString sStr;

	sStr.LoadString(IDS_ALL_LIBRARIES);
	m_cLibrary.ResetContent();
	int nIndex = m_cLibrary.AddString(sStr);
	m_cLibrary.SetItemData(nIndex, g_pTreeCtrl->GetItemData(g_pTreeCtrl->GetRootItem()));
	SetLibraryComboBox(m_cLibrary);
}
