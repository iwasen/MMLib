// MMLibTextView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "MMLibTextView.h"
#include "MMLibDoc.h"
#include "MainFrm.h"
#include "TextSearchDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMMLibTextView

IMPLEMENT_DYNCREATE(CMMLibTextView, CEditView)

CMMLibTextView::CMMLibTextView()
{
	m_pCurrentItem = NULL;
}

BEGIN_MESSAGE_MAP(CMMLibTextView, CEditView)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_MESSAGE(WM_GETSELECTEDITEM, OnGetSelectedItem)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMMLibTextView 描画

void CMMLibTextView::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CMMLibTextView メッセージ ハンドラ

BOOL CMMLibTextView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | WS_VSCROLL;
	if (theApp.GetProfileInt(g_sSectionSettings, "TextWordwrap", 0) == 0)
		cs.style |= ES_AUTOHSCROLL | WS_HSCROLL;

	return CEditView::PreCreateWindow(cs);
}

int CMMLibTextView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_oCreateStruct = *lpCreateStruct;

	m_oDropTarget.Register(this);

	return 0;
}

void CMMLibTextView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
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
	case VIEWHINT_UPDATEWINDOW:
		DrawItems();
		break;
	case VIEWHINT_SETFONT:
		SetViewFont();
		break;
	case VIEWHINT_SAVEDATA:
		SaveData((CItem *)pHint);
		break;
	case VIEWHINT_TEXTFILE:
		LoadTextFile((CStdioFile *)pHint);
		break;
	case VIEWHINT_WORDWRAP:
		SetTextWordwrap();
		break;
	case VIEWHINT_GETSEARCHCOUNT:
		((CTextSearchParams *)pHint)->m_nCount = GetSearchCount(((CTextSearchParams *)pHint)->m_sSearchText);
		break;
	case VIEWHINT_SETSEL:
		SetSel(((CTextSearchParams *)pHint)->m_sSearchText, ((CTextSearchParams *)pHint)->m_nCount);
		break;
	case VIEWHINT_RESETSEL:
		ResetSel();
		break;
	}
}

void CMMLibTextView::SaveData(CItem *pItem)
{
	if (pItem != NULL && GetEditCtrl().GetModify()) {
		CItemFolder *pItemFolder;
		CItemData *pItemData;

		CString sText;
		GetWindowText(sText);

		switch (pItem->m_nItemType) {
		case ITEM_FOLDER:
			pItemFolder = (CItemFolder *)pItem;
			pItemFolder->m_pPcsDB->WriteTextFolderEx(pItemFolder->m_oFolderRec.sFolderID, &sText);
			break;
		case ITEM_DATA:
			pItemData = (CItemData *)pItem;
			pItemData->m_pPcsDB->WriteTextDataEx(pItemData->m_oDataRec.sDataID, sText);
			break;
		}
	}
}

void CMMLibTextView::DrawItems()
{
	if (HTREEITEM hItem = g_pTreeCtrl->GetSelectedItem(); hItem != NULL) {
		m_pCurrentItem = (CItem *)g_pTreeCtrl->GetItemData(hItem);
		switch (m_pCurrentItem->m_nItemType) {
		case ITEM_FOLDER:
			DrawFolderText();
			break;
		case ITEM_DATA:
			DrawDataText();
			break;
		default:
			DrawOtherText();
			break;
		}
	}

	GetEditCtrl().SetModify(FALSE);

	((CMainFrame *)::AfxGetMainWnd())->ResetTextSearch();
}

void CMMLibTextView::DrawFolderText()
{
	CItemFolder *pItemFolder = (CItemFolder *)m_pCurrentItem;
	CString sText;

	EnableWindow(TRUE);
	pItemFolder->m_pPcsDB->ReadTextFolderEx(pItemFolder->m_oFolderRec.sFolderID, &sText);
	SetWindowText(sText);
}

void CMMLibTextView::DrawDataText()
{
	CItemData *pItemData = (CItemData *)m_pCurrentItem;
	CString sText;

	EnableWindow(TRUE);
	pItemData->m_pPcsDB->ReadTextDataEx(pItemData->m_oDataRec.sDataID, &sText);
	SetWindowText(sText);
}

void CMMLibTextView::DrawOtherText()
{
	EnableWindow(FALSE);
	SetWindowText("");

	((CMainFrame *)::AfxGetMainWnd())->CloseTextSearchDlg();
}

void CMMLibTextView::SetViewFont()
{
	m_oFont.DeleteObject();
	m_oFont.CreatePointFont(g_nViewFontSize, g_sViewFontName);
	SetFont(&m_oFont);
}

