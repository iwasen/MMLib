// MMLibInfoView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "MainFrm.h"
#include "MMLibInfoView.h"
#include "FindDb.h"
#include "Shlwapi.h"
#include "afxinet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMLibInfoView

IMPLEMENT_DYNCREATE(CMMLibInfoView, CFormView)

CMMLibInfoView::CMMLibInfoView()
	: CFormView(CMMLibInfoView::IDD)
{
	m_pCurrentItem = NULL;
	m_nFormHeight = 0;
}

void CMMLibInfoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UPDATEDATE, m_cUpdateDate);
	DDX_Control(pDX, IDC_CREATEDATE, m_cCreateDate);
	DDX_Control(pDX, IDC_CODE, m_cCode);
	DDX_Control(pDX, IDC_UPDATE_INFO, m_cUpdateInfo);
	DDX_Control(pDX, IDC_TITLE, m_cTitle);
	DDX_Control(pDX, IDC_KEYWORD, m_cKeyword);
	DDX_Control(pDX, IDC_REMARK, m_cRemark);
	DDX_Control(pDX, IDC_CREATOR, m_cCreator);
	DDX_Control(pDX, IDC_BUTTON_OCR, m_cButtonOCR);
	DDX_Control(pDX, IDC_STATIC_DATA_COUNT, m_cStaticDataCount);
	DDX_Control(pDX, IDC_EDIT_DATA_COUNT, m_cEditDataCount);
	DDX_Control(pDX, IDC_CHECK_SOUND_PLAYBACK, m_cCheckSoundPlayback);
	DDX_Control(pDX, IDC_CHECK_SOUND_RECORD, m_cCheckSoundRecord);
	DDX_Control(pDX, IDC_BUTTON_SOUND_TEXT, m_cButtonSoundText);
	DDX_Control(pDX, IDC_BUTTON_SOUND_FILE, m_cButtonSoundFile);
}

BEGIN_MESSAGE_MAP(CMMLibInfoView, CFormView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_UPDATE_INFO, OnUpdateInfo)
	ON_EN_CHANGE(IDC_CODE, OnChangeCode)
	ON_EN_CHANGE(IDC_TITLE, OnChangeTitle)
	ON_EN_CHANGE(IDC_CREATOR, OnChangeCreator)
	ON_EN_CHANGE(IDC_REMARK, OnChangeRemark)
	ON_EN_CHANGE(IDC_KEYWORD, OnChangeKeyword)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_MESSAGE(WM_GETSELECTEDITEM, OnGetSelectedItem)
	ON_BN_CLICKED(IDC_BUTTON_OCR, &CMMLibInfoView::OnBnClickedButtonOcr)
	ON_BN_CLICKED(IDC_CHECK_SOUND_PLAYBACK, &CMMLibInfoView::OnBnClickedCheckSoundPlayback)
	ON_MESSAGE(WM_MMDB_NOTIFY, OnMmdbNotify)
	ON_BN_CLICKED(IDC_CHECK_SOUND_RECORD, &CMMLibInfoView::OnBnClickedCheckSoundRecord)
	ON_BN_CLICKED(IDC_BUTTON_SOUND_FILE, &CMMLibInfoView::OnBnClickedButtonSoundFile)
	ON_BN_CLICKED(IDC_BUTTON_SOUND_TEXT, &CMMLibInfoView::OnBnClickedButtonSoundText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMMLibInfoView メッセージ ハンドラ

void CMMLibInfoView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	CMainFrame *pMainFrame = (CMainFrame *)::AfxGetApp()->m_pMainWnd;
	if (pMainFrame != NULL)
		pMainFrame->MaxInfoView(m_nFormHeight);
}

void CMMLibInfoView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	switch (lHint) {
	case VIEWHINT_ADDITEM:
	case VIEWHINT_SELECTITEM:
	case VIEWHINT_OPENITEM:
	case VIEWHINT_CLOSEITEM:
	case VIEWHINT_REMOVEITEM:
	case VIEWHINT_UPDATEITEM:
	case VIEWHINT_RELOADITEM:
	case VIEWHINT_REFINDITEM:
	case VIEWHINT_DRAWALL:
	case VIEWHINT_SETFONT:
	case VIEWHINT_UPDATEWINDOW:
		CheckSave();
		DrawItem();
		break;
	case VIEWHINT_MODIFYDATA:
		ModifyData();
		break;
	case VIEWHINT_SAVEDATA:
		SaveData((CItem *)pHint);
		break;
	case VIEWHINT_CHECKSAVE:
		CheckSave();
		break;
	}
}

