// MMLibDoc.cpp : CMMLibDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "MMLib.h"
#include "MMLibDoc.h"
#include "LibOpenDlg.h"
#include "LibNewDlg.h"
#include "FolderDlg.h"
#include "DataDlg.h"
#include "FindDlg.h"
#include "FindSimpleDlg.h"
#include "OptProperty.h"
#include "BackupDlg.h"
#include "RestoreDlg.h"
#include "TransRegDlg.h"
#include "PictRotateDlg.h"
#include "PasswordDlg.h"
#include "PermissionDlg.h"
#include "FindKeywordFrame.h"
#include "FindKeywordDoc.h"
#include "MailAttDlg.h"
#include "FindPictDlg.h"
#include "AccessLogDlg.h"
#include "PropertyDlg.h"
#include "MainFrm.h"
#include "DbArchive.h"
#include "Shlwapi.h"
#include "OutputImagesDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMMLibDoc

IMPLEMENT_DYNCREATE(CMMLibDoc, CDocument)

BEGIN_MESSAGE_MAP(CMMLibDoc, CDocument)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(IDM_VIEW_PICT, OnViewPict)
	ON_COMMAND(IDM_VIEW_LIST, OnViewList)
	ON_COMMAND(IDM_VIEW_DETAIL, OnViewDetail)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateFileClose)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_PICT, OnUpdateViewPict)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_LIST, OnUpdateViewList)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_DETAIL, OnUpdateViewDetail)
	ON_COMMAND(ID_FILE_UPDATE, OnFileUpdate)
	ON_COMMAND(IDM_FILE_DELETE, OnFileDelete)
	ON_COMMAND(IDM_PICT_OPEN, OnPictOpen)
	ON_COMMAND(IDM_VIEW_RELOAD, OnViewReload)
	ON_COMMAND(IDM_VIEW_PREV, OnViewPrev)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_PREV, OnUpdateViewPrev)
	ON_COMMAND(IDM_VIEW_NEXT, OnViewNext)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_NEXT, OnUpdateViewNext)
	ON_COMMAND(IDM_PICT_ENLARGE, OnPictEnlarge)
	ON_UPDATE_COMMAND_UI(IDM_PICT_ENLARGE, OnUpdatePictEnlarge)
	ON_COMMAND(IDM_PICT_REDUCE, OnPictReduce)
	ON_UPDATE_COMMAND_UI(IDM_PICT_REDUCE, OnUpdatePictReduce)
	ON_COMMAND(IDM_PICT_RIGHT_ROTATE, OnPictRightRotate)
	ON_UPDATE_COMMAND_UI(IDM_PICT_RIGHT_ROTATE, OnUpdatePictRightRotate)
	ON_COMMAND(IDM_PICT_LEFT_ROTATE, OnPictLeftRotate)
	ON_UPDATE_COMMAND_UI(IDM_PICT_LEFT_ROTATE, OnUpdatePictLeftRotate)
	ON_COMMAND(IDM_PICT_AUTOSIZE, OnPictAutosize)
	ON_UPDATE_COMMAND_UI(IDM_PICT_AUTOSIZE, OnUpdatePictAutosize)
	ON_COMMAND(IDM_DETAIL_NAME, OnDetailName)
	ON_UPDATE_COMMAND_UI(IDM_DETAIL_NAME, OnUpdateDetailName)
	ON_COMMAND(IDM_DETAIL_CODE, OnDetailCode)
	ON_UPDATE_COMMAND_UI(IDM_DETAIL_CODE, OnUpdateDetailCode)
	ON_COMMAND(IDM_DETAIL_CDATE, OnDetailCdate)
	ON_UPDATE_COMMAND_UI(IDM_DETAIL_CDATE, OnUpdateDetailCdate)
	ON_COMMAND(IDM_DETAIL_UDATE, OnDetailUdate)
	ON_UPDATE_COMMAND_UI(IDM_DETAIL_UDATE, OnUpdateDetailUdate)
	ON_COMMAND(IDM_DETAIL_CREATOR, OnDetailCreator)
	ON_UPDATE_COMMAND_UI(IDM_DETAIL_CREATOR, OnUpdateDetailCreator)
	ON_COMMAND(IDM_DETAIL_REMARK, OnDetailRemark)
	ON_UPDATE_COMMAND_UI(IDM_DETAIL_REMARK, OnUpdateDetailRemark)
	ON_COMMAND(IDM_DETAIL_KEYWORD, OnDetailKeyword)
	ON_UPDATE_COMMAND_UI(IDM_DETAIL_KEYWORD, OnUpdateDetailKeyword)
	ON_COMMAND(IDM_DETAIL_SERVER, OnDetailServer)
	ON_UPDATE_COMMAND_UI(IDM_DETAIL_SERVER, OnUpdateDetailServer)
	ON_COMMAND(IDM_DETAIL_DIRECTORY, OnDetailDirectory)
	ON_UPDATE_COMMAND_UI(IDM_DETAIL_DIRECTORY, OnUpdateDetailDirectory)
	ON_COMMAND(IDM_VIEW_UPFOLDER, OnViewUpfolder)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_UPFOLDER, OnUpdateViewUpfolder)
	ON_UPDATE_COMMAND_UI(ID_FILE_UPDATE, OnUpdateFileUpdate)
	ON_UPDATE_COMMAND_UI(IDM_FILE_DELETE, OnUpdateFileDelete)
	ON_COMMAND(IDM_FIND, OnFind)
	ON_COMMAND(ID_CACHE_READ, OnCacheRead)
	ON_UPDATE_COMMAND_UI(ID_CACHE_READ, OnUpdateCacheRead)
	ON_COMMAND(ID_CACHE_CLEAR, OnCacheClear)
	ON_UPDATE_COMMAND_UI(ID_CACHE_CLEAR, OnUpdateCacheClear)
	ON_COMMAND(IDM_OPTION, OnOption)
	ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(IDM_PICT_AUTOSIZE2, OnPictAutosize2)
	ON_UPDATE_COMMAND_UI(IDM_PICT_AUTOSIZE2, OnUpdatePictAutosize2)
	ON_CBN_SELCHANGE(IDW_COMBO, OnSelchangeCombo)
	ON_CBN_EDITUPDATE(IDW_COMBO, OnEditchangeCombo)
	ON_UPDATE_COMMAND_UI(IDW_COMBO, OnUpdateCombo)
	ON_COMMAND(IDM_REFIND, OnRefind)
	ON_UPDATE_COMMAND_UI(IDM_REFIND, OnUpdateRefind)
	ON_COMMAND(IDM_FILE_PACK, OnFilePack)
	ON_UPDATE_COMMAND_UI(IDM_FILE_PACK, OnUpdateFilePack)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT, OnUpdateFileExport)
	ON_COMMAND(IDM_BACKUP, OnBackup)
	ON_COMMAND(IDM_RESTORE, OnRestore)
	ON_COMMAND(IDM_TRANS_REG, OnTransReg)
	ON_COMMAND(IDM_IMPORT_BMP, OnImportBmp)
	ON_UPDATE_COMMAND_UI(IDM_IMPORT_BMP, OnUpdateImportBmp)
	ON_COMMAND(IDM_IMPORT_MML, OnImportMml)
	ON_UPDATE_COMMAND_UI(IDM_IMPORT_MML, OnUpdateImportMml)
	ON_COMMAND(IDM_IMPORT_PPM, OnImportPpm)
	ON_UPDATE_COMMAND_UI(IDM_IMPORT_PPM, OnUpdateImportPpm)
	ON_COMMAND(IDM_PICT_ALL_ROTATE, OnPictAllRotate)
	ON_UPDATE_COMMAND_UI(IDM_PICT_ALL_ROTATE, OnUpdatePictAllRotate)
	ON_COMMAND(IDM_PICT_WINDOW, OnPictWindow)
	ON_UPDATE_COMMAND_UI(IDM_PICT_WINDOW, OnUpdatePictWindow)
	ON_COMMAND(IDM_FILE_PASSWORD, OnFilePassword)
	ON_COMMAND(IDM_FILE_PERMISSION, OnFilePermission)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_NEW_DATA, OnNewData)
	ON_COMMAND(IDM_VIEW_MAX, OnViewMax)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_MAX, OnUpdateViewMax)
	ON_COMMAND(IDM_KEYWORD, OnKeyword)
	ON_COMMAND(IDM_EDIT_IMAGE, OnEditImage)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_IMAGE, OnUpdateEditImage)
	ON_COMMAND(IDM_DETAIL_FIRSTDATA, OnDetailFirstdata)
	ON_UPDATE_COMMAND_UI(IDM_DETAIL_FIRSTDATA, OnUpdateDetailFirstdata)
	ON_COMMAND(IDM_FIND_PICT, &CMMLibDoc::OnFindPict)
	ON_UPDATE_COMMAND_UI(IDM_FIND_PICT, &CMMLibDoc::OnUpdateFindPict)
	ON_COMMAND(IDM_ACCESS_HISTORY, &CMMLibDoc::OnAccessHistory)
	ON_COMMAND(ID_FILE_PAGE_SETUP, &CMMLibDoc::OnFilePageSetup)
	ON_COMMAND(IDM_PROPERTY, &CMMLibDoc::OnProperty)
	ON_COMMAND(IDM_VIEW_TEXT_WORDWRAP, &CMMLibDoc::OnViewTextWordwrap)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_TEXT_WORDWRAP, &CMMLibDoc::OnUpdateViewTextWordwrap)
	ON_COMMAND(IDM_IMPORT_MMDB, &CMMLibDoc::OnImportMmdb)
	ON_UPDATE_COMMAND_UI(IDM_IMPORT_MMDB, &CMMLibDoc::OnUpdateImportMmdb)
	ON_COMMAND(IDM_IMPORT_PDF, &CMMLibDoc::OnImportPdf)
	ON_UPDATE_COMMAND_UI(IDM_IMPORT_PDF, &CMMLibDoc::OnUpdateImportPdf)
	ON_UPDATE_COMMAND_UI(IDM_IMAGE_FILES_OUTPUT, &CMMLibDoc::OnUpdateImageFileOutput)
	ON_COMMAND(IDM_IMAGE_FILES_OUTPUT, &CMMLibDoc::OnImageFilesOutput)
	ON_UPDATE_COMMAND_UI(IDM_TEXT_SEARCH, &CMMLibDoc::OnUpdateTextSearch)
	ON_COMMAND(IDM_TEXT_SEARCH, &CMMLibDoc::OnTextSearch)
	END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMMLibDoc クラスの構築/消滅

CMMLibDoc::CMMLibDoc()
{
	static constexpr int defaultFlag[3][DETAIL_NUM]{
		{1, 0, 1, 0, 1, 0, 0, 1, 1, 0},
		{1, 0, 1, 0, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 0, 1, 0, 0, 0, 0, 0}
	};
	static constexpr int defaultWidth[DETAIL_NUM]{
		200, 100, 110, 110, 100, 100, 100, 100, 200, 200
	};
	char entry[16];

	m_nViewMode = theApp.GetProfileInt(g_sSectionSettings, "ViewMode", VIEWMODE_DETAIL);
	m_nAutoSize = theApp.GetProfileInt(g_sSectionSettings, "AutoSize", 0);
	m_bTextWordwrap = theApp.GetProfileInt(g_sSectionSettings, "TextWordwrap", 0);
	m_bPictView = FALSE;
	m_bMaxPict = FALSE;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < DETAIL_NUM; j++) {
			sprintf(entry, "Flag%d-%d", i, j);
			m_bDetailFlag[i][j] = theApp.GetProfileInt(g_sSectionDetails, entry, defaultFlag[i][j]);

			sprintf(entry, "Width%d-%d", i, j);
			m_nDetailWidth[i][j] = theApp.GetProfileInt(g_sSectionDetails, entry, defaultWidth[j]);
		}
	}

	theApp.m_pDocument = this;
}

CMMLibDoc::~CMMLibDoc()
{
	char entry[16];

	theApp.WriteProfileInt(g_sSectionSettings, "ViewMode", m_nViewMode);
	theApp.WriteProfileInt(g_sSectionSettings, "AutoSize", m_nAutoSize);
	theApp.WriteProfileInt(g_sSectionSettings, "TextWordwrap", m_bTextWordwrap);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < DETAIL_NUM; j++) {
			sprintf(entry, "Flag%d-%d", i, j);
			theApp.WriteProfileInt(g_sSectionDetails, entry, m_bDetailFlag[i][j]);

			sprintf(entry, "Width%d-%d", i, j);
			theApp.WriteProfileInt(g_sSectionDetails, entry, m_nDetailWidth[i][j]);
		}
	}
}

BOOL CMMLibDoc::OnNewDocument()
{
//	if (!CDocument::OnNewDocument())
//		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMMLibDoc コマンド

void CMMLibDoc::OpenCurrentLibrary()
{
	CWinApp *pWinApp = ::AfxGetApp();
	CString sEntry;
	CString server, directory;
	CString libraryID;
	CString title;
	int chType;
	int cacheMode;

	const int n = pWinApp->GetProfileInt(g_sSectionLibrary, "Num", 0);
	for (int i = 0; i < n; i++) {
		sEntry.Format("Server%d", i);
		server = pWinApp->GetProfileString(g_sSectionLibrary, sEntry);
		sEntry.Format("ChType%d", i);
		chType = pWinApp->GetProfileInt(g_sSectionLibrary, sEntry, 0);
		sEntry.Format("Directory%d", i);
		directory = pWinApp->GetProfileString(g_sSectionLibrary, sEntry);
		sEntry.Format("CacheMode%d", i);
		cacheMode = pWinApp->GetProfileInt(g_sSectionLibrary, sEntry, 0);
		sEntry.Format("Title%d", i);
		title = pWinApp->GetProfileString(g_sSectionLibrary, sEntry);
		sEntry.Format("LibraryID%d", i);
		libraryID = pWinApp->GetProfileString(g_sSectionLibrary, sEntry);

		CAddItemPending oAddItem;
		oAddItem.m_sTitle = title;
		oAddItem.m_sServerName = server;
		oAddItem.m_nChType = chType;
		oAddItem.m_sDirName = directory;
		oAddItem.m_sLibraryID = libraryID;
		oAddItem.m_nCacheMode = cacheMode;
		UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItem);
	}
}

void CMMLibDoc::OpenFind(CItem *pItemParent)
{
	CFindDB oFindDB;
	if (!oFindDB.Open(FALSE))
		return;

	LPCSTR pFindID;
	if (pItemParent->m_nItemType == ITEM_FOLDER)
		pFindID = ((CItemFolder *)pItemParent)->m_oFolderRec.sParentFolderID;
	else if (pItemParent->m_nItemType == ITEM_ROOT)
		pFindID = "";
	else
		return;

	CAddItemFind oAddItem;
	FINDREC findRec;
	while (oFindDB.ReadFind(pFindID, &findRec)) {
		oAddItem.m_pItemParent = pItemParent;
		oAddItem.m_pFindRec = &findRec;
		oAddItem.m_bSelect = FALSE;
		oAddItem.m_bSave = TRUE;

		UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItem);

		pFindID = NULL;
	}
}

