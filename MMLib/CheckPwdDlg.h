#pragma once

// CheckPwdDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CCheckPwdDlg ダイアログ

class CCheckPwdDlg : public CDialogAF
{
public:
	CCheckPwdDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_CHECK_PWD };
	CString	m_sLibrary;
	CString	m_sPassword;

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
};
