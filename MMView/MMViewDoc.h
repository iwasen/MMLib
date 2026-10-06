// MMViewDoc.h : CMMViewDoc クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMMViewDoc : public CDocument
{
protected: // シリアライズ機能のみから作成します。
	CMMViewDoc();
	DECLARE_DYNCREATE(CMMViewDoc)

public:
	virtual ~CMMViewDoc();

	int m_nViewMode;
	int m_nMaxPage;
	int m_nCurrentPage;
	CMmlRead *m_pMMFile;
	CItem *m_pItem;
	CCriticalSection m_oCriticalSection1;
	CString m_sText;
	CFont m_oFont;

	CItem *GetCurrentItem() {return &m_pItem[m_nCurrentPage];}
	void ReadHeader();
	void ReadPage(int nPage);
	void ReadAllPages();
	void DispStatus();
	void ShowPicture(int nPage);

protected:
	CCriticalSection m_oCriticalSection2;
	CString m_sTitle;
	CString m_sFontName;
	int m_nFontSize;

	void CloseFile();
	void ReadCurrentPage();
	void SetStatusInfo(LPCSTR pInfoMsg);
	static UINT ReadThread(LPVOID pParam);
	void CreateReadThread();

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCSTR lpszPathName);
	afx_msg void OnPictNext();
	afx_msg void OnUpdatePictNext(CCmdUI* pCmdUI);
	afx_msg void OnPictPrev();
	afx_msg void OnUpdatePictPrev(CCmdUI* pCmdUI);
	afx_msg void OnViewList();
	afx_msg void OnViewPict();
	afx_msg void OnViewIcon();
	afx_msg void OnUpdateViewPict(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewIcon(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewList(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnSetfont();
	afx_msg void OnFilePageSetup();
};
