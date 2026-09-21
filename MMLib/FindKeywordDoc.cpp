// FindDoc.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "FindKeywordDoc.h"
#include "MakeIndexDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindKeywordDoc

IMPLEMENT_DYNCREATE(CFindKeywordDoc, CDocument)

CFindKeywordDoc::CFindKeywordDoc()
{
	m_nFindCount = 0;
}

BOOL CFindKeywordDoc::OnNewDocument()
{
	return TRUE;
}

BEGIN_MESSAGE_MAP(CFindKeywordDoc, CDocument)
	ON_COMMAND(IDM_CLOSE_FIND, OnCloseFind)
	ON_COMMAND(IDM_MAKE_INDEX, OnMakeIndex)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_PREV, OnUpdateViewPrev)
	ON_COMMAND(IDM_VIEW_PREV, OnViewPrev)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_NEXT, OnUpdateViewNext)
	ON_COMMAND(IDM_VIEW_NEXT, OnViewNext)
	ON_BN_CLICKED(IDC_FIND, OnFind)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindKeywordDoc コマンド

void CFindKeywordDoc::OnCloseFind()
{
	OnFileClose();
}

BOOL CFindKeywordDoc::OnOpenDocument(LPCTSTR /*lpszPathName*/)
{
	return TRUE;
}

void CFindKeywordDoc::OnMakeIndex()
{
	CMakeIndexDlg dlg;

	dlg.m_pDocument = m_pMMLibDoc;
	dlg.DoModal();

	UpdateAllViews(NULL, VIEWHINT_KEYWORDFIND);
}

void CFindKeywordDoc::OnFind()
{
	m_pFindFrame->m_wndKeyword.GetWindowText(m_sKeyword);
	if (m_sKeyword.IsEmpty()) {
		::AfxMessageBox(IDS_INPUT_KEYWORD);
		m_pFindFrame->m_wndKeyword.SetFocus();
	} else
		UpdateAllViews(NULL, VIEWHINT_KEYWORDFIND);
}

void CFindKeywordDoc::OnUpdateViewPrev(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nFindCount != 0);
}

void CFindKeywordDoc::OnViewPrev()
{
	UpdateAllViews(NULL, VIEWHINT_KEYWORDPREV);
}

void CFindKeywordDoc::OnUpdateViewNext(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nFindCount != 0);
}

void CFindKeywordDoc::OnViewNext()
{
	UpdateAllViews(NULL, VIEWHINT_KEYWORDNEXT);
}
