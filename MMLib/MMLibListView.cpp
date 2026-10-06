// MMLibListView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "MMLibListView.h"
#include "MMLibDoc.h"
#include "MainFrm.h"
#include "MyOleData.h"
#include "PrintDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMLibListView

IMPLEMENT_DYNCREATE(CMMLibListView, CListView)

CMMLibListView::CMMLibListView()
{
	m_pCurrentItem = NULL;
	m_nColumn = 0;
	m_nSortItem = -1;
	m_nCurrentDetailDisp = DETAIL_NONE;
}

BEGIN_MESSAGE_MAP(CMMLibListView, CListView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_GETSELECTEDITEM, OnGetSelectedItem)
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, &CMMLibListView::OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CMMLibListView::OnUpdateFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMMLibListView 描画

void CMMLibListView::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CMMLibListView メッセージ ハンドラ

int CMMLibListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style |= LVS_REPORT | LVS_EDITLABELS | LVS_SHOWSELALWAYS;
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

//	SetViewFont();

	CListCtrl &lc = GetListCtrl();
	lc.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	lc.SetImageList(&g_oImageList, LVSIL_SMALL);
	lc.SetImageList(&g_oImageList2, LVSIL_NORMAL);

	m_oDropTarget.Register(this);

	return 0;
}

void CMMLibListView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
	switch (lHint) {
	case VIEWHINT_ADDITEM:
	case VIEWHINT_SELECTITEM:
	case VIEWHINT_OPENITEM:
	case VIEWHINT_CLOSEITEM:
	case VIEWHINT_REMOVEITEM:
	case VIEWHINT_UPDATEITEM:
	case VIEWHINT_RELOADITEM:
	case VIEWHINT_REFINDITEM:
	case VIEWHINT_DRAWALL:
	case VIEWHINT_DRAWLIST:
		DrawItems();
		break;
	case VIEWHINT_SELECTALL:
		SelectAllItems();
		break;
	case VIEWHINT_SETFONT:
		SetViewFont();
		break;
	case VIEWHINT_UPDATEWINDOW:
		UpdateWindow();
		break;
	}
}

void CMMLibListView::DrawItems()
{
	CWaitCursor waitCursor;
	CMMLibDoc *pDoc = (CMMLibDoc *)GetDocument();
	CMainFrame *pMainFrame = (CMainFrame *)::AfxGetApp()->m_pMainWnd;
	HTREEITEM hItem = g_pTreeCtrl->GetSelectedItem();

	if (hItem != NULL) {
		m_pCurrentItem = (CItem *)g_pTreeCtrl->GetItemData(hItem);

		switch (m_pCurrentItem->m_nItemType) {
		case ITEM_ROOT:
			g_nDetailMode = DETAIL_LIBRARY;
			break;
		case ITEM_FOLDER:
		case ITEM_INBOX:
			switch (((CItemFolder *)m_pCurrentItem)->m_nFolderType) {
			case FTYPE_LIBRARY:
			case FTYPE_GFOLDER:
				g_nDetailMode = DETAIL_FOLDER;
				break;
			case FTYPE_DFOLDER:
				g_nDetailMode = DETAIL_DATA;
				break;
			}
			break;
		case ITEM_FIND:
			if (((CItemFind *)m_pCurrentItem)->m_nFindType == FIND_TYPE_FOLDER)
				g_nDetailMode = DETAIL_FOLDER;
			else
				g_nDetailMode = DETAIL_DATA;
			break;
		default:
			g_nDetailMode = DETAIL_NONE;
			return;
		}

		switch (g_nDetailMode) {
		case DETAIL_LIBRARY:
			pMainFrame->ShowPictView(FALSE);
			DrawLibrary((CItemRoot *)m_pCurrentItem);
			break;
		case DETAIL_FOLDER:
			pMainFrame->ShowPictView(FALSE);
			DrawGroupFolder(m_pCurrentItem);
			break;
		case DETAIL_DATA:
			if (pDoc->m_nViewMode == VIEWMODE_PICT)
				return;
			pMainFrame->ShowPictView(FALSE);
			DrawDataFolder(m_pCurrentItem);
			break;
		}
	}

	m_nSortItem = -1;

	pDoc->m_bPictView = FALSE;

	SetViewMode();
}

