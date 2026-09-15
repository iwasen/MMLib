// AccessLogDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "AccessLogDlg.h"

// CAccessLogDlg ダイアログ

CAccessLogDlg::CAccessLogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx2(CAccessLogDlg::IDD, pParent)
{
}

void CAccessLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx2::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ACCESS_LOG, m_cListAccessLog);
}

BEGIN_MESSAGE_MAP(CAccessLogDlg, CDialogEx2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ACCESS_LOG, &CAccessLogDlg::OnLvnItemchangedListAccessLog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CAccessLogDlg メッセージ ハンドラ

BOOL CAccessLogDlg::OnInitDialog()
{
	CDialogEx2::OnInitDialog();

	m_cListAccessLog.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	InsertColumns();
	SetListData();

	SetCtlPosition(IDC_LIST_ACCESS_LOG, 0, 0, 1, 1);
	SetCtlPosition(IDCANCEL, 1, 1, 1, 1);

	return TRUE;
}

void CAccessLogDlg::OnLvnItemchangedListAccessLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV->uNewState & LVIS_SELECTED) {
		m_pDocument->UpdateAllViews(NULL, VIEWHINT_SELECTDATA, (CItem *)pNMLV->lParam);
	}

	*pResult = 0;
}

void CAccessLogDlg::InsertColumns()
{
	struct LISTCOLUMN {
		UINT text;
		int fmt;
		int width;
	};
	static const LISTCOLUMN tColumn[] = {
		{IDS_ACCESS_TIME, LVCFMT_LEFT, 125},
		{IDS_NAME, LVCFMT_LEFT, 130},
		{IDS_FOLDER, LVCFMT_LEFT, 200},
		{IDS_CREATE_DATE, LVCFMT_LEFT, 110}
	};
	LV_COLUMN lvc;

	for (int i = 0; i < sizeof(tColumn) / sizeof(LISTCOLUMN); i++) {
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
		lvc.fmt = tColumn[i].fmt;
		lvc.pszText = (LPTSTR)(LPCSTR)GetString(tColumn[i].text);
		lvc.cx = CDpi::AdjustDpi(tColumn[i].width);
		m_cListAccessLog.InsertColumn(i, &lvc);
	}
}

void CAccessLogDlg::SetListData()
{
	CPSDB &oAccessLog = g_pMMLibDB->m_oAccessLog;
	BOOL bBof;
	ACCESSLOGBUF oAccessLogBuf;
	ACCESSLOGREC oAccessLogRec;
	CString sCreateDate;

	oAccessLog.DBChgIdx(1);
	oAccessLog.DBBottom();

	int nItem = 0;
	while (true) {
		oAccessLog.DBBof(&bBof);
		if (bBof)
			break;

		oAccessLog.DBRead(&oAccessLogBuf);
		oAccessLog.DBGetFieldBuf(&oAccessLogBuf, &oAccessLogRec);

		FormatDate(oAccessLogRec.sCreateDate, sCreateDate);

		m_cListAccessLog.InsertItem(nItem, oAccessLogRec.sAccessTime);
		m_cListAccessLog.SetItemText(nItem, 1, oAccessLogRec.sTitle);
		m_cListAccessLog.SetItemText(nItem, 2, oAccessLogRec.sFolderPath);
		m_cListAccessLog.SetItemText(nItem, 3, sCreateDate);

		CSelectItem *pSelectItem = new CSelectItem;
		pSelectItem->m_sLibraryID = oAccessLogRec.sLibraryID;
		pSelectItem->m_sDataID = oAccessLogRec.sDataID;
		m_cListAccessLog.SetItemData(nItem, (DWORD_PTR)pSelectItem);

		nItem++;

		oAccessLog.DBSkip(-1);
	}
}

void CAccessLogDlg::OnDestroy()
{
	CDialogEx2::OnDestroy();

	int nCount = m_cListAccessLog.GetItemCount();
	for (int i = 0; i < nCount; i++)
		delete (CSelectItem *)m_cListAccessLog.GetItemData(i);
}