void CMMLibDoc::OnFileNew()
{
	CItem *pItem;

	if ((pItem = GetSelectedItem()) != NULL) {
		switch (pItem->m_nItemType) {
		case ITEM_ROOT:
			CreateLibrary();
			break;
		case ITEM_FOLDER:
		case ITEM_INBOX:
			switch (((CItemFolder *)pItem)->m_nFolderType) {
			case FTYPE_LIBRARY:
			case FTYPE_GFOLDER:
				CreateFolder((CItemFolder *)pItem);
				break;
			case FTYPE_DFOLDER:
				CreateData((CItemFolder *)pItem);
				break;
			}
			break;
		}

		NotifyUpdate();
	}
}

void CMMLibDoc::OnUpdateFileNew(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CheckSelectedItemType(ITEM_ROOT | ITEM_FOLDER | ITEM_INBOX));
}

void CMMLibDoc::CreateLibrary()
{
	CLibNewDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	CWaitCursor wait;

	if (dlg.m_iChType == CHT_LOCAL)
		CreateDirectoryAll(dlg.m_sDirectory);

	FOLDERREC oFolderRec{
		.sFolderType = FTYPE_LIBRARY,
		.sFolderName = dlg.m_sLibName,
		.sCreator = dlg.m_sCreator,
		.sRemark = dlg.m_sRemark,
		.sKeyword = dlg.m_sKeyword
	};

	CMMPcsDB *pPcsDB;
	if ((pPcsDB = g_pMMLibDB->CreateMMLibrary(dlg.m_iChType, dlg.m_sServerName, dlg.m_sDirectory, dlg.m_bCache, &oFolderRec)) == NULL)
		return;

	CAddItemLibrary oAddItem;
	oAddItem.m_pPcsDB = pPcsDB;
	UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItem);
}

void CMMLibDoc::CreateFolder(CItemFolder *pItemFolder)
{
	if (!OfflineCheck(pItemFolder))
		return;

	DWORD userNameSize = MAXUSERNAME;

	CFolderNewDlg dlg;
	GetUserName(dlg.m_sCreator.GetBuffer(MAXUSERNAME), &userNameSize);
	dlg.m_sCreator.ReleaseBuffer();
	dlg.m_oFolderRec = m_oFolderRec;

	if (dlg.DoModal() != IDOK)
		return;

	FOLDERREC oFolderRec{
		.sParentFolderID = pItemFolder->m_oFolderRec.sFolderID,
		.sFolderType = (dlg.m_iFolderType == 0) ? FTYPE_GFOLDER : FTYPE_DFOLDER,
		.sFolderName = dlg.m_sFolderName,
		.sCreator = dlg.m_sCreator,
		.sRemark = dlg.m_sRemark,
		.sKeyword = dlg.m_sKeyword
	};

	if (pItemFolder->m_pPcsDB->AddFolderEx(&oFolderRec)) {
		pItemFolder->m_pPcsDB->WriteTextFolderEx(oFolderRec.sFolderID, &dlg.m_sTextData);

		CAddItemFolder oAddItem;
		oAddItem.m_pItemFolder = pItemFolder;
		oAddItem.m_pFolderRec = &oFolderRec;
		UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItem);

		m_oFolderRec = oFolderRec;
	}
}

void CMMLibDoc::CreateData(CItemFolder *pItemFolder)
{
	if (!OfflineCheck(pItemFolder))
		return;

	CDataDlg dlg;
	dlg.m_pItemFolder = pItemFolder;
	dlg.m_pDocument = this;
	dlg.m_sCaption.LoadString(IDS_NEW_DATA2);
	dlg.m_oDataRec = m_oDataRec;

	dlg.DoModal();

	m_oDataRec = dlg.m_oDataRec;
}

void CMMLibDoc::OnFileOpen()
{
	CItem *pItem;

	if ((pItem = GetSelectedItem()) != NULL) {
		switch (pItem->m_nItemType) {
		case ITEM_ROOT:
			OpenLibrary();
			break;
		case ITEM_FOLDER:
		case ITEM_FIND:
		case ITEM_INBOX:
			UpdateAllViews(NULL, VIEWHINT_OPENITEM, pItem);
			break;
		}
	}
}

void CMMLibDoc::OnUpdateFileOpen(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CheckSelectedItemType(ITEM_ROOT | ITEM_FOLDER | ITEM_FIND | ITEM_INBOX));
}

void CMMLibDoc::OpenLibrary()
{
	CLibOpenDlg dlg;

	if (dlg.DoModal() != IDOK)
		return;

	CWaitCursor wait;

	CMMPcsDB *pPcsDB;

	if ((pPcsDB = g_pMMLibDB->OpenMMLibrary(dlg.m_iChType, dlg.m_sServerName, dlg.m_sDirectory, dlg.m_bCache)) != NULL) {
		CAddItemLibrary oAddItem;
		oAddItem.m_pPcsDB = pPcsDB;
		UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItem);
	}
}

void CMMLibDoc::OnFileClose()
{
	CItem *pItem, *pNextSelected;
	CItemFolder *pItemFolder;
	CItemFind *pItemFind;

	for (pItem = GetSelectedItem(); pItem != NULL; pItem = pNextSelected) {
		pNextSelected = pItem->m_pNextSelected;

		switch (pItem->m_nItemType) {
		case ITEM_FOLDER:
		case ITEM_INBOX:
			pItemFolder = (CItemFolder *)pItem;
			if (pItemFolder->m_nFolderType == FTYPE_LIBRARY) {
				CMMPcsDB *pPcsDB = pItemFolder->m_pPcsDB;
				UpdateAllViews(NULL, VIEWHINT_REMOVEITEM, pItem);
				g_pMMLibDB->CloseMMLibrary(pPcsDB);
			} else
				UpdateAllViews(NULL, VIEWHINT_CLOSEITEM, pItem);
			break;
		case ITEM_FIND:
			pItemFind = (CItemFind *)pItem;
			if (pItemFind->m_bSave) {
				CFindDB oFindDB;
				if (oFindDB.Open(FALSE))
					oFindDB.DeleteFind(pItemFind->m_oFindRec.sFindID);
			}
			UpdateAllViews(NULL, VIEWHINT_REMOVEITEM, pItem);
			break;
		case ITEM_PENDING:
			UpdateAllViews(NULL, VIEWHINT_REMOVEITEM, pItem);
			break;
		}
	}
}

void CMMLibDoc::OnUpdateFileClose(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CheckSelectedItemType(ITEM_ROOT | ITEM_FOLDER | ITEM_PENDING | ITEM_FIND | ITEM_INBOX));
}

void CMMLibDoc::OnViewPict()
{
	m_nViewMode = VIEWMODE_PICT;
	UpdateAllViews(NULL, VIEWHINT_DRAWALL);
}

void CMMLibDoc::OnUpdateViewPict(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(m_nViewMode == VIEWMODE_PICT);
}

void CMMLibDoc::OnViewList()
{
	m_nViewMode = VIEWMODE_LIST;
	UpdateAllViews(NULL, VIEWHINT_DRAWLIST);
}

void CMMLibDoc::OnUpdateViewList(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(m_nViewMode == VIEWMODE_LIST);
}

void CMMLibDoc::OnViewDetail()
{
	m_nViewMode = VIEWMODE_DETAIL;
	UpdateAllViews(NULL, VIEWHINT_DRAWLIST);
}

void CMMLibDoc::OnUpdateViewDetail(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(m_nViewMode == VIEWMODE_DETAIL);
}

void CMMLibDoc::OnFileUpdate()
{
	CItem *pItem, *pNextSelected;

	for (pItem = GetSelectedItem(); pItem != NULL; pItem = pNextSelected) {
		pNextSelected = pItem->m_pNextSelected;

		switch (pItem->m_nItemType) {
		case ITEM_FOLDER:
			UpdateFolder((CItemFolder *)pItem);
			break;
		case ITEM_DATA:
			UpdateData((CItemData *)pItem);
			break;
		case ITEM_FIND:
			UpdateFind((CItemFind *)pItem);
			break;
		}
	}

	NotifyUpdate();
}

void CMMLibDoc::OnUpdateFileUpdate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CheckSelectedItemType(ITEM_FOLDER | ITEM_DATA | ITEM_FIND));
}

void CMMLibDoc::UpdateFolder(CItemFolder *pItemFolder)
{
	if (!OfflineCheck(pItemFolder))
		return;

	FOLDERREC &oFolderRec = pItemFolder->m_oFolderRec;

	CFolderDlg dlg;
	dlg.m_sFolderName = oFolderRec.sFolderName;
	dlg.m_sCreator = oFolderRec.sCreator;
	dlg.m_sRemark = oFolderRec.sRemark;
	dlg.m_sKeyword = oFolderRec.sKeyword;
	pItemFolder->m_pPcsDB->ReadTextFolderEx(pItemFolder->GetID(), &dlg.m_sTextData);
	dlg.m_sCaption.LoadString(pItemFolder->m_nFolderType == FTYPE_LIBRARY ? IDS_LIBRARY_UPDATE : IDS_FOLDER_UPDATE);
	dlg.m_oFolderRec = m_oFolderRec;

	if (dlg.DoModal() != IDOK)
		return;

	oFolderRec.sFolderName = dlg.m_sFolderName;
	oFolderRec.sCreator = dlg.m_sCreator;
	oFolderRec.sRemark = dlg.m_sRemark;
	oFolderRec.sKeyword = dlg.m_sKeyword;

	if (pItemFolder->m_pPcsDB->UpdateFolderEx(&pItemFolder->m_oFolderRec)) {
		pItemFolder->m_pPcsDB->WriteTextFolderEx(pItemFolder->GetID(), &dlg.m_sTextData);
		UpdateAllViews(NULL, VIEWHINT_UPDATEITEM, pItemFolder);

		m_oFolderRec = oFolderRec;
	}
}

void CMMLibDoc::UpdateData(CItemData *pItemData)
{
	if (!OfflineCheck(pItemData))
		return;

	CDataDlg dlg;
	dlg.m_pItemFolder = (CItemFolder *)pItemData->GetParent();
	dlg.m_pItemData = pItemData;
	dlg.m_pDocument = this;
	dlg.m_sCaption.LoadString(IDS_DATA_UPDATE);
	dlg.m_oDataRec = m_oDataRec;

	dlg.DoModal();

	m_oDataRec = dlg.m_oDataRec;
}

void CMMLibDoc::UpdateFind(CItemFind *pItemFind)
{
	if (pItemFind->m_nFindType == FIND_TYPE_SIMPLE)
		UpdateFindSimple(pItemFind);
	else
		UpdateFindExpert(pItemFind);
}

void CMMLibDoc::UpdateFindSimple(CItemFind *pItemFind)
{
	CFindSimpleDlg dlg;
	dlg.m_pDocument = this;
	dlg.m_bUpdateMode = TRUE;
	dlg.m_pItemParent = pItemFind->GetParent();
	dlg.m_sFindFolder = pItemFind->m_oFindRec.sFolderName;
	dlg.m_bSave = pItemFind->m_bSave;
	dlg.m_sSearchText = pItemFind->m_oFindRec.conditions[0].sKeyword;
	if (dlg.DoModal() == IDCANCEL)
		return;

	if (pItemFind->m_bSave && !dlg.m_bSave) {
		CFindDB oFindDB;
		if (oFindDB.Open(FALSE))
			oFindDB.DeleteFind(pItemFind->m_oFindRec.sFindID);
	}

	pItemFind->m_oFindRec.conditions[0].sKeyword = dlg.m_sSearchText;

	if (pItemFind->m_oFindRec.sFolderName != dlg.m_sFindFolder) {
		pItemFind->m_oFindRec.sFolderName = dlg.m_sFindFolder;
		UpdateAllViews(NULL, VIEWHINT_UPDATEITEM, pItemFind);
	}

	if (dlg.m_bSave) {
		CFindDB oFindDB;
		if (oFindDB.Open(TRUE)) {
			if (pItemFind->m_bSave)
				oFindDB.UpdateFind(&pItemFind->m_oFindRec);
			else
				oFindDB.AddFind(&pItemFind->m_oFindRec);
		}
	}

	UpdateAllViews(NULL, VIEWHINT_REFINDITEM, pItemFind);
}

void CMMLibDoc::UpdateFindExpert(CItemFind *pItemFind)
{
	int i;

	CFindDlg dlg;
	dlg.m_pDocument = this;
	dlg.m_bUpdateMode = TRUE;
	dlg.m_pItemParent = pItemFind->GetParent();
	dlg.m_sFindFolder = pItemFind->m_oFindRec.sFolderName;
	dlg.m_nFindType = pItemFind->m_nFindType;
	dlg.m_bSave = pItemFind->m_bSave;

	for (i = 0; i < N_FIND_CONDITIONS; i++)
		dlg.m_aFindConditions[i] = pItemFind->m_oFindRec.conditions[i];

	if (dlg.DoModal() == IDCANCEL)
		return;

	if (pItemFind->m_bSave && !dlg.m_bSave) {
		CFindDB oFindDB;
		if (oFindDB.Open(FALSE))
			oFindDB.DeleteFind(pItemFind->m_oFindRec.sFindID);
	}

	for (i = 0; i < N_FIND_CONDITIONS; i++)
		pItemFind->m_oFindRec.conditions[i] = dlg.m_aFindConditions[i];

	if (pItemFind->m_oFindRec.sFolderName != dlg.m_sFindFolder || pItemFind->m_nFindType != dlg.m_nFindType) {
		pItemFind->m_nFindType = dlg.m_nFindType;
		pItemFind->m_oFindRec.sFindType = (char)pItemFind->m_nFindType;
		pItemFind->m_oFindRec.sFolderName = dlg.m_sFindFolder;
		UpdateAllViews(NULL, VIEWHINT_UPDATEITEM, pItemFind);
	}

	if (dlg.m_bSave) {
		CFindDB oFindDB;
		if (oFindDB.Open(TRUE)) {
			if (pItemFind->m_bSave)
				oFindDB.UpdateFind(&pItemFind->m_oFindRec);
			else
				oFindDB.AddFind(&pItemFind->m_oFindRec);
		}
	}

	UpdateAllViews(NULL, VIEWHINT_REFINDITEM, pItemFind);
}

