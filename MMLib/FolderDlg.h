#pragma once

// FolderDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CFolderDlg ダイアログ

class CFolderDlg : public CDialogAF
{
public:
	CFolderDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_FOLDER };
	CButton	m_cCopyData;
	CString	m_sCreator;
	CString	m_sFolderName;
	CString	m_sRemark;
	CString	m_sKeyword;
	CString m_sCaption;
	CString m_sTextData;
	FOLDERREC m_oFolderRec;

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	afx_msg void OnTextData();
	afx_msg void OnCopyData();
};

/////////////////////////////////////////////////////////////////////////////
// CFolderNewDlg ダイアログ

class CFolderNewDlg : public CDialogAF
{
public:
	CFolderNewDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_FOLDERNEW };
	CButton	m_cCopyData;
	CString	m_sCreator;
	CString	m_sFolderName;
	int		m_iFolderType;
	CString	m_sRemark;
	CString	m_sKeyword;
	CString m_sTextData;
	FOLDERREC m_oFolderRec;

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnTextData();
	afx_msg void OnCopyData();
};
