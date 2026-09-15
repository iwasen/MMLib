#pragma once

#include "DialogEx.h"

// CAccessLogDlg ダイアログ

class CAccessLogDlg : public CDialogEx2
{
public:
	CAccessLogDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_ACCESS_LOG };

	CDocument *m_pDocument;

protected:
	CListCtrl m_cListAccessLog;

	void InsertColumns();
	void SetListData();

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedListAccessLog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
};