void CMMLibDoc::NameChange(CItem *pItem, LPCSTR pName)
{
	CItemFolder *pItemFolder;
	CItemData *pItemData;
	CItemFind *pItemFind;

	switch (pItem->m_nItemType) {
	case ITEM_FOLDER:
	case ITEM_INBOX:
		if (!OfflineCheck(pItem))
			return;

		pItemFolder = (CItemFolder *)pItem;
		pItemFolder->m_oFolderRec.sFolderName = pName;
		if (!pItemFolder->m_pPcsDB->UpdateFolderEx(&pItemFolder->m_oFolderRec))
			return;
		break;
	case ITEM_DATA:
		if (!OfflineCheck(pItem))
			return;

		pItemData = (CItemData *)pItem;
		pItemData->m_oDataRec.sTitle = pName;
		if (!pItemData->m_pPcsDB->UpdateDataEx(&pItemData->m_oDataRec))
			return;
		break;
	case ITEM_FIND:
		pItemFind = (CItemFind *)pItem;
		pItemFind->m_oFindRec.sFolderName = pName;
		if (pItemFind->m_bSave) {
			CFindDB oFindDB;
			if (oFindDB.Open(FALSE))
				oFindDB.UpdateFind(&pItemFind->m_oFindRec);
		}
		break;
	default:
		return;
	}

	UpdateAllViews(NULL, VIEWHINT_UPDATEITEM, pItem);
}

void CMMLibDoc::OnFileDelete()
{

	CItem *pItem;
	if ((pItem = GetSelectedItem()) == NULL)
		return;

	if (!OfflineCheck(pItem))
		return;

	LPCSTR text;
	switch (pItem->m_nItemType) {
	case ITEM_FOLDER:
		switch (((CItemFolder *)pItem)->m_nFolderType) {
		case FTYPE_LIBRARY:
			text = GetString(IDS_LIBRARY);
			break;
		default:
			text = GetString(IDS_FOLDER);
			break;
		}
		break;
	case ITEM_DATA:
		text = GetString(IDS_DATA);
		break;
	default:
		PROGRAM_ERROR;
		return;
	}

	CString sMsg;
	sMsg.Format(IDS_DELETE_SELECT, text);
	if (::AfxMessageBox(sMsg, MB_ICONQUESTION | MB_OKCANCEL) == IDCANCEL)
		return;

	CWaitCursor wait;

	CItem *pNextSelected;
	for ( ; pItem != NULL; pItem = pNextSelected) {
		pNextSelected = pItem->m_pNextSelected;

		switch (pItem->m_nItemType) {
		case ITEM_FOLDER:
			DeleteFolder((CItemFolder *)pItem);
			break;
		case ITEM_DATA:
			DeleteData((CItemData *)pItem);
			break;
		}
	}

	NotifyUpdate();
}

void CMMLibDoc::OnUpdateFileDelete(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CheckSelectedItemType(ITEM_FOLDER | ITEM_DATA));
}

void CMMLibDoc::DeleteFolder(CItemFolder *pItemFolder)
{
	if (!OfflineCheck(pItemFolder))
		return;

	if (pItemFolder->m_nFolderType == FTYPE_LIBRARY) {
		if (g_pMMLibDB->DeleteMMLibrary(pItemFolder->m_pPcsDB))
			UpdateAllViews(NULL, VIEWHINT_REMOVEITEM, pItemFolder);
		else
			::AfxMessageBox(IDS_DELETE_LIBRARY_FAIL);
	} else {
		if (pItemFolder->m_pPcsDB->DeleteFolderEx(pItemFolder->m_oFolderRec.sFolderID))
			UpdateAllViews(NULL, VIEWHINT_REMOVEITEM, pItemFolder);
	}
}

void CMMLibDoc::DeleteData(CItemData *pItemData)
{
	if (!OfflineCheck(pItemData))
		return;

	if (pItemData->m_pPcsDB->DeleteDataEx(pItemData->m_oDataRec.sDataID))
		UpdateAllViews(NULL, VIEWHINT_REMOVEITEM, pItemData);
}

void CMMLibDoc::CopyItem(CItem *pItemSrc, CItem *pItemDst)
{
	if (!OfflineCheck(pItemDst))
		return;

	if (pItemDst->m_nItemType != ITEM_FOLDER && pItemDst->m_nItemType != ITEM_INBOX)
		return;

	CWaitCursor wait;

	while (pItemSrc != NULL) {
		if (pItemSrc != pItemDst) {
			switch (pItemSrc->m_nItemType) {
			case ITEM_FOLDER:
			case ITEM_INBOX:
				CopyFolder((CItemFolder *)pItemSrc, (CItemFolder *)pItemDst);
				break;
			case ITEM_DATA:
				CopyData((CItemData *)pItemSrc, (CItemFolder *)pItemDst);
				break;
			}
		}

		pItemSrc = pItemSrc->m_pNextSelected;
	}
}

void CMMLibDoc::CopyFolder(CItemFolder *pItemSrc, CItemFolder *pItemDst)
{
	if (pItemDst->m_nFolderType == FTYPE_DFOLDER)
		return;

	FOLDERREC oFolderRec;
	if (CMMPcsDB::CopyFolderEx(pItemSrc->m_pPcsDB, pItemSrc->m_oFolderRec.sFolderID, pItemDst->m_pPcsDB, pItemDst->m_oFolderRec.sFolderID, &oFolderRec)) {
		CAddItemFolder oAddItem;
		oAddItem.m_pItemFolder = pItemDst;
		oAddItem.m_pFolderRec = &oFolderRec;
		UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItem);
	}
}

void CMMLibDoc::CopyData(CItemData *pItemSrc, CItemFolder *pItemDst)
{
	if (pItemDst->m_nFolderType != FTYPE_DFOLDER)
		return;

	DATAREC oDataRec;
	if (CMMPcsDB::CopyDataEx(pItemSrc->m_pPcsDB, pItemSrc->m_oDataRec.sDataID, pItemDst->m_pPcsDB, pItemDst->m_oFolderRec.sFolderID, &oDataRec)) {
		CAddItemData oAddItem;
		oAddItem.m_pItemFolder = pItemDst;
		oAddItem.m_pDataRec = &oDataRec;
		UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItem);
	}
}

void CMMLibDoc::MoveItem(CItem *pItemSrc, CItem *pItemDst)
{
	if (!OfflineCheck(pItemDst))
		return;

	if (pItemDst->m_nItemType != ITEM_FOLDER && pItemDst->m_nItemType != ITEM_INBOX)
		return;

	CWaitCursor wait;

	CItem *pNextSelected;
	while (pItemSrc != NULL) {
		pNextSelected = pItemSrc->m_pNextSelected;

		if (!CheckParent(pItemSrc, pItemDst)) {
			switch (pItemSrc->m_nItemType) {
			case ITEM_FOLDER:
			case ITEM_INBOX:
				MoveFolder((CItemFolder *)pItemSrc, (CItemFolder *)pItemDst);
				break;
			case ITEM_DATA:
				MoveData((CItemData *)pItemSrc, (CItemFolder *)pItemDst);
				break;
			}
		}

		pItemSrc = pNextSelected;
	}
}

void CMMLibDoc::MoveFolder(CItemFolder *pItemSrc, CItemFolder *pItemDst)
{
	if (pItemDst->m_nFolderType == FTYPE_DFOLDER)
		return;

	BOOL rc;

	FOLDERREC oFolderRec;
	if (pItemSrc->m_pPcsDB == pItemDst->m_pPcsDB)
		rc = pItemSrc->m_pPcsDB->MoveFolderEx(pItemSrc->m_oFolderRec.sFolderID, pItemDst->m_oFolderRec.sFolderID, &oFolderRec);
	else {
		rc = CMMPcsDB::CopyFolderEx(pItemSrc->m_pPcsDB, pItemSrc->m_oFolderRec.sFolderID, pItemDst->m_pPcsDB, pItemDst->m_oFolderRec.sFolderID, &oFolderRec);
		if (rc)
			rc = pItemSrc->m_pPcsDB->DeleteFolderEx(pItemSrc->m_oFolderRec.sFolderID);
	}

	if (rc) {
		UpdateAllViews(NULL, VIEWHINT_REMOVEITEM, pItemSrc);

		CAddItemFolder oAddItem;
		oAddItem.m_pItemFolder = pItemDst;
		oAddItem.m_pFolderRec = &oFolderRec;
		UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItem);
	}
}

void CMMLibDoc::MoveData(CItemData *pItemSrc, CItemFolder *pItemDst)
{
	if (pItemDst->m_nFolderType != FTYPE_DFOLDER)
		return;

	DATAREC oDataRec;
	BOOL rc;

	if (pItemSrc->m_pPcsDB == pItemDst->m_pPcsDB)
		rc = pItemSrc->m_pPcsDB->MoveDataEx(pItemSrc->m_oDataRec.sDataID, pItemDst->m_oFolderRec.sFolderID, &oDataRec);
	else {
		rc = CMMPcsDB::CopyDataEx(pItemSrc->m_pPcsDB, pItemSrc->m_oDataRec.sDataID, pItemDst->m_pPcsDB, pItemDst->m_oFolderRec.sFolderID, &oDataRec);
		if (rc)
			rc = pItemSrc->m_pPcsDB->DeleteDataEx(pItemSrc->m_oDataRec.sDataID);
	}

	if (rc) {
		UpdateAllViews(NULL, VIEWHINT_REMOVEITEM, pItemSrc);

		CAddItemData oAddItem;
		oAddItem.m_pItemFolder = pItemDst;
		oAddItem.m_pDataRec = &oDataRec;
		UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItem);
	}
}

BOOL CMMLibDoc::CheckParent(CItem *pItemSrc, CItem *pItemDst)
{
	HTREEITEM hItemSrc = pItemSrc->m_hItem;
	HTREEITEM hItemDst = pItemDst->m_hItem;

	for (;;) {
		if (hItemSrc == hItemDst)
			return TRUE;

		if ((hItemDst = g_pTreeCtrl->GetParentItem(hItemDst)) == NULL)
			return FALSE;
	}
}

CItem *CMMLibDoc::GetSelectedItem()
{
	CWinApp *pWinApp = ::AfxGetApp();

	CView *pView = ((CFrameWnd *)pWinApp->m_pMainWnd)->GetActiveView();

	return (CItem *)pView->SendMessage(WM_GETSELECTEDITEM);
}

void CMMLibDoc::OnPictOpen()
{
	CItem *pItem;

	if ((pItem = GetSelectedItem()) != NULL) {
		switch (pItem->m_nItemType) {
		case ITEM_DATA:
			UpdateAllViews(NULL, VIEWHINT_SELECTITEM, pItem);
			break;
		}
	}
}

void CMMLibDoc::OnViewReload()
{
	CWaitCursor wait;

	Reload();
}

void CMMLibDoc::OnViewPrev()
{
	CItem *pItem;

	if ((pItem = GetSelectedItem()) != NULL) {
		HTREEITEM hItem = g_pTreeCtrl->GetPrevSiblingItem(pItem->m_hItem);
		if (hItem != NULL)
			UpdateAllViews(NULL, VIEWHINT_SELECTITEM, (CItem *)g_pTreeCtrl->GetItemData(hItem));
	}
}

void CMMLibDoc::OnUpdateViewPrev(CCmdUI* pCmdUI)
{
	CItem *pItem;
	BOOL bEnable = FALSE;

	if ((pItem = GetSelectedItem()) != NULL) {
		HTREEITEM hItem = g_pTreeCtrl->GetPrevSiblingItem(pItem->m_hItem);
		if (hItem != NULL)
			bEnable = TRUE;
	}

	pCmdUI->Enable(bEnable);
}

void CMMLibDoc::OnViewNext()
{
	CItem *pItem;

	if ((pItem = GetSelectedItem()) != NULL) {
		HTREEITEM hItem = g_pTreeCtrl->GetNextSiblingItem(pItem->m_hItem);
		if (hItem != NULL)
			UpdateAllViews(NULL, VIEWHINT_SELECTITEM, (CItem *)g_pTreeCtrl->GetItemData(hItem));
	}
}

void CMMLibDoc::OnUpdateViewNext(CCmdUI* pCmdUI)
{
	CItem *pItem;
	BOOL bEnable = FALSE;

	if ((pItem = GetSelectedItem()) != NULL) {
		HTREEITEM hItem = g_pTreeCtrl->GetNextSiblingItem(pItem->m_hItem);
		if (hItem != NULL)
			bEnable = TRUE;
	}

	pCmdUI->Enable(bEnable);
}

void CMMLibDoc::OnPictAutosize()
{
	m_nAutoSize = (m_nAutoSize == AUTOSIZE_BOTH ? AUTOSIZE_NONE : AUTOSIZE_BOTH);
	UpdateAllViews(NULL, VIEWHINT_DRAWPICT);
}

void CMMLibDoc::OnUpdatePictAutosize(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bPictView);
	pCmdUI->SetCheck(m_nAutoSize == AUTOSIZE_BOTH);
}

void CMMLibDoc::OnPictAutosize2()
{
	m_nAutoSize = (m_nAutoSize == AUTOSIZE_WIDTH ? AUTOSIZE_NONE : AUTOSIZE_WIDTH);
	UpdateAllViews(NULL, VIEWHINT_DRAWPICT);
}

void CMMLibDoc::OnUpdatePictAutosize2(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bPictView);
	pCmdUI->SetCheck(m_nAutoSize == AUTOSIZE_WIDTH);
}

void CMMLibDoc::OnPictEnlarge()
{
	m_nAutoSize = AUTOSIZE_NONE;
	UpdateAllViews(NULL, VIEWHINT_ENLARGE);
}

void CMMLibDoc::OnUpdatePictEnlarge(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bPictView);
}

void CMMLibDoc::OnPictReduce()
{
	m_nAutoSize = AUTOSIZE_NONE;
	UpdateAllViews(NULL, VIEWHINT_REDUCE);
}

void CMMLibDoc::OnUpdatePictReduce(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bPictView);
}

void CMMLibDoc::OnPictRightRotate()
{
	UpdateAllViews(NULL, VIEWHINT_RROTATION);
}

void CMMLibDoc::OnUpdatePictRightRotate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bPictView);
}

void CMMLibDoc::OnPictLeftRotate()
{
	UpdateAllViews(NULL, VIEWHINT_LROTATION);
}

void CMMLibDoc::OnUpdatePictLeftRotate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bPictView);
}

int CMMLibDoc::GetSelectedItemType()
{
	CItem *pItem;

	if ((pItem = GetSelectedItem()) == NULL)
		return ITEM_NONE;

	return pItem->m_nItemType;
}

BOOL CMMLibDoc::CheckSelectedItemType(int nItemType)
{
	return (GetSelectedItemType() & nItemType) ? TRUE : FALSE;
}

void CMMLibDoc::OnDetailName()
{
	InverseDetailDisp(DETAIL_NAME);
	UpdateAllViews(NULL, VIEWHINT_DRAWLIST);
}

void CMMLibDoc::OnUpdateDetailName(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CheckDetailDisp(DETAIL_NAME));
}

void CMMLibDoc::OnDetailCode()
{
	InverseDetailDisp(DETAIL_CODE);
	UpdateAllViews(NULL, VIEWHINT_DRAWLIST);
}

void CMMLibDoc::OnUpdateDetailCode(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CheckDetailDisp(DETAIL_CODE));
}

