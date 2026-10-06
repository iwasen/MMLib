#pragma once

// MMLibTreeView.h : ヘッダー ファイル
//

#include "MMLibDB.h"
#include "Item.h"

/////////////////////////////////////////////////////////////////////////////
// CMMLibTreeView ビュー

class CMMLibTreeView : public CTreeView
{
protected:
	CMMLibTreeView();           // 動的生成に使用されるプロテクト コンストラクタ
	DECLARE_DYNCREATE(CMMLibTreeView)

	COleDropTarget m_oDropTarget;
	HTREEITEM m_hTreeRoot;
	BOOL m_bInUpdate;
	BOOL m_bDragEntry;
	CFont m_oFont;
	CString m_sOrder;
	int m_nTextMode;

	void AddLibrary(CMMPcsDB *pPcsDB);
	void AddPendingLibrary(CAddItemPending *pAddItemPending);
	CItem *OpenLibrary(CItemPending *pItemPending);
	CItemFolder *AddFolder(HTREEITEM hItem, CMMPcsDB *pPcsDB, FOLDERREC *pFolderRec);
	CItemData *AddData(HTREEITEM hItem, CMMPcsDB *pPcsDB, DATAREC *pDataRec);
	void AddFind(CAddItemFind *pAddItemFind);
	void ReadChildren(CItemFolder *pItemFolder);
	void FindData(CItemFind *pItemFind);
	void FindDataSub(CItemFind *pItemFind, CMMPcsDB *pPcsDB, LPCSTR pFolderFilter, LPCSTR pDataFilter, BOOL &bHasChild);
	void ReFindItem(CItem *pItem);
	void SetChildren(HTREEITEM hItem, BOOL bChild);
	void AddItem(CAddItem *pAddItem);
	void RemoveItem(CItem *pItem, BOOL bCheck);
	void RemoveItemSub(CItem *pItem);
	void UpdateItem(CItem *pItem);
	void UpdateItemSub(CItem *pItem);
	DROPEFFECT DragOverItem(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	DROPEFFECT DragOverFile(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	BOOL DropItem(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	BOOL DropFile(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	CItem *GetTargetItem(CPoint point);
	CItem *GetSourceItem(COleDataObject* pDataObject);
	int GetSourceFileType(COleDataObject* pDataObject);
	void ReloadItem(CItemFolder *pItemFolder, BOOL bSubitem);
	CItemFolder *FindChildFolder(CItem *pItem, LPCSTR pFolderID);
	CItemData *FindChildData(CItem *pItem, LPCSTR pDataID);
	void ReloadAllItems();
	void SortItem(CItem *pItem);
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	int CompareFolder(CItemFolder *pItemFolder1, CItemFolder *pItemFolder2);
	int CompareData(CItemData *pItemData1, CItemData *pItemData2);
	void SetViewFont();
	void SortAllItems();
	void SetTextDataItems();
	LPCSTR DataItemText(CItemData *pItemData) const;
	void SelectData(CSelectItem *pSelectItem);
	BOOL SelectDataSub(CSelectItem *pSelectItem);
	HTREEITEM SelectFolder(HTREEITEM hItemLibrary, CMMPcsDB *pPcsDB, CString &sFolderID, int nCount);
	void CloseAllFind(CItem *pItem);
	void CloseRootFind();
	void CloseFindSub(HTREEITEM hItem);
	void OpenAllLibrary();

	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnGetSelectedItem(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateFilePrint(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI *pCmdUI);
};
