#pragma once

// FindDlg.h : ヘッダー ファイル
//

#include "Item.h"
#include "MyCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CFindDlg ダイアログ

class CFindDlg : public CDialogAF
{
public:
	CFindDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_FIND };
	BOOL m_bUpdateMode;
	int m_nFindType;
	CItem *m_pItemParent;
	FIND_CONDITION m_aFindConditions[N_FIND_CONDITIONS];
	BOOL m_bSave;
	CString m_sFindFolder;
	CDocument *m_pDocument;

protected:
	CEdit	m_cText;
	CEdit	m_cCode;
	CMyEdit	m_cFindNum;
	CMyDate	m_cUpdateDateTo;
	CMyDate	m_cUpdateDateFrom;
	CMyDate	m_cCreateDateTo;
	CMyDate	m_cCreateDateFrom;
	CTabCtrl	m_cConditionTab;
	CComboBox	m_cLibrary;
	CString	m_sCode;
	CString	m_sCreateDateBefore;
	CString	m_sCreator;
	CString	m_sRemark;
	CString	m_sKeyword;
	CString	m_sTitle;
	CString	m_sUpdateDateBefore;
	BOOL	m_bNoCase;
	int		m_iMatching;
	int		m_iFindType;
	CString	m_sText;

	void GetFindCondition();
	void SetFindCondition();
	void SetLibraryCombo();
	long GetFindCount(const CItemFolder *pItemFolder);

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangingConditionTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeConditionTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetfindnum();
	afx_msg void OnFolder();
	afx_msg void OnData();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
