#pragma once

#include "Item.h"
#include "afxwin.h"

// COutputImagesDlg ダイアログ

class COutputImagesDlg : public CDialogAF
{
	DECLARE_DYNAMIC(COutputImagesDlg)

public:
	COutputImagesDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_OUTPUTIMAGESDLG };

	CItemFolder *m_pItemFolder;
	CString m_sOutputFolder;

protected:
	CProgressCtrl m_cProgressCtrl;
	CStatic m_cStaticProgress;

	CMMPcsDB *m_pPcsDB;
	BOOL m_bCancel;
	int m_nTotalCount;
	int m_nOutputCount;
	int m_nPercent;

	static UINT ThreadProc(LPVOID pParam);
	void Output();
	BOOL CountFolder(const FOLDERREC &oFolderRec);
	BOOL CountImages(const FOLDERREC &oFolderRec);
	BOOL OutputFolder(const CString &sParentFolder, const FOLDERREC &oFolderRec);
	BOOL OutputImages(const CString &sParentFolder, const FOLDERREC &oFolderRec);
	void OutputImage(const CString &sParentFolder, const DATAREC &oDataRec, int nPage, int nPageNum);
	static int CompareProc(const void *p1, const void *p2);

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg LRESULT OnEndDialog(WPARAM wParam, LPARAM lParam);
};
