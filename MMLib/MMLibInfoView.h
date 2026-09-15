#pragma once
#include "afxwin.h"

// MMLibInfoView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMMLibInfoView フォーム ビュー

class CMMLibInfoView : public CFormView
{
protected:
	CMMLibInfoView();           // 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CMMLibInfoView)

	enum { IDD = IDD_INFO_VIEW };

	CMyDate	m_cUpdateDate;
	CMyDate	m_cCreateDate;
	CMyEdit	m_cCode;
	CButton	m_cUpdateInfo;
	CMyEdit	m_cTitle;
	CMyEdit	m_cRemark;
	CMyEdit	m_cKeyword;
	CMyEdit	m_cCreator;
	CButton m_cButtonOCR;
	CStatic m_cStaticDataCount;
	CMyEdit m_cEditDataCount;
	CButton m_cCheckSoundPlayback;
	CButton m_cCheckSoundRecord;
	CButton m_cButtonSoundText;
	CButton m_cButtonSoundFile;

	CItem *m_pCurrentItem;
	int m_nFormHeight;

	void CheckSave();
	void DrawItem();
	void DrawRootInfo();
	void DrawFolderInfo();
	void DrawDataInfo();
	void DrawFindInfo();
	void DrawInboxInfo();
	void DrawOtherInfo();
	void SaveData(CItem *pItem);
	void SaveFolderInfo(CItemFolder *pItemFolder);
	void SaveDataInfo(CItemData *pItemData);
	void SaveFindInfo(CItemFind *pItemFind);
	void ModifyData();
	CString GetDataCount(CMMPcsDB *pPcsDB);

	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateInfo();
	afx_msg void OnChangeCode();
	afx_msg void OnChangeTitle();
	afx_msg void OnChangeCreator();
	afx_msg void OnChangeRemark();
	afx_msg void OnChangeKeyword();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg LRESULT OnGetSelectedItem(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonOcr();
	afx_msg LRESULT OnMmdbNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedCheckSoundPlayback();
	afx_msg void OnBnClickedCheckSoundRecord();
	afx_msg void OnBnClickedButtonSoundFile();
	afx_msg void OnBnClickedButtonSoundText();
};
