#pragma once

// MakeIndexDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMakeIndexDlg ダイアログ

class CMakeIndexDlg : public CDialogAF
{
public:
	CMakeIndexDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_MAKE_INDEX };
	CListBox	m_cLibraryList;

	CDocument *m_pDocument;

protected:
	int SetLibraryListBox();
	BOOL CreateAllIndex();
	BOOL CreateKeywordIndex(CPcserve &oPcserve, CPSDB &oDB);
	BOOL WriteKeywordIndex(CItemFolder *pItemFolder, CPSDB &oDB);
	BOOL WriteSub(CPSDB *pDB, CPSDB &oDB, char type, LPCSTR pLibraryID, LPCSTR pLibraryName);

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAllSelect();
};
