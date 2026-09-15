#pragma once

// DataDlg.h : ヘッダー ファイル
//

#include "PictureWnd.h"
#include "LocalDll.h"

/////////////////////////////////////////////////////////////////////////////
// CDataDlg ダイアログ

class CDataDlg : public CDialogAF
{
public:
	CDataDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CDataDlg();

	enum { IDD = IDD_DATA_LARGE };

	CItemFolder *m_pItemFolder;
	CItemData *m_pItemData;
	CDocument *m_pDocument;
	CString m_sCaption;
	DATAREC m_oDataRec;
	const CString *m_pReadFileName;
	int m_nReadFileName;

protected:
	CMyButton	m_cPictWindow;
	CButton	m_cPictClipBoard;
	CButton	m_cCopyData;
	CComboBox	m_cAutoRotation;
	CButton	m_cAutoNumbering;
	CMyButton	m_cAutoSave;
	CButton	m_cPictErase;
	CButton	m_cCancel;
	CButton	m_cPictRotation;
	CPictureWnd	m_cPictPicture;
	CButton	m_cSave;
	CEdit	m_cTitle;
	CEdit	m_cRemark;
	CEdit	m_cKeyword;
	CEdit	m_cCreator;
	CEdit	m_cCode;
	CString	m_sCode;
	CString	m_sCreator;
	CString	m_sPictColor;
	CString	m_sPictDPI;
	CString	m_sPictHeight;
	CString	m_sPictWidth;
	CString	m_sRemark;
	CString	m_sKeyword;
	CString	m_sTitle;

	HANDLE	m_hDIB;
	int m_nDPI;
	BOOL m_bUpdateData;
	BOOL m_bUpdatePict;
	BOOL m_bUpdateSound;
	BOOL m_bUpdateText;
	CString m_sCreateDate;
	CString m_sTextData;
	int m_nReadCount;
	HWND m_hWndNext;
	HWND m_hWndSel;

	void NextReadFile();
	void PictErase();
	void GetPicture(int nMode);
	void SetPicture(HDIB hDIB, const SCAN_IMAGE_INFO *pImageInfo);
	void GetImageInfo(HDIB hDIB, SCAN_IMAGE_INFO *pImageInfo);
	BOOL CheckColorMode(HDIB hDIB);
	void ExitDialog();
	void SetUpdateData();
	void ResetUpdateData();
	void EnableButton();
	void AutoNumbering(BOOL nNext);
	BOOL CheckClipBoard();
	HDIB GetDIBFromBmp(HBITMAP hBmp);
	void DrawWindowFrame(CWnd *pWnd);
	HDIB GetWindowDIB(CWnd *pWnd);
	CWnd *GetTopParentWindow(CWnd *pWnd);
	BOOL GetWindowTitle(CWnd *pWnd, CString &sTitle);

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnSave();
	afx_msg void OnPictCamera();
	afx_msg void OnPictScanner();
	afx_msg void OnPictFile();
	afx_msg void OnPictRotation();
	afx_msg void OnChangeTitle();
	afx_msg void OnChangeCode();
	afx_msg void OnChangeCreator();
	afx_msg void OnChangeRemark();
	afx_msg void OnChangeKeyword();
	afx_msg void OnPictErase();
	afx_msg void OnAutoNumbering();
	afx_msg void OnAutoSave();
	afx_msg void OnTextData();
	afx_msg void OnCopyData();
	afx_msg void OnPictClipboard();
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnFrameBmp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScanBmp(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDrawClipboard();
	afx_msg void OnChangeCbChain(HWND hWndRemove, HWND hWndAfter);
	afx_msg LRESULT OnPictWindow(WPARAM wParam, LPARAM lParam);
};