void CMMLibInfoView::CheckSave()
{
	if (m_pCurrentItem != NULL && m_cUpdateInfo.IsWindowEnabled()) {
		if (::AfxMessageBox(IDS_UPDATE_DATA_SAVE, MB_ICONQUESTION | MB_YESNO) == IDYES) {
			OnUpdateInfo();
		}
	}
}

void CMMLibInfoView::DrawItem()
{
	HTREEITEM hItem = g_pTreeCtrl->GetSelectedItem();
	BOOL bEnableOCR = FALSE;

	if (hItem != NULL) {
		m_pCurrentItem = (CItem *)g_pTreeCtrl->GetItemData(hItem);
		switch (m_pCurrentItem->m_nItemType) {
		case ITEM_ROOT:
			DrawRootInfo();
			break;
		case ITEM_FOLDER:
			DrawFolderInfo();
			break;
		case ITEM_DATA:
			DrawDataInfo();
			bEnableOCR = TRUE;
			break;
		case ITEM_FIND:
			DrawFindInfo();
			break;
		case ITEM_INBOX:
			DrawInboxInfo();
			break;
		default:
			DrawOtherInfo();
			break;
		}
	}

	m_cUpdateInfo.EnableWindow(FALSE);
	m_cButtonOCR.EnableWindow(bEnableOCR);

	MMWaveRecStop();
	MMWavePlayStop();
}

void CMMLibInfoView::DrawRootInfo()
{
	m_cTitle.LoadString(IDS_MULTIMEDIA_LIBRARY);
	m_cCode = "";
	m_cCreator = "";
	m_cRemark = "";
	m_cKeyword = "";
	m_cCreateDate = "";
	m_cUpdateDate = "";

	CItem *pItem = CItem::GetHeadItem();
	long nTotalCount = 0;
	while (pItem != NULL) {
		if (pItem->m_nItemType == ITEM_INBOX || (pItem->m_nItemType == ITEM_FOLDER && ((CItemFolder *)pItem)->m_nFolderType == FTYPE_LIBRARY)) {
			long nCount;
			((CItemFolder *)pItem)->m_pPcsDB->GetDataCountEx(&nCount);
			nTotalCount += nCount;
		}
		pItem = pItem->GetNextItem();
	}
	m_cEditDataCount = IntToCommaSeparateString(nTotalCount);

	m_cTitle.EnableWindow(FALSE);
	m_cCode.EnableWindow(FALSE);
	m_cCreator.EnableWindow(FALSE);
	m_cRemark.EnableWindow(FALSE);
	m_cKeyword.EnableWindow(FALSE);
	m_cStaticDataCount.ShowWindow(SW_SHOW);
	m_cEditDataCount.ShowWindow(SW_SHOW);
	m_cCheckSoundPlayback.EnableWindow(FALSE);
	m_cCheckSoundRecord.EnableWindow(FALSE);
	m_cButtonSoundText.EnableWindow(FALSE);
	m_cButtonSoundFile.EnableWindow(FALSE);
}

void CMMLibInfoView::DrawFolderInfo()
{
	CItemFolder *pItemFolder = (CItemFolder *)m_pCurrentItem;

	m_cTitle = pItemFolder->m_oFolderRec.sFolderName;
	m_cCode = "";
	m_cCreator = pItemFolder->m_oFolderRec.sCreator;
	m_cRemark = pItemFolder->m_oFolderRec.sRemark;
	m_cKeyword = pItemFolder->m_oFolderRec.sKeyword;
	m_cCreateDate = pItemFolder->m_oFolderRec.sCreateDate;
	m_cUpdateDate = pItemFolder->m_oFolderRec.sUpdateDate;

	m_cTitle.EnableWindow(TRUE);
	m_cCode.EnableWindow(FALSE);
	m_cCreator.EnableWindow(TRUE);
	m_cRemark.EnableWindow(TRUE);
	m_cKeyword.EnableWindow(TRUE);

	if (pItemFolder->m_nFolderType == FTYPE_LIBRARY) {
		m_cEditDataCount = GetDataCount(pItemFolder->m_pPcsDB);
		m_cStaticDataCount.ShowWindow(SW_SHOW);
		m_cEditDataCount.ShowWindow(SW_SHOW);
	} else {
		m_cStaticDataCount.ShowWindow(SW_HIDE);
		m_cEditDataCount.ShowWindow(SW_HIDE);
	}

	m_cCheckSoundPlayback.EnableWindow(FALSE);
	m_cCheckSoundRecord.EnableWindow(FALSE);
	m_cButtonSoundText.EnableWindow(FALSE);
	m_cButtonSoundFile.EnableWindow(FALSE);
}