void CMMLibListView::DrawLibrary(CItemRoot *pItem)
{
	CMMLibDoc *pDoc = (CMMLibDoc *)GetDocument();
	CListCtrl &listCtrl = GetListCtrl();
	HTREEITEM hItem;
	CItem *pItemChild;
	CItemFolder *pItemFolder;
	CItemPending *pItemPending;
	CItemFind *pItemFind;
	int i, j;
	int nRow, column;
	CString sStr;
	LPCSTR p;

	InsertColumns(DETAIL_LIBRARY);

	nRow = 0;
	for (i = 0, hItem = g_pTreeCtrl->GetChildItem(pItem->m_hItem); hItem != NULL; i++, hItem = g_pTreeCtrl->GetNextSiblingItem(hItem)) {
		pItemChild = (CItem *)g_pTreeCtrl->GetItemData(hItem);

		switch (pItemChild->m_nItemType) {
		case ITEM_FOLDER:
		case ITEM_INBOX:
			pItemFolder = (CItemFolder *)pItemChild;

			column = 0;
			for (j = 0; j < DETAIL_NUM; j++) {
				if (!pDoc->m_bDetailFlag[DETAIL_LIBRARY][j])
					continue;

				switch (j) {
				case DETAIL_NAME:
					p = pItemFolder->m_oFolderRec.sFolderName;
					break;
				case DETAIL_CDATE:
					FormatDate(pItemFolder->m_oFolderRec.sCreateDate, sStr);
					p = sStr;
					break;
				case DETAIL_UDATE:
					FormatDate(pItemFolder->m_oFolderRec.sUpdateDate, sStr);
					p = sStr;
					break;
				case DETAIL_CREATOR:
					p = pItemFolder->m_oFolderRec.sCreator;
					break;
				case DETAIL_REMARK:
					p = pItemFolder->m_oFolderRec.sRemark;
					break;
				case DETAIL_KEYWORD:
					p = pItemFolder->m_oFolderRec.sKeyword;
					break;
				case DETAIL_SERVER:
					p = pItemFolder->m_pPcsDB->m_pPcserve->m_sServerName;
					break;
				case DETAIL_DIRECTORY:
					p = pItemFolder->m_pPcsDB->m_sDirName;
					break;
				default:
					p = "";
					break;
				}

				if (column == 0)
					listCtrl.InsertItem(nRow, p, pItemFolder->m_nItemType == ITEM_FOLDER ? IMAGE_LIBRARY : IMAGE_INBOX);
				else
					listCtrl.SetItemText(nRow, column, p);

				column++;
			}
			break;
		case ITEM_PENDING:
			pItemPending = (CItemPending *)pItemChild;

			column = 0;
			for (j = 0; j < DETAIL_NUM; j++) {
				if (!pDoc->m_bDetailFlag[DETAIL_LIBRARY][j])
					continue;

				switch (j) {
				case DETAIL_NAME:
					p = pItemPending->m_sTitle;
					break;
				case DETAIL_CDATE:
				case DETAIL_UDATE:
				case DETAIL_CREATOR:
				case DETAIL_REMARK:
				case DETAIL_KEYWORD:
					p = "-----";
					break;
				case DETAIL_SERVER:
					if (pItemPending->m_nChType == CHT_LOCAL)
						p = "(Local)";
					else
						p = pItemPending->m_sServerName;
					break;
				case DETAIL_DIRECTORY:
					p = pItemPending->m_sDirName;
					break;
				default:
					p = "";
					break;
				}

				if (column == 0)
					listCtrl.InsertItem(nRow, p, IMAGE_PENDING);
				else
					listCtrl.SetItemText(nRow, column, p);

				column++;
			}
			break;
		case ITEM_FIND:
			pItemFind = (CItemFind *)pItemChild;
			if (pDoc->m_bDetailFlag[DETAIL_LIBRARY][DETAIL_NAME])
				listCtrl.InsertItem(nRow, pItemFind->m_oFindRec.sFolderName,
						pItemFind->m_nFindType == FIND_TYPE_FOLDER ? IMAGE_FFOLDER : IMAGE_FDATA);
			break;
		default:
			continue;
		}

		listCtrl.SetItemData(nRow, (DWORD_PTR)pItemChild);

		nRow++;
	}
}

