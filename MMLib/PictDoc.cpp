// PictDoc.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "PictDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CPictDoc

IMPLEMENT_DYNCREATE(CPictDoc, CDocument)

BOOL CPictDoc::OnNewDocument()
{
	return TRUE;
}

CPictDoc::~CPictDoc()
{
	if (m_hDIB != NULL)
		MMPictureFree(m_hDIB);
}

BEGIN_MESSAGE_MAP(CPictDoc, CDocument)
	ON_COMMAND(IDM_PICT_AUTOSIZE, OnPictAutosize)
	ON_COMMAND(IDM_PICT_AUTOSIZE2, OnPictAutosize2)
	ON_COMMAND(IDM_PICT_ENLARGE, OnPictEnlarge)
	ON_COMMAND(IDM_PICT_REDUCE, OnPictReduce)
	ON_COMMAND(IDM_PICT_LEFT_ROTATE, OnPictLeftRotate)
	ON_COMMAND(IDM_PICT_RIGHT_ROTATE, OnPictRightRotate)
	ON_UPDATE_COMMAND_UI(IDM_PICT_AUTOSIZE, OnUpdatePictAutosize)
	ON_UPDATE_COMMAND_UI(IDM_PICT_AUTOSIZE2, OnUpdatePictAutosize2)
	ON_COMMAND(IDM_CLOSE_PICT, OnClosePict)
	ON_CBN_SELCHANGE(IDW_COMBO, OnSelchangeCombo)
	ON_CBN_EDITUPDATE(IDW_COMBO, OnEditchangeCombo)
	ON_COMMAND(ID_FILE_PAGE_SETUP, &CPictDoc::OnFilePageSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPictDoc コマンド

BOOL CPictDoc::OnOpenDocument(LPCSTR /*lpszPathName*/)
{
	return TRUE;
}

void CPictDoc::SetPathName(LPCSTR /*lpszPathName*/, BOOL /*bAddToMRU*/)
{
}

void CPictDoc::ShowPicture(LPCSTR pTitle, HDIB hDIB, int nAutoSize, double fPictRatio)
{
	CPictFrame *pPictFrame;

	if ((pPictFrame = GetPictFrame()) != NULL)
		pPictFrame->SetWindowText(pTitle);

	m_hDIB = hDIB;
	m_nAutoSize = nAutoSize;
	m_fPictRatio = fPictRatio;

	UpdateAllViews(NULL);
}

void CPictDoc::OnPictAutosize()
{
	m_nAutoSize = (m_nAutoSize == AUTOSIZE_BOTH ? AUTOSIZE_NONE : AUTOSIZE_BOTH);
	UpdateAllViews(NULL);
}

void CPictDoc::OnPictAutosize2()
{
	m_nAutoSize = (m_nAutoSize == AUTOSIZE_WIDTH ? AUTOSIZE_NONE : AUTOSIZE_WIDTH);
	UpdateAllViews(NULL);
}

void CPictDoc::OnPictEnlarge()
{
	m_nAutoSize = AUTOSIZE_NONE;
	m_fPictRatio *= 1.5;
	UpdateAllViews(NULL);
}

void CPictDoc::OnPictReduce()
{
	m_nAutoSize = AUTOSIZE_NONE;
	m_fPictRatio /= 1.5;
	UpdateAllViews(NULL);
}

void CPictDoc::OnPictLeftRotate()
{
	MMPictureRotate(m_hDIB, 3);
	UpdateAllViews(NULL);
}

void CPictDoc::OnPictRightRotate()
{
	MMPictureRotate(m_hDIB, 1);
	UpdateAllViews(NULL);
}

void CPictDoc::OnSelchangeCombo()
{
	CComboBox &comboBox = GetPictFrame()->GetComboBox();

	const int n = comboBox.GetCurSel();
	if (n != LB_ERR) {
		CString sStr;

		comboBox.GetLBText(n, sStr);
		comboBox.SetWindowText(sStr);
		m_nAutoSize = AUTOSIZE_NONE;
		m_fPictRatio = atof(sStr) / 100;
		UpdateAllViews(NULL);
	}
}

void CPictDoc::OnEditchangeCombo()
{
	CComboBox &comboBox = GetPictFrame()->GetComboBox();
	CString sStr1, sStr2;

	comboBox.GetWindowText(sStr1);
	sStr2 = sStr1.SpanIncluding("0123456789%");
	if (sStr1.GetLength() != sStr2.GetLength()) {
		comboBox.SetWindowText(sStr2);
		comboBox.SetEditSel(sStr2.GetLength(), sStr2.GetLength());
	}
}

CPictFrame *CPictDoc::GetPictFrame()
{
	CPictFrame *pPictFrame = NULL;

	POSITION pos = GetFirstViewPosition();
	if (pos != NULL) {
		CView *pView = GetNextView(pos);
		if (pView != NULL)
			pPictFrame = (CPictFrame *)pView->GetParent();
	}

	return pPictFrame;
}

BOOL CPictDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nID == 1 && nCode == 0) {
		CString sStr;
		m_nAutoSize = AUTOSIZE_NONE;
		GetPictFrame()->GetComboBox().GetWindowText(sStr);
		m_fPictRatio = atof(sStr) / 100;
		UpdateAllViews(NULL);
	}

	return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CPictDoc::OnUpdatePictAutosize(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAutoSize == AUTOSIZE_BOTH);
}

void CPictDoc::OnUpdatePictAutosize2(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAutoSize == AUTOSIZE_WIDTH);
}

void CPictDoc::OnClosePict()
{
	OnFileClose();
}

void CPictDoc::OnFilePageSetup()
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
