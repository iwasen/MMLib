#pragma once
#include "afxwin.h"

// CMakePictIndex ダイアログ

class CMakePictIndex : public CDialogAF
{
public:
	CMakePictIndex(CWnd* pParent = NULL);   // 標準コンストラクタ

	enum { IDD = IDD_MAKE_PICT_INDEX };

	CMMPcsDB *m_pPcsDB;

protected:
	CProgressCtrl m_cProgressBar;
	CButton m_cOK;

	BOOL m_bCancel;

	void MakePictIndex();

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnOK();
	virtual void OnCancel();
};
