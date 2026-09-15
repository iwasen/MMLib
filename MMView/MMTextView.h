#pragma once

// MMTextView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMMTextView ビュー

class CMMTextView : public CEditView
{
protected:
	DECLARE_DYNCREATE(CMMTextView)

	void DrawText();

	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
};