void CMMLibInfoView::DrawDataInfo()
{
	CItemData *pItemData = (CItemData *)m_pCurrentItem;
	LONG nSoundDataSize;

	m_cTitle = pItemData->m_oDataRec.sTitle;
	m_cCode = pItemData->m_oDataRec.sCode;
	m_cCreator = pItemData->m_oDataRec.sCreator;
	m_cRemark = pItemData->m_oDataRec.sRemark;
	m_cKeyword = pItemData->m_oDataRec.sKeyword;
	m_cCreateDate = pItemData->m_oDataRec.sCreateDate;
	m_cUpdateDate = pItemData->m_oDataRec.sUpdateDate;

	m_cTitle.EnableWindow(TRUE);
	m_cCode.EnableWindow(TRUE);
	m_cCreator.EnableWindow(TRUE);
	m_cRemark.EnableWindow(TRUE);
	m_cKeyword.EnableWindow(TRUE);
	m_cStaticDataCount.ShowWindow(SW_HIDE);
	m_cEditDataCount.ShowWindow(SW_HIDE);

	pItemData->m_pPcsDB->m_oDataDB.DBChgIdx(2);
	pItemData->m_pPcsDB->m_oDataDB.DBSearch(pItemData->m_oDataRec.sDataID, LEN_ID);
	pItemData->m_pPcsDB->m_oDataDB.DBGetBinarySize(SOUND_FIELD, &nSoundDataSize);
	m_cCheckSoundPlayback.EnableWindow(nSoundDataSize != 0);
	m_cCheckSoundRecord.EnableWindow(TRUE);
	m_cButtonSoundText.EnableWindow(nSoundDataSize != 0);
	m_cButtonSoundFile.EnableWindow(TRUE);
}

void CMMLibInfoView::DrawFindInfo()
{
	CItemFind *pItemFind = (CItemFind *)m_pCurrentItem;

	m_cTitle = pItemFind->m_oFindRec.sFolderName;
	m_cCode = "";
	m_cCreator = "";
	m_cRemark = "";
	m_cKeyword = "";
	m_cCreateDate = "";
	m_cUpdateDate = "";

	m_cTitle.EnableWindow(TRUE);
	m_cCode.EnableWindow(FALSE);
	m_cCreator.EnableWindow(FALSE);
	m_cRemark.EnableWindow(FALSE);
	m_cKeyword.EnableWindow(FALSE);
	m_cStaticDataCount.ShowWindow(SW_HIDE);
	m_cEditDataCount.ShowWindow(SW_HIDE);
	m_cCheckSoundPlayback.EnableWindow(FALSE);
	m_cCheckSoundRecord.EnableWindow(FALSE);
	m_cButtonSoundText.EnableWindow(FALSE);
	m_cButtonSoundFile.EnableWindow(FALSE);
}

void CMMLibInfoView::DrawInboxInfo()
{
	CItemInbox *pItemInbox = (CItemInbox *)m_pCurrentItem;

	m_cTitle = "";
	m_cCode = "";
	m_cCreator = "";
	m_cRemark = "";
	m_cKeyword = "";
	m_cCreateDate = "";
	m_cUpdateDate = "";
	m_cEditDataCount = GetDataCount(pItemInbox->m_pPcsDB);

	m_cTitle.EnableWindow(FALSE);
	m_cCode.EnableWindow(FALSE);
	m_cCreator.EnableWindow(FALSE);
	m_cRemark.EnableWindow(FALSE);
	m_cKeyword.EnableWindow(FALSE);
	m_cStaticDataCount.ShowWindow(SW_SHOW);
	m_cEditDataCount.ShowWindow(SW_SHOW);
	m_cCheckSoundPlayback.EnableWindow(FALSE);
	m_cCheckSoundRecord.EnableWindow(FALSE);
	m_cButtonSoundText.EnableWindow(FALSE);
	m_cButtonSoundFile.EnableWindow(FALSE);
}

