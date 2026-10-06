// PictRotateDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "PictRotateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CPictRotateDlg ダイアログ

CPictRotateDlg::CPictRotateDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CPictRotateDlg::IDD, pParent)
{
	m_iRotate = -1;
}

void CPictRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RIGHT_ROTATE, m_iRotate);
}

BEGIN_MESSAGE_MAP(CPictRotateDlg, CDialogAF)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPictRotateDlg メッセージ ハンドラ

BOOL CPictRotateDlg::OnInitDialog()
{
	m_iRotate = theApp.GetProfileInt(g_sSectionSettings, "PictRotate", -1);

	CDialogAF::OnInitDialog();

	return TRUE;
}

void CPictRotateDlg::OnOK()
{
	UpdateData(TRUE);
	if (m_iRotate == -1) {
		::AfxMessageBox(IDS_SELECT_ROTATION);
		return;
	}

	theApp.WriteProfileInt(g_sSectionSettings, "PictRotate", m_iRotate);

	switch (m_iRotate) {
	case 0:
		m_nRotate = 1;
		break;
	case 1:
		m_nRotate = 3;
		break;
	case 2:
		m_nRotate = 2;
		break;
	}

	CDialogAF::OnOK();
}
