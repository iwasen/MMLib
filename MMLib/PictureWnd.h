#pragma once

// PictureWnd.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPictureWnd ウィンドウ

class CPictureWnd : public CWnd
{
public:
	CPictureWnd();
	virtual ~CPictureWnd();

	void DrawDIB(HDIB hDIB);

protected:
	HDIB m_hDIB;
	short m_nWidth, m_nHeight;
	short m_nLeft, m_nTop;
	CBrush m_oBrushBkgnd;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