void CMMLibInfoView::DrawOtherInfo()
{
	m_cTitle = "";
	m_cCode = "";
	m_cCreator = "";
	m_cRemark = "";
	m_cKeyword = "";
	m_cCreateDate = "";
	m_cUpdateDate = "";

	m_cTitle.EnableWindow(FALSE);
	m_cCode.EnableWindow(FALSE);
	m_cCreator.EnableWindow(FALSE);
	m_cRemark.EnableWindow(FALSE);
	m_cKeyword.EnableWindow(FALSE);
	m_cStaticDataCount.ShowWindow(SW_HIDE);
	m_cEditDataCount.ShowWindow(SW_HIDE);
	m_cCheckSoundPlayback.EnableWindow(FALSE);
	m_cCheckSoundRecord.EnableWindow(FALSE);
	m_cButtonSoundText.EnableWindow(FALSE);
	m_cButtonSoundFile.EnableWindow(FALSE);
}

void CMMLibInfoView::OnUpdateInfo()
{
	CDocument *pDoc = GetDocument();

	m_cUpdateInfo.EnableWindow(FALSE);

	if (m_pCurrentItem != NULL) {
		pDoc->UpdateAllViews(NULL, VIEWHINT_SAVEDATA, m_pCurrentItem);
		pDoc->UpdateAllViews(NULL, VIEWHINT_UPDATEITEM, m_pCurrentItem);

		NotifyUpdate();
	}
}

void CMMLibInfoView::SaveData(CItem *pItem)
{
	if (pItem != NULL) {
		switch (pItem->m_nItemType) {
		case ITEM_FOLDER:
			SaveFolderInfo((CItemFolder *)pItem);
			break;
		case ITEM_DATA:
			SaveDataInfo((CItemData *)pItem);
			break;
		case ITEM_FIND:
			SaveFindInfo((CItemFind *)pItem);
			break;
		}
	}
}

void CMMLibInfoView::SaveFolderInfo(CItemFolder *pItemFolder)
{
	FOLDERREC &oFolderRec = pItemFolder->m_oFolderRec;

	oFolderRec.sFolderName = m_cTitle;
	oFolderRec.sCreator = m_cCreator;
	oFolderRec.sRemark = m_cRemark;
	oFolderRec.sKeyword = m_cKeyword;

	pItemFolder->m_pPcsDB->UpdateFolderEx(&pItemFolder->m_oFolderRec);
}

void CMMLibInfoView::SaveDataInfo(CItemData *pItemData)
{
	DATAREC &DataRec = pItemData->m_oDataRec;

	DataRec.sTitle = m_cTitle;
	DataRec.sCode = m_cCode;
	DataRec.sCreator = m_cCreator;
	DataRec.sRemark = m_cRemark;
	DataRec.sKeyword = m_cKeyword;

	pItemData->m_pPcsDB->UpdateDataEx(&pItemData->m_oDataRec);
}

void CMMLibInfoView::SaveFindInfo(CItemFind *pItemFind)
{
	FINDREC &FindRec = pItemFind->m_oFindRec;

	FindRec.sFolderName = m_cTitle;

	if (pItemFind->m_bSave) {
		CFindDB findDB;
		if (findDB.Open(FALSE))
			findDB.UpdateFind(&pItemFind->m_oFindRec);
	}
}

void CMMLibInfoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_cCode.LimitText(LEN_CODE);
	m_cTitle.LimitText(LEN_TITLE);
	m_cCreator.LimitText(LEN_CREATOR);
	m_cRemark.LimitText(LEN_REMARK);
	m_cKeyword.LimitText(LEN_KEYWORD);

	SIZE size = GetTotalSize();
	m_nFormHeight = size.cy;

	size.cx = 0;
	SetScrollSizes(MM_TEXT, size);

	CMainFrame *pMainFrame = (CMainFrame *)::AfxGetApp()->m_pMainWnd;
	if (pMainFrame != NULL)
		pMainFrame->MaxInfoView(m_nFormHeight);

//	if (::PathFileExists(PDF_TEXT_DLL))
		m_cButtonOCR.ShowWindow(SW_SHOW);
}

void CMMLibInfoView::ModifyData()
{
	m_cUpdateInfo.EnableWindow(TRUE);
}

void CMMLibInfoView::OnChangeCode()
{
	ModifyData();
}

void CMMLibInfoView::OnChangeTitle()
{
	ModifyData();
}

void CMMLibInfoView::OnChangeCreator()
{
	ModifyData();
}

void CMMLibInfoView::OnChangeRemark()
{
	ModifyData();
}

void CMMLibInfoView::OnChangeKeyword()
{
	ModifyData();
}

LRESULT CMMLibInfoView::OnGetSelectedItem(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return (LRESULT)m_pCurrentItem;
}

