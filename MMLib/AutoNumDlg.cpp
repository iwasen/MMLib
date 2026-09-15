// AutoNumDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "AutoNumDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoNumDlg ダイアログ

CAutoNumDlg::CAutoNumDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CAutoNumDlg::IDD, pParent)
{
	m_lStep = 0;
	m_bAutoNumbering = FALSE;
}

void CAutoNumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STEP, m_cStep);
	DDX_Control(pDX, IDC_CODE, m_cCode);
	DDX_Text(pDX, IDC_CODE, m_sCode);
	DDX_Text(pDX, IDC_STEP, m_lStep);
	DDX_Check(pDX, IDC_AUTO_NUMBERING, m_bAutoNumbering);
}

BEGIN_MESSAGE_MAP(CAutoNumDlg, CDialogAF)
	ON_BN_CLICKED(IDC_AUTO_NUMBERING, OnAutoNumbering)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoNumDlg メッセージ ハンドラ

BOOL CAutoNumDlg::OnInitDialog()
{
	CDialogAF::OnInitDialog();

	EnableControl();

	return TRUE;
}

void CAutoNumDlg::OnAutoNumbering()
{
	EnableControl();
}

void CAutoNumDlg::EnableControl()
{
	UpdateData(TRUE);

	if (m_bAutoNumbering) {
		m_cCode.EnableWindow(TRUE);
		m_cStep.EnableWindow(TRUE);
	} else {
		m_cCode.EnableWindow(FALSE);
		m_cStep.EnableWindow(FALSE);
	}
}
