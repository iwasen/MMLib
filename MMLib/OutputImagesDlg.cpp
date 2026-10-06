// OutputImagesDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "OutputImagesDlg.h"
#include "Shlwapi.h"
#include "math.h"

// COutputImagesDlg ダイアログ

IMPLEMENT_DYNAMIC(COutputImagesDlg, CDialogAF)

COutputImagesDlg::COutputImagesDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(COutputImagesDlg::IDD, pParent)
{
}

void COutputImagesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_cProgressCtrl);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_cStaticProgress);
}

BEGIN_MESSAGE_MAP(COutputImagesDlg, CDialogAF)
	ON_MESSAGE(WM_END_DIALOG, OnEndDialog)
END_MESSAGE_MAP()

// COutputImagesDlg メッセージ ハンドラ

BOOL COutputImagesDlg::OnInitDialog()
{
	CDialogAF::OnInitDialog();

	m_bCancel = FALSE;

	AfxBeginThread(ThreadProc, this);

	return TRUE;
}

LRESULT COutputImagesDlg::OnEndDialog(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	EndDialog(0);

	return (LRESULT)0;
}

UINT COutputImagesDlg::ThreadProc(LPVOID pParam)
{
	((COutputImagesDlg *)pParam)->Output();

	return 0;
}

void COutputImagesDlg::Output()
{
	m_pPcsDB = m_pItemFolder->m_pPcsDB;

	m_nTotalCount = 0;
	CountFolder(m_pItemFolder->m_oFolderRec);

	m_nOutputCount = 0;
	m_nPercent = 0;
	OutputFolder(m_sOutputFolder, m_pItemFolder->m_oFolderRec);

	Sleep(1000);

	this->PostMessage(WM_END_DIALOG);
}

BOOL COutputImagesDlg::CountFolder(const FOLDERREC &oFolderRec)
{
	if (oFolderRec.sFolderType[0] == FTYPE_DFOLDER) {
		if (!CountImages(oFolderRec))
			return FALSE;
	} else {
		CStringArray aFolderID;
		FOLDERREC oFolderRecChild;
		BOOL bFirst = TRUE;
		while (m_pPcsDB->ReadChildFolderEx(oFolderRec.sFolderID, &oFolderRecChild, bFirst)) {
			bFirst = FALSE;

			aFolderID.Add(oFolderRecChild.sFolderID);
		}

		for (int i = 0; i < aFolderID.GetCount(); i++) {
			m_pPcsDB->ReadFolderEx(aFolderID[i], &oFolderRecChild);

			if (!CountFolder(oFolderRecChild))
				return FALSE;
		}
	}

	return TRUE;
}

BOOL COutputImagesDlg::CountImages(const FOLDERREC &oFolderRec)
{
	DATAREC oDataRec;
	BOOL bFirst = TRUE;
	while (m_pPcsDB->ReadChildDataEx(oFolderRec.sFolderID, &oDataRec, bFirst)) {
		bFirst = FALSE;

		if (m_bCancel)
			return FALSE;

		m_nTotalCount++;
	}

	return TRUE;
}

BOOL COutputImagesDlg::OutputFolder(const CString &sParentFolder, const FOLDERREC &oFolderRec)
{
	CString sFolderName = oFolderRec.sFolderName;
	CheckFileName(sFolderName);
	CString sFolderPath = sParentFolder + "\\" + sFolderName;

	if (!::PathFileExists(sFolderPath)) {
		if (!::CreateDirectory(sFolderPath, NULL))
			return FALSE;
	}

	if (oFolderRec.sFolderType[0] == FTYPE_DFOLDER) {
		if (!OutputImages(sFolderPath, oFolderRec))
			return FALSE;
	} else {
		CStringArray aFolderID;
		FOLDERREC oFolderRecChild;
		BOOL bFirst = TRUE;
		while (m_pPcsDB->ReadChildFolderEx(oFolderRec.sFolderID, &oFolderRecChild, bFirst)) {
			bFirst = FALSE;

			aFolderID.Add(oFolderRecChild.sFolderID);
		}

		for (int i = 0; i < aFolderID.GetCount(); i++) {
			m_pPcsDB->ReadFolderEx(aFolderID[i], &oFolderRecChild);

			if (!OutputFolder(sFolderPath, oFolderRecChild))
				return FALSE;
		}
	}

	return TRUE;
}

