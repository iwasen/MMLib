#pragma once

#include "Item.h"

// CPropertyDlg ダイアログ

class CPropertyDlg : public CDialogAF
{
	DECLARE_DYNAMIC(CPropertyDlg)

public:
	CPropertyDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_PROPERTY };

	CItem *m_pItem;

protected:
	CString m_sEditName;
	CString m_sEditType;
	CString m_sEditLibrary;
	CString m_sEditFolder;
	CButton m_cButtonFind;

	void DispFolder();
	void DispData();
	void GetFolder(CMMPcsDB *pPcsDB, const CString &sFolderId, CString &sFolder);

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonFind();
};
