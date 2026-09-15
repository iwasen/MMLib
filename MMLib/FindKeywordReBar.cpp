// FindKeywordReBar.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "FindKeywordReBar.h"

// CFindKeywordReBar

IMPLEMENT_DYNAMIC(CFindKeywordReBar, CReBar)

BEGIN_MESSAGE_MAP(CFindKeywordReBar, CReBar)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CFindKeywordReBar メッセージ ハンドラ

BOOL CFindKeywordReBar::OnEraseBkgnd(CDC* pDC)
{
	RECT rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect, 0xeeeeee);

	return TRUE;
}
