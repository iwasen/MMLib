// FindPictDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "FindPictDlg.h"
#include "MakePictIndex.h"
#include <math.h>

#define FIND_LIST_NUM	1000
#define DISP_LIST_NUM	200

// CFindPictDlg ダイアログ

CFindPictDlg::CFindPictDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx2(CFindPictDlg::IDD, pParent)
{
	m_nInit = FALSE;
}

void CFindPictDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx2::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FIND, m_cListFind);
}

BEGIN_MESSAGE_MAP(CFindPictDlg, CDialogEx2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FIND, &CFindPictDlg::OnLvnItemchangedListFind)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_INDEX, &CFindPictDlg::OnBnClickedButtonMakeIndex)
	ON_BN_CLICKED(IDC_BUTTON_PREV, &CFindPictDlg::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CFindPictDlg::OnBnClickedButtonNext)
END_MESSAGE_MAP()

// CFindPictDlg メッセージ ハンドラ

BOOL CFindPictDlg::OnInitDialog()
{
	CDialogEx2::OnInitDialog();

	m_cListFind.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	InsertColumns();
	SetListData();

	SetCtlPosition(IDC_LIST_FIND, 0, 0, 1, 1);
	SetCtlPosition(IDC_BUTTON_MAKE_INDEX, 1, 1, 1, 1);
	SetCtlPosition(IDCANCEL, 1, 1, 1, 1);

	m_nInit = TRUE;

	return TRUE;
}

void CFindPictDlg::InsertColumns()
{
	struct LISTCOLUMN {
		UINT nText;
		int nFmt;
		int nWidth;
	};
	static constexpr LISTCOLUMN tColumn[] = {
		{IDS_NAME, LVCFMT_LEFT, 130},
		{IDS_FOLDER, LVCFMT_LEFT, 150},
		{IDS_CREATE_DATE, LVCFMT_LEFT, 110},
		{IDS_REMARK, LVCFMT_LEFT, 150},
		{IDS_KEYWORD, LVCFMT_LEFT, 150}
	};

	for (int i = 0; i < ARRAY_SIZE(tColumn); i++) {
		LV_COLUMN lvc{
			.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT,
			.fmt = tColumn[i].nFmt,
			.cx = CDpi::AdjustDpi(tColumn[i].nWidth, i == 2 ? 100 : 90),
			.pszText = (LPSTR)(LPCSTR)GetString(tColumn[i].nText),
		};
		m_cListFind.InsertColumn(i, &lvc);
	}
}

void CFindPictDlg::OnBnClickedButtonMakeIndex()
{
	CMakePictIndex dlg;

	dlg.m_pPcsDB = m_pItemData->m_pPcsDB;
	dlg.DoModal();

	SetListData();
}

void CFindPictDlg::SetListData()
{
	CWaitCursor wait;

	m_cListFind.DeleteAllItems();
	m_aDataID.RemoveAll();

	CPSDB &oFindPictDB = m_pItemData->m_pPcsDB->m_oFindPictDB;
	if (oFindPictDB.GetFCB() == NULL)
		return;

	CArray <PICT_LIST, PICT_LIST &> aPictListR, aPictListG, aPictListB, aPictList;
	GetPictList(1, aPictListR);
	GetPictList(2, aPictListG);
	GetPictList(3, aPictListB);

	PICT_LIST oPictList;
	for (int i = 0; i < aPictListR.GetSize(); i++) {
		for (int j = 0; j < aPictListG.GetSize(); j++) {
			if (strcmp(aPictListR[i].did, aPictListG[j].did) == 0) {
				for (int k = 0; k < aPictListB.GetSize(); k++) {
					if (strcmp(aPictListR[i].did, aPictListB[k].did) == 0) {
						strcpy(oPictList.did, aPictListR[i].did);
						oPictList.diff = aPictListR[i].diff + aPictListG[j].diff + aPictListB[k].diff;
						aPictList.Add(oPictList);
						break;
					}
				}
				break;
			}
		}
	}

	qsort(aPictList.GetData(), aPictList.GetSize(), sizeof(PICT_LIST), Compare);

	strcpy(oPictList.did, m_pItemData->m_oDataRec.sDataID);
	oPictList.diff = 0;
	aPictList.InsertAt(0, oPictList);

	m_pItemData->m_pPcsDB->SetFolderIndex(1);
	m_pItemData->m_pPcsDB->SetDataIndex(2);

	CPSDB &oDataDB = m_pItemData->m_pPcsDB->m_oDataDB;
	CString sFolder;
	CString sCreateDate;
	BOOL bEof;
	int nItem = 0;
	int index = 0;
	DATABUF oDataBuf;
	DATAREC oDataRec;
	while (nItem < DISP_LIST_NUM && index < aPictList.GetSize()) {
		oDataDB.DBSearch(aPictList[index++].did, LEN_ID);
		oDataDB.DBEof(&bEof);
		if (!bEof) {
			oDataDB.DBRead(&oDataBuf);
			oDataDB.DBGetFieldBuf(&oDataBuf, &oDataRec);

			sFolder.Empty();
			GetFolder(oDataRec.sFolderID, sFolder);

			FormatDate(oDataRec.sCreateDate, sCreateDate);

			m_cListFind.InsertItem(nItem, oDataRec.sTitle);
			m_aDataID.Add(oDataRec.sDataID);
			m_cListFind.SetItemText(nItem, 1, sFolder);
			m_cListFind.SetItemText(nItem, 2, sCreateDate);
			m_cListFind.SetItemText(nItem, 3, oDataRec.sRemark);
			m_cListFind.SetItemText(nItem, 4, oDataRec.sKeyword);

			nItem++;
		}
	}

	m_cListFind.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
}

