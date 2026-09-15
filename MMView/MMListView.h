#pragma once

// MMListView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMMListView ビュー

class CMMListView : public CListView
{
protected:
	CMMListView();           // 動的生成に使用されるプロテクト コンストラクタ
	DECLARE_DYNCREATE(CMMListView)

	virtual ~CMMListView();

	int m_nColumn;
	BOOL m_bDetailFlag[DETAIL_NUM];
	int m_nDetailWidth[DETAIL_NUM];
	int m_nSortItem;
	int m_nSortDir;

	void DrawItems();
	void InsertColumns();
	static int CALLBACK CompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDetailCdate();
	afx_msg void OnUpdateDetailCdate(CCmdUI* pCmdUI);
	afx_msg void OnDetailCode();
	afx_msg void OnUpdateDetailCode(CCmdUI* pCmdUI);
	afx_msg void OnDetailCreator();
	afx_msg void OnUpdateDetailCreator(CCmdUI* pCmdUI);
	afx_msg void OnDetailName();
	afx_msg void OnUpdateDetailName(CCmdUI* pCmdUI);
	afx_msg void OnDetailRemark1();
	afx_msg void OnUpdateDetailRemark1(CCmdUI* pCmdUI);
	afx_msg void OnDetailRemark2();
	afx_msg void OnUpdateDetailRemark2(CCmdUI* pCmdUI);
	afx_msg void OnDetailUdate();
	afx_msg void OnUpdateDetailUdate(CCmdUI* pCmdUI);
};
