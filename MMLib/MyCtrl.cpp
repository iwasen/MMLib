// MyCtrl.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "MyCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CMyDate

BEGIN_MESSAGE_MAP(CMyDate, CEdit)
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDate メッセージ ハンドラ

CString &CMyDate::operator=(CString &sParam)
{
	CString sStr;

	if (!sParam.IsEmpty())
		sStr.Format("%s/%s/%s", (LPCSTR)sParam.Left(4), (LPCSTR)sParam.Mid(4, 2), (LPCSTR)sParam.Mid(6, 2));

	SetWindowText(sStr);

	return sParam;
}

LPCSTR CMyDate::operator=(LPCSTR pParam)
{
	SetWindowText(pParam);

	return pParam;
}

CMyDate::operator CString()
{
	int nYear, nMonth, nDay;
	CString sStr;

	if (GetDate(nYear, nMonth, nDay))
		sStr.Format("%04d%02d%02d", nYear, nMonth, nDay);

	return sStr;
}

BOOL CMyDate::GetDate(int &nYear, int &nMonth, int &nDay)
{
	CString sStr;

	GetWindowText(sStr);

	if (sStr.IsEmpty())
		return FALSE;

	CString sTemp = sStr.SpanExcluding("/");
	nYear = atoi(sTemp);
	if (nYear < 50)
		nYear += 2000;
	else if (nYear < 100)
		nYear += 1900;

	if (sStr.GetLength() == sTemp.GetLength())
		sStr.Empty();
	else
		sStr = sStr.Mid(sTemp.GetLength() + 1);

	sTemp = sStr.SpanExcluding("/");
	nMonth = atoi(sTemp);
	if (nMonth == 0)
		nMonth = 1;

	if (sStr.GetLength() == sTemp.GetLength())
		sStr.Empty();
	else
		sStr = sStr.Mid(sTemp.GetLength() + 1);

	sTemp = sStr.SpanExcluding("/");
	nDay = atoi(sTemp);
	if (nDay == 0)
		nDay = 1;

	return TRUE;
}

void CMyDate::OnUpdate()
{
	CString sStr;
	CString sTemp;

	GetWindowText(sStr);
	sTemp = sStr.SpanIncluding("0123456789/");
	if (sStr.GetLength() != sTemp.GetLength()) {
		SetWindowText(sTemp);
		SetSel(sTemp.GetLength(), sTemp.GetLength());
	}
}
/////////////////////////////////////////////////////////////////////////////
// CMyEdit

CMyEdit::CMyEdit()
{
	m_bSetTextColor = FALSE;
	m_nTextColor = (COLORREF)-1;
}

BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit メッセージ ハンドラ

int CMyEdit::operator=(int nParam)
{
	CString sStr1, sStr2;

	sStr1.Format("%d", nParam);
	GetWindowText(sStr2);

	if (sStr1 != sStr2)
		SetWindowText(sStr1);

	return nParam;
}

long CMyEdit::operator=(long nParam)
{
	CString sStr1, sStr2;

	sStr1.Format("%ld", nParam);
	GetWindowText(sStr2);

	if (sStr1 != sStr2)
		SetWindowText(sStr1);

	return nParam;
}

double CMyEdit::operator=(double fParam)
{
	CString sStr1, sStr2;

	sStr1.Format("%g", fParam);
	GetWindowText(sStr2);

	if (sStr1 != sStr2)
		SetWindowText(sStr1);

	return fParam;
}

CString &CMyEdit::operator=(CString &sParam)
{
	CString sStr2;

	GetWindowText(sStr2);

	if (sParam != sStr2)
		SetWindowText(sParam);

	return sParam;
}

LPCSTR CMyEdit::operator=(LPCSTR pParam)
{
	CString sStr2;

	GetWindowText(sStr2);

	if (sStr2 != pParam)
		SetWindowText(pParam);

	return pParam;
}

CMyEdit &CMyEdit::operator=(CMyEdit &oParam)
{
	CString sStr;

	oParam.GetWindowText(sStr);
	SetWindowText(sStr);

	return oParam;
}

int CMyEdit::operator==(int nParam)
{
	GetWindowText(m_sStr);
	return atoi(m_sStr) == nParam;
}

