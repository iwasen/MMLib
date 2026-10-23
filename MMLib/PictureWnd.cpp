// PictureWnd.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "PictureWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CPictureWnd

CPictureWnd::CPictureWnd()
{
	m_hDIB = NULL;
	m_oBrushBkgnd.CreateStockObject(GRAY_BRUSH);
}

CPictureWnd::~CPictureWnd()
{
	if (m_hDIB != NULL)
		MMPictureFree(m_hDIB);
}

BEGIN_MESSAGE_MAP(CPictureWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPictureWnd メッセージ ハンドラ

void CPictureWnd::OnPaint()
{
	if (m_hDIB != NULL) {
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		MMPictureDraw(m_hDIB, dc.m_hDC, m_nLeft, m_nTop, m_nWidth, m_nHeight, 0, 0, -1, -1);
	} else
		CWnd::OnPaint();
}

void CPictureWnd::DrawDIB(HDIB hDIB)
{
	if (m_hDIB != NULL) {
		MMPictureFree(m_hDIB);
		m_hDIB = NULL;
	}

	if (hDIB != NULL) {
		RECT rect;
		GetClientRect(&rect);

		if ((m_hDIB = MakeSmallPicture(hDIB, rect.right, rect.bottom)) != NULL) {
			short nColor;
			MMPictureGetInfo(m_hDIB, &m_nWidth, &m_nHeight, &nColor);
			m_nLeft = (short)((rect.right - m_nWidth) / 2);
			m_nTop = (short)((rect.bottom - m_nHeight) / 2);
		}
	}

	Invalidate(TRUE);
}

BOOL CPictureWnd::OnEraseBkgnd(CDC* pDC)
{
	RECT rect;
	GetClientRect(&rect);
	pDC->FillRect(&rect, &m_oBrushBkgnd);
	return TRUE;
}
