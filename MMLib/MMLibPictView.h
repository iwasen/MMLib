#pragma once

// MMLibPictView.h : ヘッダー ファイル
//

#include "Item.h"

/////////////////////////////////////////////////////////////////////////////
// CMMLibPictView ビュー

class CMMLibPictView : public CScrollView
{
protected:
	CMMLibPictView();           // 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CMMLibPictView();
	DECLARE_DYNCREATE(CMMLibPictView)

	HDIB m_hDIB;
	CBrush m_hBrushBkgnd;
	CBrush m_hBrushSelect;
	CFont m_oFont;
	SIZE m_sizePict;
	SIZE m_sizeDraw;
	BOOL m_bScrollPicture;
	CPoint m_posPicture;
	int m_nDispMode;
	CItemData *m_pSelectItem;
	CItem *m_pCurrentItem;
	double m_fPictRatio;
	int m_nTextMode;
	int m_nSmallSize;
	BOOL m_bIgnoreUpdate;
	COleDropTarget m_oDropTarget;
	BOOL m_bModify;

	void DrawItems();
	BOOL ReadPicture(CItemData *pItem);
	void ReadPictureInfo();
	void DrawList(CDC *pDC, CItem *pItem);
	void DrawPicture(CDC *pDC);
	CItemData *GetItemByPoint(CPoint point) const;
	void DrawSelect(CDC* pDC, BOOL bSelect = TRUE);
	void SetViewFont();
	void OpenPictFrame();
	void ChangePicture(HDIB hDIB, BOOL bUpdateView = TRUE);
	void SaveData(CItem *pItem);
	void EditPicture();
	static UINT EditPictureThread(LPVOID pParam);
	void EditPictureSub();

	DECLARE_MESSAGE_MAP()
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual void OnInitialUpdate();     // 構築後、最初の処理。
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnGetSelectedItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdatePict(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateFilePrint(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI *pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