void CMMLibDoc::OnDetailCdate()
{
	InverseDetailDisp(DETAIL_CDATE);
	UpdateAllViews(NULL, VIEWHINT_DRAWLIST);
}

void CMMLibDoc::OnUpdateDetailCdate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CheckDetailDisp(DETAIL_CDATE));
}

void CMMLibDoc::OnDetailUdate()
{
	InverseDetailDisp(DETAIL_UDATE);
	UpdateAllViews(NULL, VIEWHINT_DRAWLIST);
}

void CMMLibDoc::OnUpdateDetailUdate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CheckDetailDisp(DETAIL_UDATE));
}

void CMMLibDoc::OnDetailCreator()
{
	InverseDetailDisp(DETAIL_CREATOR);
	UpdateAllViews(NULL, VIEWHINT_DRAWLIST);
}

void CMMLibDoc::OnUpdateDetailCreator(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CheckDetailDisp(DETAIL_CREATOR));
}

void CMMLibDoc::OnDetailRemark()
{
	InverseDetailDisp(DETAIL_REMARK);
	UpdateAllViews(NULL, VIEWHINT_DRAWLIST);
}

void CMMLibDoc::OnUpdateDetailRemark(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CheckDetailDisp(DETAIL_REMARK));
}

void CMMLibDoc::OnDetailKeyword()
{
	InverseDetailDisp(DETAIL_KEYWORD);
	UpdateAllViews(NULL, VIEWHINT_DRAWLIST);
}

void CMMLibDoc::OnUpdateDetailKeyword(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CheckDetailDisp(DETAIL_KEYWORD));
}

void CMMLibDoc::OnDetailServer()
{
	InverseDetailDisp(DETAIL_SERVER);
	UpdateAllViews(NULL, VIEWHINT_DRAWLIST);
}

void CMMLibDoc::OnUpdateDetailServer(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CheckDetailDisp(DETAIL_SERVER));
}

void CMMLibDoc::OnDetailDirectory()
{
	InverseDetailDisp(DETAIL_DIRECTORY);
	UpdateAllViews(NULL, VIEWHINT_DRAWLIST);
}

void CMMLibDoc::OnUpdateDetailDirectory(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CheckDetailDisp(DETAIL_DIRECTORY));
}

void CMMLibDoc::OnDetailFirstdata()
{
	InverseDetailDisp(DETAIL_FIRSTDATA);
	UpdateAllViews(NULL, VIEWHINT_DRAWLIST);
}

void CMMLibDoc::OnUpdateDetailFirstdata(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CheckDetailDisp(DETAIL_FIRSTDATA));
}

void CMMLibDoc::InverseDetailDisp(int nDetailDisp)
{
	CItem *pItem;
	int nIndex;

	if ((pItem = GetSelectedItem()) == NULL)
		return;

	switch (pItem->m_nItemType) {
	case ITEM_ROOT:
		nIndex = DETAIL_LIBRARY;
		break;
	case ITEM_FOLDER:
	case ITEM_INBOX:
		if (((CItemFolder *)pItem)->m_nFolderType != FTYPE_DFOLDER)
			nIndex = DETAIL_FOLDER;
		else
			nIndex = DETAIL_DATA;
		break;
	case ITEM_FIND:
		if (((CItemFind *)pItem)->m_nFindType == FIND_TYPE_FOLDER)
			nIndex = DETAIL_FOLDER;
		else
			nIndex = DETAIL_DATA;
		break;
	default:
		return;
	}

	if (nDetailDisp == DETAIL_FIRSTDATA && nIndex != DETAIL_FOLDER && !m_bDetailFlag[nIndex][nDetailDisp])
		return;

	m_bDetailFlag[nIndex][nDetailDisp] = !m_bDetailFlag[nIndex][nDetailDisp];
}

BOOL CMMLibDoc::CheckDetailDisp(int nDetailDisp)
{
	CItem *pItem;
	int nIndex;

	if ((pItem = GetSelectedItem()) == NULL)
		return FALSE;

	switch (pItem->m_nItemType) {
	case ITEM_ROOT:
		nIndex = DETAIL_LIBRARY;
		break;
	case ITEM_FOLDER:
	case ITEM_INBOX:
		if (((CItemFolder *)pItem)->m_nFolderType != FTYPE_DFOLDER)
			nIndex = DETAIL_FOLDER;
		else
			nIndex = DETAIL_DATA;
		break;
	case ITEM_FIND:
		if (((CItemFind *)pItem)->m_nFindType == FIND_TYPE_FOLDER)
			nIndex = DETAIL_FOLDER;
		else
			nIndex = DETAIL_DATA;
		break;
	default:
		return FALSE;
	}

	return m_bDetailFlag[nIndex][nDetailDisp];
}

void CMMLibDoc::OnViewUpfolder()
{
	HTREEITEM hItem = g_pTreeCtrl->GetSelectedItem();
	if (hItem == NULL)
		return;

	CItem *pItem = (CItem *)g_pTreeCtrl->GetItemData(hItem);

	if ((pItem = (CItem *)pItem->GetParent()) == NULL)
		return;

	UpdateAllViews(NULL, VIEWHINT_SELECTITEM, pItem);
}

void CMMLibDoc::OnUpdateViewUpfolder(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!CheckSelectedItemType(ITEM_ROOT));
}

void CMMLibDoc::OnFind()
{
	NewFindSimple();
}

void CMMLibDoc::NewFindSimple()
{
	CFindSimpleDlg dlg;

	dlg.m_pDocument = this;

	CItem *pItem = GetSelectedItem();
	if (pItem != NULL) {
		if (pItem->m_nItemType == ITEM_ROOT)
			dlg.m_pItemParent = pItem;
		else {
			while (pItem != NULL) {
				if (pItem->m_nItemType == ITEM_FOLDER) {
					if (((CItemFolder *)pItem)->m_nFolderType == FTYPE_LIBRARY) {
						dlg.m_pItemParent = pItem;
						break;
					}
				} else if (pItem->m_nItemType != ITEM_DATA && pItem->m_nItemType != ITEM_FIND)
					break;

				pItem = pItem->GetParent();
			}
		}
	}

	switch (dlg.DoModal()) {
	case IDCANCEL:
		return;
	case IDC_FIND_EXPERT:
		NewFindExpert();
		return;
	}

	CAddItemFind oAddItem;

	FINDREC findRec;
	if (dlg.m_pItemParent->m_nItemType == ITEM_FOLDER)
		findRec.sLibraryID = ((CItemFolder *)dlg.m_pItemParent)->m_oFolderRec.sParentFolderID;
	findRec.sFindType = FIND_TYPE_SIMPLE;
	findRec.sFolderName = dlg.m_sFindFolder;
	findRec.conditions[0].sKeyword = dlg.m_sSearchText;

	if (dlg.m_bSave) {
		CFindDB oFindDB;
		if (oFindDB.Open(TRUE))
			oFindDB.AddFind(&findRec);
	}

	oAddItem.m_pItemParent = dlg.m_pItemParent;
	oAddItem.m_pFindRec = &findRec;
	oAddItem.m_bSelect = TRUE;
	oAddItem.m_bSave = dlg.m_bSave;
	UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItem);
}

void CMMLibDoc::NewFindExpert()
{
	int i;
	CFindDlg dlg;

	dlg.m_pDocument = this;

	CItem *pItem = GetSelectedItem();
	if (pItem != NULL) {
		if (pItem->m_nItemType == ITEM_ROOT)
			dlg.m_pItemParent = pItem;
		else {
			while (pItem != NULL) {
				if (pItem->m_nItemType == ITEM_FOLDER) {
					if (((CItemFolder *)pItem)->m_nFolderType == FTYPE_LIBRARY) {
						dlg.m_pItemParent = pItem;
						break;
					}
				} else if (pItem->m_nItemType != ITEM_DATA && pItem->m_nItemType != ITEM_FIND)
					break;

				pItem = pItem->GetParent();
			}
		}
	}

	for (i = 0; i < N_FIND_CONDITIONS; i++)
		dlg.m_aFindConditions[i].sMatching = "N";

	if (dlg.DoModal() == IDCANCEL)
		return;

	CAddItemFind oAddItem;

	FINDREC findRec;
	if (dlg.m_pItemParent->m_nItemType == ITEM_FOLDER)
		findRec.sLibraryID = ((CItemFolder *)dlg.m_pItemParent)->m_oFolderRec.sParentFolderID;
	findRec.sFindType = (char)dlg.m_nFindType;
	findRec.sFolderName = dlg.m_sFindFolder;
	for (i = 0; i < N_FIND_CONDITIONS; i++)
		findRec.conditions[i] = dlg.m_aFindConditions[i];

	if (dlg.m_bSave) {
		CFindDB oFindDB;
		if (oFindDB.Open(TRUE))
			oFindDB.AddFind(&findRec);
	}

	oAddItem.m_pItemParent = dlg.m_pItemParent;
	oAddItem.m_pFindRec = &findRec;
	oAddItem.m_bSelect = TRUE;
	oAddItem.m_bSave = dlg.m_bSave;
	UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItem);
}

void CMMLibDoc::OnRefind()
{
	CItem *pItem;

	for (pItem = GetSelectedItem(); pItem != NULL; pItem = pItem->m_pNextSelected) {
		if (pItem->m_nItemType == ITEM_FIND)
			UpdateAllViews(NULL, VIEWHINT_REFINDITEM, pItem);
	}
}

void CMMLibDoc::OnUpdateRefind(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CheckSelectedItemType(ITEM_FIND));
}

BOOL CMMLibDoc::OnOpenDocument(LPCSTR /*lpszPathName*/)
{
	OpenCurrentLibrary();

	OpenFind((CItem *)g_pTreeCtrl->GetItemData(g_pTreeCtrl->GetRootItem()));

	return FALSE;
}

void CMMLibDoc::OnCloseDocument()
{
	HTREEITEM hItem;
	CItem *pItemChild;
	CItemFolder *pItemFolder;
	CItemPending *pItemPending;
	CMMPcsDB *pPcsDB;
	CString sEntry;
	int n = 0;
	LPCSTR server, directory;
	LPCSTR libraryID;
	LPCSTR title;
	int chType;
	int cacheMode;

	for (hItem = g_pTreeCtrl->GetChildItem(g_pTreeCtrl->GetRootItem()); hItem != NULL; hItem = g_pTreeCtrl->GetNextSiblingItem(hItem)) {
		pItemChild = (CItem *)g_pTreeCtrl->GetItemData(hItem);

		switch (pItemChild->m_nItemType) {
		case ITEM_FOLDER:
			pItemFolder = (CItemFolder *)pItemChild;
			pPcsDB = pItemFolder->m_pPcsDB;

			server = pPcsDB->m_sServerName;
			chType = pPcsDB->m_nChType;
			directory = pPcsDB->m_sDirName;
			cacheMode = pPcsDB->m_nCacheMode;
			title = pPcsDB->m_sTitle;
			libraryID = pPcsDB->m_sLibraryID;
			break;
		case ITEM_PENDING:
			pItemPending = (CItemPending *)pItemChild;
			server = pItemPending->m_sServerName;
			chType = pItemPending->m_nChType;
			directory = pItemPending->m_sDirName;
			cacheMode = pItemPending->m_nCacheMode;
			title = pItemPending->m_sTitle;
			libraryID = pItemPending->m_sLibraryID;
			break;
		default:
			continue;
		}

		sEntry.Format("Server%d", n);
		theApp.WriteProfileString(g_sSectionLibrary, sEntry, server);

		sEntry.Format("ChType%d", n);
		theApp.WriteProfileInt(g_sSectionLibrary, sEntry, chType);

		sEntry.Format("Directory%d", n);
		theApp.WriteProfileString(g_sSectionLibrary, sEntry, directory);

		sEntry.Format("CacheMode%d", n);
		theApp.WriteProfileInt(g_sSectionLibrary, sEntry, (cacheMode == CACHEMODE_OFFLINE ? CACHEMODE_CACHE : cacheMode));

		sEntry.Format("Title%d", n);
		theApp.WriteProfileString(g_sSectionLibrary, sEntry, title);

		sEntry.Format("LibraryID%d", n);
		theApp.WriteProfileString(g_sSectionLibrary, sEntry, libraryID);

		n++;
	}

	theApp.WriteProfileInt(g_sSectionLibrary, "Num", n);

	CDocument::OnCloseDocument();
}

