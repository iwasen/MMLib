// MakePictIndex.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "MakePictIndex.h"

// CMakePictIndex ダイアログ

CMakePictIndex::CMakePictIndex(CWnd* pParent /*=NULL*/)
	: CDialogAF(CMakePictIndex::IDD, pParent)
{
	m_bCancel = FALSE;
}

void CMakePictIndex::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_cProgressBar);
	DDX_Control(pDX, IDOK, m_cOK);
}

BEGIN_MESSAGE_MAP(CMakePictIndex, CDialogAF)
END_MESSAGE_MAP()

// CMakePictIndex メッセージ ハンドラ

void CMakePictIndex::OnOK()
{
	m_cOK.EnableWindow(FALSE);

	MakePictIndex();

	CDialogAF::OnOK();
}

void CMakePictIndex::MakePictIndex()
{
	long nRecNum;
	BOOL bEof;
	HDIB hDIB;
	DATABUF oDataBuf;
	DATAREC oDataRec;

	m_pPcsDB->m_oDataDB.DBLRecCount(&nRecNum);
	m_cProgressBar.SetRange32(0, nRecNum);

	m_pPcsDB->CreateFindPictDB();

	m_pPcsDB->SetDataIndex(0);
	m_pPcsDB->m_oDataDB.DBTop();
	int nCount = 0;
	while (true) {
		MSG	oMsg;

		while (::PeekMessage(&oMsg, NULL, 0, 0, PM_REMOVE)) {
			::TranslateMessage((LPMSG)&oMsg);
			::DispatchMessage((LPMSG)&oMsg);
		}

		if (m_bCancel) {
			m_pPcsDB->DeleteFindPictDB();
			break;
		}

		m_pPcsDB->m_oDataDB.DBEof(&bEof);
		if (bEof)
			break;

		m_pPcsDB->m_oDataDB.DBRead(&oDataBuf);
		m_pPcsDB->m_oDataDB.DBGetFieldBuf(&oDataBuf, &oDataRec);

		if (m_pPcsDB->CheckFolderID(oDataRec.sFolderID)) {
			if (MMPictureReadDB(&hDIB, m_pPcsDB->m_oDataDB.GetFCB(), PICT1_FIELD) == 0) {
				m_pPcsDB->UpdateFindPictDB(oDataRec.sDataID, hDIB);

				MMPictureFree(hDIB);
			}
		}

		m_pPcsDB->m_oDataDB.DBSkip(1);

		m_cProgressBar.SetPos(++nCount);
	}
}

void CMakePictIndex::OnCancel()
{
	m_bCancel = TRUE;

	CDialogAF::OnCancel();
}
