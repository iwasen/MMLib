#pragma once

// TransRegDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CTransRegDlg ダイアログ

class CTransRegDlg : public CDialogAF
{
public:
	CTransRegDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_TRANS_REG };
	CString	m_sAppFile;
	CString	m_sAppName;
	int		m_iMenuIcon;
	BOOL	m_bPictBMP;
	BOOL	m_bPictGIF;
	BOOL	m_bPictJPG;
	BOOL	m_bPictPNG;

protected:
	CListCtrl	m_cAppList;

	void SetAppList();

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDelete();
	afx_msg void OnClose();
	afx_msg void OnReference();
	afx_msg void OnItemchangedAppList(NMHDR* pNMHDR, LRESULT* pResult);
};
