#pragma once

// PasswordDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg ダイアログ

class CPasswordDlg : public CDialogAF
{
public:
	CPasswordDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_PASSWORD };
	CString	m_sPassword;
	CString	m_sPassword2;

	CItemFolder *m_pItemFolder;

protected:
	CComboBox	m_cLibrary;

	void SetPassword();

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeLibrary();
};
