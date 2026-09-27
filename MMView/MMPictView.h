// MMPictView.h : CMMPictView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#pragma once


class CMMPictView : public CScrollView
{
protected: // シリアライズ機能のみから作成します。
	CMMPictView();
	DECLARE_DYNCREATE(CMMPictView)

public:
	virtual ~CMMPictView();
	CMMViewDoc* GetDocument();

protected:
	double m_fPictRatio;
	SIZE m_sizeDraw;
	CBrush m_oBrushBkgndPict;
	CBrush m_oBrushBkgndIcon;
	int m_nAutoSize;
	BOOL m_bScrollPicture;
	CPoint m_posPicture;
	BOOL m_bIgnoreUpdate;

	void DrawItems();
	void DrawPicture(CDC *pDC);
	void DrawIcon(CDC *pDC);
	int GetItemByPoint(CPoint point);

	DECLARE_MESSAGE_MAP()
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void OnInitialUpdate(); // 構築後の最初の１度だけ呼び出されます。
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPictLeftRotate();
	afx_msg void OnUpdatePictLeftRotate(CCmdUI* pCmdUI);
	afx_msg void OnPictRightRotate();
	afx_msg void OnUpdatePictRightRotate(CCmdUI* pCmdUI);
	afx_msg void OnPictEnlarge();
	afx_msg void OnUpdatePictEnlarge(CCmdUI* pCmdUI);
	afx_msg void OnPictReduce();
	afx_msg void OnUpdatePictReduce(CCmdUI* pCmdUI);
	afx_msg void OnPictAutosize();
	afx_msg void OnUpdatePictAutosize(CCmdUI* pCmdUI);
	afx_msg void OnPictAutosize2();
	afx_msg void OnUpdatePictAutosize2(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnUpdateCombo(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnSelchangeCombo();
	afx_msg void OnEditchangeCombo();
};

#ifndef _DEBUG  // MMPictView.cpp ファイルがデバッグ環境の時使用されます。
inline CMMViewDoc* CMMPictView::GetDocument()
   { return (CMMViewDoc*)m_pDocument; }
#endif
