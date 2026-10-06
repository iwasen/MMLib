// OptOrder.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "OptOrder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEFAULT_ORDER	"0123456"

/////////////////////////////////////////////////////////////////////////////
// COptOrder プロパティ ページ

COptOrder::COptOrder() : CPropertyPage(COptOrder::IDD)
{
	m_iViewText = -1;
	m_iSmallSize = -1;

	m_bChangeOrder = FALSE;
	m_bChangeText = FALSE;
	m_bChangeSmallSize = FALSE;
}

void COptOrder::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ORDER_LIST, m_cOrderList);
	DDX_Radio(pDX, IDC_TITLE_CODE, m_iViewText);
	DDX_Radio(pDX, IDC_SMALL_NORMAL, m_iSmallSize);
}

BEGIN_MESSAGE_MAP(COptOrder, CPropertyPage)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptOrder メッセージ ハンドラ

BOOL COptOrder::OnInitDialog()
{
	m_nViewText = ::AfxGetApp()->GetProfileInt(g_sSectionOptions, "ViewText", 0);
	m_iViewText = m_nViewText;

	m_nSmallSize = ::AfxGetApp()->GetProfileInt(g_sSectionOptions, "SmallSize", 0);
	m_iSmallSize = m_nSmallSize;

	CPropertyPage::OnInitDialog();

	m_sOrder = ::AfxGetApp()->GetProfileString(g_sSectionOptions, "ViewOrder", DEFAULT_ORDER);
	SetListBox(m_sOrder);

	return TRUE;
}

void COptOrder::OnUp()
{
	int nIndex;

	if ((nIndex = m_cOrderList.GetCurSel()) == LB_ERR)
		return;

	if (nIndex == 0)
		return;

	CString sStr;
	m_cOrderList.GetText(nIndex, sStr);
	const DWORD data = (DWORD)m_cOrderList.GetItemData(nIndex);
	m_cOrderList.DeleteString(nIndex);

	m_cOrderList.InsertString(--nIndex, sStr);
	m_cOrderList.SetItemData(nIndex, data);

	m_cOrderList.SetCurSel(nIndex);
}

void COptOrder::OnDown()
{
	int nIndex;

	if ((nIndex = m_cOrderList.GetCurSel()) == LB_ERR)
		return;

	if (nIndex == m_cOrderList.GetCount() - 1)
		return;

	CString sStr;
	m_cOrderList.GetText(nIndex, sStr);
	const DWORD data = (DWORD)m_cOrderList.GetItemData(nIndex);
	m_cOrderList.DeleteString(nIndex);

	m_cOrderList.InsertString(++nIndex, sStr);
	m_cOrderList.SetItemData(nIndex, data);

	m_cOrderList.SetCurSel(nIndex);
}

void COptOrder::OnOK()
{
	UpdateData(TRUE);

	const int nCount = m_cOrderList.GetCount();
	CString sStr(' ', nCount);

	for (int i = 0; i < nCount; i++)
		sStr.SetAt(i, (char)(m_cOrderList.GetItemData(i) + '0'));

	if (sStr != m_sOrder) {
		::AfxGetApp()->WriteProfileString(g_sSectionOptions, "ViewOrder", sStr);
		m_bChangeOrder = TRUE;
	}

	if (m_iViewText != m_nViewText) {
		::AfxGetApp()->WriteProfileInt(g_sSectionOptions, "ViewText", m_iViewText);
		m_bChangeText = TRUE;
	}

	if (m_iSmallSize != m_nSmallSize) {
		::AfxGetApp()->WriteProfileInt(g_sSectionOptions, "SmallSize", m_iSmallSize);
		m_bChangeSmallSize = TRUE;
	}

	CPropertyPage::OnOK();
}

void COptOrder::OnDefault()
{
	SetListBox(DEFAULT_ORDER);

	m_iViewText = 0;
	m_iSmallSize = 0;
	UpdateData(FALSE);
}

void COptOrder::SetListBox(LPCSTR pOrder)
{
	static constexpr UINT item[] = {
		IDS_NAME, IDS_CODE, IDS_CREATE_DATE, IDS_UPDATE_DATE, IDS_CREATOR, IDS_REMARK, IDS_KEYWORD
	};

	m_cOrderList.ResetContent();
	for (int i = 0; i < m_sOrder.GetLength(); i++) {
		const int nIndex = pOrder[i] - '0';
		m_cOrderList.InsertString(i, GetString(item[nIndex]));
		m_cOrderList.SetItemData(i, nIndex);
	}
}