void CMMLibListView::DrawGroupFolder(CItem *pItem)
{
	CMMLibDoc *pDoc = (CMMLibDoc *)GetDocument();
	CListCtrl &listCtrl = GetListCtrl();
	HTREEITEM hItem;
	CItem *pItemChild;
	CItemFolder *pItemFolder;
	CItemFind *pItemFind;
	int i, j;
	int nRow, column;
	CString sStr;
	LPCSTR p;
	int nDetailNo;

	InsertColumns(DETAIL_FOLDER);

	nRow = 0;
	for (i = 0, hItem = g_pTreeCtrl->GetChildItem(pItem->m_hItem); hItem != NULL; i++, hItem = g_pTreeCtrl->GetNextSiblingItem(hItem)) {
		pItemChild = (CItem *)g_pTreeCtrl->GetItemData(hItem);
		switch (pItemChild->m_nItemType) {
		case ITEM_FOLDER:
			pItemFolder = (CItemFolder *)pItemChild;
			column = 0;
			for (j = 0; j < DETAIL_NUM; j++) {
				nDetailNo = g_tColumnOrder[j];

				if (!pDoc->m_bDetailFlag[DETAIL_FOLDER][nDetailNo])
					continue;

				switch (nDetailNo) {
				case DETAIL_NAME:
					p = pItemFolder->m_oFolderRec.sFolderName;
					break;
				case DETAIL_FIRSTDATA:
					sStr = pItemFolder->GetFirstData();
					p = sStr;
					break;
				case DETAIL_CDATE:
					FormatDate(pItemFolder->m_oFolderRec.sCreateDate, sStr);
					p = sStr;
					break;
				case DETAIL_UDATE:
					FormatDate(pItemFolder->m_oFolderRec.sUpdateDate, sStr);
					p = sStr;
					break;
				case DETAIL_CREATOR:
					p = pItemFolder->m_oFolderRec.sCreator;
					break;
				case DETAIL_REMARK:
					p = pItemFolder->m_oFolderRec.sRemark;
					break;
				case DETAIL_KEYWORD:
					p = pItemFolder->m_oFolderRec.sKeyword;
					break;
				default:
					p = "";
					break;
				}

				if (column == 0)
					listCtrl.InsertItem(nRow, p, pItemFolder->m_nFolderType == FTYPE_GFOLDER ? IMAGE_GFOLDER : IMAGE_DFOLDER);
				else
					listCtrl.SetItemText(nRow, column, p);

				column++;
			}
			break;
		case ITEM_FIND:
			pItemFind = (CItemFind *)pItemChild;
			if (pDoc->m_bDetailFlag[DETAIL_FOLDER][DETAIL_NAME])
				listCtrl.InsertItem(nRow, pItemFind->m_oFindRec.sFolderName,
						pItemFind->m_nFindType == FIND_TYPE_FOLDER ? IMAGE_FFOLDER : IMAGE_FDATA);
			break;
		}

		listCtrl.SetItemData(nRow, (DWORD_PTR)pItemChild);

		nRow++;
	}
}

void CMMLibListView::DrawDataFolder(CItem *pItem)
{
	CMMLibDoc *pDoc = (CMMLibDoc *)GetDocument();
	CListCtrl &listCtrl = GetListCtrl();
	HTREEITEM hItem;
	CItem *pItemChild;
	CItemFolder *pItemFolder;
	CItemData *pItemData;
	int i, j;
	int nRow, column;
	CString sStr;
	LPCSTR p;

	InsertColumns(DETAIL_DATA);

	nRow = 0;
	for (i = 0, hItem = g_pTreeCtrl->GetChildItem(pItem->m_hItem); hItem != NULL; i++, hItem = g_pTreeCtrl->GetNextSiblingItem(hItem)) {
		pItemChild = (CItem *)g_pTreeCtrl->GetItemData(hItem);

		switch (pItemChild->m_nItemType) {
		case ITEM_FOLDER:
			pItemFolder = (CItemFolder *)pItemChild;
			column = 0;
			for (j = 0; j < DETAIL_NUM; j++) {
				if (!pDoc->m_bDetailFlag[DETAIL_DATA][j])
					continue;

				switch (j) {
				case DETAIL_NAME:
					p = pItemFolder->m_oFolderRec.sFolderName;
					break;
				case DETAIL_FIRSTDATA:
					sStr = pItemFolder->GetFirstData();
					p = sStr;
					break;
				case DETAIL_CDATE:
					FormatDate(pItemFolder->m_oFolderRec.sCreateDate, sStr);
					p = sStr;
					break;
				case DETAIL_UDATE:
					FormatDate(pItemFolder->m_oFolderRec.sUpdateDate, sStr);
					p = sStr;
					break;
				case DETAIL_CREATOR:
					p = pItemFolder->m_oFolderRec.sCreator;
					break;
				case DETAIL_REMARK:
					p = pItemFolder->m_oFolderRec.sRemark;
					break;
				case DETAIL_KEYWORD:
					p = pItemFolder->m_oFolderRec.sKeyword;
					break;
				default:
					p = "";
					break;
				}

				if (column == 0)
					listCtrl.InsertItem(nRow, p, pItemFolder->m_nFolderType == FTYPE_GFOLDER ? IMAGE_GFOLDER : IMAGE_DFOLDER);
				else
					listCtrl.SetItemText(nRow, column, p);

				column++;
			}
			break;
		case ITEM_DATA:
			pItemData = (CItemData *)pItemChild;

			column = 0;
			for (j = 0; j < DETAIL_NUM; j++) {
				if (!pDoc->m_bDetailFlag[DETAIL_DATA][j])
					continue;

				switch (j) {
				case DETAIL_NAME:
					p = pItemData->m_oDataRec.sTitle;
					break;
				case DETAIL_CODE:
					p = pItemData->m_oDataRec.sCode;
					break;
				case DETAIL_CDATE:
					FormatDate(pItemData->m_oDataRec.sCreateDate, sStr);
					p = sStr;
					break;
				case DETAIL_UDATE:
					FormatDate(pItemData->m_oDataRec.sUpdateDate, sStr);
					p = sStr;
					break;
				case DETAIL_CREATOR:
					p = pItemData->m_oDataRec.sCreator;
					break;
				case DETAIL_REMARK:
					p = pItemData->m_oDataRec.sRemark;
					break;
				case DETAIL_KEYWORD:
					p = pItemData->m_oDataRec.sKeyword;
					break;
				default:
					p = "";
					break;
				}

				if (column == 0)
					listCtrl.InsertItem(nRow, p, IMAGE_DATA);
				else
					listCtrl.SetItemText(nRow, column, p);

				column++;
			}
			break;
		}

		listCtrl.SetItemData(nRow, (DWORD_PTR)pItemChild);

		nRow++;
	}
}

