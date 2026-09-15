#pragma once

// MMLibListView.h : ヘッダー ファイル
//

#include "Item.h"

/////////////////////////////////////////////////////////////////////////////
// CMMLibListView ビュー

class CMMLibListView : public CListView
{
protected:
	CMMLibListView();           // 動的生成に使用されるプロテクト コンストラクタ
	DECLARE_DYNCREATE(CMMLibListView)

	COleDropTarget m_oDropTarget;
	CItem *m_pCurrentItem;
	int m_nColumn;
	int m_nSortItem;
	int m_nSortDir;
	CItemData *m_pPrintTreeItemData;
	CFont m_oFont;
	BOOL m_bTitleDisp;
	int m_nCurrentDetailDisp;
	int m_nCurrentDetailNo[DETAIL_NUM];

	void DrawItems();
	void DrawLibrary(CItemRoot *pItem);
	void DrawGroupFolder(CItem *pItem);
	void DrawDataFolder(CItem *pItem);
	void InsertColumns(int nDetailDisp);
	void SaveColumnWidth();
	void SetViewMode();
	DROPEFFECT DragOverItem(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	DROPEFFECT DragOverFile(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	BOOL DropItem(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	BOOL DropFile(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	CItem *GetTargetItem(CPoint point);
	CItem *GetSourceItem(COleDataObject* pDataObject);
	int GetSourceFileType(COleDataObject* pDataObject);
	CItem *GetSelectedItems();
	static int CALLBACK CompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void SelectAllItems();
	void SetViewFont();

	DECLARE_MESSAGE_MAP()
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnGetSelectedItem(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateFilePrint(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI *pCmdUI);
	afx_msg void OnFilePageSetup();
};