BOOL COutputImagesDlg::OutputImages(const CString &sParentFolder, const FOLDERREC &oFolderRec)
{
	CArray<DATAREC, DATAREC &> aDataArray;
	DATAREC oDataRec;
	BOOL bFirst = TRUE;
	while (m_pPcsDB->ReadChildDataEx(oFolderRec.sFolderID, &oDataRec, bFirst)) {
		bFirst = FALSE;

		aDataArray.Add(oDataRec);
	}

	qsort(aDataArray.GetData(), aDataArray.GetSize(), sizeof(DATAREC), CompareProc);

	const int nPageNum = (int)log10((double)aDataArray.GetSize()) + 1;
	int nPage = 1;
	for (int i = 0; i < aDataArray.GetCount(); i++) {
		if (m_bCancel)
			return FALSE;

		m_pPcsDB->ReadDataEx(aDataArray[i].sDataID, &oDataRec);

		OutputImage(sParentFolder, oDataRec, nPage, nPageNum);
		nPage++;
	}

	return TRUE;
}

int COutputImagesDlg::CompareProc(const void *p1, const void *p2)
{
	DATAREC *pDataRec1 = (DATAREC *)p1;
	DATAREC *pDataRec2 = (DATAREC *)p2;

	if (!pDataRec1->sCode.IsEmpty() && !pDataRec2->sCode.IsEmpty()) {
		return atoi(pDataRec1->sCode) - atoi(pDataRec2->sCode);
	} else {
		return pDataRec1->sDataID.Compare(pDataRec2->sDataID);
	}
}

void COutputImagesDlg::OutputImage(const CString &sParentFolder, const DATAREC &oDataRec, int nPage, int nPageNum)
{
	CString sFormat;
	sFormat.Format("P%%0%dd_", nPageNum);
	CString sPage;
	sPage.Format(sFormat, nPage);

	CString sFileName = sPage + oDataRec.sTitle;
	CheckFileName(sFileName);
	CString sFilePath = sParentFolder + "\\" + sFileName;

	SHORT	mmType;
	PSDBReadBinary(m_pPcsDB->m_oDataDB.GetFCB(), PICT1_FIELD, (char *)&mmType, 2);
	if (mmType == MMTYPE_JPG || mmType == MMTYPE_PNG) {
		MMDownloadFile(m_pPcsDB->m_oDataDB.GetFCB(), PICT1_FIELD, (LPSTR)(LPCSTR)sFilePath);
	} else {
		HDIB hDIB;
		if (MMPictureReadDB(&hDIB, m_pPcsDB->m_oDataDB.GetFCB(), PICT1_FIELD) == 0) {
			CString sExt;
			switch (GetPictureBitCount(hDIB)) {
			case 0:
				return;
			case 24:
				sExt = "jpg";
				break;
			default:
				sExt = "png";
				break;
			}

			sFilePath += "." + sExt;

			MMPictureWriteFile(hDIB, (LPSTR)(LPCSTR)sFilePath);

			::GlobalFree(hDIB);
		}
	}

	m_nOutputCount++;
	const int nPercent = (int)(100.0 * m_nOutputCount / m_nTotalCount);
	if (nPercent != m_nPercent) {
		// プログレスバーのアニメーションを無効化（戻すときは即座に反映されることを利用）
		UpdateProgressBar(m_cProgressCtrl, nPercent);
		CString sPercent;
		sPercent.Format("%d %%", nPercent);
		m_cStaticProgress.SetWindowText(sPercent);
		m_nPercent = nPercent;
	}
}

void COutputImagesDlg::OnCancel()
{
	m_bCancel = TRUE;
}
