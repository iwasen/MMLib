// PermissionDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "PermissionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPermissionDlg ダイアログ

CPermissionDlg::CPermissionDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CPermissionDlg::IDD, pParent)
{
	m_iPermission = -1;

	m_pItemFolder = NULL;
}

void CPermissionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIBRARY, m_cLibrary);
	DDX_Radio(pDX, IDC_RADIO1, m_iPermission);
}

BEGIN_MESSAGE_MAP(CPermissionDlg, CDialogAF)
	ON_CBN_SELCHANGE(IDC_LIBRARY, OnSelchangeLibrary)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPermissionDlg メッセージ ハンドラ

BOOL CPermissionDlg::OnInitDialog()
{
	SetRadioButton();

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

void CPermissionDlg::OnOK()
{
	UpdateData(TRUE);

	int nIndex;
	if ((nIndex = m_cLibrary.GetCurSel()) == LB_ERR) {
		::AfxMessageBox(IDS_SELECT_LIBRARY);
		return;
	}
	m_pItemFolder = (CItemFolder *)m_cLibrary.GetItemDataPtr(nIndex);
	GetRadioButton();

	CDialogAF::OnOK();
}

void CPermissionDlg::SetRadioButton()
{
	if (m_pItemFolder == NULL)
		m_iPermission = -1;
	else {
		switch (m_pItemFolder->m_pPcsDB->m_dwPermission) {
		case PERMISSION_READ:
			m_iPermission = 0;
			break;
		case PERMISSION_READ | PERMISSION_ADD:
			m_iPermission = 1;
			break;
		case PERMISSION_READ | PERMISSION_ADD | PERMISSION_UPDATE:
			m_iPermission = 2;
			break;
		default:
			m_iPermission = 3;
			break;
		}
	}
}

void CPermissionDlg::GetRadioButton()
{
	switch (m_iPermission) {
	case 0:
		m_dwPermission = PERMISSION_READ;
		break;
	case 1:
		m_dwPermission = PERMISSION_READ | PERMISSION_ADD;
		break;
	case 2:
		m_dwPermission = PERMISSION_READ | PERMISSION_ADD | PERMISSION_UPDATE;
		break;
	case 3:
		m_dwPermission = PERMISSION_FULL;
		break;
	}
}

void CPermissionDlg::OnSelchangeLibrary()
{
	int nIndex;

	if ((nIndex = m_cLibrary.GetCurSel()) == LB_ERR)
		return;

	m_pItemFolder = (CItemFolder *)m_cLibrary.GetItemDataPtr(nIndex);
	SetRadioButton();
	UpdateData(FALSE);
}
