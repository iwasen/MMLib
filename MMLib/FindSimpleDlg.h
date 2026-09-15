#pragma once

// FindSimpleDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CFindSimpleDlg ダイアログ

class CFindSimpleDlg : public CDialogAF
{
public:
	CFindSimpleDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_FIND_SIMPLE };
	CString	m_sFindFolder;
	BOOL	m_bSave;
	CString	m_sSearchText;
	CDocument *m_pDocument;

	CItem *m_pItemParent;
	BOOL m_bUpdateMode;

protected:
	CButton	m_cFindExpert;
	CMyEdit	m_cFindNum;
	CEdit	m_cSearchText;
	CComboBox	m_cLibrary;

	void SetLibraryCombo();
	long GetFindCount(const CItemFolder *pItemFolder);

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnFindExpert();
	afx_msg void OnGetfindnum();
};
