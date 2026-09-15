#pragma once

// LibOpenDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CLibOpenDlg ダイアログ

class CLibOpenDlg : public CDialogAF
{
public:
	CLibOpenDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_LIBOPEN };
	CString	m_sDirectory;
	int		m_iChType;
	CString	m_sServerName;
	BOOL	m_bCache;

protected:
	CListBox	m_cLibraryList;
	CButton	m_cExternal;
	CStatic	m_cSeparator;
	CComboBox	m_cDirectory;
	CComboBox	m_cServerName;
	CButton	m_cCache;

	CRect m_rectDlg;

	void SetLibraryList();
	void ReferenceLocal();
	void ReferenceNetwork();

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnTcpip();
	afx_msg void OnNetbios();
	afx_msg void OnLocal();
	afx_msg void OnReference();
	afx_msg void OnExternal();
	afx_msg void OnSelchangeLibraryList();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkLibraryList();
};
