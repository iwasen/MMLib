#pragma once

// MMLibTextView.h : ヘッダー ファイル
//

typedef CList<CStringW, CStringW &> CStringWList;

/////////////////////////////////////////////////////////////////////////////
// CMMLibTextView ビュー

class CMMLibTextView : public CEditView
{
protected:
	CMMLibTextView();           // 動的生成に使用されるプロテクト コンストラクタ
	DECLARE_DYNCREATE(CMMLibTextView)

	CItem *m_pCurrentItem;
	CFont m_oFont;
	COleDropTarget m_oDropTarget;
	CREATESTRUCT m_oCreateStruct;

	void SaveData(CItem *pItem);
	void DrawItems();
	void DrawFolderText();
	void DrawDataText();
	void DrawOtherText();
	void SetViewFont();
	void LoadTextFile(CStdioFile *pFile);
	void SetTextWordwrap();
	BOOL DropFile(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	int GetSearchCount(const CString &sSearchText);
	void SetSel(const CString &sSearchText, int nCount);
	void ResetSel();
	void SplitSearchText(const CString &sSearchText, CStringWList &lSearchText);

	DECLARE_MESSAGE_MAP()
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnInitialUpdate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChange();
	afx_msg LRESULT OnGetSelectedItem(WPARAM wParam, LPARAM lParam);
};