void CFindPictDlg::GetPictList(int nFactorIndex, CArray <PICT_LIST, PICT_LIST &> &aPictList)
{
	FINDPICTBUF oFindPictBuf;
	FINDPICTREC oFindPictRec;
	int nIndexNo;
	CString *pFactor;
	BOOL bBof, bEof;

	switch (nFactorIndex) {
	case 1:
		nIndexNo = 2;
		pFactor = &oFindPictRec.sFactor_r;
		break;
	case 2:
		nIndexNo = 3;
		pFactor = &oFindPictRec.sFactor_g;
		break;
	case 3:
		nIndexNo = 4;
		pFactor = &oFindPictRec.sFactor_b;
		break;
	default:
		return;
	}

	CPSDB &oFindPictDB = m_pItemData->m_pPcsDB->m_oFindPictDB;
	oFindPictDB.DBChgIdx(1);
	oFindPictDB.DBSearch(m_pItemData->m_oDataRec.sDataID, LEN_ID);
	oFindPictDB.DBRead(&oFindPictBuf);
	oFindPictDB.DBGetFieldBuf(&oFindPictBuf, &oFindPictRec);
	oFindPictDB.DBChgIdx(nIndexNo);
	oFindPictDB.DBSearch(*pFactor + oFindPictRec.sDataID, LEN_PFACTOR + LEN_ID);
	oFindPictDB.DBSkip(-(FIND_LIST_NUM / 2));
	oFindPictDB.DBBof(&bBof);
	if (bBof)
		oFindPictDB.DBTop();

	PICT_LIST oPictList;
	int nItem = 0;
	const double fPictFactor = atof(*pFactor);
	while (nItem < FIND_LIST_NUM) {
		oFindPictDB.DBEof(&bEof);
		if (bEof)
			break;

		oFindPictDB.DBRead(&oFindPictBuf);
		oFindPictDB.DBGetFieldBuf(&oFindPictBuf, &oFindPictRec);

		if (oFindPictRec.sDataID != m_pItemData->m_oDataRec.sDataID) {
			strcpy(oPictList.did, oFindPictRec.sDataID);
			oPictList.diff = fabs(fPictFactor - atof(*pFactor));
			aPictList.Add(oPictList);
		}

		oFindPictDB.DBSkip(1);
	}
}

void CFindPictDlg::OnLvnItemchangedListFind(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (m_nInit) {
		if (pNMLV->uNewState & LVIS_SELECTED) {
			CPSDB &oDataDB = m_pItemData->m_pPcsDB->m_oDataDB;
			CSelectItem oSelectItem;

			oDataDB.DBSearch(m_aDataID[pNMLV->iItem], LEN_ID);
			DATABUF oDataBuf;
			oDataDB.DBRead(&oDataBuf);
			DATAREC oDataRec;
			oDataDB.DBGetFieldBuf(&oDataBuf, &oDataRec);

			oSelectItem.m_sLibraryID = m_pItemData->m_pPcsDB->m_sLibraryID;
			oSelectItem.m_sDataID = oDataRec.sDataID;
			m_pDocument->UpdateAllViews(NULL, VIEWHINT_SELECTDATA, &oSelectItem);
		}
	}

	*pResult = 0;
}

int CFindPictDlg::Compare(const void *p1, const void *p2)
{
	if (((PICT_LIST *)p1)->diff - ((PICT_LIST *)p2)->diff < 0)
		return -1;
	else if (((PICT_LIST *)p1)->diff - ((PICT_LIST *)p2)->diff > 0)
		return 1;
	else
		return 0;
}

void CFindPictDlg::GetFolder(const CString &sFolderId, CString &sFolder)
{
	CPSDB &oFolderDB = m_pItemData->m_pPcsDB->m_oFolderDB;
	int nFind;

	FOLDERBUF oFolderBuf;
	if (oFolderDB.DBReadKey((LPCSTR)sFolderId, LEN_ID, &oFolderBuf, 0, &nFind) == 0) {
		if (nFind) {
			FOLDERREC oFolderRec;
			oFolderDB.DBGetFieldBuf(&oFolderBuf, &oFolderRec);

			if (oFolderRec.sFolderType != FTYPE_LIBRARY) {
				sFolder = oFolderRec.sFolderName + (sFolder.IsEmpty() ? "" : "/") + sFolder;
				GetFolder(oFolderRec.sParentFolderID, sFolder);
			}
		}
	}
}

void CFindPictDlg::OnBnClickedButtonPrev()
{
	if (m_cListFind.GetItemCount() != 0) {
		int nItem = m_cListFind.GetItemCount() - 1;
		POSITION pos = m_cListFind.GetFirstSelectedItemPosition();
		if (pos != NULL) {
			const int nPrevItem = m_cListFind.GetNextSelectedItem(pos) - 1;
			if (nPrevItem >= 0)
				nItem = nPrevItem;
		}
		m_cListFind.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		m_cListFind.EnsureVisible(nItem, FALSE);
	}
}

void CFindPictDlg::OnBnClickedButtonNext()
{
	if (m_cListFind.GetItemCount() != 0) {
		int nItem = 0;
		POSITION pos = m_cListFind.GetFirstSelectedItemPosition();
		if (pos != NULL) {
			const int nNextItem = m_cListFind.GetNextSelectedItem(pos) + 1;
			if (nNextItem < m_cListFind.GetItemCount())
				nItem = nNextItem;
		}
		m_cListFind.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		m_cListFind.EnsureVisible(nItem, FALSE);
	}
}
