#pragma once

// AutoNumDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CAutoNumDlg ダイアログ

class CAutoNumDlg : public CDialogAF
{
public:
	CAutoNumDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_AUTO_NUMBERING };
	CEdit	m_cStep;
	CEdit	m_cCode;
	CString	m_sCode;
	long	m_lStep;
	BOOL	m_bAutoNumbering;

protected:
	void EnableControl();

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	afx_msg void OnAutoNumbering();
};
