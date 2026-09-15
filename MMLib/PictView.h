#pragma once

// PictView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPictView ビュー

class CPictView : public CScrollView
{
protected:
	CPictView();           // 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CPictView)

	SIZE m_sizeDraw;
	BOOL m_bIgnoreUpdate;
	CBrush m_hBrushBkgnd;
	BOOL m_bScrollPicture;
	CPoint m_posPicture;

	void DrawPicture(CDC *pDC);

	DECLARE_MESSAGE_MAP()
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual void OnInitialUpdate();     // 構築後、最初の処理。
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
};
