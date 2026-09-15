#pragma once

// FindKeywordView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CFindKeywordView ビュー

class CFindKeywordView : public CListView
{
protected:
	DECLARE_DYNCREATE(CFindKeywordView)

	int m_nSortDir;
	int m_nSortItem;

	void InsertColumns();
	CString GetFilter(CString &sKeywords);
	BOOL FindKeyword(LPCTSTR pFilter);
	void DeleteAllItems();
	static int CALLBACK CompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void SelectPrev();
	void SelectNext();

	DECLARE_MESSAGE_MAP()
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
};
