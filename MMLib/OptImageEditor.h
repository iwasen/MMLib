#pragma once

// OptImageEditor.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// COptImageEditor ダイアログ

class COptImageEditor : public CPropertyPage
{
public:
	COptImageEditor();

	enum { IDD = IDD_OPT_IMAGE_EDITOR };

	CStatic	m_cImageEditorIcon;
	CString	m_sImageEditor;
	BOOL	m_bEditDblClick;

protected:
	void DispIcon();

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnOK();
	afx_msg void OnSelectFile();
	virtual BOOL OnInitDialog();
};