void CMMLibTextView::LoadTextFile(CStdioFile *pFile)
{
	if (IsWindowEnabled()) {
		CString sText, sLine;
		while (pFile->ReadString(sLine)) {
			sText += sLine;
			sText += "\r\n";
		}

		SetWindowText(sText);

		GetEditCtrl().SetModify();
		GetDocument()->UpdateAllViews(NULL, VIEWHINT_MODIFYDATA);
	}
}

DROPEFFECT CMMLibTextView::OnDragOver(COleDataObject* pDataObject, DWORD /*dwKeyState*/, CPoint /*point*/)
{
	if (IsWindowEnabled() && pDataObject->IsDataAvailable(CF_HDROP) && !pDataObject->IsDataAvailable((CLIPFORMAT)g_nCFMMLibrary))
		return DROPEFFECT_COPY;
	else
		return DROPEFFECT_NONE;
}

BOOL CMMLibTextView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	if (pDataObject->IsDataAvailable(CF_HDROP))
		return DropFile(pDataObject, dropEffect, point);
	else
		return FALSE;
}

BOOL CMMLibTextView::DropFile(COleDataObject* pDataObject, DROPEFFECT /*dropEffect*/, CPoint /*point*/)
{
	HDROP hDrop;
	if ((hDrop = (HDROP)pDataObject->GetGlobalData(CF_HDROP, NULL)) == NULL)
		return FALSE;

	const int nNumFiles = ::DragQueryFile(hDrop, (UINT)-1, NULL, 0);
	if (nNumFiles > 0) {
		char fileNameBuf[256];
		::DragQueryFile(hDrop, 0, fileNameBuf, sizeof(fileNameBuf));
		CStdioFile file;
		if (file.Open(fileNameBuf, CFile::modeRead | CFile::shareDenyNone)) {
			LoadTextFile(&file);
			file.Close();
		}
	}

	::GlobalFree(hDrop);

	return TRUE;
}

void CMMLibTextView::OnChange()
{
	GetDocument()->UpdateAllViews(NULL, VIEWHINT_MODIFYDATA);

	((CMainFrame *)::AfxGetMainWnd())->ResetTextSearch();
}

LRESULT CMMLibTextView::OnGetSelectedItem(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return (LRESULT)m_pCurrentItem;
}

void CMMLibTextView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	SetViewFont();
}

void CMMLibTextView::SetTextWordwrap()
{
	CMMLibDoc *pDoc = (CMMLibDoc *)GetDocument();

	// WRAPモードを設定する（ウィンドウ再作成）。

	// 古い情報を取り出す。
	int	bufLen = GetWindowTextLength();	// エディトバッファ内の文字数。NULLを含まない
	BYTE *pSaveText = new BYTE[bufLen+1];	// セーブエリアを確保
	GetWindowText((LPSTR)pSaveText, bufLen+1);	// エディットバッファ内のデータをコピー

	// WRAPスタイルを設定する。
	DWORD dwStyle;
	if (pDoc->m_bTextWordwrap) {
		dwStyle = GetStyle() & ~(ES_AUTOHSCROLL | WS_HSCROLL);
	} else {
		dwStyle = GetStyle() | (ES_AUTOHSCROLL | WS_HSCROLL);
	}

	// 現在のウィンドウの位置を取得する。
	CWnd* pParent = GetParent();	// 親ウィンドウのハンドラを取得
	CRect rect;
	GetWindowRect(rect);		// ウィンドウの位置とサイズを取得
	pParent->ScreenToClient(rect);	// 座標を親ウィンドウとの相対位置に変換
	CWnd* pFocus = GetFocus();	// 入力のフォーカスを持つウィンドウを取得

	const UINT nID = GetDlgCtrlID();	// ウィンドウの子ウィンドウの識別子

	// 新たにウィンドウを作成する。
	const DWORD dwExStyle = GetExStyle();	// 拡張スタイルを取得
	HWND hWnd =
		::CreateWindowEx(dwExStyle,	// 拡張ウィンドウスタイル
				"Edit",// クラス名
				NULL,	// ウィンドウの名称
				dwStyle,	// スタイル
				rect.left,	// x 座標
				rect.top,	// y 座標
				rect.Width(),	// 幅
				rect.Height(),	// 高さ
				pParent->m_hWnd,	// 親ウィンドウハンドラ
				(HMENU)(INT_PTR)nID,		// 子ウィンドウ識別子
				::AfxGetInstanceHandle(),	// アプリケーションインスタンス
				m_oCreateStruct.lpCreateParams);	// ウィンドウ作成データ

	if (hWnd == NULL)	// ウィンドウ作成に失敗した場合
	{
		delete[] pSaveText;
		return;
	}

	m_oDropTarget.Revoke();

	// 新ウィンドウのフォントを再設定
	::SendMessage(hWnd, WM_SETFONT, (WPARAM)m_oFont.m_hObject, 0);

	// 旧ウィンドウのエディットバッファをクリア
	SetWindowText(NULL);

	// 新しいウィンドウのエディットバッファにテキストをセット
	::SetWindowText(hWnd, (LPSTR)pSaveText);
	delete[] pSaveText;

	// 旧ウィンドウを切り離す
	SetDlgCtrlID(nID+1);	// 子ウィンドウ識別子を変更
	HWND hWndOld = Detach();// ウィンドウからC++のオブジェクトを切り離す

	// 新ウィンドウを取り付ける
	::SetWindowLongPtr(hWndOld, GWLP_WNDPROC, (LONG_PTR)*GetSuperWndProcAddr());
	SubclassWindow(hWnd);

	// 旧ウィンドウを破棄する
	::DestroyWindow(hWndOld);

	// その他の状態設定
	GetEditCtrl().LimitText(nMaxSize);	// エディット可能サイズ
	if (pFocus == this) {
		SetFocus();
	}

	m_oDropTarget.Register(this);
}

