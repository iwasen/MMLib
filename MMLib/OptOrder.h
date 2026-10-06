#pragma once

// OptOrder.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// COptOrder ダイアログ

class COptOrder : public CPropertyPage
{
public:
	COptOrder();

	enum { IDD = IDD_OPT_ORDER };

	BOOL m_bChangeOrder;
	BOOL m_bChangeText;
	BOOL m_bChangeSmallSize;

protected:
	CListBox	m_cOrderList;
	int		m_iViewText;
	int		m_iSmallSize;

	CString m_sOrder;
	int m_nViewText;
	int m_nSmallSize;

	void SetListBox(LPCSTR pOrder);

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnDefault();
};
