#pragma once

// LibNewDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CLibNewDlg ダイアログ

class CLibNewDlg : public CDialogAF
{
public:
	CLibNewDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_LIBNEW };
	CString	m_sCreator;
	CString	m_sDirectory;
	CString	m_sLibName;
	int		m_iChType;
	CString	m_sServerName;
	CString	m_sRemark;
	CString	m_sKeyword;
	BOOL	m_bCache;

protected:
	CStatic	m_cSeparator;
	CButton	m_cExternal;
	CButton	m_cReference;
	CComboBox	m_cServerName;
	CComboBox	m_cDirectory;
	CButton	m_cCache;

	CRect m_rectDlg;

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnLocal();
	afx_msg void OnTcpip();
	afx_msg void OnNetbios();
	afx_msg void OnReference();
	afx_msg void OnExternal();
};