int CMMLibTextView::GetSearchCount(const CString &sSearchText)
{
	CString sText;
	GetWindowText(sText);

	CStringW sTextW;
	sTextW = sText;

	CStringWList lSearchTextW;
	SplitSearchText(sSearchText, lSearchTextW);

	int nCount = 0;
	int nPosText = -1;
	while (TRUE) {
		int nPosMin = INT_MAX;
		POSITION nPosSearch = lSearchTextW.GetHeadPosition();
		while (nPosSearch != NULL) {
			const int nPos = sTextW.Find(lSearchTextW.GetNext(nPosSearch), nPosText + 1);
			if (nPos != -1) {
				if (nPos < nPosMin) {
					nPosMin = nPos;
				}
			}
		}
		if (nPosMin == INT_MAX)
			break;

		nPosSearch = lSearchTextW.GetHeadPosition();
		while (nPosSearch != NULL) {
			const int nPos = sTextW.Find(lSearchTextW.GetNext(nPosSearch), nPosText + 1);
			if (nPos == nPosMin) {
				nCount++;
			}
		}

		nPosText = nPosMin;
	}

	ResetSel();

	return nCount;
}

void CMMLibTextView::SetSel(const CString &sSearchText, int nCount)
{
	CString sText;
	GetWindowText(sText);

	CStringW sTextW;
	sTextW = sText;

	CStringWList lSearchTextW;
	SplitSearchText(sSearchText, lSearchTextW);

	int nCount2 = 0;
	int nPosText = -1;
	while (TRUE) {
		int nPosMin = INT_MAX;
		POSITION nPosSearch = lSearchTextW.GetHeadPosition();
		while (nPosSearch != NULL) {
			CStringW &sSearchTextW = lSearchTextW.GetNext(nPosSearch);
			const int nPos = sTextW.Find(sSearchTextW, nPosText + 1);
			if (nPos != -1) {
				if (nPos < nPosMin) {
					nPosMin = nPos;
				}
			}
		}
		if (nPosMin == INT_MAX)
			break;

		nPosSearch = lSearchTextW.GetHeadPosition();
		while (nPosSearch != NULL) {
			CStringW &sSearchTextW = lSearchTextW.GetNext(nPosSearch);
			const int nPos = sTextW.Find(sSearchTextW, nPosText + 1);
			if (nPos == nPosMin) {
				if (nCount == nCount2) {
					const int nLen = sSearchTextW.GetLength();
					GetEditCtrl().SetSel(nPos, nPos + nLen);
					return;
				}
				nCount2++;
			}
		}

		nPosText = nPosMin;
	}
}

void CMMLibTextView::ResetSel()
{
	GetEditCtrl().SetSel((DWORD)-1, 0, TRUE);
}

void CMMLibTextView::SplitSearchText(const CString &sSearchText, CStringWList &lSearchText)
{
	CStringArray aText;
	StrSplit(sSearchText, " ", aText);

	CStringW sStrW;
	for (int i = 0; i < aText.GetSize(); i++) {
		sStrW = aText[i].Trim();
		lSearchText.AddTail(sStrW);
	}
}
