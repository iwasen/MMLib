// OptFont.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "OptFont.h"

/////////////////////////////////////////////////////////////////////////////
// COptFont プロパティ ページ

COptFont::COptFont() : CPropertyPage(COptFont::IDD)
{
	m_iFontSize = -1;
	m_iViewFontSize = 0;

	m_bChangeViewFont = FALSE;
}

void COptFont::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_NORMAL_FONT, m_iFontSize);
	DDX_Text(pDX, IDC_VIEW_FONT_SIZE, m_iViewFontSize);
	DDX_Text(pDX, IDC_VIEW_FONT_NAME, m_sViewFontName);
}

BEGIN_MESSAGE_MAP(COptFont, CPropertyPage)
	ON_BN_CLICKED(IDC_VIEW_FONT_CHANGE, OnViewFontChange)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptFont メッセージ ハンドラ

BOOL COptFont::OnInitDialog()
{
	m_sViewFontName = g_sViewFontName;
	m_iViewFontSize = g_nViewFontSize / 10;

	m_iFontSize = g_nFontSize;

	CPropertyPage::OnInitDialog();

	return TRUE;
}

void COptFont::OnOK()
{
	CWinApp *pWinApp = ::AfxGetApp();

	UpdateData(TRUE);

	if (m_bChangeViewFont) {
		g_sViewFontName = m_sViewFontName;
		pWinApp->WriteProfileString(g_sSectionOptions, "ViewFontName", g_sViewFontName);

		g_nViewFontSize = m_iViewFontSize * 10;
		pWinApp->WriteProfileInt(g_sSectionOptions, "ViewFontSize", g_nViewFontSize);
	}

	g_nFontSize = m_iFontSize;
	pWinApp->WriteProfileInt(g_sSectionOptions, "FontSize", m_iFontSize);

	CPropertyPage::OnOK();
}

void COptFont::OnViewFontChange()
{
	LOGFONT lf{};
	char szStyle[LF_FACESIZE];

	CFontDialog	fontDialog(&lf, 0, NULL, this);
	fontDialog.m_cf.lStructSize = sizeof(CHOOSEFONT);
	fontDialog.m_cf.hwndOwner = m_hWnd;
	fontDialog.m_cf.lpLogFont = &lf;
	fontDialog.m_cf.lpszStyle = szStyle;
	fontDialog.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;

	lstrcpy(fontDialog.m_cf.lpLogFont->lfFaceName, g_sViewFontName);
	HDC hDC = ::GetDC(m_hWnd);
	fontDialog.m_cf.lpLogFont->lfHeight = -MulDiv(g_nViewFontSize, GetDeviceCaps(hDC, LOGPIXELSY), 720);
	::ReleaseDC(m_hWnd, hDC);

	if (fontDialog.DoModal() == IDOK) {
		m_sViewFontName = fontDialog.GetFaceName();
		m_iViewFontSize = fontDialog.GetSize() / 10;
		m_bChangeViewFont = TRUE;

		UpdateData(FALSE);
	}
}

void COptFont::OnDefault()
{
	m_sViewFontName.LoadString(IDS_DEFAULT_FONT);
	m_iViewFontSize = 9;
	m_iFontSize = 0;
	m_bChangeViewFont = TRUE;

	UpdateData(FALSE);
}