BOOL CMMLibDoc::OfflineCheck(CItem *pItem)
{
	switch (pItem->m_nItemType) {
	case ITEM_FOLDER:
		if (((CItemFolder *)pItem)->m_pPcsDB->m_nCacheMode == CACHEMODE_OFFLINE) {
			::AfxMessageBox(IDS_OFFLINE_FOLDER_UPDATE);
			return FALSE;
		}
		break;
	case ITEM_DATA:
		if (((CItemData *)pItem)->m_pPcsDB->m_nCacheMode == CACHEMODE_OFFLINE) {
			::AfxMessageBox(IDS_OFFLINE_DATA_UPDATE);
			return FALSE;
		}
		break;
	case ITEM_INBOX:
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

void CMMLibDoc::OnCacheRead()
{
	if (::AfxMessageBox(IDS_READ_CACHE, MB_ICONQUESTION | MB_OKCANCEL) == IDCANCEL)
		return;

	CWaitCursor wait;

	CItem *pItem, *pNextSelected;
	CItemFolder *pItemFolder;

	for (pItem = GetSelectedItem(); pItem != NULL; pItem = pNextSelected) {
		pNextSelected = pItem->m_pNextSelected;

		switch (pItem->m_nItemType) {
		case ITEM_FOLDER:
			pItemFolder = (CItemFolder *)pItem;
			pItemFolder->m_pPcsDB->FillCacheEx(pItemFolder->m_oFolderRec.sFolderID);
			break;
		}
	}
}

void CMMLibDoc::OnUpdateCacheRead(CCmdUI* pCmdUI)
{
	CItem *pItem;
	BOOL bEnable = FALSE;

	if ((pItem = GetSelectedItem()) != NULL) {
		if (pItem->m_nItemType == ITEM_FOLDER) {
			if (((CItemFolder *)pItem)->m_pPcsDB->m_nCacheMode == CACHEMODE_CACHE)
				bEnable = TRUE;
		}
	}
	pCmdUI->Enable(bEnable);
}

void CMMLibDoc::OnCacheClear()
{
	if (::AfxMessageBox(IDS_DELETE_CACHE, MB_ICONQUESTION | MB_OKCANCEL) == IDCANCEL)
		return;

	CWaitCursor wait;

	CItem *pItem, *pNextSelected;
	for (pItem = GetSelectedItem(); pItem != NULL; pItem = pNextSelected) {
		pNextSelected = pItem->m_pNextSelected;

		switch (pItem->m_nItemType) {
		case ITEM_FOLDER:
			((CItemFolder *)pItem)->m_pPcsDB->ClearCacheEx();
			break;
		}
	}
}

void CMMLibDoc::OnUpdateCacheClear(CCmdUI* pCmdUI)
{
	CItem *pItem;
	BOOL bEnable = FALSE;

	if ((pItem = GetSelectedItem()) != NULL) {
		if (pItem->m_nItemType == ITEM_FOLDER) {
			if (((CItemFolder *)pItem)->m_pPcsDB->m_nCacheMode == CACHEMODE_CACHE)
				bEnable = TRUE;
		}
	}
	pCmdUI->Enable(bEnable);
}

void CMMLibDoc::OnOption()
{
	COptProperty dlg(IDS_SET_ENVIRONMENT, ::AfxGetMainWnd(), 0);
	if (dlg.DoModal() == IDCANCEL)
		return;

	if (dlg.m_oOptFont.m_bChangeViewFont)
		UpdateAllViews(NULL, VIEWHINT_SETFONT);

	if (dlg.m_oOptOrder.m_bChangeOrder)
		UpdateAllViews(NULL, VIEWHINT_SORTITEM);

	if (dlg.m_oOptOrder.m_bChangeText)
		UpdateAllViews(NULL, VIEWHINT_SETTEXT);

	if (dlg.m_oOptOrder.m_bChangeSmallSize) {
		UpdateAllViews(NULL, VIEWHINT_RELOADITEM);
		UpdateAllViews(NULL, VIEWHINT_SETSMALLSIZE);
	}
}

void CMMLibDoc::OnFileSendMail()
{
	CMailAttDlg dlg;

	if (dlg.DoModal() == IDCANCEL)
		return;

	CString sFileName;
	if (!MakeTempFile(GetSelectedItem(), sFileName, PICTFILE_ALL, TRUE)) {
		::AfxMessageBox(IDS_SEND_DATA_FOLDER);
		return;
	}

	if (sFileName.IsEmpty()) {
		::AfxMessageBox(IDS_NO_IMAGE_DATA);
		return;
	}

	SendDocuments(sFileName);
}

void CMMLibDoc::OnUpdateFileSendMail(CCmdUI* pCmdUI)
{
	BOOL bEnable = FALSE;
	CItem *pItem;

	if ((pItem = GetSelectedItem()) != NULL) {
		switch (pItem->m_nItemType) {
		case ITEM_FOLDER:
			if (((CItemFolder *)pItem)->m_nFolderType == FTYPE_DFOLDER)
				bEnable = TRUE;
			break;
		case ITEM_FIND:
			if (((CItemFind *)pItem)->m_nFindType != FIND_TYPE_FOLDER)
				bEnable = TRUE;
			break;
		case ITEM_DATA:
			bEnable = TRUE;
			break;
		}
	}

	pCmdUI->Enable(bEnable);
}

void CMMLibDoc::OnEditSelectAll()
{
	UpdateAllViews(NULL, VIEWHINT_SELECTALL);
}

void CMMLibDoc::OnFileSave()
{
}

void CMMLibDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CMMLibDoc::OnSelchangeCombo()
{
	CComboBox &comboBox = ((CMainFrame *)::AfxGetMainWnd())->GetComboBox();

	const int n = comboBox.GetCurSel();
	if (n != LB_ERR) {
		CString sStr;

		comboBox.GetLBText(n, sStr);
		comboBox.SetWindowText(sStr);
		m_nAutoSize = AUTOSIZE_NONE;
		UpdateAllViews(NULL, VIEWHINT_SETPICTRATIO);
	}
}

void CMMLibDoc::OnEditchangeCombo()
{
	CComboBox &comboBox = ((CMainFrame *)::AfxGetMainWnd())->GetComboBox();
	CString sStr1, sStr2;

	comboBox.GetWindowText(sStr1);
	sStr2 = sStr1.SpanIncluding("0123456789%");
	if (sStr1.GetLength() != sStr2.GetLength()) {
		comboBox.SetWindowText(sStr2);
		comboBox.SetEditSel(sStr2.GetLength(), sStr2.GetLength());
	}
}

void CMMLibDoc::OnUpdateCombo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bPictView);
	if (!m_bPictView) {
		CComboBox &comboBox = ((CMainFrame *)::AfxGetMainWnd())->GetComboBox();
		comboBox.SetEditSel(-1, -1);
	}
}

BOOL CMMLibDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nID == 1 && nCode == 0) {
		m_nAutoSize = AUTOSIZE_NONE;
		UpdateAllViews(NULL, VIEWHINT_SETPICTRATIO);
	}

	// pHandlerInfo が NULL の場合、メッセージを処理します。
	if (pHandlerInfo == NULL)
	{
		CMainFrame *pMainWnd = (CMainFrame *)::AfxGetMainWnd();
		int i;

		// フィルター コマンドはテキスト カラー メニュー オプションへ送られます。
		if (pMainWnd != NULL) {
			for (i = 0; i < pMainWnd->m_nTransMenu; i++) {
				if (nID == (UINT)(IDM_TRANS_APP + i)) {
					if (nCode == CN_COMMAND) {
						// WM_COMMAND メッセージを処理します。
						OnTransApp(i);
					} else if (nCode == CN_UPDATE_COMMAND_UI) {
						// UI 要素の状態を更新します。
						((CCmdUI*)pExtra)->Enable(TRUE);
					}
					return TRUE;
				}
			}
		}
	}

	return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMMLibDoc::OnFilePack()
{
	CItem *pItem;

	if ((pItem = GetSelectedItem()) == NULL)
		return;

	if (pItem->m_nItemType == ITEM_FOLDER || pItem->m_nItemType == ITEM_INBOX) {
		if (::AfxMessageBox(IDS_FILE_PACK, MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL)
			return;

		CWaitCursor wait;

		if (((CItemFolder *)pItem)->m_pPcsDB->PackFileEx()) {
			wait.Restore();
			::AfxMessageBox(IDS_FILE_PACK_END);
		}

		NotifyUpdate();
	}
}

void CMMLibDoc::OnUpdateFilePack(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CheckSelectedItemType(ITEM_FOLDER | ITEM_INBOX));
}

void CMMLibDoc::OnFileExport()
{
	CWaitCursor wait;

	CItem *pItem = GetSelectedItem();
	if (pItem == NULL) {
		::AfxMessageBox(IDS_SELECT_EXPORT);
		return;
	}

	for ( ; pItem != NULL; pItem = pItem->m_pNextSelected) {
		switch (pItem->m_nItemType) {
		case ITEM_FOLDER:
			ExportMmlFile(pItem);
			break;
		case ITEM_FIND:
			if (((CItemFind *)pItem)->m_nFindType == FIND_TYPE_FOLDER)
				goto error;
			ExportMmlFile(pItem);
			break;
		case ITEM_DATA:
			ExportData((CItemData *)pItem);
			break;
		default:
		error:
			::AfxMessageBox(IDS_EXPORT_DATA_FOLDER);
			return;
		}
	}
}

BOOL CMMLibDoc::ExportData(CItemData *pItemData)
{
	HDIB hDIB;

	if (!pItemData->m_pPcsDB->ReadPictureDataEx(pItemData->m_oDataRec.sDataID, &hDIB, NULL))
		return FALSE;

	CString sFileName = pItemData->m_oDataRec.sTitle;
	CheckFileName(sFileName);

	CString sExt, sExt2;
	CString sFilter;

	switch (GetPictureBitCount(hDIB)) {
	case 0:
		return FALSE;
	case 1:
	case 8:
		sExt = "png";
		break;
	case 24:
		sExt = "jpg";
		break;
	default:
		sExt = "bmp";
		break;
	}

	sExt2 = "." + sExt;
	if (sExt2.CompareNoCase(sFileName.Right(4)) != 0)
		sFileName += sExt2;
	sFilter = GetString(IDS_PICTURE_FILE) + " (*.bmp;*.jpg;*.png)|*.bmp;*.jpg;*.png||";

	CFileDialog dlg(FALSE, sExt, sFileName, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, sFilter, ::AfxGetMainWnd());

	dlg.m_ofn.lpstrTitle = GetString(IDS_EXPORT_FILE);
	if (dlg.DoModal() == IDOK) {
		if (MMPictureWriteFile(hDIB, (LPSTR)(LPCSTR)dlg.GetPathName()) != 0) {
			::GlobalFree(hDIB);
			return FALSE;
		}
	}

	::GlobalFree(hDIB);

	return TRUE;
}

BOOL CMMLibDoc::ExportMmlFile(CItem *pItem)
{
	CString sFileName;
	CString sFilter;
	CString sExt;
	BOOL bMml = FALSE;
	BOOL bMmdb = FALSE;

	switch (pItem->m_nItemType) {
	case ITEM_FOLDER:
		sFileName = ((CItemFolder *)pItem)->m_oFolderRec.sFolderName;
		bMmdb = TRUE;
		if (((CItemFolder *)pItem)->m_nFolderType == FTYPE_DFOLDER)
			bMml = TRUE;
		break;
	case ITEM_FIND:
		sFileName = ((CItemFind *)pItem)->m_oFindRec.sFolderName;
		bMml = TRUE;
		break;
	default:
		return FALSE;
	}

	CheckFileName(sFileName);
//	sFileName += ".mml";
	if (bMml) {
		sFilter += GetString(IDS_MML_FILE) + " (*.mml)|*.mml|";
		sExt += "mml|";
	}
	if (bMmdb) {
		sFilter += GetString(IDS_MMDB_FILE) + " (*.mmdb)|*.mmdb|";
		sExt += "mmdb|";
	}
	sFilter += "|";
	sExt += "|";

	CFileDialog dlg(FALSE, sExt, sFileName, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, sFilter, ::AfxGetMainWnd());

	dlg.m_ofn.lpstrTitle = GetString(IDS_EXPORT_FILE);
	if (dlg.DoModal() == IDOK) {
		CWaitCursor wait;

		sExt = ::PathFindExtension(dlg.GetFileName());

		if (sExt.CompareNoCase(".mml") == 0) {
			if (!MakeMmlFile(pItem, dlg.GetPathName(), FALSE))
				return FALSE;
		} else if (sExt.CompareNoCase(".mmdb") == 0) {
			CDbArchive oDbArchive;
			oDbArchive.Export(dlg.GetPathName(), ((CItemFolder *)pItem)->m_pPcsDB, ((CItemFolder *)pItem)->m_oFolderRec);
		}
	}

	return TRUE;
}

void CMMLibDoc::OnUpdateFileExport(CCmdUI* pCmdUI)
{
	BOOL bEnable = FALSE;
	CItem *pItem;

	if ((pItem = GetSelectedItem()) != NULL) {
		switch (pItem->m_nItemType) {
		case ITEM_FOLDER:
			bEnable = TRUE;
			break;
		case ITEM_FIND:
			if (((CItemFind *)pItem)->m_nFindType != FIND_TYPE_FOLDER)
				bEnable = TRUE;
			break;
		case ITEM_DATA:
			bEnable = TRUE;
			break;
		}
	}

	pCmdUI->Enable(bEnable);
}

void CMMLibDoc::OnImportBmp()
{
	ImportFile(GetString(IDS_PICTURE_FILE) + " (*.bmp;*.jpg;*.png)|*.bmp;*.jpg;*.png|" + GetString(IDS_MML_FILE, 1) + " (*.mml)|*.mml||");

	NotifyUpdate();
}

void CMMLibDoc::OnUpdateImportBmp(CCmdUI* pCmdUI)
{
	BOOL bEnable = FALSE;
	CItem *pItem;

	if ((pItem = GetSelectedItem()) != NULL) {
		switch (pItem->m_nItemType) {
		case ITEM_FOLDER:
		case ITEM_DATA:
			bEnable = TRUE;
			break;
		}
	}

	pCmdUI->Enable(bEnable);
}

void CMMLibDoc::OnImportMml()
{
	ImportFile(GetString(IDS_MML_FILE) + " (*.mml)|*.mml|" + GetString(IDS_PICTURE_FILE, 1) + " (*.bmp;*.jpg;*.png)|*.bmp;*.jpg;*.png||");

	NotifyUpdate();
}

void CMMLibDoc::OnUpdateImportMml(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CheckSelectedItemType(ITEM_FOLDER | ITEM_INBOX | ITEM_DATA));
}

void CMMLibDoc::OnImportPpm()
{
	CItem *pItem;

	pItem = GetSelectedItem();

	if (pItem == NULL || (pItem->m_nItemType != ITEM_FOLDER && pItem->m_nItemType != ITEM_INBOX)) {
		::AfxMessageBox(IDS_SELECT_IMPORT_FOLDER);
		return;
	}

	if (!OfflineCheck(pItem))
		return;

	CFileDialog dlg(TRUE, NULL, "docu*", OFN_HIDEREADONLY | OFN_NOVALIDATE, GetString(IDS_PRESTO_PAGEMANAGER_FOLDER) + "|||", ::AfxGetMainWnd());

	dlg.m_ofn.lpstrTitle = GetString(IDS_IMPORT_FOLDER);
	if (dlg.DoModal() == IDOK) {
		ImportPpmFile(dlg.GetPathName(), (CItemFolder *)pItem);

		NotifyUpdate();
	}
}

void CMMLibDoc::OnUpdateImportPpm(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CheckSelectedItemType(ITEM_FOLDER | ITEM_INBOX | ITEM_DATA));
}

void CMMLibDoc::ImportFile(LPCSTR pFilter)
{
	CItem *pItem = GetSelectedItem();

	if (pItem == NULL) {
		::AfxMessageBox(IDS_SELECT_IMPORT_FOLDER);
		return;
	}

	if (pItem->m_nItemType == ITEM_DATA)
		pItem = pItem->GetParent();

	if (pItem->m_nItemType != ITEM_FOLDER && pItem->m_nItemType != ITEM_INBOX)
		return;

	if (!OfflineCheck(pItem))
		return;

	CItemFolder *pItemFolderOrg = (CItemFolder *)pItem;
	CItemFolder *pItemFolder = pItemFolderOrg;

	BOOL bCreateDataFolder = FALSE;
	if (pItemFolder->m_nFolderType == FTYPE_GFOLDER || pItemFolder->m_nFolderType == FTYPE_LIBRARY) {
		bCreateDataFolder = TRUE;
	}

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, pFilter, ::AfxGetMainWnd());

	dlg.m_ofn.lpstrTitle = GetString(IDS_IMPORT_FILE);
	char szFileNameBuffer[10000]{};
	dlg.m_ofn.lpstrFile = szFileNameBuffer;
	dlg.m_ofn.nMaxFile = 10000;
	if (dlg.DoModal() == IDOK) {
		POSITION pos = dlg.GetStartPosition();
		while(pos) {
			CString sPathName = dlg.GetNextPathName(pos);
			CString sFileName = ::RemoveDir(sPathName);
 			int nFileType = ::CheckFileType(sFileName);

			if (bCreateDataFolder && nFileType != SRCFILETYPE_MMDB) {
				pItemFolder = pItemFolderOrg;

				FOLDERREC oFolderRec;
				oFolderRec.sParentFolderID = pItemFolder->m_oFolderRec.sFolderID;
				oFolderRec.sFolderType = FTYPE_DFOLDER;
				oFolderRec.sFolderName = ::RemoveExt(::RemoveDir(sFileName));
				DWORD userNameSize = MAXUSERNAME;
				GetUserName(oFolderRec.sCreator.GetBuffer(MAXUSERNAME), &userNameSize);
				oFolderRec.sCreator.ReleaseBuffer();
				oFolderRec.sRemark.Empty();
				oFolderRec.sKeyword.Empty();

				if (pItemFolder->m_pPcsDB->AddFolderEx(&oFolderRec)) {
					CAddItemFolder oAddItem;
					oAddItem.m_pItemFolder = pItemFolder;
					oAddItem.m_pFolderRec = &oFolderRec;
					UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItem);
					pItemFolder = oAddItem.m_pItemAddFolder;
					UpdateAllViews(NULL, VIEWHINT_SELECTITEM, pItemFolder);
				}
			}

			switch (nFileType) {
			case SRCFILETYPE_MML:
				ImportMmlFile(sPathName, pItemFolder);
				break;
			case SRCFILETYPE_MMDB:
				ImportMmdbFile(sPathName, pItemFolder);
				break;
			case SRCFILETYPE_PICT:
				ImportPictFile(&sPathName, 1, pItemFolder, FALSE);
				bCreateDataFolder = FALSE;
				break;
			case SRCFILETYPE_PDF:
				ImportPdfFile(sPathName, pItemFolder);
				break;
			}
		}
	}
}

