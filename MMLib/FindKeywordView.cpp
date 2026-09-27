// FindView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "FindKeywordDoc.h"
#include "FindKeywordView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindKeywordView

IMPLEMENT_DYNCREATE(CFindKeywordView, CListView)

BEGIN_MESSAGE_MAP(CFindKeywordView, CListView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindKeywordView 描画

void CFindKeywordView::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CFindKeywordView メッセージ ハンドラ

int CFindKeywordView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style |= LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS;
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT);
	GetListCtrl().SetImageList(&g_oImageList, LVSIL_SMALL);
	InsertColumns();

	return 0;
}

void CFindKeywordView::InsertColumns()
{
	struct LISTCOLUMN {
		UINT nText;
		int nFmt;
		int nWidth;
	};
	static const LISTCOLUMN tColumn[] = {
		{IDS_NAME, LVCFMT_LEFT, 130},
		{IDS_LIBRARY, LVCFMT_LEFT, 100},
		{IDS_CREATOR, LVCFMT_LEFT, 100},
		{IDS_REMARK, LVCFMT_LEFT, 100},
		{IDS_KEYWORD, LVCFMT_LEFT, 100},
		{IDS_CREATE_DATE, LVCFMT_LEFT, 110}
	};

	CStringArray saColumnWidth;
	CString sColumnWidth = theApp.GetProfileString(g_sSectionFindWindow, "ColumnWidth");
	StrSplit(sColumnWidth, ",", saColumnWidth);
	BOOL bColumnWidth = saColumnWidth.GetCount() != 0 && atoi(saColumnWidth[0]) == 1;

	for (int i = 0; i < sizeof(tColumn) / sizeof(LISTCOLUMN); i++) {
		LV_COLUMN lvc{
			.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT,
			.fmt = tColumn[i].nFmt,
			.cx = bColumnWidth ? atoi(saColumnWidth[i + 1]) : tColumn[i].nWidth,
			.pszText = (LPTSTR)(LPCSTR)GetString(tColumn[i].nText)
		};
		GetListCtrl().InsertColumn(i, &lvc);
	}
}

void CFindKeywordView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
	switch (lHint) {
	case VIEWHINT_KEYWORDFIND:
		{
			CFindKeywordDoc *pDoc = (CFindKeywordDoc *)GetDocument();
			CString sKeywords, sFilter;

			sKeywords = pDoc->m_sKeyword;
			sKeywords.Replace("　", " ");
			sKeywords.Trim();
			if (!sKeywords.IsEmpty()) {
				sFilter = GetFilter(sKeywords);

				if (!FindKeyword(sFilter))
					::AfxMessageBox(IDS_FIND_ERROR);
			}
		}
		break;
	case VIEWHINT_KEYWORDPREV:
		SelectPrev();
		break;
	case VIEWHINT_KEYWORDNEXT:
		SelectNext();
		break;
	}
}

CString CFindKeywordView::GetFilter(CString &sKeywords)
{
	CString sFilter;
	CString sKeyword;
	int nAndOr = 0;

	while (!sKeywords.IsEmpty()) {
		sKeyword = sKeywords.SpanExcluding(" ");
		sKeywords = sKeywords.Mid(sKeyword.GetLength());
		sKeywords.TrimLeft();

		if (sKeyword.CompareNoCase("AND") == 0) {
			nAndOr = 0;
			continue;
		} else if (sKeyword.CompareNoCase("OR") == 0) {
			nAndOr = 1;
			continue;
		}


		if (!sFilter.IsEmpty()) {
			if (nAndOr == 0)
				sFilter += "&";
			else
				sFilter += "|";
		}
		sFilter += "*SKEYWORD='" + sKeyword + "'";

		nAndOr = 0;
	}

	return sFilter;
}

