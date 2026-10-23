// TextDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "TextDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CTextDlg ダイアログ

CTextDlg::CTextDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CTextDlg::IDD, pParent)
{
	CWinApp *pWinApp = ::AfxGetApp();
	m_sFontName = pWinApp->GetProfileString(g_sSectionSettings, "FontName", GetString(IDS_DEFAULT_FONT));
	m_nFontSize = pWinApp->GetProfileInt(g_sSectionSettings, "FontSize", 90);
	m_oFont.CreatePointFont(m_nFontSize, m_sFontName);
}

void CTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT, m_cText);
	DDX_Text(pDX, IDC_TEXT, m_sText);
}

BEGIN_MESSAGE_MAP(CTextDlg, CDialogAF)
	ON_COMMAND(IDM_FONT, OnFont)
	ON_WM_SIZE()
	ON_COMMAND(IDM_CLOSE_TEXT, OnCloseText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextDlg メッセージ ハンドラ

BOOL CTextDlg::OnInitDialog()
{
	CDialogAF::OnInitDialog();

	m_cText.SetFont(&m_oFont);
	m_cText.SetFocus();
	m_cText.SetSel(-1, 0);

	return FALSE;
}

void CTextDlg::OnFont()
{
	CWinApp *pWinApp = ::AfxGetApp();
	LOGFONT	lf{};
	char	szStyle[LF_FACESIZE];

	CFontDialog	fontDialog(&lf, 0, NULL, this);
	fontDialog.m_cf.lStructSize = sizeof(CHOOSEFONT);
	fontDialog.m_cf.hwndOwner = m_hWnd;
	fontDialog.m_cf.iPointSize = m_nFontSize;
	fontDialog.m_cf.lpLogFont = &lf;
	fontDialog.m_cf.lpszStyle = szStyle;
	fontDialog.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;

	lstrcpy(lf.lfFaceName, m_sFontName);
	lf.lfHeight = -MulDiv(m_nFontSize, GetDC()->GetDeviceCaps(LOGPIXELSY), 720);
	lf.lfCharSet = SHIFTJIS_CHARSET;

	if (fontDialog.DoModal()) {
		m_oFont.DeleteObject();
		m_oFont.CreateFontIndirect(&lf);
		m_cText.SetFont(&m_oFont, TRUE);

		m_sFontName = fontDialog.m_cf.lpLogFont->lfFaceName;
		m_nFontSize = fontDialog.m_cf.iPointSize;
		pWinApp->WriteProfileString(g_sSectionSettings, "FontName", m_sFontName);
		pWinApp->WriteProfileInt(g_sSectionSettings, "FontSize", m_nFontSize);
	}
}

void CTextDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogAF::OnSize(nType, cx, cy);

	if (m_cText.m_hWnd != NULL) {
		CRect rect;
		GetClientRect(rect);
		m_cText.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOMOVE);
	}
}

void CTextDlg::OnCloseText()
{
	UpdateData(TRUE);

	EndDialog(IDOK);
}
