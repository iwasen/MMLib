#pragma once

// PictRotateDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPictRotateDlg ダイアログ

class CPictRotateDlg : public CDialogAF
{
public:
	CPictRotateDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	enum { IDD = IDD_PICT_ROTATE };

	int m_nRotate;

protected:
	int		m_iRotate;

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};
