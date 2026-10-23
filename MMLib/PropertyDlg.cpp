// PropertyDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "PropertyDlg.h"

// CPropertyDlg ダイアログ

IMPLEMENT_DYNAMIC(CPropertyDlg, CDialogAF)

CPropertyDlg::CPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CPropertyDlg::IDD, pParent)
	, m_sEditName("")
{
}

void CPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_sEditName);
	DDX_Text(pDX, IDC_EDIT_TYPE, m_sEditType);
	DDX_Text(pDX, IDC_EDIT_LIBRARY, m_sEditLibrary);
	DDX_Text(pDX, IDC_EDIT_FOLDER, m_sEditFolder);
	DDX_Control(pDX, IDC_BUTTON_FIND, m_cButtonFind);
}

BEGIN_MESSAGE_MAP(CPropertyDlg, CDialogAF)
	ON_BN_CLICKED(IDC_BUTTON_FIND, &CPropertyDlg::OnBnClickedButtonFind)
END_MESSAGE_MAP()

// CPropertyDlg メッセージ ハンドラ

BOOL CPropertyDlg::OnInitDialog()
{
	switch (m_pItem->m_nItemType) {
	case ITEM_FOLDER:
		DispFolder();
		break;
	case ITEM_DATA:
		DispData();
		break;
	}

	CDialogAF::OnInitDialog();

	for (CItem *pItem = m_pItem; pItem != NULL; pItem = pItem->GetParent()) {
		if (pItem->m_nItemType == ITEM_FIND) {
			m_cButtonFind.ShowWindow(SW_SHOW);
			break;
		}
	}

	return TRUE;
}

void CPropertyDlg::DispFolder()
{
	CItemFolder *pItem = (CItemFolder *)m_pItem;

	m_sEditName = pItem->m_oFolderRec.sFolderName;
	switch (pItem->m_nFolderType) {
	case FTYPE_GFOLDER:
		m_sEditType.LoadString(IDS_GFOLDER);
		break;
	case FTYPE_DFOLDER:
		m_sEditType.LoadString(IDS_DFOLDER);
		break;
	}
	m_sEditLibrary = pItem->m_pPcsDB->m_sTitle;
	GetFolder(pItem->m_pPcsDB, pItem->m_oFolderRec.sFolderID, m_sEditFolder);
}

void CPropertyDlg::DispData()
{
	CItemData *pItem = (CItemData *)m_pItem;

	m_sEditName = pItem->m_oDataRec.sTitle;
	m_sEditType.LoadString(IDS_DATA);
	m_sEditLibrary = pItem->m_pPcsDB->m_sTitle;
	GetFolder(pItem->m_pPcsDB, pItem->m_oDataRec.sFolderID, m_sEditFolder);
}

void CPropertyDlg::GetFolder(CMMPcsDB *pPcsDB, const CString &sFolderId, CString &sFolder)
{
	CPSDB &oFolderDB = pPcsDB->m_oFolderDB;
	int nFind;

	pPcsDB->SetFolderIndex(1);
	FOLDERBUF oFolderBuf;
	if (oFolderDB.DBReadKey((LPCSTR)sFolderId, LEN_ID, &oFolderBuf, 0, &nFind) == 0) {
		if (nFind) {
			FOLDERREC oFolderRec;
			oFolderDB.DBGetFieldBuf(&oFolderBuf, &oFolderRec);

			if (oFolderRec.sFolderType != FTYPE_LIBRARY) {
				sFolder = "[" + oFolderRec.sFolderName + "]" + sFolder;
				GetFolder(pPcsDB, oFolderRec.sParentFolderID, sFolder);
			}
		}
	}
}

void CPropertyDlg::OnBnClickedButtonFind()
{
	EndDialog(IDC_BUTTON_FIND);
}
