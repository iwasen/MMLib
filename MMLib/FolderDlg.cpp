// FolderDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "FolderDlg.h"
#include "TextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFolderDlg ダイアログ

CFolderDlg::CFolderDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CFolderDlg::IDD, pParent)
{
}

void CFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COPY_DATA, m_cCopyData);
	DDX_Text(pDX, IDC_CREATOR, m_sCreator);
	DDX_Text(pDX, IDC_FOLDERNAME, m_sFolderName);
	DDX_Text(pDX, IDC_REMARK, m_sRemark);
	DDX_Text(pDX, IDC_KEYWORD, m_sKeyword);
}

BEGIN_MESSAGE_MAP(CFolderDlg, CDialogAF)
	ON_BN_CLICKED(IDC_TEXT_DATA, OnTextData)
	ON_BN_CLICKED(IDC_COPY_DATA, OnCopyData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFolderDlg メッセージ ハンドラ

BOOL CFolderDlg::OnInitDialog()
{
	CDialogAF::OnInitDialog();

	SetWindowText(m_sCaption);

	if (m_oFolderRec.sFolderName.IsEmpty())
		m_cCopyData.EnableWindow(FALSE);

	return TRUE;
}

void CFolderDlg::OnTextData()
{
	CTextDlg dlg;

	dlg.m_sText = m_sTextData;
	dlg.DoModal();
	m_sTextData = dlg.m_sText;
}

void CFolderDlg::OnCopyData()
{
	m_sCreator = m_oFolderRec.sCreator;
	m_sFolderName = m_oFolderRec.sFolderName;
	m_sRemark = m_oFolderRec.sRemark;
	m_sKeyword = m_oFolderRec.sKeyword;

	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CFolderNewDlg ダイアログ

CFolderNewDlg::CFolderNewDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CFolderNewDlg::IDD, pParent)
{
	m_iFolderType = -1;
}

void CFolderNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COPY_DATA, m_cCopyData);
	DDX_Text(pDX, IDC_CREATOR, m_sCreator);
	DDX_Text(pDX, IDC_FOLDERNAME, m_sFolderName);
	DDX_Radio(pDX, IDC_GROUP_FOLDER, m_iFolderType);
	DDX_Text(pDX, IDC_REMARK, m_sRemark);
	DDX_Text(pDX, IDC_KEYWORD, m_sKeyword);
}

BEGIN_MESSAGE_MAP(CFolderNewDlg, CDialogAF)
	ON_BN_CLICKED(IDC_TEXT_DATA, OnTextData)
	ON_BN_CLICKED(IDC_COPY_DATA, OnCopyData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFolderNewDlg メッセージ ハンドラ

BOOL CFolderNewDlg::OnInitDialog()
{
	CDialogAF::OnInitDialog();

	if (m_oFolderRec.sFolderName.IsEmpty())
		m_cCopyData.EnableWindow(FALSE);

	return TRUE;
}

void CFolderNewDlg::OnOK()
{
	UpdateData(TRUE);

	if (m_iFolderType == -1) {
		::AfxMessageBox(IDS_SELECT_FOLDER_TYPE);
		return;
	}

	CDialogAF::OnOK();
}

void CFolderNewDlg::OnTextData()
{
	CTextDlg dlg;

	dlg.m_sText = m_sTextData;
	dlg.DoModal();
	m_sTextData = dlg.m_sText;
}

void CFolderNewDlg::OnCopyData()
{
	m_sCreator = m_oFolderRec.sCreator;
	m_sFolderName = m_oFolderRec.sFolderName;
	m_sRemark = m_oFolderRec.sRemark;
	m_sKeyword = m_oFolderRec.sKeyword;

	UpdateData(FALSE);
}
