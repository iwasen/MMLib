#pragma once

// OptFolder.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// COptFolder ダイアログ

class COptFolder : public CPropertyPage
{
public:
	COptFolder();

	enum { IDD = IDD_OPT_FOLDER };

	CString	m_sBackupFolder;
	CString	m_sCacheFolder;
	CString	m_sLibraryFolder;
	CString	m_sInboxFolder;
	CString	m_sKeywordFolder;
	CString m_sAccessLogFolder;

protected:
	void SelectFolder(CString &sFolder, int nTitleId);

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnLibraryReference();
	afx_msg void OnCacheReference();
	afx_msg void OnBackupReference();
	afx_msg void OnDefault();
	afx_msg void OnInboxReference();
	afx_msg void OnKeywordReference();
	afx_msg void OnAccessLogReference();
};