void CMMLibDoc::ImportPictFile(const CString *pFileName, int nFileName, CItemFolder *pItemFolder, BOOL bDataDlg)
{
	if (!OfflineCheck(pItemFolder))
		return;

	if (bDataDlg) {
		CDataDlg dlg;

		((CFrameWnd *)::AfxGetMainWnd())->ActivateFrame();

		dlg.m_pItemFolder = pItemFolder;
		dlg.m_pDocument = this;
		dlg.m_sCaption.LoadString(IDS_NEW_DATA2);
		dlg.m_pReadFileName = pFileName;
		dlg.m_nReadFileName = nFileName;

		dlg.DoModal();
	} else {
		HDIB hDIB, hDIB2;
		hDIB = hDIB2 = NULL;
		if (MMPictureReadFile(&hDIB, (LPSTR)(LPCSTR)*pFileName) != 0)
			return;
		hDIB2 = MakeSmallPicture(hDIB, PICT_WIDTH, PICT_HEIGHT);

		CString name = ::RemoveExt(::RemoveDir(*pFileName));

		CString creator;
		DWORD userNameSize = MAXUSERNAME;
		GetUserName(creator.GetBuffer(MAXUSERNAME), &userNameSize);
		creator.ReleaseBuffer();

		CString sFolderID = pItemFolder->GetID();
		DATAREC oDataRec;
		oDataRec.sTitle = name;
//		oDataRec.sCreateDate = date;
//		oDataRec.sUpdateDate = date;
		oDataRec.sCreator = creator;
		oDataRec.sFolderID = sFolderID;
		if (pItemFolder->m_pPcsDB->AddDataEx(&oDataRec))
			pItemFolder->m_pPcsDB->WritePictureDataEx(oDataRec.sDataID, hDIB, hDIB2, GetMMType(hDIB));

		if (hDIB != NULL)
			::GlobalFree(hDIB);
		if (hDIB2 != NULL)
			::GlobalFree(hDIB2);

		CAddItemData oAddItemData;
		oAddItemData.m_pItemFolder = pItemFolder;
		oAddItemData.m_pDataRec = &oDataRec;
		UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItemData);
		UpdateAllViews(NULL, VIEWHINT_UPDATEWINDOW);
	}
}

void CMMLibDoc::ImportMmlFile(LPCSTR pFileName, CItemFolder *pItemFolder)
{
	if (!OfflineCheck(pItemFolder))
		return;

	CMmlRead mml;
	if (!mml.Open(pFileName))
		return;

	FOLDERREC oFolderRec;
	DATAREC oDataRec;
	CString sText;
//	CAddItemFolder oAddItemFolder;
	CAddItemData oAddItemData;
	CWaitCursor wait;

/*
	switch (pItemFolder->m_nFolderType) {
	case FTYPE_LIBRARY:
	case FTYPE_GFOLDER:
		if (!ReadMmlHeader(&mml, &oFolderRec, &sText))
			return;

		oFolderRec.sParentFolderID = pItemFolder->m_oFolderRec.sFolderID;
		oFolderRec.sFolderType = FTYPE_DFOLDER;

		if (!pItemFolder->m_pPcsDB->AddFolderEx(&oFolderRec))
			return;

		if (sText.GetLength() != 0)
			pItemFolder->m_pPcsDB->WriteTextFolderEx(oFolderRec.sFolderID, &sText);

		UpdateAllViews(NULL, VIEWHINT_SELECTITEM, pItemFolder);

		oAddItemFolder.m_pItemFolder = pItemFolder;
		oAddItemFolder.m_pFolderRec = &oFolderRec;
		UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItemFolder);
		pItemFolder = oAddItemFolder.m_pItemAddFolder;
		if (pItemFolder == NULL)
			return;
		break;
	case FTYPE_DFOLDER:
		break;
	}
*/
	int nPage = 0;
	HDIB hDIB = NULL, hDIB2 = NULL;
	while (ReadMmlData(++nPage, &mml, &oDataRec, &hDIB, &sText)) {
		oDataRec.sFolderID = pItemFolder->m_oFolderRec.sFolderID;
		if (!pItemFolder->m_pPcsDB->AddDataEx(&oDataRec))
			break;

		if (hDIB != NULL) {
			hDIB2 = MakeSmallPicture(hDIB, PICT_WIDTH, PICT_HEIGHT);
			pItemFolder->m_pPcsDB->WritePictureDataEx(oDataRec.sDataID, hDIB, hDIB2, GetMMType(hDIB));
		}

		oAddItemData.m_pItemFolder = pItemFolder;
		oAddItemData.m_pDataRec = &oDataRec;
		UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItemData);
		UpdateAllViews(NULL, VIEWHINT_UPDATEWINDOW);

		if (hDIB != NULL) {
			::GlobalFree(hDIB);
			hDIB = NULL;
		}
		if (hDIB2 != NULL) {
			::GlobalFree(hDIB2);
			hDIB2 = NULL;
		}

		if (sText.GetLength() != 0)
			pItemFolder->m_pPcsDB->WriteTextDataEx(oDataRec.sDataID, sText);
	}
}

void CMMLibDoc::ImportMmdbFile(LPCSTR pFileName, CItemFolder *pItemFolder)
{
	CWaitCursor wait;

	CDbArchive oDbArchive;
	oDbArchive.Import(pFileName, pItemFolder->m_pPcsDB, pItemFolder->m_oFolderRec);

	UpdateAllViews(NULL, VIEWHINT_RELOADITEM);
}

void CMMLibDoc::ImportPpmFile(LPCSTR pFileName, CItemFolder *pItemFolder)
{
	if (!OfflineCheck(pItemFolder))
		return;

	CString creator;
	DWORD userNameSize = MAXUSERNAME;
	GetUserName(creator.GetBuffer(MAXUSERNAME), &userNameSize);
	creator.ReleaseBuffer();

	int n;
	CString dir = pFileName;
	if ((n = dir.ReverseFind('\\')) != -1)
		dir = dir.Left(n);

	WIN32_FIND_DATA	oFindBuf;
	HANDLE hdir1 = FindFirstFile(pFileName, &oFindBuf);
	if (hdir1 != INVALID_HANDLE_VALUE) {
		HDIB hDIB;
		HDIB hDIB2;
		DATAREC oDataRec;
		CAddItemData oAddItemData;
		CWaitCursor wait;
		HANDLE	hdir2;
		CString path;
		CString name, date;
		CString sFolderID;

		do {
			if (oFindBuf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (strcmp(oFindBuf.cFileName, ".") != 0 && strcmp(oFindBuf.cFileName, "..") != 0) {
					path = dir + "\\" + oFindBuf.cFileName;
					name = oFindBuf.cFileName;
					date = ConvFileTime(&oFindBuf.ftLastWriteTime);

					if (pItemFolder->m_nFolderType == FTYPE_DFOLDER)
						sFolderID = pItemFolder->GetID();
					else
						sFolderID.Empty();
					hdir2 = FindFirstFile(path + "\\*", &oFindBuf);
					if (hdir2 != INVALID_HANDLE_VALUE) {
						do {
							if (!(oFindBuf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
								hDIB = hDIB2 = NULL;
								if (MMPictureReadFile(&hDIB, (LPSTR)(LPCSTR)(path + "\\" + oFindBuf.cFileName)) != 0)
									continue;
								if (hDIB != NULL)
									hDIB2 = MakeSmallPicture(hDIB, PICT_WIDTH, PICT_HEIGHT);
								else
									continue;

								if (sFolderID.IsEmpty()) {
									sFolderID = CreateDFolder(pItemFolder, name, creator, date);
									if (sFolderID.IsEmpty()) {
										if (hDIB != NULL)
											::GlobalFree(hDIB);
										if (hDIB2 != NULL)
											::GlobalFree(hDIB2);
										break;
									}
								}

								if (pItemFolder->m_nFolderType == FTYPE_DFOLDER)
									oDataRec.sCode = name;
								oDataRec.sCreateDate = ConvFileTime(&oFindBuf.ftLastWriteTime);
								oDataRec.sUpdateDate = ConvFileTime(&oFindBuf.ftLastWriteTime);
								oDataRec.sCreator = creator;
								oDataRec.sTitle = RemoveExt((CString)oFindBuf.cFileName);
								oDataRec.sFolderID = sFolderID;
								if (pItemFolder->m_pPcsDB->AddDataEx(&oDataRec))
									pItemFolder->m_pPcsDB->WritePictureDataEx(oDataRec.sDataID, hDIB, hDIB2, GetMMType(hDIB));

								if (hDIB != NULL)
									::GlobalFree(hDIB);
								if (hDIB2 != NULL)
									::GlobalFree(hDIB2);

								if (pItemFolder->m_nFolderType == FTYPE_DFOLDER) {
									oAddItemData.m_pItemFolder = pItemFolder;
									oAddItemData.m_pDataRec = &oDataRec;
									UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItemData);
								}
							}
						} while (FindNextFile(hdir2, &oFindBuf));
						FindClose(hdir2);
					}
				}
			} else {
				hDIB = hDIB2 = NULL;
				if (MMPictureReadFile(&hDIB, (LPSTR)(LPCSTR)(dir + "\\" + oFindBuf.cFileName)) != 0)
					continue;
				if (hDIB != NULL)
					hDIB2 = MakeSmallPicture(hDIB, PICT_WIDTH, PICT_HEIGHT);
				else
					continue;

				name = RemoveExt((CString)oFindBuf.cFileName);
				date = ConvFileTime(&oFindBuf.ftLastWriteTime);

				if (pItemFolder->m_nFolderType == FTYPE_DFOLDER) {
					sFolderID = pItemFolder->GetID();
					oDataRec.sCode = name;
				} else {
					sFolderID = CreateDFolder(pItemFolder, name, creator, date);
					if (sFolderID.IsEmpty()) {
						if (hDIB != NULL)
							::GlobalFree(hDIB);
						if (hDIB2 != NULL)
							::GlobalFree(hDIB2);
						continue;
					}
				}

				oDataRec.sCreateDate = date;
				oDataRec.sUpdateDate = date;
				oDataRec.sCreator = creator;
				oDataRec.sTitle.LoadString(IDS_NO_TITLE_1);
				oDataRec.sFolderID = sFolderID;
				if (pItemFolder->m_pPcsDB->AddDataEx(&oDataRec))
					pItemFolder->m_pPcsDB->WritePictureDataEx(oDataRec.sDataID, hDIB, hDIB2, GetMMType(hDIB));

				if (hDIB != NULL)
					::GlobalFree(hDIB);
				if (hDIB2 != NULL)
					::GlobalFree(hDIB2);

				if (pItemFolder->m_nFolderType == FTYPE_DFOLDER) {
					oAddItemData.m_pItemFolder = pItemFolder;
					oAddItemData.m_pDataRec = &oDataRec;
					UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItemData);
				}
			}
		} while (FindNextFile(hdir1, &oFindBuf));

		FindClose(hdir1);
	}
}

CString CMMLibDoc::CreateDFolder(CItemFolder *pItemFolder, LPCSTR pName, LPCSTR pCreator, LPCSTR pDate)
{
	CString sFolderID;

	FOLDERREC oFolderRec{
		.sParentFolderID = pItemFolder->m_oFolderRec.sFolderID,
		.sFolderType = FTYPE_DFOLDER,
		.sFolderName = pName,
		.sCreateDate = pDate,
		.sUpdateDate = pDate,
		.sCreator = pCreator
	};
	if (pItemFolder->m_pPcsDB->AddFolderEx(&oFolderRec)) {
		CAddItemFolder oAddItemFolder;
		oAddItemFolder.m_pItemFolder = pItemFolder;
		oAddItemFolder.m_pFolderRec = &oFolderRec;
		UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItemFolder);
		UpdateAllViews(NULL, VIEWHINT_UPDATEWINDOW);
		sFolderID = oAddItemFolder.m_pItemAddFolder->m_oFolderRec.sFolderID;
	}

	return sFolderID;
}

BOOL CMMLibDoc::ReadMmlHeader(CMmlRead *pMmlRead, FOLDERREC *pFolderRec, CString *pText)
{
	char aItemName[11];
	LPVOID pItemData;
	long nItemLen;

	pText->Empty();

	if (!pMmlRead->SetPage(0))
		return FALSE;

	while (pMmlRead->ReadItem(aItemName, &pItemData, &nItemLen)) {
		if (strcmp(aItemName, "FNAME") == 0)
			pFolderRec->sFolderName = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "CDATE") == 0)
			pFolderRec->sCreateDate = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "UDATE") == 0)
			pFolderRec->sUpdateDate = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "CREATOR") == 0)
			pFolderRec->sCreator = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "REMARK1") == 0)
			pFolderRec->sRemark = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "REMARK2") == 0)
			pFolderRec->sKeyword = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "TEXT") == 0) {
			if (nItemLen != 0)
				*pText = CString((LPCSTR)pItemData, nItemLen);
		}

		delete pItemData;
	}

	return TRUE;
}