BOOL CFindKeywordView::FindKeyword(LPCTSTR pFilter)
{
	CPcserve oPcserve;
	CPSDB oDb;
	CString sDir, sPath;
	int nRead;
	KEYWORDBUF oKeywordBuf;
	KEYWORDREC oKeywordRec;
	CListCtrl &lc = GetListCtrl();
	int nItem = 0;
	int nImage;
	CString sStr;
	CSelectItem *pSelectItem;
	int nRecSize;

	DeleteAllItems();

	if (oPcserve.Connect(CHT_LOCAL, "", "MMLIB") != 0)
		return FALSE;

	::GetKeywordDir(sDir);
	sPath.Format("%s\\%s", (LPCSTR)sDir, FN_KEYWORD_DBF);
	if (oDb.DBOpen(oPcserve, sPath) != 0) {
		::AfxMessageBox(IDS_NO_KEYWORD_INDEX);
		return TRUE;
	}

	if (oDb.DBRecSize(&nRecSize) != 0)
		return FALSE;

	if (nRecSize - 1 != sizeof(KEYWORDBUF)) {
		::AfxMessageBox(IDS_OLD_KEYWORD_INDEX);
		return TRUE;
	}

	CWaitCursor wait;

	if (oDb.DBSetFilter(pFilter) != 0)
		return FALSE;

	while (oDb.DBReadNext(1, &oKeywordBuf, &nRead) == 0) {
		if (nRead == 0)
			break;

		oDb.DBGetFieldBuf(&oKeywordBuf, &oKeywordRec);

		pSelectItem = new CSelectItem;
		pSelectItem->m_sLibraryID = oKeywordRec.sLibraryID;

		if (oKeywordRec.sIdType == FIND_TYPE_FOLDER) {
			pSelectItem->m_sFolderID = oKeywordRec.sId;
			if (oKeywordRec.sFolderType == FTYPE_GFOLDER)
				nImage = IMAGE_GFOLDER;
			else
				nImage = IMAGE_DFOLDER;
		} else {
			pSelectItem->m_sDataID = oKeywordRec.sId;
			nImage = IMAGE_DATA;
		}

		lc.InsertItem(nItem, oKeywordRec.sTitle, nImage);
		lc.SetItemText(nItem, 1, oKeywordRec.sLibraryName);
		lc.SetItemText(nItem, 2, oKeywordRec.sCreator);
		lc.SetItemText(nItem, 3, oKeywordRec.sRemark);
		lc.SetItemText(nItem, 4, oKeywordRec.sKeyword);
		FormatDate(oKeywordRec.sCreateDate, sStr);
		lc.SetItemText(nItem, 5, sStr);

		lc.SetItemData(nItem, (DWORD_PTR)pSelectItem);
		nItem++;
	}

	sStr.Format(IDS_FIND_RESULT, nItem);
	GetParent()->SetWindowText(sStr);

	CFindKeywordDoc *pDoc = (CFindKeywordDoc *)GetDocument();
	pDoc->m_nFindCount = nItem;

	m_nSortItem = 0;
	m_nSortDir = 1;
	lc.SortItems(CompareItems, (LPARAM)this);

	return TRUE;
}

void CFindKeywordView::DeleteAllItems()
{
	CListCtrl &lc = GetListCtrl();
	int nItem = lc.GetItemCount();

	for (int i = 0; i < nItem; i++)
		delete (CSelectItem *)lc.GetItemData(i);

	lc.DeleteAllItems();
}

void CFindKeywordView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (m_nSortItem != pNMListView->iSubItem)
		m_nSortDir = 1;
	else
		m_nSortDir *= -1;

	m_nSortItem = pNMListView->iSubItem;
	GetListCtrl().SortItems(CompareItems, (LPARAM)this);

	*pResult = 0;
}

int CALLBACK CFindKeywordView::CompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CFindKeywordView *pView = (CFindKeywordView *)lParamSort;
	CListCtrl &lc = pView->GetListCtrl();
	int nIndex1, nIndex2;

	LV_FINDINFO oFindInfo1{
		.flags = LVFI_PARAM,
		.lParam = lParam1
	};
	nIndex1 = lc.FindItem(&oFindInfo1);

	LV_FINDINFO oFindInfo2{
		.flags = LVFI_PARAM,
		.lParam = lParam2
	};
	nIndex2 = lc.FindItem(&oFindInfo2);

	return lc.GetItemText(nIndex1, pView->m_nSortItem).CompareNoCase(lc.GetItemText(nIndex2, pView->m_nSortItem)) * pView->m_nSortDir;
}

void CFindKeywordView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->uNewState & LVIS_SELECTED) {
		CFindKeywordDoc *pFindDoc = (CFindKeywordDoc *)GetDocument();
		pFindDoc->m_pMMLibDoc->UpdateAllViews(NULL, VIEWHINT_SELECTDATA, (CSelectItem *)pNMListView->lParam);
	}

	*pResult = 0;
}

void CFindKeywordView::OnDestroy()
{
	CStringArray saColumnWidth;
	CString sWidth;
	CString sColumnWidth;
	int nColumn = GetListCtrl().GetHeaderCtrl()->GetItemCount();
	saColumnWidth.Add("1");
	for (int i = 0; i < nColumn; i++) {
		sWidth.Format("%d", GetListCtrl().GetColumnWidth(i));
		saColumnWidth.Add(sWidth);
	}
	StrJoin(saColumnWidth, ",", sColumnWidth);
	theApp.WriteProfileString(g_sSectionFindWindow, "ColumnWidth", sColumnWidth);

	DeleteAllItems();

	CListView::OnDestroy();
}

void CFindKeywordView::SelectPrev()
{
	CListCtrl &lc = GetListCtrl();

	if (lc.GetItemCount() != 0) {
		int nItem = lc.GetItemCount() - 1;
		POSITION pos = lc.GetFirstSelectedItemPosition();
		if (pos != NULL) {
			int nPrevItem = lc.GetNextSelectedItem(pos) - 1;
			if (nPrevItem >= 0)
				nItem = nPrevItem;
		}
		lc.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		lc.EnsureVisible(nItem, FALSE);
	}
}

void CFindKeywordView::SelectNext()
{
	CListCtrl &lc = GetListCtrl();

	if (lc.GetItemCount() != 0) {
		int nItem = 0;
		POSITION pos = lc.GetFirstSelectedItemPosition();
		if (pos != NULL) {
			int nNextItem = lc.GetNextSelectedItem(pos) + 1;
			if (nNextItem < lc.GetItemCount())
				nItem = nNextItem;
		}
		lc.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		lc.EnsureVisible(nItem, FALSE);
	}
}