void CMMLibInfoView::OnEditCopy()
{
	CWnd *pWnd;

	if ((pWnd = GetFocus()) != NULL)
		pWnd->SendMessage(WM_COPY);
}

void CMMLibInfoView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMMLibInfoView::OnEditCut()
{
	CWnd *pWnd;

	if ((pWnd = GetFocus()) != NULL)
		pWnd->SendMessage(WM_CUT);
}

void CMMLibInfoView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMMLibInfoView::OnEditPaste()
{
	CWnd *pWnd;

	if ((pWnd = GetFocus()) != NULL)
		pWnd->SendMessage(WM_PASTE);
}

void CMMLibInfoView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	UINT nFormat = 0;
	BOOL bEnable  = FALSE;

	if (OpenClipboard()) {
		while ((nFormat = EnumClipboardFormats(nFormat)) != 0) {
			if (nFormat == CF_TEXT) {
				bEnable = TRUE;
				break;
			}
		}
		CloseClipboard();
	}

	pCmdUI->Enable(bEnable);
}

void CMMLibInfoView::OnBnClickedButtonOcr()
{
	CWaitCursor wait;
	CItemData *pItemData = (CItemData *)m_pCurrentItem;
	HDIB hDIB;
	HANDLE hMem;
	long nImageSize;

	if (!pItemData->m_pPcsDB->ReadPictureDataEx(pItemData->m_oDataRec.sDataID, &hDIB, NULL))
		return;

	SHORT mmType = GetMMType(hDIB);
	CString sType;
	if (mmType == MMTYPE_JPG) {
		sType = "jpeg";
	} else {
		mmType = MMTYPE_PNG;
		sType = "png";
	}

	if (MMPictureWriteMem(hDIB, &hMem, mmType, &nImageSize) == 0) {
		BYTE *pImageBuf = (BYTE *)::GlobalLock(hMem);

		CString url = "http://mmlib.troot.co.jp/ocr.php";

		CString sServer, sObject;
		INTERNET_PORT nPort;
		DWORD dwServiceType;
		::AfxParseURL(url , dwServiceType, sServer, sObject, nPort);

		CString strHeaders = "Content-Type: image/" + sType;

		CInternetSession session;
		CHttpConnection* pConnection = session.GetHttpConnection(sServer, nPort);
		CHttpFile* pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, sObject);

		if (pFile->SendRequest(strHeaders, pImageBuf + sizeof(SHORT), nImageSize - sizeof(SHORT))) {
			CDocument *pDoc = GetDocument();
			pDoc->UpdateAllViews(NULL, VIEWHINT_TEXTFILE, pFile);
		}

		::GlobalUnlock(hMem);
		::GlobalFree(hMem);
	}

	::GlobalFree(hDIB);
}

CString CMMLibInfoView::GetDataCount(CMMPcsDB *pPcsDB)
{
	long nCount;
	pPcsDB->GetDataCountEx(&nCount);
	return IntToCommaSeparateString(nCount);
}

void CMMLibInfoView::OnBnClickedCheckSoundPlayback()
{
	CItemData *pItemData = (CItemData *)m_pCurrentItem;

	MMWaveRecStop();

	if (m_cCheckSoundPlayback.GetCheck() == 0) {
		pItemData->m_pPcsDB->m_oDataDB.DBChgIdx(2);
		pItemData->m_pPcsDB->m_oDataDB.DBSearch(pItemData->m_oDataRec.sDataID, LEN_ID);
		if (MMWavePlayDB(pItemData->m_pPcsDB->m_oDataDB.GetFCB(), SOUND_FIELD, m_hWnd) == 0) {
			m_cCheckSoundPlayback.SetCheck(1);
			m_cCheckSoundRecord.EnableWindow(FALSE);
			m_cButtonSoundText.EnableWindow(FALSE);
			m_cButtonSoundFile.EnableWindow(FALSE);
		}
	} else {
		MMWavePlayStop();
	}
}

LRESULT CMMLibInfoView::OnMmdbNotify(WPARAM wParam, LPARAM /*lParam*/)
{
	switch (wParam) {
	case MMNOTIFY_PLAYEND:
		m_cCheckSoundPlayback.SetCheck(0);
		m_cCheckSoundRecord.EnableWindow(TRUE);
		m_cButtonSoundText.EnableWindow(TRUE);
		m_cButtonSoundFile.EnableWindow(TRUE);
		break;
	case MMNOTIFY_RECEND:
		m_cCheckSoundRecord.SetCheck(0);
		m_cCheckSoundPlayback.EnableWindow(TRUE);
		m_cButtonSoundText.EnableWindow(TRUE);
		m_cButtonSoundFile.EnableWindow(TRUE);
		break;
	}

	return 0;
}