BOOL CMMLibDoc::ReadMmlData(int nPage, CMmlRead *pMmlRead, DATAREC *pDataRec, HDIB *phDIB, CString *pText)
{
	char aItemName[11];
	LPVOID pItemData;
	long nItemLen;

	*phDIB = NULL;
	pText->Empty();

	if (!pMmlRead->SetPage(nPage))
		return FALSE;

	pDataRec->nBit = 0;
	pDataRec->nDpi = 0;
	pDataRec->nWidth = 0;
	pDataRec->nHeight = 0;

	while (pMmlRead->ReadItem(aItemName, &pItemData, &nItemLen)) {
		if (strcmp(aItemName, "CDATE") == 0)
			pDataRec->sCreateDate = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "UDATE") == 0)
			pDataRec->sUpdateDate = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "CREATOR") == 0)
			pDataRec->sCreator = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "CODE") == 0)
			pDataRec->sCode = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "TITLE") == 0)
			pDataRec->sTitle = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "REMARK1") == 0)
			pDataRec->sRemark = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "REMARK2") == 0)
			pDataRec->sKeyword = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "PICT1") == 0) {
			if (nItemLen != 0) {
				MMPictureReadMem(phDIB, pItemData, nItemLen);
				if (*phDIB != NULL) {
					LPBITMAPINFOHEADER pbi = (LPBITMAPINFOHEADER)::GlobalLock(*phDIB);
					pDataRec->nBit = pbi->biBitCount;
					pDataRec->nWidth = pbi->biWidth;
					pDataRec->nHeight = pbi->biHeight;
					::GlobalUnlock(*phDIB);
				}
			}
		} else if (strcmp(aItemName, "TEXT") == 0) {
			if (nItemLen != 0)
				*pText = CString((LPCSTR)pItemData, nItemLen);
		}
		delete pItemData;
	}

	return TRUE;
}

void CMMLibDoc::OnBackup()
{
	CBackupDlg dlg;

	CItem *pItem = GetSelectedItem();
	while (pItem != NULL) {
		if (pItem->m_nItemType == ITEM_FOLDER) {
			if (((CItemFolder *)pItem)->m_nFolderType == FTYPE_LIBRARY) {
				dlg.m_pItemFolder = (CItemFolder *)pItem;
				break;
			}
		} else if (pItem->m_nItemType != ITEM_DATA && pItem->m_nItemType != ITEM_FIND)
			break;

		pItem = pItem->GetParent();
	}

	dlg.DoModal();
}

void CMMLibDoc::OnRestore()
{
	CRestoreDlg dlg;

	dlg.DoModal();

	NotifyUpdate();
}

void CMMLibDoc::OnTransReg()
{
	CTransRegDlg dlg;

	if (dlg.DoModal() == IDOK) {
		CMainFrame *pMainWnd = (CMainFrame *)::AfxGetMainWnd();
		pMainWnd->RefreshTransMenu();
	}
}

void CMMLibDoc::OnTransApp(int nIndex)
{
	CItem *pItem = GetSelectedItem();
	if (pItem == NULL) {
		::AfxMessageBox(IDS_SELECT_TRANSFER_DATA);
		return;
	}

	for ( ; pItem != NULL; pItem = pItem->m_pNextSelected) {
		switch (pItem->m_nItemType) {
		case ITEM_DATA:
			break;
		default:
			::AfxMessageBox(IDS_STANSFER_DATA);
			return;
		}
	}

	CWaitCursor wait;

	CMainFrame *pMainWnd = (CMainFrame *)::AfxGetMainWnd();
	CString sFileName;
	if (!MakeTempFile(GetSelectedItem(), sFileName, pMainWnd->m_pTransMenu[nIndex].nSupportFile, FALSE, FALSE))
		return;

	if (sFileName.IsEmpty()) {
		::AfxMessageBox(IDS_NO_IMAGE_DATA);
		return;
	}

	m_sExecFile = pMainWnd->m_pTransMenu[nIndex].sFileName;
	m_sTempFile = sFileName;

	::AfxBeginThread(ExecApp, this, THREAD_PRIORITY_NORMAL);
}

UINT CMMLibDoc::ExecApp(LPVOID pParam)
{
	CMMLibDoc *pDoc = (CMMLibDoc *)pParam;
	CString sTempFile;
	CString execCmd;

	sTempFile = pDoc->m_sTempFile;
	sTempFile.Replace(';', ' ');
	sTempFile.TrimRight();
	execCmd.Format("\"%s\" %s", (LPCSTR)pDoc->m_sExecFile, (LPCSTR)sTempFile);

	ExecProcess(execCmd);

	DeleteTempFile(pDoc->m_sTempFile);

	return 0;
}

void CMMLibDoc::OnPictAllRotate()
{
	CItem *pItem, *pNextSelected;

	if ((pItem = GetSelectedItem()) == NULL)
		return;

	if (!OfflineCheck(pItem))
		return;

	CPictRotateDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;

	CWaitCursor wait;

	for ( ; pItem != NULL; pItem = pNextSelected) {
		pNextSelected = pItem->m_pNextSelected;

		switch (pItem->m_nItemType) {
		case ITEM_FOLDER:
		case ITEM_INBOX:
			((CItemFolder *)pItem)->m_pPcsDB->RotateFolderEx(pItem->GetID(), dlg.m_nRotate);
			break;
		case ITEM_DATA:
			((CItemData *)pItem)->m_pPcsDB->RotateDataEx(pItem->GetID(), dlg.m_nRotate);
			break;
		}
	}

	UpdateAllViews(NULL, VIEWHINT_RELOADITEM);
}

void CMMLibDoc::OnUpdatePictAllRotate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CheckSelectedItemType(ITEM_FOLDER | ITEM_DATA | ITEM_INBOX));
}

void CMMLibDoc::OnPictWindow()
{
	UpdateAllViews(NULL, VIEWHINT_PICTFRAME);
}

void CMMLibDoc::OnUpdatePictWindow(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bPictView);
}

void CMMLibDoc::OnFilePassword()
{
	CPasswordDlg dlg;

	CItem *pItem = GetSelectedItem();
	while (pItem != NULL) {
		if (pItem->m_nItemType == ITEM_FOLDER) {
			if (((CItemFolder *)pItem)->m_nFolderType == FTYPE_LIBRARY) {
				dlg.m_pItemFolder = (CItemFolder *)pItem;
				break;
			}
		} else if (pItem->m_nItemType != ITEM_DATA && pItem->m_nItemType != ITEM_FIND)
			break;

		pItem = pItem->GetParent();
	}

	if (dlg.DoModal() == IDOK) {
		dlg.m_pItemFolder->m_pPcsDB->SetPassword(dlg.m_sPassword);
	}
}

void CMMLibDoc::OnFilePermission()
{
	CPermissionDlg dlg;

	CItem *pItem = GetSelectedItem();
	while (pItem != NULL) {
		if (pItem->m_nItemType == ITEM_FOLDER) {
			if (((CItemFolder *)pItem)->m_nFolderType == FTYPE_LIBRARY) {
				dlg.m_pItemFolder = (CItemFolder *)pItem;
				break;
			}
		} else if (pItem->m_nItemType != ITEM_DATA && pItem->m_nItemType != ITEM_FIND)
			break;

		pItem = pItem->GetParent();
	}

	if (dlg.DoModal() == IDOK) {
		dlg.m_pItemFolder->m_pPcsDB->SetPermission(dlg.m_dwPermission);
	}
}

void CMMLibDoc::OnEditCopy()
{
	if (CopyToClipboard())
		m_bEditCut = FALSE;
}

void CMMLibDoc::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CheckSelectedItemType(ITEM_FOLDER | ITEM_DATA));
}

void CMMLibDoc::OnEditCut()
{
	if (CopyToClipboard())
		m_bEditCut = TRUE;
}

void CMMLibDoc::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CheckSelectedItemType(ITEM_FOLDER | ITEM_DATA));
}

void CMMLibDoc::OnEditPaste()
{
	if (::AfxGetMainWnd()->OpenClipboard()) {
		HANDLE hMem;
		if ((hMem = ::GetClipboardData(g_nCFMMLibrary)) != NULL) {
			CItem *pItemClip = *(CItem **)::GlobalLock(hMem);
			CItem *pItemSelect;
			if ((pItemSelect = GetSelectedItem()) != NULL) {
				if (m_bEditCut)
					MoveItem(pItemClip, pItemSelect);
				else
					CopyItem(pItemClip, pItemSelect);
			}
			::GlobalUnlock(hMem);
		}
	}
}

void CMMLibDoc::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	BOOL bEnable = FALSE;

	if (::AfxGetMainWnd()->OpenClipboard()) {
		HANDLE hMem;
		if ((hMem = ::GetClipboardData(g_nCFMMLibrary)) != NULL) {
			CItem *pItemClip = *(CItem **)::GlobalLock(hMem);
			if (CItem *pItemSelect = GetSelectedItem(); pItemSelect != NULL) {
				if (pItemClip != pItemSelect && pItemClip->GetParent() != pItemSelect && !CheckParent(pItemClip, pItemSelect)) {
					if (pItemSelect->m_nItemType == ITEM_FOLDER || pItemSelect->m_nItemType == ITEM_INBOX) {
						switch (((CItemFolder *)pItemSelect)->m_nFolderType) {
						case FTYPE_LIBRARY:
						case FTYPE_GFOLDER:
							if (pItemClip->m_nItemType == ITEM_FOLDER)
								bEnable = TRUE;
							break;
						case FTYPE_DFOLDER:
							if (pItemClip->m_nItemType == ITEM_DATA)
								bEnable = TRUE;
							break;
						}
					}
				}
			}
			::GlobalUnlock(hMem);
		}
		::CloseClipboard();
	}

	pCmdUI->Enable(bEnable);
}

BOOL CMMLibDoc::CopyToClipboard()
{
	CItem *pItem;

	if ((pItem = GetSelectedItem()) != NULL) {
		HANDLE hMem = ::GlobalAlloc(GMEM_DDESHARE, sizeof(CItem *));
		CItem **ppItem = (CItem **)::GlobalLock(hMem);
		*ppItem = pItem;
		::GlobalUnlock(hMem);
		if (::AfxGetApp()->m_pMainWnd->OpenClipboard()) {
			BeginWaitCursor();
			::EmptyClipboard();
			::SetClipboardData(g_nCFMMLibrary, hMem);
			::CloseClipboard();
			EndWaitCursor();
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CMMLibDoc::SaveModified()
{
	UpdateAllViews(NULL, VIEWHINT_CHECKSAVE);

	return TRUE;
}

void CMMLibDoc::OnNewData()
{
	if (g_pItemInbox == NULL)
		return;

	STIME stime;
	CMMPcsDB *pPcsDB = g_pItemInbox->m_pPcsDB;
	pPcsDB->m_pPcserve->m_oPcserve.GetTime(&stime);

	CString sFolderName;
	sFolderName.Format("Inbox%04d%02d%02d%02d%02d%02d", stime.year, stime.month, stime.day, stime.hour, stime.minute, stime.second);
	FOLDERREC oFolderRec{
		.sParentFolderID = g_pItemInbox->m_oFolderRec.sFolderID,
		.sFolderType = FTYPE_DFOLDER,
		.sFolderName = sFolderName,
		.sCreator = GetDefaultUserName(),
		.sCodeDef = "00000010",
		.nCodeStep = 10
	};

	UpdateAllViews(NULL, VIEWHINT_OPENITEM, g_pItemInbox);
	if (pPcsDB->AddFolderEx(&oFolderRec)) {
		CAddItemFolder oAddItem;
		oAddItem.m_pItemFolder = g_pItemInbox;
		oAddItem.m_pFolderRec = &oFolderRec;
		UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItem);
		UpdateAllViews(NULL, VIEWHINT_SELECTITEM, oAddItem.m_pItemAddFolder);

		CreateData(oAddItem.m_pItemAddFolder);

		DATAREC oDataRec;
		if (!pPcsDB->ReadChildDataEx(oFolderRec.sFolderID, &oDataRec, TRUE))
			DeleteFolder(oAddItem.m_pItemAddFolder);
	}

	NotifyUpdate();
}

void CMMLibDoc::OnViewMax()
{
	CMainFrame *pMainWnd = (CMainFrame *)::AfxGetMainWnd();

	m_bMaxPict = !m_bMaxPict;
	pMainWnd->MaxPictView(m_bMaxPict);
}

void CMMLibDoc::OnUpdateViewMax(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bMaxPict);
}

void CMMLibDoc::OnKeyword()
{
	CMMLibApp *pWinApp = (CMMLibApp *)::AfxGetApp();
	CDocTemplate *pTemplate = pWinApp->m_pFindDocTemplate;
	CRuntimeClass* pDocClass = RUNTIME_CLASS(CFindKeywordDoc);
	CFindKeywordDoc *pFindDoc = (CFindKeywordDoc *)(pDocClass->CreateObject());
	CFindKeywordFrame *pFrame = (CFindKeywordFrame *)pTemplate->CreateNewFrame(pFindDoc, NULL);
	pFindDoc->m_pMMLibDoc = this;
	pFindDoc->m_pFindFrame = (CFindKeywordFrame *)pFrame;
	pTemplate->InitialUpdateFrame(pFrame, pFindDoc);
	pFrame->m_wndKeyword.SetFocus();
}

void CMMLibDoc::InsertData(LPCSTR pFolderTitle, LPCSTR pFolderComment, LPCSTR pDataTitle, LPCSTR pDataComment, HDIB hDIB, LPCSTR pText)
{
	if (g_pItemInbox == NULL)
		return;

	CMMPcsDB *pPcsDB = g_pItemInbox->m_pPcsDB;
	BOOL bFind = FALSE;
	CItemFolder *pItemFolder = NULL;

	UpdateAllViews(NULL, VIEWHINT_OPENITEM, g_pItemInbox);

	for (HTREEITEM hItem = g_pTreeCtrl->GetChildItem(g_pItemInbox->m_hItem); hItem != NULL; hItem = g_pTreeCtrl->GetNextSiblingItem(hItem)) {
		CItem *pItem = (CItem *)g_pTreeCtrl->GetItemData(hItem);
		if (pItem->m_nItemType == ITEM_FOLDER) {
			pItemFolder = (CItemFolder *)pItem;
			if (pItemFolder->m_nFolderType == FTYPE_DFOLDER && pItemFolder->m_oFolderRec.sFolderName == pFolderTitle) {
				bFind = TRUE;
				break;
			}
		}
	}

	if (!bFind) {
		FOLDERREC oFolderRec{
			.sParentFolderID = g_pItemInbox->m_oFolderRec.sFolderID,
			.sFolderType = FTYPE_DFOLDER,
			.sFolderName = pFolderTitle,
			.sCreator = GetDefaultUserName(),
			.sRemark = pFolderComment,
			.sCodeDef = "00000010",
			.nCodeStep = 10
		};

		if (pPcsDB->AddFolderEx(&oFolderRec)) {
			CAddItemFolder oAddItem;
			oAddItem.m_pItemFolder = g_pItemInbox;
			oAddItem.m_pFolderRec = &oFolderRec;
			UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItem);
			pItemFolder = oAddItem.m_pItemAddFolder;
		} else
			return;
	}

	UpdateAllViews(NULL, VIEWHINT_OPENITEM, pItemFolder);

	DATAREC oDataRec{
		.sFolderID = pItemFolder->m_oFolderRec.sFolderID,
		.sCreator = GetDefaultUserName(),
		.sCode = pItemFolder->m_oFolderRec.sCodeDef,
		.sTitle = pDataTitle,
		.sRemark = pDataComment
	};
	if (!pItemFolder->m_pPcsDB->AddDataEx(&oDataRec))
		return;

	if (hDIB != NULL) {
		HDIB hDIB2 = MakeSmallPicture(hDIB, PICT_WIDTH, PICT_HEIGHT);
		pItemFolder->m_pPcsDB->WritePictureDataEx(oDataRec.sDataID, hDIB, hDIB2, GetMMType(hDIB));
	}

	CAddItemData oAddItemData;
	oAddItemData.m_pItemFolder = pItemFolder;
	oAddItemData.m_pDataRec = &oDataRec;
	UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItemData);

	if (strlen(pText) > 0)
		pItemFolder->m_pPcsDB->WriteTextDataEx(oDataRec.sDataID, (CString)pText);

	UpdateAllViews(NULL, VIEWHINT_SELECTITEM, oAddItemData.m_pItemAddData);

	AutoNextNumbering(pItemFolder->m_oFolderRec.sCodeDef, pItemFolder->m_oFolderRec.nCodeStep);
	pItemFolder->m_pPcsDB->UpdateFolderEx(&pItemFolder->m_oFolderRec);

	MessageBeep(0);
}

