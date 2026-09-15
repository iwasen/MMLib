#pragma once

#define N_SEARCH_HISTORY	10

// CTextSearchDlg ダイアログ

class CTextSearchDlg : public CDialogAF
{
	DECLARE_DYNAMIC(CTextSearchDlg)

public:
	CTextSearchDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_TEXT_SEARCH };

	CDocument *m_pDocument;

	void ResetTextSearch();

protected:
	CComboBox m_cComboSearch;
	CEdit m_cEditSearchResult;

	int m_nTotalCount;
	int	m_nCurrentCount;
	CStringList m_lSearchHistory;

	void GetSearchCount(CString &sSearchText);
	void DispSearchCount();
	void GetFindText(CString &sSearchText);
	void ReadHistoryIni();
	void WriteHistoryIni();
	void AddHistory(const CString &sSearchText);
	void SetSearchHistory();

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnCbnEditchangeComboSearch();
	afx_msg void OnCbnSelchangeComboSearch();

public:
	virtual BOOL DestroyWindow();
};

struct CTextSearchParams : public CObject
{
	CString m_sSearchText;
	int m_nCount;
};
