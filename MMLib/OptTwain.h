#pragma once

// OptTwain.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// COptTwain ダイアログ

class COptTwain : public CPropertyPage
{
public:
	COptTwain();

	enum { IDD = IDD_OPT_TWAIN };

	CString	m_sTwainDriver;

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectDriver();
};
