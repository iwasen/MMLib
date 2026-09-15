#pragma once

// MailAttDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMailAttDlg ダイアログ

class CMailAttDlg : public CDialogAF
{
public:
	CMailAttDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_MAIL_ATT };

protected:
	CMyEdit	m_cAttVPixel;
	CMyButton	m_cAttVLimit;
	CMyEdit	m_cAttScale;
	CMyEdit	m_cAttHPixel;
	CMyButton	m_cAttHLimit;
	int		m_iAttActual;

	void EnableControls();

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAttActual();
	afx_msg void OnAttReduction();
	afx_msg void OnAttHlimit();
	afx_msg void OnAttVlimit();
};