int CMyEdit::operator!=(int nParam)
{
	GetWindowText(m_sStr);
	return atoi(m_sStr) != nParam;
}

CMyEdit::operator int()
{
	GetWindowText(m_sStr);
	return atoi(m_sStr);
}

CMyEdit::operator double()
{
	GetWindowText(m_sStr);
	return atof(m_sStr);
}

CMyEdit::operator CString()
{
	GetWindowText(m_sStr);
	return m_sStr;
}

void CMyEdit::Format(LPCSTR lpszFormat, ...)
{
	va_list vl;
	char str[256];
	CString sStr2;

	va_start(vl, lpszFormat);
	vsprintf(str, lpszFormat, vl);

	GetWindowText(sStr2);

	if (sStr2 != str)
		SetWindowText(str);
}

void CMyEdit::Blank()
{
	CString sStr;

	GetWindowText(sStr);
	if (!sStr.IsEmpty())
		SetWindowText("");
}

void CMyEdit::LoadString(UINT nStringID)
{
	CString sStr1, sStr2;

	sStr1.LoadString(nStringID);

	GetWindowText(sStr2);

	if (sStr2 != sStr1)
		SetWindowText(sStr1);
}

void CMyEdit::FormatMessage(UINT nStringID, ...)
{
	va_list vl;
	char str[256];
	CString sStr2;

	va_start(vl, nStringID);
	sStr2.LoadString(nStringID);
	vsprintf(str, (LPCSTR)sStr2, vl);

	GetWindowText(sStr2);

	if (sStr2 != str)
		SetWindowText(str);
}

void CMyEdit::SetTextColor(COLORREF color)
{
	if (!m_bSetTextColor) {
		m_oBrush.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
		m_bSetTextColor = TRUE;
	}

	if (color != m_nTextColor) {
		m_nTextColor = color;
		Invalidate();
	}
}

HBRUSH CMyEdit::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	if (m_bSetTextColor) {
		pDC->SetTextColor(m_nTextColor);
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)m_oBrush;
	}

	return NULL;
}
/////////////////////////////////////////////////////////////////////////////
// CMyButton

CMyButton::CMyButton()
{
	m_bNotifyButtonDown = FALSE;
}

BEGIN_MESSAGE_MAP(CMyButton, CButton)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyButton メッセージ ハンドラ

BOOL CMyButton::operator=(BOOL bParam)
{
	if (GetCheck() != bParam)
		SetCheck(bParam);

	return bParam;
}

CMyButton::operator BOOL()
{
	return GetCheck();
}

void CMyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bNotifyButtonDown)
		GetParent()->SendMessage(WM_BUTTON_DOWN, 0, 0);
	else
		CButton::OnLButtonDown(nFlags, point);
}

// XPスタイルでグリップが正常に表示されないバグの対処
//////////////////////////////////////////////////////
// CMyToolBar

BEGIN_MESSAGE_MAP(CMyToolBar, CToolBar)
	ON_WM_NCPAINT()
END_MESSAGE_MAP()

void CMyToolBar::OnNcPaint()
{
	EraseNonClient();
}
void CMyToolBar::EraseNonClient()
{
	// Get window DC that is clipped to the non-client area.
	CWindowDC dc(this);
	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	rectClient.OffsetRect(-rectWindow.left, -rectWindow.top);
	dc.ExcludeClipRect(rectClient);

	// Draw the borders in the non-client area.
	rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);
	DrawBorders(&dc, rectWindow);

	// Erase the parts that are not drawn.
	dc.IntersectClipRect(rectWindow);
	SendMessage(WM_ERASEBKGND, (WPARAM)dc.m_hDC);

	// Draw the gripper in the non-client area.
	DrawGripper(&dc, rectWindow);
}

void CMyToolBar::DoPaint(CDC* pDC)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	// Paint inside the client area.
	CRect rect;
	GetClientRect(rect);
	DrawBorders(pDC, rect);
	DrawGripper(pDC, rect);
}

void CMyToolBar::DrawGripper(CDC* pDC, const CRect& rect)
{
	pDC->FillSolidRect( &rect, ::GetSysColor(COLOR_BTNFACE)); // Fill in the background.
	CToolBar::DrawGripper(pDC,rect);
}