void CMMLibListView::InsertColumns(int nDetailDisp)
{
	static constexpr UINT header[] = {
		IDS_NAME, IDS_CODE, IDS_CREATE_DATE, IDS_UPDATE_DATE, IDS_CREATOR,
		IDS_REMARK, IDS_KEYWORD, IDS_SERVER_NAME, IDS_DIRECTORY, IDS_FIRST_DATA
	};

	int i, nDetailNo;
	CListCtrl &listCtrl = GetListCtrl();
	CMMLibDoc *pDoc = (CMMLibDoc *)GetDocument();

	SaveColumnWidth();

	listCtrl.DeleteAllItems();

	for (i = 0; i < m_nColumn; i++)
		listCtrl.DeleteColumn(0);

	m_nColumn = 0;
	for (i = 0; i < DETAIL_NUM; i++) {
		nDetailNo = g_tColumnOrder[i];
		if (pDoc->m_bDetailFlag[nDetailDisp][nDetailNo]) {
			LVCOLUMN lvc{
				.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT,
				.fmt = LVCFMT_LEFT,
				.cx = CDpi::AdjustDpi(pDoc->m_nDetailWidth[nDetailDisp][nDetailNo]),
				.pszText = (LPSTR)(LPCSTR)GetString(header[nDetailNo])
			};
			listCtrl.InsertColumn(m_nColumn, &lvc);
			m_nCurrentDetailNo[m_nColumn] = nDetailNo;
			m_nColumn++;
		}
	}

	m_nCurrentDetailDisp = nDetailDisp;

	m_bTitleDisp = pDoc->m_bDetailFlag[nDetailDisp][0];
}

void CMMLibListView::SaveColumnWidth()
{
	CListCtrl &listCtrl = GetListCtrl();
	CMMLibDoc *pDoc = (CMMLibDoc *)GetDocument();
	int i, nDetailNo;

	if (m_nCurrentDetailDisp != DETAIL_NONE) {
		for (i = 0; i < m_nColumn; i++) {
			LVCOLUMN lvc{
				.mask = LVCF_WIDTH
			};
			listCtrl.GetColumn(i, &lvc);
			nDetailNo = m_nCurrentDetailNo[i];
			pDoc->m_nDetailWidth[m_nCurrentDetailDisp][nDetailNo] = CDpi::OriginalDpi(lvc.cx);
		}
	}
}

void CMMLibListView::SetViewMode()
{
	CMMLibDoc *pDoc = (CMMLibDoc *)GetDocument();
	DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);

	switch (pDoc->m_nViewMode) {
	case VIEWMODE_PICT:
		dwStyle &= ~(LVS_ICON | LVS_LIST | LVS_REPORT);
		dwStyle |= LVS_ICON | LVS_EDITLABELS | LVS_SHOWSELALWAYS;
		break;
	case VIEWMODE_LIST:
		dwStyle &= ~(LVS_ICON | LVS_LIST | LVS_REPORT);
		dwStyle |= LVS_LIST | LVS_EDITLABELS | LVS_SHOWSELALWAYS;
		break;
	case VIEWMODE_DETAIL:
		dwStyle &= ~(LVS_ICON | LVS_LIST | LVS_REPORT);
		dwStyle |= LVS_REPORT | LVS_EDITLABELS | LVS_SHOWSELALWAYS;
		break;
	}

	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
}

