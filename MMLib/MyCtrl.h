#pragma once

// MyCtrl.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMyDate ウィンドウ

class CMyDate : public CEdit
{
public:
	CString &operator=(CString &sParam);
	LPCTSTR operator=(LPCTSTR pParam);
	operator CString();

protected:
	BOOL GetDate(int &nYear, int &nMonth, int &nDay);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnUpdate();
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CMyEdit ウィンドウ

class CMyEdit : public CEdit
{
public:
	CMyEdit();

	int operator=(int nParam);
	long operator=(long nParam);
	double operator=(double fParam);
	CString &operator=(CString &sParam);
	LPCTSTR operator=(LPCTSTR pParam);
	CMyEdit &operator=(CMyEdit &oParam);
	int operator==(int nParam);
	int operator!=(int nParam);
	operator int();
	operator double();
	operator CString();
	void Format(LPCTSTR lpszFormat, ...);
	void Blank();
	void LoadString(UINT nStringID);
	void FormatMessage(UINT nStringID, ...);
	void SetTextColor(COLORREF color);

protected:
	CString m_sStr;
	BOOL m_bSetTextColor;
	CBrush m_oBrush;
	COLORREF m_nTextColor;

	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CMyButton ウィンドウ

class CMyButton : public CButton
{
public:
	CMyButton();

	BOOL m_bNotifyButtonDown;

	BOOL operator=(BOOL bParam);
	operator BOOL();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

class CMyToolBar : public CToolBar
{
public:
	void DrawGripper(CDC* pDC, const CRect& rect);
	void EraseNonClient();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoPaint(CDC* pDC);
	afx_msg void OnNcPaint();
};
