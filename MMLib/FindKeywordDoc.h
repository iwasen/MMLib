#pragma once

// FindKeywordDoc.h : ヘッダー ファイル
//

#include "FindKeywordFrame.h"

/////////////////////////////////////////////////////////////////////////////
// CFindKeywordDoc ドキュメント

class CFindKeywordDoc : public CDocument
{
protected:
	CFindKeywordDoc();           // 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CFindKeywordDoc)

	CFindKeywordFrame *m_pFindFrame;
	CMMLibDoc *m_pMMLibDoc;
	CString m_sKeyword;
	int m_nFindCount;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnNewDocument();
	afx_msg void OnCloseFind();
	afx_msg void OnMakeIndex();
	afx_msg void OnFind();
	afx_msg void OnUpdateViewPrev(CCmdUI* pCmdUI);
	afx_msg void OnViewPrev();
	afx_msg void OnUpdateViewNext(CCmdUI* pCmdUI);
	afx_msg void OnViewNext();
};
