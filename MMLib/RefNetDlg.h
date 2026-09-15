#pragma once

// RefNetDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRefNetDlg ダイアログ

class CRefNetDlg : public CDialogAF
{
public:
	CRefNetDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_REF_NET };

	CString m_sServerName;
	int m_nChType;
	CString m_sDirectory;

protected:
	CListBox	m_cLibList;
	int m_nItem;
	CString m_sPath[20];

	BOOL SetLibList();
	void AddRefList(CPcserve &oPcserve, CString &sDirName);

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkLiblist();
};
