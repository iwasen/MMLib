#pragma once

// PermissionDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPermissionDlg ダイアログ

class CPermissionDlg : public CDialogAF
{
public:
	CPermissionDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_PERMISSION };
	CComboBox	m_cLibrary;
	int		m_iPermission;

	CItemFolder *m_pItemFolder;
	DWORD m_dwPermission;

protected:
	void SetRadioButton();
	void GetRadioButton();

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeLibrary();
};
