// OptTwain.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "OptTwain.h"
#include "LocalDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptTwain プロパティ ページ

COptTwain::COptTwain() : CPropertyPage(COptTwain::IDD)
{
}

void COptTwain::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TWAIN_DRIVER, m_sTwainDriver);
}

BEGIN_MESSAGE_MAP(COptTwain, CPropertyPage)
	ON_BN_CLICKED(IDC_SELECT_DRIVER, OnSelectDriver)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptTwain メッセージ ハンドラ

BOOL COptTwain::OnInitDialog()
{
	TW_IDENTITY dsID;
	if (GetDefaultDriver(m_hWnd, &dsID))
		m_sTwainDriver = dsID.ProductName;

	CPropertyPage::OnInitDialog();

	return TRUE;
}

void COptTwain::OnSelectDriver()
{
	SelectTwainDriver(m_hWnd);

	TW_IDENTITY dsID;
	if (GetDefaultDriver(m_hWnd, &dsID)) {
		m_sTwainDriver = dsID.ProductName;
		UpdateData(FALSE);
		CancelToClose();
	}
}