void CMMLibListView::OnDblclk(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	CListCtrl &listCtrl = GetListCtrl();
	int i, nItem;

	nItem = listCtrl.GetItemCount();
	for (i = 0; i < nItem; i++) {
		if (listCtrl.GetItemState(i, LVIS_SELECTED) != 0)
			break;
	}

	if (i < nItem) {
		CItem *pItem = (CItem *)listCtrl.GetItemData(i);
		GetDocument()->UpdateAllViews(NULL, VIEWHINT_SELECTITEM, pItem);
	}

	*pResult = 0;
}

void CMMLibListView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (pDispInfo->item.pszText != NULL) {
		CMMLibDoc *pDoc = (CMMLibDoc *)GetDocument();
		pDoc->NameChange((CItem *)GetListCtrl().GetItemData(pDispInfo->item.iItem), pDispInfo->item.pszText);

		NotifyUpdate();
	}

	*pResult = 0;
}

void CMMLibListView::OnRclick(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	CListCtrl &lvc = GetListCtrl();
	POINT posScreen, posClient;
	UINT flag;
	int nItem;
	UINT nMenuID = 0;
	CItem *pItem;

	::GetCursorPos(&posScreen);

	posClient = posScreen;
	ScreenToClient(&posClient);

	if ((nItem = lvc.HitTest(posClient, &flag)) != -1)
		pItem = (CItem *)lvc.GetItemData(nItem);
	else
		pItem = m_pCurrentItem;

	switch (pItem->m_nItemType) {
	case ITEM_ROOT:
		nMenuID = IDR_ROOT;
		break;
	case ITEM_INBOX:
		nMenuID = IDR_INBOX;
		break;
	case ITEM_FOLDER:
		switch (((CItemFolder *)pItem)->m_nFolderType) {
		case FTYPE_LIBRARY:
			nMenuID = IDR_LIBRARY;
			break;
		case FTYPE_GFOLDER:
			nMenuID = IDR_GFOLDER;
			break;
		case FTYPE_DFOLDER:
			nMenuID = IDR_DFOLDER;
			break;
		}
		break;
	case ITEM_DATA:
		nMenuID = IDR_DATA;
		break;
//	case ITEM_PENDING:
//		nMenuID = IDR_LIBRARY;
//		break;
	case ITEM_FIND:
		nMenuID = IDR_FIND;
		break;
	}

	if (nMenuID != 0) {
		CMenu menu;
		menu.LoadMenu(nMenuID);
		CMenu* pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, posScreen.x, posScreen.y, ::AfxGetMainWnd());
	}

	*pResult = 0;
}

void CMMLibListView::OnBegindrag(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	CItem *pItem = GetSelectedItems();
	if (pItem == NULL || pItem->m_nItemType != ITEM_FOLDER && pItem->m_nItemType != ITEM_DATA) {
		*pResult = 1;
		return;
	}

	HGLOBAL hMem = ::GlobalAlloc(GMEM_DDESHARE, sizeof(CItem *));
	CItem **ppItem = (CItem **)::GlobalLock(hMem);
	*ppItem = pItem;
	::GlobalUnlock(hMem);

	CMyOleDataSource* pods = new CMyOleDataSource;
	pods->m_pItem = pItem;

	pods->CacheGlobalData((CLIPFORMAT)g_nCFMMLibrary, hMem);
	pods->DelayRenderData(CF_HDROP);
	pods->DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE);

	delete pods;

	*pResult = 0;
}

DROPEFFECT CMMLibListView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if (pDataObject->IsDataAvailable((CLIPFORMAT)g_nCFMMLibrary))
		return DragOverItem(pDataObject, dwKeyState, point);
	else if (pDataObject->IsDataAvailable(CF_HDROP))
		return DragOverFile(pDataObject, dwKeyState, point);
	else
		return DROPEFFECT_NONE;
}

