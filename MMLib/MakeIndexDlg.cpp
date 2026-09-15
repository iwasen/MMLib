// MakeIndexDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "MMLibDoc.h"
#include "MakeIndexDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMakeIndexDlg ダイアログ

CMakeIndexDlg::CMakeIndexDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CMakeIndexDlg::IDD, pParent)
{
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
}

void CMakeIndexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIBRARY_LIST, m_cLibraryList);
}

BEGIN_MESSAGE_MAP(CMakeIndexDlg, CDialogAF)
	ON_BN_CLICKED(IDC_ALL_SELECT, OnAllSelect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeIndexDlg メッセージ ハンドラ

BOOL CMakeIndexDlg::OnInitDialog()
{
	CDialogAF::OnInitDialog();

	SetLibraryListBox();

	return TRUE;
}

void CMakeIndexDlg::OnOK()
{
	if (CreateAllIndex())
		CDialogAF::OnOK();
}

void CMakeIndexDlg::OnAllSelect()
{
	int n = m_cLibraryList.GetCount();

	for (int i = 0; i < n; i++)
		m_cLibraryList.SetSel(i);
}

int CMakeIndexDlg::SetLibraryListBox()
{
	CItem *pItem;
	int i, nIndex;

	HTREEITEM hItem = g_pTreeCtrl->GetChildItem(g_pTreeCtrl->GetRootItem());
	for (i = 0; hItem != NULL; i++) {
		pItem = (CItem *)g_pTreeCtrl->GetItemData(hItem);
		switch (pItem->m_nItemType) {
		case ITEM_FOLDER:
		case ITEM_INBOX:
			nIndex = m_cLibraryList.AddString(((CItemFolder *)pItem)->m_oFolderRec.sFolderName);
			m_cLibraryList.SetItemDataPtr(nIndex, hItem);
			break;
		case ITEM_PENDING:
			nIndex = m_cLibraryList.AddString(((CItemPending *)pItem)->m_sTitle);
			m_cLibraryList.SetItemDataPtr(nIndex, hItem);
			break;
		}

		hItem = g_pTreeCtrl->GetNextSiblingItem(hItem);
	}

	return i;
}

BOOL CMakeIndexDlg::CreateAllIndex()
{
	HTREEITEM hItem;
	CItem *pItem;
	CPcserve oPcserve;
	CPSDB oDb;
	BOOL bSel = FALSE;
	int n = m_cLibraryList.GetCount();

	if (!CreateKeywordIndex(oPcserve, oDb)) {
		::AfxMessageBox(IDS_CREAE_KEYWORD_INDEX_FAIL);
		return FALSE;
	}

	for (int i = 0; i < n; i++) {
		if (m_cLibraryList.GetSel(i)) {
			hItem = (HTREEITEM)m_cLibraryList.GetItemData(i);

			pItem = (CItem *)g_pTreeCtrl->GetItemData(hItem);
			if (pItem->m_nItemType == ITEM_PENDING)
				m_pDocument->UpdateAllViews(NULL, VIEWHINT_SELECTITEM, pItem);

			pItem = (CItem *)g_pTreeCtrl->GetItemData(hItem);
			if (pItem->m_nItemType == ITEM_FOLDER || pItem->m_nItemType == ITEM_INBOX) {
				if (!WriteKeywordIndex((CItemFolder *)pItem, oDb)) {
					::AfxMessageBox(IDS_CREATE_KEYWORD_INDEX_ERROR);
					return FALSE;
				}
			}

			bSel = TRUE;
		}
	}

	if (!bSel) {
		::AfxMessageBox(IDS_SELECT_LIBRARY);
		return FALSE;
	}

	g_pTreeCtrl->SelectItem(g_pTreeCtrl->GetRootItem());

	::AfxMessageBox(IDS_CREATE_KEYWORD_INDEX_NORMAL);
	return TRUE;
}

BOOL CMakeIndexDlg::CreateKeywordIndex(CPcserve &oPcserve, CPSDB &oDB)
{
	static DBF_I field[] = {
		{"LID",     'C', LEN_ID,         0},	// ライブラリＩＤ
		{"LNAME",   'C', LEN_FOLDERNAME, 0},	// ライブラリ名
		{"IDTYPE",  'C', 1,              0},	// フォルダ／データ区分
		{"ID",      'C', LEN_ID,         0},	// フォルダ／データＩＤ
		{"FTYPE",   'C', LEN_FOLDERTYPE, 0},	// フォルダタイプ
		{"TITLE",   'C', LEN_TITLE,      0},	// 画像タイトル
		{"CREATOR", 'C', LEN_CREATOR,    0},	// 作成者
		{"CDATE",   'C', LEN_DATE,       0},	// 作成年月日時分
		{"REMARK",  'C', LEN_REMARK,     0},	// 備考
		{"KEYWORD", 'C', LEN_KEYWORD,    0}		// キーワード
	};
	CString dir, path;

	if (oPcserve.Connect(CHT_LOCAL, "", "MMLIB") != 0)
		return FALSE;

	::GetKeywordDir(dir);
	CreateDirectoryAll(dir);

	path.Format("%s\\%s", (LPCSTR)dir, FN_KEYWORD_DBF);
	if (oDB.DBCreate(oPcserve, path, field, sizeof(field) / sizeof(DBF_I)) != 0)
		return FALSE;

	return TRUE;
}

BOOL CMakeIndexDlg::WriteKeywordIndex(CItemFolder *pItemFolder, CPSDB &oDB)
{
	CMMPcsDB *pPcsDB = pItemFolder->m_pPcsDB;
	LPCTSTR pLibraryID = pItemFolder->m_pPcsDB->m_sLibraryID;
	CPSDB *pDB;
	int nLen;
	CWaitCursor wait;

	pPcsDB->SetFolderIndex(0);
	pDB = &pPcsDB->m_oFolderDB;
	pDB->DBSetReadField("FID+FNAME+CREATOR+CDATE+REMARK1+REMARK2+FTYPE", &nLen);
	if (!WriteSub(pDB, oDB, FIND_TYPE_FOLDER, pLibraryID, pItemFolder->m_oFolderRec.sFolderName)) {
		pDB->DBSetReadField(NULL, &nLen);
		return FALSE;
	}
	pDB->DBSetReadField(NULL, &nLen);

	pPcsDB->SetDataIndex(0);
	pDB = &pPcsDB->m_oDataDB;
	pDB->DBSetReadField("DID+TITLE+CREATOR+CDATE+REMARK1+REMARK2", &nLen);
	if (!WriteSub(pDB, oDB, FIND_TYPE_DATA, pLibraryID, pItemFolder->m_oFolderRec.sFolderName)) {
		pDB->DBSetReadField(NULL, &nLen);
		return FALSE;
	}
	pDB->DBSetReadField(NULL, &nLen);

	return TRUE;
}

BOOL CMakeIndexDlg::WriteSub(CPSDB *pDB, CPSDB &oDB, char type, LPCTSTR pLibraryID, LPCTSTR pLibraryName)
{
	KEYWORDBUF keywordBuf;
	struct {
		char	id[LEN_ID];
		char	title[LEN_TITLE];
		char	creator[LEN_CREATOR];
		char	createDate[LEN_DATE];
		char	remark[LEN_REMARK];
		char	keyword[LEN_KEYWORD];
		char	folderType[LEN_FOLDERTYPE];
	} readBuf;
	int nRead;
	int i;

	readBuf.folderType[0] = ' ';
	pDB->DBTop();
	while (pDB->DBReadNext(1, &readBuf, &nRead) == 0) {
		if (nRead == 0)
			break;

		for (i = 0; i < LEN_KEYWORD; i++) {
			if (readBuf.keyword[i] != ' ')
				break;
		}

		if (i < LEN_KEYWORD) {
			if (memcmp(readBuf.id, FID_LIBRARY, LEN_ID) != 0) {
				memcpy(keywordBuf.libraryID, pLibraryID, LEN_ID);
				memcpy(keywordBuf.libraryName, pLibraryName, LEN_FOLDERNAME);
				keywordBuf.idType[0] = type;
				memcpy(keywordBuf.id, readBuf.id, LEN_ID);
				keywordBuf.folderType[0] = readBuf.folderType[0];
				memcpy(keywordBuf.title, readBuf.title, LEN_TITLE);
				memcpy(keywordBuf.creator, readBuf.creator, LEN_CREATOR);
				memcpy(keywordBuf.createDate, readBuf.createDate, LEN_DATE);
				memcpy(keywordBuf.remark, readBuf.remark, LEN_REMARK);
				memcpy(keywordBuf.keyword, readBuf.keyword, LEN_KEYWORD);

				if (oDB.DBStore(&keywordBuf) != 0)
					return FALSE;
			}
		}
	}

	return TRUE;
}
