// OptProperty.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "OptProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// COptProperty

COptProperty::COptProperty(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPropertyPage();
}

COptProperty::COptProperty(LPCSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPropertyPage();
}

BEGIN_MESSAGE_MAP(COptProperty, CPropertySheet)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptProperty メッセージ ハンドラ

void COptProperty::AddPropertyPage()
{
	AddPage(&m_oOptFolder);
	AddPage(&m_oOptFont);
	AddPage(&m_oOptOrder);
	AddPage(&m_oOptTwain);
	AddPage(&m_oOptImageEditor);

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
}
