// TextSearchDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "TextSearchDlg.h"
#include "MainFrm.h"

// CTextSearchDlg ダイアログ

IMPLEMENT_DYNAMIC(CTextSearchDlg, CDialogAF)

CTextSearchDlg::CTextSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CTextSearchDlg::IDD, pParent)
{
	m_nTotalCount = 0;
	m_nCurrentCount = -1;
}

void CTextSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SEARCH, m_cComboSearch);
	DDX_Control(pDX, IDC_EDIT_SEARCH_RESULT, m_cEditSearchResult);
}

BEGIN_MESSAGE_MAP(CTextSearchDlg, CDialogAF)
	ON_BN_CLICKED(IDC_BUTTON_PREV, &CTextSearchDlg::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CTextSearchDlg::OnBnClickedButtonNext)
	ON_CBN_EDITCHANGE(IDC_COMBO_SEARCH, &CTextSearchDlg::OnCbnEditchangeComboSearch)
	ON_CBN_SELCHANGE(IDC_COMBO_SEARCH, &CTextSearchDlg::OnCbnSelchangeComboSearch)
END_MESSAGE_MAP()

// CTextSearchDlg メッセージ ハンドラ

BOOL CTextSearchDlg::OnInitDialog()
{
	CDialogAF::OnInitDialog();

	ReadHistoryIni();

	SetSearchHistory();

	CString sSearchText;
	GetFindText(sSearchText);
	m_cComboSearch.SetWindowText(sSearchText);

	return TRUE;
}

void CTextSearchDlg::OnOK()
{
	OnBnClickedButtonNext();
}

void CTextSearchDlg::OnCancel()
{
	DestroyWindow();
}

void CTextSearchDlg::PostNcDestroy()
{
	delete this;

	((CMainFrame *)::AfxGetMainWnd())->ClosedTextSearchDlg();
}

void CTextSearchDlg::OnBnClickedButtonPrev()
{
	CString sSearchText;
	m_cComboSearch.GetWindowText(sSearchText);

	if (sSearchText.IsEmpty()) {
		::AfxMessageBox(IDS_INPUT_SEARCH_TEXT);
		return;
	}

	if (m_nCurrentCount == -1) {
		GetSearchCount(sSearchText);
	}

	if (m_nTotalCount != 0) {
		if (m_nCurrentCount-- <= 0)
			m_nCurrentCount = m_nTotalCount - 1;

		CTextSearchParams params;
		params.m_sSearchText = sSearchText;
		params.m_nCount = m_nCurrentCount;
		m_pDocument->UpdateAllViews(NULL, VIEWHINT_SETSEL, &params);

		DispSearchCount();
	}
}

void CTextSearchDlg::OnBnClickedButtonNext()
{
	CString sSearchText;
	m_cComboSearch.GetWindowText(sSearchText);

	if (sSearchText.IsEmpty()) {
		::AfxMessageBox(IDS_INPUT_SEARCH_TEXT);
		return;
	}

	if (m_nCurrentCount == -1) {
		GetSearchCount(sSearchText);
	}

	if (m_nTotalCount != 0) {
		if (++m_nCurrentCount == m_nTotalCount)
			m_nCurrentCount = 0;

		CTextSearchParams params;
		params.m_sSearchText = sSearchText;
		params.m_nCount = m_nCurrentCount;
		m_pDocument->UpdateAllViews(NULL, VIEWHINT_SETSEL, &params);

		DispSearchCount();
	}
}

void CTextSearchDlg::GetSearchCount(CString &sSearchText)
{
	CTextSearchParams params;
	params.m_sSearchText = sSearchText;
	params.m_nCount = 0;
	m_pDocument->UpdateAllViews(NULL, VIEWHINT_GETSEARCHCOUNT, &params);
	m_nTotalCount = params.m_nCount;

	DispSearchCount();

	AddHistory(sSearchText);
}

void CTextSearchDlg::DispSearchCount()
{
	CString sStr;

	sStr.Format("%d / %d", m_nCurrentCount + 1, m_nTotalCount);
	m_cEditSearchResult.SetWindowText(sStr);
}

