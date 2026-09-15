#pragma once

#include "DialogEx.h"

// CFindPictDlg ダイアログ

class CFindPictDlg : public CDialogEx2
{
public:
	CFindPictDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

	enum { IDD = IDD_FIND_PICT };

	const CItemData *m_pItemData;
	CDocument *m_pDocument;

protected:
	struct PICT_LIST {
		char did[LEN_ID + 1];
		double diff;
	};

	CListCtrl m_cListFind;

	CStringArray m_aDataID;
	BOOL m_nInit;

	void InsertColumns();
	void SetListData();
	static int Compare(const void *p1, const void *p2);
	void GetPictList(int nFactorIndex, CArray <PICT_LIST, PICT_LIST &> &aPictList);
	void GetFolder(const CString &sFolderId, CString &sFolder);

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonMakeIndex();
	afx_msg void OnLvnItemchangedListFind(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnBnClickedButtonNext();
};
