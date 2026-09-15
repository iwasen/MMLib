// PasswordDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "PasswordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg ダイアログ

CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CPasswordDlg::IDD, pParent)
{
	m_pItemFolder = NULL;
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIBRARY, m_cLibrary);
	DDX_Text(pDX, IDC_PASSWORD, m_sPassword);
	DDX_Text(pDX, IDC_PASSWORD2, m_sPassword2);
}

BEGIN_MESSAGE_MAP(CPasswordDlg, CDialogAF)
	ON_CBN_SELCHANGE(IDC_LIBRARY, OnSelchangeLibrary)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg メッセージ ハンドラ

BOOL CPasswordDlg::OnInitDialog()
{
	SetPassword();

	CDialogAF::OnInitDialog();

	int n = SetLibraryComboBox(m_cLibrary);
	if (n == 1)
		m_cLibrary.SetCurSel(0);
	else if (n != 0) {
		for (int i = 0; i < n; i++) {
			if (m_cLibrary.GetItemDataPtr(i) == m_pItemFolder) {
				m_cLibrary.SetCurSel(i);
				break;
			}
		}
	}

	return TRUE;
}

void CPasswordDlg::OnOK()
{
	UpdateData(TRUE);

	if (m_sPassword != m_sPassword2) {
		::AfxMessageBox(IDS_PASSWORD_UNMATCH);
		return;
	}

	int nIndex;
	if ((nIndex = m_cLibrary.GetCurSel()) == LB_ERR) {
		::AfxMessageBox(IDS_SELECT_LIBRARY);
		return;
	}
	m_pItemFolder = (CItemFolder *)m_cLibrary.GetItemDataPtr(nIndex);

	CDialogAF::OnOK();
}

void CPasswordDlg::SetPassword()
{
	if (m_pItemFolder == NULL) {
		m_sPassword.Empty();
		m_sPassword2.Empty();
	} else
		m_sPassword = m_sPassword2 = m_pItemFolder->m_pPcsDB->m_sPassword;
}

void CPasswordDlg::OnSelchangeLibrary()
{
	int nIndex;

	if ((nIndex = m_cLibrary.GetCurSel()) == LB_ERR)
		return;

	m_pItemFolder = (CItemFolder *)m_cLibrary.GetItemDataPtr(nIndex);

	SetPassword();
	UpdateData(FALSE);
}
