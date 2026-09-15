#pragma once

// CPrintDlg ダイアログ

class CPrintDlg : public CPrintDialog
{
	DECLARE_DYNAMIC(CPrintDlg)

public:
	CPrintDlg(BOOL bPrintSetupOnly, DWORD dwFlags, CWnd* pParentWnd);   // 標準コンストラクタ

protected:
	CButton m_cMargin;

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
