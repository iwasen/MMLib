// PrintDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "PrintDlg.h"

// CPrintDlg ダイアログ

IMPLEMENT_DYNAMIC(CPrintDlg, CPrintDialog)

CPrintDlg::CPrintDlg(BOOL bPrintSetupOnly, DWORD dwFlags, CWnd* pParentWnd)
	: CPrintDialog(bPrintSetupOnly, dwFlags, pParentWnd)
{
}

void CPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CPrintDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPrintDlg, CPrintDialog)
END_MESSAGE_MAP()

// CPrintDlg メッセージ ハンドラ

BOOL CPrintDlg::OnInitDialog()
{
	CPrintDialog::OnInitDialog();

	CRect rcClinet;
	GetClientRect(rcClinet);

	CRect rcOK;
	GetDlgItem(IDOK)->GetWindowRect(rcOK);
	ScreenToClient(rcOK);

	m_cMargin.Create(GetString(IDS_MARGIN), BS_CHECKBOX | BS_AUTOCHECKBOX | WS_VISIBLE, CRect(20, rcOK.top, 100, rcOK.bottom), this, 999);
	m_cMargin.SetCheck(g_bPrintMargin);
	m_cMargin.SetFont(GetFont());

	return TRUE;
}

void CPrintDlg::OnOK()
{
	g_bPrintMargin = m_cMargin.GetCheck();

	CPrintDialog::OnOK();
}
