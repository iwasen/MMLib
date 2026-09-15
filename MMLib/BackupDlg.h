#pragma once

// BackupDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CBackupDlg ダイアログ

class CBackupDlg : public CDialogAF
{
public:
	CBackupDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_BACKUP };
	CButton	m_cDelete;
	CButton	m_cCancel;
	CListCtrl m_cBackupList;
	CComboBox m_cLibrary;
	CProgressCtrl m_cProgress;

	const CItemFolder *m_pItemFolder;

protected:
	int m_nSortItem;
	int m_nSortDir;

	void SetBackupList();
	static int CALLBACK CompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void RemoveBackupItem();

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDelete();
	afx_msg void OnColumnclickBackupList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
};