DROPEFFECT CMMLibListView::DragOverItem(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	CItem *pItemSrc, *pItemDst, *pItemParent;
	CMMPcsDB *pPcsDBSrc, *pPcsDBDst;

	if ((pItemDst = GetTargetItem(point)) == NULL)
		pItemDst = m_pCurrentItem;

	if (pItemDst->m_nItemType != ITEM_FOLDER && pItemDst->m_nItemType != ITEM_INBOX)
		return DROPEFFECT_NONE;

	if ((pItemSrc = GetSourceItem(pDataObject)) == NULL)
		return DROPEFFECT_NONE;

	switch (pItemSrc->m_nItemType) {
	case ITEM_FOLDER:
		if (((CItemFolder *)pItemDst)->m_nFolderType == FTYPE_DFOLDER)
			return DROPEFFECT_NONE;
		pPcsDBSrc = ((CItemFolder *)pItemSrc)->m_pPcsDB;
		break;
	case ITEM_DATA:
		if (((CItemFolder *)pItemDst)->m_nFolderType != FTYPE_DFOLDER)
			return DROPEFFECT_NONE;
		pPcsDBSrc = ((CItemData *)pItemSrc)->m_pPcsDB;
		break;
	default:
		return DROPEFFECT_NONE;
	}

	if (dwKeyState & MK_CONTROL)
		return DROPEFFECT_COPY;

	if (dwKeyState & MK_SHIFT)
		return DROPEFFECT_MOVE;

	pItemParent = pItemSrc;
	while ((pItemParent = pItemParent->GetParent()) != NULL) {
		if (pItemParent->m_nItemType == ITEM_INBOX)
			return DROPEFFECT_MOVE;
	}

	pPcsDBDst = ((CItemFolder *)pItemDst)->m_pPcsDB;
	if (pPcsDBSrc == pPcsDBDst)
		return DROPEFFECT_MOVE;
	else
		return DROPEFFECT_COPY;
}

DROPEFFECT CMMLibListView::DragOverFile(COleDataObject* pDataObject, DWORD /*dwKeyState*/, CPoint point)
{
	CItem *pItemDst;

	if ((pItemDst = GetTargetItem(point)) == NULL)
		return DROPEFFECT_NONE;

	if (pItemDst->m_nItemType != ITEM_FOLDER && pItemDst->m_nItemType != ITEM_INBOX)
		return DROPEFFECT_NONE;

	const int nFileType = GetSourceFileType(pDataObject);

	if (nFileType & SRCFILETYPE_MML)
		;
	else if (nFileType & SRCFILETYPE_PICT) {
		if (((CItemFolder *)pItemDst)->m_nFolderType != FTYPE_DFOLDER)
			return DROPEFFECT_NONE;
	} else
		return DROPEFFECT_NONE;

	return DROPEFFECT_COPY;
}

BOOL CMMLibListView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	if (pDataObject->IsDataAvailable((CLIPFORMAT)g_nCFMMLibrary))
		return DropItem(pDataObject, dropEffect, point);
	else if (pDataObject->IsDataAvailable(CF_HDROP))
		return DropFile(pDataObject, dropEffect, point);
	else
		return FALSE;
}

