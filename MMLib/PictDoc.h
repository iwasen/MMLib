#pragma once

// PictDoc.h : ヘッダー ファイル
//

#include "PictFrame.h"

/////////////////////////////////////////////////////////////////////////////
// CPictDoc ドキュメント

class CPictDoc : public CDocument
{
protected:
	DECLARE_DYNCREATE(CPictDoc)

public:
	virtual ~CPictDoc();

	HDIB m_hDIB;
	int m_nAutoSize;
	double m_fPictRatio;

	void ShowPicture(LPCTSTR pTitle, HDIB hDIB, int nAutoSize, double fPictRatio);

protected:
	CPictFrame *GetPictFrame();

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnNewDocument();
	afx_msg void OnPictAutosize();
	afx_msg void OnPictAutosize2();
	afx_msg void OnPictEnlarge();
	afx_msg void OnPictReduce();
	afx_msg void OnPictLeftRotate();
	afx_msg void OnPictRightRotate();
	afx_msg void OnUpdatePictAutosize(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePictAutosize2(CCmdUI* pCmdUI);
	afx_msg void OnClosePict();
	afx_msg void OnSelchangeCombo();
	afx_msg void OnEditchangeCombo();
	afx_msg void OnFilePageSetup();
};