BOOL CTextSearchDlg::DestroyWindow()
{
	m_pDocument->UpdateAllViews(NULL, VIEWHINT_RESETSEL);

	WriteHistoryIni();

	return CDialogAF::DestroyWindow();
}

void CTextSearchDlg::GetFindText(CString &sSearchText)
{
	HTREEITEM hItem;

	if ((hItem = g_pTreeCtrl->GetSelectedItem()) != NULL) {
		CItem *pItem = (CItem *)g_pTreeCtrl->GetItemData(hItem);
		BOOL bLoop = TRUE;
		while (bLoop && pItem != NULL && pItem->m_nItemType != ITEM_ROOT) {
			if (pItem->m_nItemType == ITEM_FIND) {
				CItemFind *pItemFind = (CItemFind *)pItem;
				switch (pItemFind->m_nFindType) {
				case FIND_TYPE_SIMPLE:
					sSearchText = pItemFind->m_oFindRec.conditions[0].sKeyword;
					bLoop = FALSE;
					break;
				default:
					for (int i = 0; i < N_FIND_CONDITIONS; i++) {
						if (!pItemFind->m_oFindRec.conditions[i].sText.IsEmpty()) {
							if (!sSearchText.IsEmpty()) {
								sSearchText += " ";
							}
							sSearchText += pItemFind->m_oFindRec.conditions[i].sText;
						}
					}
					bLoop = FALSE;
					break;
				}
			}
			pItem = pItem->GetParent();
		}
	}
}

void CTextSearchDlg::ReadHistoryIni()
{
	CWinApp *pWinApp = ::AfxGetApp();

	m_lSearchHistory.RemoveAll();

	CString sEntry;
	CString sValue;
	for (int i = 0; i < N_SEARCH_HISTORY; i++) {
		sEntry.Format("%d", i + 1);
		sValue = pWinApp->GetProfileString(g_sSectionSearchHistory, sEntry);
		if (!sValue.IsEmpty()) {
			m_lSearchHistory.AddTail(sValue);
		}
	}
}

void CTextSearchDlg::WriteHistoryIni()
{
	CWinApp *pWinApp = ::AfxGetApp();

	int n = 0;
	CString sEntry;
	POSITION pos = m_lSearchHistory.GetHeadPosition();
	while (pos != NULL) {
		sEntry.Format("%d", ++n);
		pWinApp->WriteProfileString(g_sSectionSearchHistory, sEntry, m_lSearchHistory.GetNext(pos));
	}
}

void CTextSearchDlg::AddHistory(const CString &sSearchText)
{
	POSITION pos = m_lSearchHistory.GetHeadPosition();
	while (pos != NULL) {
		POSITION pos2 = pos;
		if (sSearchText == m_lSearchHistory.GetNext(pos)) {
			m_lSearchHistory.RemoveAt(pos2);
		}
	}
	m_lSearchHistory.AddHead(sSearchText);

	while (m_lSearchHistory.GetCount() > N_SEARCH_HISTORY) {
		m_lSearchHistory.RemoveTail();
	}

	SetSearchHistory();
}

void CTextSearchDlg::SetSearchHistory()
{
	CString sCurrent;
	m_cComboSearch.GetWindowText(sCurrent);
	m_cComboSearch.ResetContent();
	m_cComboSearch.SetWindowText(sCurrent);

	POSITION pos = m_lSearchHistory.GetHeadPosition();
	while (pos != NULL) {
		m_cComboSearch.AddString(m_lSearchHistory.GetNext(pos));
	}
}

void CTextSearchDlg::ResetTextSearch()
{
	m_nTotalCount = 0;
	m_nCurrentCount = -1;

	m_cEditSearchResult.SetWindowText("");
}

void CTextSearchDlg::OnCbnEditchangeComboSearch()
{
	ResetTextSearch();
}

void CTextSearchDlg::OnCbnSelchangeComboSearch()
{
	ResetTextSearch();
}