BOOL CMMLibListView::DropItem(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	CItem *pItemSrc, *pItemDst;

	if ((pItemDst = GetTargetItem(point)) == NULL)
		pItemDst = m_pCurrentItem;

	if (pItemDst->m_nItemType != ITEM_FOLDER && pItemDst->m_nItemType != ITEM_INBOX)
		return FALSE;

	if ((pItemSrc = GetSourceItem(pDataObject)) == NULL)
		return FALSE;

	if (pItemSrc == pItemDst)
		return FALSE;

	CMMLibDoc *pDoc = (CMMLibDoc *)GetDocument();

	switch (dropEffect) {
	case DROPEFFECT_COPY:
		pDoc->CopyItem(pItemSrc, pItemDst);
		break;
	case DROPEFFECT_MOVE:
		pDoc->MoveItem(pItemSrc, pItemDst);
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

BOOL CMMLibListView::DropFile(COleDataObject* pDataObject, DROPEFFECT /*dropEffect*/, CPoint point)
{
	CMMLibDoc *pDoc = (CMMLibDoc *)GetDocument();
	CItem *pItemDst;
	HDROP hDrop;
	int nNumFiles;
	int i;
	int nFileType = 0;
	int nPictFile = 0;
	char fileNameBuf[256];

	if ((pItemDst = GetTargetItem(point)) == NULL)
		return FALSE;

	if (pItemDst->m_nItemType != ITEM_FOLDER && pItemDst->m_nItemType != ITEM_INBOX)
		return FALSE;

	if ((hDrop = (HDROP)pDataObject->GetGlobalData(CF_HDROP, NULL)) == NULL)
		return FALSE;

	nNumFiles = ::DragQueryFile(hDrop, (UINT)-1, NULL, 0);

	for (i = 0; i < nNumFiles; i++) {
		::DragQueryFile(hDrop, i, fileNameBuf, sizeof(fileNameBuf));
		nFileType = CheckFileType(fileNameBuf);

		if (nFileType & SRCFILETYPE_MML)
			pDoc->ImportMmlFile(fileNameBuf, (CItemFolder *)pItemDst);

		if (nFileType & SRCFILETYPE_PICT) {
			if (((CItemFolder *)pItemDst)->m_nFolderType == FTYPE_DFOLDER)
				nPictFile++;
		}
	}

	if (nPictFile != 0) {
		CString *pFileName = new CString[nPictFile];

		int n = 0;
		for (i = 0; i < nNumFiles; i++) {
			::DragQueryFile(hDrop, i, fileNameBuf, sizeof(fileNameBuf));
			nFileType = CheckFileType(fileNameBuf);

			if (nFileType & SRCFILETYPE_PICT)
				pFileName[n++] = fileNameBuf;
		}

		pDoc->ImportPictFile(pFileName, nPictFile, (CItemFolder *)pItemDst, TRUE);
	}

	::GlobalFree(hDrop);

	return TRUE;
}

CItem *CMMLibListView::GetTargetItem(CPoint point)
{
	CListCtrl& lvc = GetListCtrl();
	UINT flags;

	const int iItem = lvc.HitTest(point, &flags);
	if (iItem == -1)
		return m_pCurrentItem;

	return (CItem *)lvc.GetItemData(iItem);
}

CItem *CMMLibListView::GetSourceItem(COleDataObject* pDataObject)
{
	HGLOBAL hGlobal;

	if ((hGlobal = pDataObject->GetGlobalData((CLIPFORMAT)g_nCFMMLibrary, NULL)) == NULL)
		return NULL;

	CItem *pItem = *(CItem **)GlobalLock(hGlobal);
	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	return pItem;
}

int CMMLibListView::GetSourceFileType(COleDataObject* pDataObject)
{
	HDROP hDrop;
	int nNumFiles;
	int nFileType = 0;
	char fileNameBuf[256];

	if ((hDrop = (HDROP)pDataObject->GetGlobalData(CF_HDROP, NULL)) == NULL)
		return 0;

	nNumFiles = ::DragQueryFile(hDrop, (UINT)-1, NULL, 0);

	for (int i = 0; i < nNumFiles; i++) {
		::DragQueryFile(hDrop, i, fileNameBuf, sizeof(fileNameBuf));
		nFileType |= CheckFileType(fileNameBuf);
	}

	::GlobalFree(hDrop);

	return nFileType;
}

LRESULT CMMLibListView::OnGetSelectedItem(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CItem *pItem = GetSelectedItems();

	if (pItem == NULL && m_pCurrentItem != NULL) {
		pItem = m_pCurrentItem;
		pItem->m_pNextSelected = NULL;
	}

	return (LRESULT)pItem;
}

CItem *CMMLibListView::GetSelectedItems()
{
	CListCtrl& lvc = GetListCtrl();
	int iItem;
	CItem *pItem = NULL, **ppItem;

	ppItem = &pItem;
	iItem = -1;
	for (;;) {
		iItem = lvc.GetNextItem(iItem, LVNI_ALL | LVNI_SELECTED);
		if (iItem == -1)
			break;
		*ppItem = (CItem *)lvc.GetItemData(iItem);
		ppItem = &(*ppItem)->m_pNextSelected;
		*ppItem = NULL;
	}

	return pItem;
}

void CMMLibListView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
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

int CALLBACK CMMLibListView::CompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CMMLibListView *pView = (CMMLibListView *)lParamSort;
	CListCtrl &lvc = pView->GetListCtrl();

	LV_FINDINFO findInfo1{
		.flags = LVFI_PARAM,
		.lParam = lParam1
	};
	const int index1 = lvc.FindItem(&findInfo1);

	LV_FINDINFO findInfo2{
		.flags = LVFI_PARAM,
		.lParam = lParam2
	};
	const int index2 = lvc.FindItem(&findInfo2);

	if (pView->m_bTitleDisp && pView->m_nSortItem == 0)
		return CompareTitle(lvc.GetItemText(index1, pView->m_nSortItem), lvc.GetItemText(index2, pView->m_nSortItem)) * pView->m_nSortDir;
	else
		return lvc.GetItemText(index1, pView->m_nSortItem).CompareNoCase(lvc.GetItemText(index2, pView->m_nSortItem)) * pView->m_nSortDir;
}

void CMMLibListView::SelectAllItems()
{
	CListCtrl& lvc = GetListCtrl();

	int iItem = -1;
	for (;;) {
		iItem = lvc.GetNextItem(iItem, LVNI_ALL);
		if (iItem == -1)
			break;

		lvc.SetItemState(iItem, LVIS_SELECTED, LVIS_SELECTED);
	}

	((CFrameWnd *)::AfxGetMainWnd())->SetActiveView(this);
}

void CMMLibListView::SetViewFont()
{
	m_oFont.DeleteObject();
	m_oFont.CreatePointFont(g_nViewFontSize, g_sViewFontName);
	SetFont(&m_oFont);
}

BOOL CMMLibListView::OnScroll(UINT nScrollCode, UINT /*nPos*/, BOOL bDoScroll)
{
	int nMinPos, nMaxPos;
	BOOL bScroll = FALSE;

	const int y = GetScrollPos(SB_VERT);
	const int nLimit = GetScrollLimit(SB_VERT);
	GetScrollRange(SB_VERT, &nMinPos, &nMaxPos);

	switch (HIBYTE(nScrollCode)) {
	case SB_TOP:
	case SB_LINEUP:
	case SB_PAGEUP:
		bScroll = (y > nMinPos);
		break;
	case SB_BOTTOM:
	case SB_LINEDOWN:
	case SB_PAGEDOWN:
		bScroll = (y < nLimit);
		break;
	}

	if (bScroll) {
		if (bDoScroll)
			SendMessage(WM_VSCROLL, HIBYTE(nScrollCode), 0);
	}

	return bScroll;
}

BOOL CMMLibListView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.dwExStyle |= WS_EX_CLIENTEDGE;

	return CListView::PreCreateWindow(cs);
}

