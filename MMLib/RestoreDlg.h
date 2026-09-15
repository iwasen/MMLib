#pragma once

// RestoreDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRestoreDlg ダイアログ

class CRestoreDlg : public CDialogAF
{
public:
	CRestoreDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_RESTORE };

protected:
	CButton	m_cCancel;
	CProgressCtrl	m_cProgress;
	CButton	m_cDelete;
	CListCtrl	m_cBackupList;
	int m_nSortItem;
	int m_nSortDir;

	void SetBackupList();
	static int CALLBACK CompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	CMMPcsDB *CreateRestoreLibrary();
	void RemoveBackupItem();

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnColumnclickBackupList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDelete();
	afx_msg void OnDestroy();
};