void CMMLibDoc::OnEditImage()
{
	UpdateAllViews(NULL, VIEWHINT_EDITPICT);

	NotifyUpdate();
}

void CMMLibDoc::OnUpdateEditImage(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bPictView && !g_sImageEditor.IsEmpty());
}

void CMMLibDoc::Reload()
{
	g_pMMLibDB->ReOpenMMLibrary();
	UpdateAllViews(NULL, VIEWHINT_RELOADITEM);
}

void CMMLibDoc::OnFindPict()
{
	CFindPictDlg dlg;

	dlg.m_pItemData = (CItemData *)GetSelectedItem();
	dlg.m_pDocument = this;
	dlg.DoModal();
}

void CMMLibDoc::OnUpdateFindPict(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(CheckSelectedItemType(ITEM_DATA));
}

void CMMLibDoc::OnAccessHistory()
{
	CAccessLogDlg dlg;

	dlg.m_pDocument = this;
	dlg.DoModal();
}

void CMMLibDoc::OnFilePageSetup()
{
	CPageSetupDialog PSDlg(PSD_MARGINS | PSD_INHUNDREDTHSOFMILLIMETERS | PSD_DISABLEPRINTER, ::AfxGetMainWnd());

	PSDlg.m_psd.rtMargin = g_rcPrintMargin;
	if(g_pPrintDevMode)
		PSDlg.m_psd.hDevMode = g_pPrintDevMode;

	if (PSDlg.DoModal() == IDOK) {
		PSDlg.GetMargins(g_rcPrintMargin, NULL);
		g_pPrintDevMode = PSDlg.GetDevMode();
	}
}

void CMMLibDoc::OnProperty()
{
	CItem *pItem;

	if ((pItem = GetSelectedItem()) == NULL)
		return;

	CPropertyDlg dlg;

	dlg.m_pItem = pItem;

	if (dlg.DoModal() == IDC_BUTTON_FIND) {
		CSelectItem oSelectItem;

		switch (pItem->m_nItemType) {
		case ITEM_FOLDER:
			oSelectItem.m_sLibraryID = ((CItemFolder *)pItem)->m_pPcsDB->m_sLibraryID;
			oSelectItem.m_sFolderID = ((CItemFolder *)pItem)->m_oFolderRec.sFolderID;
			break;
		case ITEM_DATA:
			oSelectItem.m_sLibraryID = ((CItemData *)pItem)->m_pPcsDB->m_sLibraryID;
			oSelectItem.m_sDataID = ((CItemData *)pItem)->m_oDataRec.sDataID;
			break;
		default:
			return;
		}

		UpdateAllViews(NULL, VIEWHINT_SELECTDATA, &oSelectItem);
	}
}

void CMMLibDoc::OnViewTextWordwrap()
{
	m_bTextWordwrap = !m_bTextWordwrap;

	UpdateAllViews(NULL, VIEWHINT_WORDWRAP);
}

void CMMLibDoc::OnUpdateViewTextWordwrap(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bTextWordwrap);
}

void CMMLibDoc::OnImportMmdb()
{
	ImportFile(GetString(IDS_MMDB_FILE) + " (*.mmdb)|*.mmdb||");

	NotifyUpdate();
}

void CMMLibDoc::OnUpdateImportMmdb(CCmdUI *pCmdUI)
{
	BOOL bEnable = FALSE;
	CItem *pItem;

	if ((pItem = GetSelectedItem()) != NULL) {
		if (pItem->m_nItemType == ITEM_FOLDER) {
			switch (((CItemFolder *)pItem)->m_nFolderType) {
			case FTYPE_LIBRARY:
			case FTYPE_GFOLDER:
				bEnable = TRUE;
				break;
			}
		}
	}

	pCmdUI->Enable(bEnable);
}

void CMMLibDoc::OnImportPdf()
{
	ImportFile(GetString(IDS_PDF_FILE) + " (*.pdf)|*.pdf||");

	NotifyUpdate();
}

void CMMLibDoc::OnUpdateImportPdf(CCmdUI *pCmdUI)
{
	BOOL bEnable = FALSE;
	CItem *pItem;
	static BOOL bCheckPdfDll = FALSE;
	static BOOL bExistPdfDll = FALSE;

	if (!bCheckPdfDll) {
		bCheckPdfDll = TRUE;

		CString sGsCommand = GetGsCommand();
		if (!sGsCommand.IsEmpty()) {
			HINSTANCE	hInstDLL;
			if ((hInstDLL = ::LoadLibrary(PDF_TEXT_DLL)) != NULL) {
				bExistPdfDll = TRUE;
				::FreeLibrary(hInstDLL);
			}
		}
	}

	if ((pItem = GetSelectedItem()) != NULL) {
		if (pItem->m_nItemType == ITEM_FOLDER || pItem->m_nItemType == ITEM_DATA) {
			if (bExistPdfDll) {
				bEnable = TRUE;
			}
		}
	}

	pCmdUI->Enable(bEnable);
}

CString CMMLibDoc::GetGsCommand()
{
	CString sGsCommand;
	CString sProgramFile = "C:\\Program Files\\gs";

	CFileFind oFileFind;
	if (oFileFind.FindFile(sProgramFile + "\\*")) {
		BOOL bFind = TRUE;
		while (bFind) {
			bFind = oFileFind.FindNextFile();

			if (oFileFind.GetFileName().Left(2) == "gs") {
				sProgramFile = oFileFind.GetFilePath() + "\\bin";
				CFileFind oFileFind2;
				if (oFileFind2.FindFile(sProgramFile + "\\*")) {
					BOOL bFind2 = TRUE;
					while (bFind2) {
						bFind2 = oFileFind2.FindNextFile();

						if (oFileFind2.GetFileName() == "gswin32c.exe" || oFileFind2.GetFileName() == "gswin64c.exe") {
							sGsCommand = oFileFind2.GetFilePath();
							break;
						}
					}
				}
			}
		}
	}

	return sGsCommand;
}

void CMMLibDoc::ImportPdfFile(LPCSTR pFileName, CItemFolder *pItemFolder)
{
	CWaitCursor wait;
	CString sTempFolder;
	::GetTempPath(MAX_PATH, sTempFolder.GetBuffer(MAX_PATH));
	sTempFolder.ReleaseBuffer();
	sTempFolder += "MMLIB_PDF";
	::DeleteDirectory(sTempFolder);
	::CreateDirectory(sTempFolder, NULL);

	CString sCommand;
	sCommand.Format("\"%s\" -dBATCH -dNOPAUSE -sDEVICE=bmp16m -r200 -dTextAlphaBits=4 -dGraphicsAlphaBits=4 -sOutputFile=\"%s\" \"%s\"",
		(LPCSTR)GetGsCommand(),
		(LPCSTR)(sTempFolder + "\\" + ::RemoveExt(::RemoveDir(pFileName)) + "_%02d.bmp"),
			pFileName);

	STARTUPINFO si{
		.cb = sizeof(si),
		.dwFlags = STARTF_USESHOWWINDOW,
		.wShowWindow = SW_HIDE
	};
	PROCESS_INFORMATION pi;
	CreateProcess(NULL, (LPSTR)(LPCSTR)sCommand, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	CloseHandle(pi.hThread);
	WaitForSingleObject(pi.hProcess,INFINITE);
	CloseHandle(pi.hProcess);

	CStringArray aText;
	HINSTANCE	hInstDLL;
	if ((hInstDLL = ::LoadLibrary(PDF_TEXT_DLL)) != NULL) {
		typedef int (*FUNCTYPE)(BSTR lpFilePath, bool bProp,  BSTR*lpFileText);
		FUNCTYPE ExtractText = (FUNCTYPE)GetProcAddress(hInstDLL, "ExtractText");
		if (ExtractText != NULL) {
			BSTR fileText = NULL;
			BSTR fileName = ((CString)pFileName).AllocSysString();
			ExtractText(fileName, false, &fileText);

			CString sText = fileText;

			::SysFreeString(fileText);
			::SysFreeString(fileName);

			CStringArray aTextLine;
			StrSplit(sText, "\r\n", aTextLine);

			CString sTextPage;
			int nPage, nTotal;
			for (int i = 0; i < aTextLine.GetCount(); i++) {
				if (sscanf_s(aTextLine[i], "--%d/%d--", &nPage, &nTotal) == 2) {
					if (nPage <= aText.GetSize()) {
						sTextPage = aText[nPage - 1] + "\r\n" + sTextPage;
					}
					aText.SetAtGrow(nPage - 1, sTextPage);
					sTextPage.Empty();
				} else {
					sTextPage += aTextLine[i] + "\r\n";
				}
			}
		}
	}

	CFileFind oFileFind;
	if (oFileFind.FindFile(sTempFolder + "\\*.bmp")) {
		HDIB hDIB, hDIB2;
		CString name, date;
		CString sFolderID;
		CString creator;
		CAddItemData oAddItemData;
		CString sFileName;
		int nPageNo;

		DWORD userNameSize = MAXUSERNAME;
		GetUserName(creator.GetBuffer(MAXUSERNAME), &userNameSize);
		creator.ReleaseBuffer();

		BOOL bFind = TRUE;
		while (bFind) {
			bFind = oFileFind.FindNextFile();

			sFileName = oFileFind.GetFileName();
			nPageNo = atoi(sFileName.Right(sFileName.GetLength() - sFileName.ReverseFind('_') - 1));

			hDIB = hDIB2 = NULL;
			if (MMPictureReadFile(&hDIB, (LPSTR)(LPCSTR)(oFileFind.GetFilePath())) != 0)
				continue;
			if (hDIB != NULL)
				hDIB2 = MakeSmallPicture(hDIB, PICT_WIDTH, PICT_HEIGHT);
			else
				continue;

			name = RemoveExt(oFileFind.GetFileName());
			FILETIME oFileTime;
			oFileFind.GetLastWriteTime(&oFileTime);
			date = ConvFileTime(&oFileTime);

			sFolderID = pItemFolder->GetID();
			DATAREC oDataRec{
				.sFolderID = sFolderID,
				.sCreateDate = date,
				.sCreator = creator,
				.sTitle = name
			};
			if (pItemFolder->m_pPcsDB->AddDataEx(&oDataRec))
				pItemFolder->m_pPcsDB->WritePictureDataEx(oDataRec.sDataID, hDIB, hDIB2, GetMMType(hDIB));

			if (hDIB != NULL)
				::GlobalFree(hDIB);
			if (hDIB2 != NULL)
				::GlobalFree(hDIB2);

			const int nPageIndex = nPageNo - 1;
			if (nPageIndex < aText.GetCount() && aText[nPageIndex].GetLength() != 0)
				pItemFolder->m_pPcsDB->WriteTextDataEx(oDataRec.sDataID, aText[nPageIndex]);

			oAddItemData.m_pItemFolder = pItemFolder;
			oAddItemData.m_pDataRec = &oDataRec;
			UpdateAllViews(NULL, VIEWHINT_ADDITEM, &oAddItemData);
			UpdateAllViews(NULL, VIEWHINT_UPDATEWINDOW);
		}
	}

	::DeleteDirectory(sTempFolder);
}

void CMMLibDoc::OnUpdateImageFileOutput(CCmdUI *pCmdUI)
{
	BOOL bEnable = FALSE;
	CItem *pItem;

	if ((pItem = GetSelectedItem()) != NULL) {
		if (pItem->m_nItemType == ITEM_FOLDER) {
			bEnable = TRUE;
		}
	}

	pCmdUI->Enable(bEnable);
}

void CMMLibDoc::OnImageFilesOutput()
{
	char dst_file[MAX_PATH];
	LPITEMIDLIST idlist;
	CString sTitle;

	for (CItem *pItem = GetSelectedItem(); pItem != NULL; pItem = pItem->m_pNextSelected) {
		switch (pItem->m_nItemType) {
		case ITEM_FOLDER:
			sTitle.LoadString(IDS_OUTPUT_IMAGES_FOLDER);
			strcpy_s(dst_file, m_OutputImagesFolder);
			BROWSEINFO binfo{
				.hwndOwner = ::AfxGetMainWnd()->m_hWnd,
				.pidlRoot = NULL,
				.pszDisplayName = dst_file,
				.lpszTitle = sTitle,
				.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI,
				.lpfn = &BrowseCallbackProc,
				.lParam = (LPARAM)dst_file,
				.iImage = (int)NULL
			};
			idlist = ::SHBrowseForFolder(&binfo);
			if (idlist) {
				::SHGetPathFromIDList(idlist, dst_file);
				::CoTaskMemFree(idlist);
				m_OutputImagesFolder = dst_file;

				COutputImagesDlg oOutputImagesDlg;
				oOutputImagesDlg.m_pItemFolder = (CItemFolder *)pItem;
				oOutputImagesDlg.m_sOutputFolder = m_OutputImagesFolder;
				oOutputImagesDlg.DoModal();
			}
			break;
		}
	}
}

void CMMLibDoc::OnUpdateTextSearch(CCmdUI *pCmdUI)
{
	CMainFrame *pMainWnd = (CMainFrame *)::AfxGetMainWnd();
	pCmdUI->Enable(pMainWnd->IsEnableTextView());
	pCmdUI->SetCheck(pMainWnd->IsOpenTextView());
}

void CMMLibDoc::OnTextSearch()
{
	CMainFrame *pMainWnd = (CMainFrame *)::AfxGetMainWnd();
	if (pMainWnd->IsOpenTextView()) {
		pMainWnd->CloseTextSearchDlg();
	} else {
		pMainWnd->OpenTextSearchDlg();
	}
}