BOOL CMMLibListView::OnPreparePrinting(CPrintInfo* pInfo)
{
	delete pInfo->m_pPD;
	pInfo->m_pPD = new CPrintDlg(FALSE, PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOSELECTION, NULL);
	pInfo->SetMinPage(1);

	if (m_pCurrentItem == NULL || m_pCurrentItem->m_nItemType != ITEM_FOLDER ||
				((CItemFolder *)m_pCurrentItem)->m_nFolderType != FTYPE_DFOLDER) {
		::AfxMessageBox(IDS_PRINT_FOLDER);
		return FALSE;
	}

	CItem *pItem;
	if ((pItem = GetSelectedItems()) == NULL) {
		::AfxMessageBox(IDS_SELECT_PRINT_DATA);
		return FALSE;
	}

	m_pPrintTreeItemData = (CItemData *)pItem;

	int nSelected = 0;
	for ( ; pItem != NULL; pItem = pItem->m_pNextSelected)
		nSelected++;

	pInfo->SetMaxPage(nSelected);

	return DoPreparePrinting(pInfo);
}

void CMMLibListView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if (g_pPrintDevMode != NULL)
		pDC->ResetDC(g_pPrintDevMode);

	CListView::OnBeginPrinting(pDC, pInfo);
}

void CMMLibListView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	HDIB hDIB;
	CItemData *pItemData;

	pItemData = m_pPrintTreeItemData;
	for (UINT i = 1; i < pInfo->m_nCurPage; i++) {
		pItemData = (CItemData *)pItemData->m_pNextSelected;
		if (pItemData == NULL)
			return;
	}

	pItemData->m_pPcsDB->ReadPictureDataEx(pItemData->m_oDataRec.sDataID, &hDIB, NULL);
	PrintPicture(pDC, hDIB, g_bPrintMargin ? g_rcPrintMargin : CRect(0, 0, 0, 0));

	MMPictureFree(hDIB);
}

void CMMLibListView::OnDestroy()
{
	SaveColumnWidth();

	CListView::OnDestroy();
}

void CMMLibListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	SetViewFont();
}

void CMMLibListView::OnUpdateFilePrint(CCmdUI *pCmdUI)
{
	CItem *pItem;
	BOOL bEnable = TRUE;

	if (m_pCurrentItem == NULL || m_pCurrentItem->m_nItemType != ITEM_FOLDER ||
				((CItemFolder *)m_pCurrentItem)->m_nFolderType != FTYPE_DFOLDER) {
		bEnable = FALSE;
	}

	if ((pItem = GetSelectedItems()) == NULL)
		bEnable = FALSE;

	pCmdUI->Enable(bEnable);
}

void CMMLibListView::OnUpdateFilePrintPreview(CCmdUI *pCmdUI)
{
	OnUpdateFilePrint(pCmdUI);
}
