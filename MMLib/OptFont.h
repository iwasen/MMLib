#pragma once

// OptFont.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// COptFont ダイアログ

class COptFont : public CPropertyPage
{
public:
	COptFont();

	enum { IDD = IDD_OPT_FONT };

	int		m_iFontSize;
	int		m_iViewFontSize;
	CString	m_sViewFontName;
	BOOL m_bChangeViewFont;

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnViewFontChange();
	afx_msg void OnDefault();
};