void CMMLibInfoView::OnBnClickedCheckSoundRecord()
{
	CItemData *pItemData = (CItemData *)m_pCurrentItem;

	MMWavePlayStop();

	if (m_cCheckSoundRecord.GetCheck() == 0) {
		if (::AfxMessageBox(IDS_SOUND_RECORD, MB_ICONQUESTION | MB_OKCANCEL) == IDCANCEL)
			return;

		pItemData->m_pPcsDB->m_oDataDB.DBChgIdx(2);
		pItemData->m_pPcsDB->m_oDataDB.DBSearch(pItemData->m_oDataRec.sDataID, LEN_ID);
		if  (MMWaveRecDB(pItemData->m_pPcsDB->m_oDataDB.GetFCB(), SOUND_FIELD, 1, 16000, 16, MMTYPE_FLAC, m_hWnd) == 0) {
			m_cCheckSoundRecord.SetCheck(1);
			m_cCheckSoundPlayback.EnableWindow(FALSE);
			m_cButtonSoundText.EnableWindow(FALSE);
			m_cButtonSoundFile.EnableWindow(FALSE);
		}
	} else {
		MMWaveRecStop();
	}
}

void CMMLibInfoView::OnBnClickedButtonSoundFile()
{
	CItemData *pItemData = (CItemData *)m_pCurrentItem;

	MMWaveRecStop();
	MMWavePlayStop();

	CFileDialog dlg(TRUE, "wav;flac", NULL,
		OFN_HIDEREADONLY,
		GetString(IDS_AUDIO_FILE) + " (*.wav;*.flac)|*.wav;*.flac|All Files (*.*)|*.*||", this);
	dlg.m_ofn.lpstrTitle = GetString(IDS_SELECT_AUDIO_FILE);
	if (dlg.DoModal() == IDOK) {
		CWaitCursor waitCursor;
		HWAVEDATA hWaveData;

		if (MMWaveReadFile(&hWaveData, (LPTSTR)(LPCSTR)dlg.GetPathName()) == 0) {
			pItemData->m_pPcsDB->m_oDataDB.DBChgIdx(2);
			pItemData->m_pPcsDB->m_oDataDB.DBSearch(pItemData->m_oDataRec.sDataID, LEN_ID);
			MMWaveWriteDB(hWaveData, pItemData->m_pPcsDB->m_oDataDB.GetFCB(), SOUND_FIELD, MMTYPE_FLAC);
			::GlobalFree(hWaveData);

			m_cCheckSoundPlayback.EnableWindow(TRUE);
			m_cButtonSoundText.EnableWindow(TRUE);
		}
	}
}

void CMMLibInfoView::OnBnClickedButtonSoundText()
{
	CWaitCursor waitCursor;
	CItemData *pItemData = (CItemData *)m_pCurrentItem;
	LONG nDataSize, nReadSize;

	MMWaveRecStop();
	MMWavePlayStop();

	pItemData->m_pPcsDB->m_oDataDB.DBChgIdx(2);
	pItemData->m_pPcsDB->m_oDataDB.DBSearch(pItemData->m_oDataRec.sDataID, LEN_ID);
	pItemData->m_pPcsDB->m_oDataDB.DBGetBinarySize(SOUND_FIELD, &nDataSize);
	BYTE *pData = new BYTE[nDataSize];
	pItemData->m_pPcsDB->m_oDataDB.DBReadBinary(SOUND_FIELD, pData, nDataSize, &nReadSize);

	if (nReadSize != 0) {
		CString url = "http://mmlib.troot.co.jp/speech.php";

		CString sServer, sObject;
		INTERNET_PORT nPort;
		DWORD dwServiceType;
		::AfxParseURL(url , dwServiceType, sServer, sObject, nPort);

		CString strHeaders = "Content-Type: audio/x-flac";

		CInternetSession session;
		CHttpConnection* pConnection = session.GetHttpConnection(sServer, nPort);
		CHttpFile* pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, sObject);

		if (pFile->SendRequest(strHeaders, pData + sizeof(SHORT), nReadSize - sizeof(SHORT))) {
			CDocument *pDoc = GetDocument();
			pDoc->UpdateAllViews(NULL, VIEWHINT_TEXTFILE, pFile);
		}
	}

	delete [] pData;
}
