#pragma once

// TextDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CTextDlg ダイアログ

class CTextDlg : public CDialogAF
{
public:
	CTextDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_TEXT };

	CString	m_sText;

protected:
	CEdit	m_cText;
	CFont m_oFont;
	CString m_sFontName;
	int m_nFontSize;

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	afx_msg void OnFont();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCloseText();
};
