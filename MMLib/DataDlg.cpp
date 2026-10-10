// DataDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "DataDlg.h"
#include "AutoNumDlg.h"
#include "TextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataDlg ダイアログ

CDataDlg::CDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(g_nFontSize == 0 ? IDD_DATA : IDD_DATA_LARGE, pParent)
{
	m_pItemData = NULL;
	m_hDIB = NULL;
	m_bUpdateData = FALSE;
	m_bUpdatePict = FALSE;
	m_bUpdateSound = FALSE;
	m_bUpdateText = FALSE;
	m_pReadFileName = NULL;
	m_nReadFileName = 0;
	m_nReadCount = 0;
	m_hWndNext = NULL;
	m_hWndSel = NULL;
}

CDataDlg::~CDataDlg()
{
	if (m_hDIB != NULL)
		::GlobalFree(m_hDIB);
}

void CDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICT_WINDOW, m_cPictWindow);
	DDX_Control(pDX, IDC_PICT_CLIPBOARD, m_cPictClipBoard);
	DDX_Control(pDX, IDC_COPY_DATA, m_cCopyData);
	DDX_Control(pDX, IDC_AUTO_ROTATION, m_cAutoRotation);
	DDX_Control(pDX, IDC_AUTO_NUMBERING, m_cAutoNumbering);
	DDX_Control(pDX, IDC_AUTO_SAVE, m_cAutoSave);
	DDX_Control(pDX, IDC_PICT_ERASE, m_cPictErase);
	DDX_Control(pDX, IDCANCEL, m_cCancel);
	DDX_Control(pDX, IDC_PICT_ROTATION, m_cPictRotation);
	DDX_Control(pDX, IDC_PICT_PICTURE, m_cPictPicture);
	DDX_Control(pDX, IDC_SAVE, m_cSave);
	DDX_Control(pDX, IDC_TITLE, m_cTitle);
	DDX_Control(pDX, IDC_REMARK, m_cRemark);
	DDX_Control(pDX, IDC_KEYWORD, m_cKeyword);
	DDX_Control(pDX, IDC_CREATOR, m_cCreator);
	DDX_Control(pDX, IDC_CODE, m_cCode);
	DDX_Text(pDX, IDC_CODE, m_sCode);
	DDX_Text(pDX, IDC_CREATOR, m_sCreator);
	DDX_Text(pDX, IDC_PICT_COLOR, m_sPictColor);
	DDX_Text(pDX, IDC_PICT_DPI, m_sPictDPI);
	DDX_Text(pDX, IDC_PICT_HEIGHT, m_sPictHeight);
	DDX_Text(pDX, IDC_PICT_WIDTH, m_sPictWidth);
	DDX_Text(pDX, IDC_REMARK, m_sRemark);
	DDX_Text(pDX, IDC_KEYWORD, m_sKeyword);
	DDX_Text(pDX, IDC_TITLE, m_sTitle);
}

BEGIN_MESSAGE_MAP(CDataDlg, CDialogAF)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_PICT_CAMERA, OnPictCamera)
	ON_BN_CLICKED(IDC_PICT_SCANNER, OnPictScanner)
	ON_BN_CLICKED(IDC_PICT_FILE, OnPictFile)
	ON_BN_CLICKED(IDC_PICT_ROTATION, OnPictRotation)
	ON_EN_CHANGE(IDC_TITLE, OnChangeTitle)
	ON_EN_CHANGE(IDC_CODE, OnChangeCode)
	ON_EN_CHANGE(IDC_CREATOR, OnChangeCreator)
	ON_EN_CHANGE(IDC_REMARK, OnChangeRemark)
	ON_EN_CHANGE(IDC_KEYWORD, OnChangeKeyword)
	ON_BN_CLICKED(IDC_PICT_ERASE, OnPictErase)
	ON_BN_CLICKED(IDC_AUTO_NUMBERING, OnAutoNumbering)
	ON_BN_CLICKED(IDC_AUTO_SAVE, OnAutoSave)
	ON_BN_CLICKED(IDC_TEXT_DATA, OnTextData)
	ON_BN_CLICKED(IDC_COPY_DATA, OnCopyData)
	ON_BN_CLICKED(IDC_PICT_CLIPBOARD, OnPictClipboard)
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_FRAMEBMP, OnFrameBmp)
	ON_MESSAGE(WM_SCANBMP, OnScanBmp)
	ON_WM_DRAWCLIPBOARD()
	ON_WM_CHANGECBCHAIN()
	ON_MESSAGE(WM_BUTTON_DOWN, OnPictWindow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataDlg メッセージ ハンドラ

BOOL CDataDlg::OnInitDialog()
{
	CDialogAF::OnInitDialog();

	CString sCaption;
	sCaption.Format("%s - [%s]", (LPCSTR)m_sCaption, (LPCSTR)m_pItemFolder->m_oFolderRec.sFolderName);
	SetWindowText(sCaption);

	m_cCode.LimitText(LEN_CODE);
	m_cTitle.LimitText(LEN_TITLE);
	m_cRemark.LimitText(LEN_REMARK);
	m_cKeyword.LimitText(LEN_KEYWORD);

	m_cPictWindow.m_bNotifyButtonDown = TRUE;

	if (m_pItemData == NULL) {
		m_sTitle.LoadString(IDS_NO_TITLE);
		m_sCreator = GetDefaultUserName();

		AutoNumbering(FALSE);

		if (m_hDIB != NULL) {
			SCAN_IMAGE_INFO oImageInfo;
			HDIB hDIB = m_hDIB;
			m_hDIB = NULL;
			GetImageInfo(hDIB, &oImageInfo);
			SetPicture(hDIB, &oImageInfo);
		}

		m_cCancel.SetWindowText(GetString(IDS_CLOSE));

		m_cAutoSave = theApp.GetProfileInt(g_sSectionSettings, "AutoSave", 0);

		m_cAutoRotation.AddString(GetString(IDS_NO_ROTATION));
		m_cAutoRotation.AddString(GetString(IDS_RIGHT_ROTATION));
		m_cAutoRotation.AddString(GetString(IDS_HALF_ROTATION));
		m_cAutoRotation.AddString(GetString(IDS_LEFT_ROTATION));
		m_cAutoRotation.SetCurSel(0);
		m_cAutoRotation.EnableWindow((BOOL)m_cAutoSave);
	} else {
		DATAREC *pDataRec = &m_pItemData->m_oDataRec;
		m_sTitle = pDataRec->sTitle;
		m_sCode = pDataRec->sCode;
		m_sCreateDate = pDataRec->sCreateDate;
		m_sCreator = pDataRec->sCreator;
		m_sRemark = pDataRec->sRemark;
		m_sKeyword = pDataRec->sKeyword;

		m_pItemData->m_pPcsDB->ReadPictureDataEx(pDataRec->sDataID, &m_hDIB, NULL);
		if (m_hDIB != NULL) {
			m_sPictWidth.Format("%d", pDataRec->nWidth);
			m_sPictHeight.Format("%d", pDataRec->nHeight);
			m_sPictColor.Format("%d", pDataRec->nBit);
			if (pDataRec->nDpi != 0)
				m_sPictDPI.Format("%d", pDataRec->nDpi);
			else
				m_sPictDPI.LoadString(IDS_UNKNOWN);

			m_cPictPicture.DrawDIB(m_hDIB);

			m_cAutoSave.EnableWindow(FALSE);
			m_cAutoRotation.EnableWindow(FALSE);
			m_cAutoNumbering.EnableWindow(FALSE);
		}

		m_pItemData->m_pPcsDB->ReadTextDataEx(pDataRec->sDataID, &m_sTextData);

		m_cCancel.SetWindowText(GetString(IDS_CANCEL));
	}

	UpdateData(FALSE);

	EnableButton();
	m_cCopyData.EnableWindow(!m_oDataRec.sTitle.IsEmpty());

	NextReadFile();

	m_hWndNext = SetClipboardViewer();
	m_cPictClipBoard.EnableWindow(m_cAutoSave ? TRUE : CheckClipBoard());

	return TRUE;
}

void CDataDlg::OnSave()
{
	if (!m_bUpdateData)
		return;

	if (!CHECK_LICENSE1(0x01))
		return;

	CWaitCursor wait;
	UpdateData(TRUE);

	DATAREC oDataRec{
		.sCreateDate = m_sCreateDate,
		.sCreator = m_sCreator,
		.sCode = m_sCode,
		.sTitle = m_sTitle,
		.sRemark = m_sRemark,
		.sKeyword = m_sKeyword,
		.nDpi = m_nDPI,
		.nWidth = atoi(m_sPictWidth),
		.nHeight = atoi(m_sPictHeight),
		.nBit = atoi(m_sPictColor)
	};

	if (m_pItemData == NULL) {
		oDataRec.sFolderID = m_pItemFolder->m_oFolderRec.sFolderID;
		if (!m_pItemFolder->m_pPcsDB->AddDataEx(&oDataRec))
			return;
	} else {
		oDataRec.sFolderID = m_pItemData->m_oDataRec.sFolderID;
		oDataRec.sDataID = m_pItemData->m_oDataRec.sDataID;
		if (!m_pItemData->m_pPcsDB->UpdateDataEx(&oDataRec))
			return;
	}

	m_oDataRec = oDataRec;

	if (m_bUpdatePict) {
		if (m_pItemData == NULL)
			WritePicture(m_pItemFolder->m_pPcsDB, oDataRec.sDataID, m_hDIB);
		else
			WritePicture(m_pItemData->m_pPcsDB, oDataRec.sDataID, m_hDIB);

		PictErase();
	}

	if (m_bUpdateText) {
		if (m_pItemData == NULL)
			m_pItemFolder->m_pPcsDB->WriteTextDataEx(oDataRec.sDataID, m_sTextData);
		else
			m_pItemData->m_pPcsDB->WriteTextDataEx(oDataRec.sDataID, m_sTextData);
	}

	if (m_pItemData == NULL) {
		CAddItemData addData;
		addData.m_pItemFolder = m_pItemFolder;
		addData.m_pDataRec = &oDataRec;
		m_pDocument->UpdateAllViews(NULL, VIEWHINT_ADDITEM, &addData);

		ResetUpdateData();

		AutoNumbering(TRUE);

		m_cCopyData.EnableWindow(!m_oDataRec.sTitle.IsEmpty());
	} else {
		m_pItemData->m_oDataRec = oDataRec;
		m_pDocument->UpdateAllViews(NULL, VIEWHINT_UPDATEITEM, m_pItemData);

		EndDialog(IDOK);
	}

	NextReadFile();
}

void CDataDlg::NextReadFile()
{
	if (m_nReadCount < m_nReadFileName) {
		const CString &fileName = m_pReadFileName[m_nReadCount++];

		HDIB hDIB = NULL;
		if (MMPictureReadFile(&hDIB, (LPSTR)(LPCSTR)fileName) == 0) {
			SCAN_IMAGE_INFO oImageInfo;
			GetImageInfo(hDIB, &oImageInfo);
			UpdateData(TRUE);
			m_sTitle = RemoveDir(fileName);
			m_sTitle = RemoveExt(m_sTitle);
			UpdateData(FALSE);
			SetPicture(hDIB, &oImageInfo);
		}
	}
}

void CDataDlg::OnPictCamera()
{
	GetPicture(PHOTO_CAMERA);
}

void CDataDlg::OnPictScanner()
{
	GetPicture(PHOTO_SCANNER);
}

void CDataDlg::OnPictFile()
{
	GetPicture(PHOTO_FILE);
}

void CDataDlg::OnPictErase()
{
	PictErase();

	m_bUpdatePict = TRUE;
	SetUpdateData();
}

void CDataDlg::PictErase()
{
	if (m_hDIB) {
		::GlobalFree(m_hDIB);
		m_hDIB = NULL;
		m_cPictPicture.DrawDIB(NULL);
		EnableButton();
	}
}

void CDataDlg::GetPicture(int nMode)
{
	if (m_bUpdatePict) {
		if (::AfxMessageBox(IDS_READ_NOTIFY, MB_OKCANCEL) == IDCANCEL)
			return;
	}

	HDIB hDIB = NULL;
	SCAN_IMAGE_INFO oImageInfo;

	switch (nMode) {
	case PHOTO_CAMERA:
		if (m_cAutoSave) {
			PSCD_GetBmpContinue(m_hWnd);
			hDIB = NULL;
		} else {
			if ((hDIB = PSCD_GetBmp(m_hWnd)) != NULL)
				GetImageInfo(hDIB, &oImageInfo);
		}
		break;
	case PHOTO_SCANNER:
		if (m_cAutoSave) {
			ScanBmpContinue(m_hWnd);
			hDIB = NULL;
		} else
			hDIB = ScanBmp(m_hWnd, &oImageInfo);
		break;
	case PHOTO_FILE:
		{
			CFileDialog dlg(TRUE, "bmp;jpg;png", NULL,
				OFN_HIDEREADONLY,
				GetString(IDS_PICTURE_FILE) + " (*.bmp;*.jpg;*.png)|*.bmp;*.jpg;*.png|All Files (*.*)|*.*||", this);

			if (m_cAutoSave) {
				CString sFileBuf;
				dlg.m_ofn.Flags |= OFN_ALLOWMULTISELECT;
				dlg.m_ofn.lpstrTitle = GetString(IDS_SELECT_PICT_FILE_MULTI);
				dlg.m_ofn.lpstrFile = sFileBuf.GetBuffer(32768);
				dlg.m_ofn.nMaxFile = sFileBuf.GetAllocLength();
				if (dlg.DoModal() == IDOK) {
					POSITION pos;
					CString pathName;
					CWaitCursor wait;

					pos = dlg.GetStartPosition();
					while (pos != NULL) {
						UpdateWindow();
						pathName = dlg.GetNextPathName(pos);
						if (MMPictureReadFile(&hDIB, (LPSTR)(LPCSTR)pathName) == 0) {
							m_sTitle = RemoveDir(pathName);
							m_sTitle = RemoveExt(m_sTitle);
							UpdateData(FALSE);
							UpdateWindow();
							OnFrameBmp((WPARAM)hDIB, 0);
						}
					}
					hDIB = NULL;
				} else
					return;
			} else {
				dlg.m_ofn.lpstrTitle = GetString(IDS_SELECT_PICT_FILE);
				if (dlg.DoModal() == IDOK) {
					if (MMPictureReadFile(&hDIB, (LPSTR)(LPCSTR)dlg.GetPathName()) == 0)
						GetImageInfo(hDIB, &oImageInfo);
					else
						::AfxMessageBox(IDS_READ_PICT_FILE_FAIL);
				} else
					return;
			}
		}
		break;
	case PHOTO_CLIPBOARD:
		if (OpenClipboard()) {
			HANDLE hMem;
			if ((hMem = GetClipboardData(CF_BITMAP)) != NULL)
				hDIB = GetDIBFromBmp((HBITMAP)hMem);

			if ((hMem = GetClipboardData(CF_TEXT)) != NULL) {
				char *pText = (char *)::GlobalLock(hMem);
				m_sTextData = pText;
				m_bUpdateText = TRUE;
				SetUpdateData();
				::GlobalUnlock(hMem);
			}

			::CloseClipboard();
		}
		break;
	case PHOTO_WINDOW:
		if (m_hWndSel != NULL) {
			CWnd *pWnd = FromHandle(m_hWndSel);

			DrawWindowFrame(pWnd);
			pWnd->SetForegroundWindow();
			pWnd->UpdateWindow();

			hDIB = GetWindowDIB(FromHandle(m_hWndSel));

			if (GetWindowTitle(pWnd, m_sTitle))
				UpdateData(FALSE);
		}
		break;
	}

	if (hDIB == NULL)
		return;

	SetPicture(hDIB, &oImageInfo);
}

LRESULT CDataDlg::OnScanBmp(WPARAM wParam, LPARAM lParam)
{
	HDIB hDIB = (HDIB)wParam;

	const int nRotation = m_cAutoRotation.GetCurSel();
	if (nRotation >= 1 && nRotation <= 3)
		MMPictureRotate2(hDIB, &hDIB, (SHORT)nRotation);

	SetPicture(hDIB, (SCAN_IMAGE_INFO *)lParam);
	m_cPictPicture.UpdateWindow();

	OnSave();

	return 0;
}

LRESULT CDataDlg::OnFrameBmp(WPARAM wParam, LPARAM /*lParam*/)
{
	HDIB hDIB = (HDIB)wParam;

	const int nRotation = m_cAutoRotation.GetCurSel();
	if (nRotation >= 1 && nRotation <= 3)
		MMPictureRotate2(hDIB, &hDIB, (SHORT)nRotation);

	SCAN_IMAGE_INFO oImageInfo;
	GetImageInfo(hDIB, &oImageInfo);

	SetPicture(hDIB, &oImageInfo);
	m_cPictPicture.UpdateWindow();

	OnSave();

	return 0;
}

void CDataDlg::SetPicture(HDIB hDIB, const SCAN_IMAGE_INFO *pImageInfo)
{
	if (CheckColorMode(hDIB)) {
		if (m_hDIB != NULL)
			::GlobalFree(m_hDIB);
		m_hDIB = hDIB;

		UpdateData(TRUE);
		m_sPictWidth.Format("%d", pImageInfo->ImageWidth);
		m_sPictHeight.Format("%d", pImageInfo->ImageLength);
		m_sPictColor.Format("%d", pImageInfo->BitsPerPixel);
		if (pImageInfo->XResolution != 0) {
			LPBITMAPINFO lpBmi = (LPBITMAPINFO)::GlobalLock(m_hDIB);
			if (lpBmi != NULL && lpBmi->bmiHeader.biXPelsPerMeter == 0) {
				lpBmi->bmiHeader.biXPelsPerMeter = (long)(pImageInfo->XResolution / 0.0254 + 0.5);
				lpBmi->bmiHeader.biYPelsPerMeter = (long)(pImageInfo->YResolution / 0.0254 + 0.5);
			}
			::GlobalUnlock(m_hDIB);

			m_sPictDPI.Format("%d", pImageInfo->XResolution);
		} else
			m_sPictDPI.LoadString(IDS_UNKNOWN);
		UpdateData(FALSE);

		m_nDPI = pImageInfo->XResolution;

		m_cPictPicture.DrawDIB(m_hDIB);

		m_bUpdatePict = TRUE;
		SetUpdateData();
		EnableButton();
	} else
		::GlobalFree(hDIB);
}

void CDataDlg::GetImageInfo(HDIB hDIB, SCAN_IMAGE_INFO *pImageInfo)
{
	if (LPBITMAPINFO pBmi = (LPBITMAPINFO)::GlobalLock(hDIB); pBmi != NULL) {
		pImageInfo->XResolution = (int)(pBmi->bmiHeader.biXPelsPerMeter * 0.0254 + 0.5);
		pImageInfo->YResolution = (int)(pBmi->bmiHeader.biYPelsPerMeter * 0.0254 + 0.5);
		pImageInfo->ImageWidth = pBmi->bmiHeader.biWidth;
		pImageInfo->ImageLength = pBmi->bmiHeader.biHeight;
		pImageInfo->BitsPerPixel = pBmi->bmiHeader.biBitCount;
		::GlobalUnlock(hDIB);
	}
}

BOOL CDataDlg::CheckColorMode(HDIB hDIB)
{
	BOOL bResult = FALSE;

	if (LPBITMAPINFO pBmi = (LPBITMAPINFO)GlobalLock(hDIB); pBmi != NULL) {
		if (pBmi->bmiHeader.biCompression != 0) {
			::AfxMessageBox(IDS_ONLY_COMPRESS, MB_ICONEXCLAMATION);
		} else {
			switch (pBmi->bmiHeader.biBitCount) {
			case 1:
			case 4:
			case 8:
			case 24:
				bResult = TRUE;
				break;
			default:
				::AfxMessageBox(IDS_VALID_COLOR, MB_ICONEXCLAMATION);
				break;
			}
		}

		::GlobalUnlock(hDIB);
	}

	return bResult;
}

void CDataDlg::OnPictRotation()
{
	if (m_hDIB != NULL) {
		MMPictureRotate2(m_hDIB, &m_hDIB, 1);
		m_cPictPicture.DrawDIB(m_hDIB);

		short nWidth, nHeight, nColor;
		MMPictureGetInfo(m_hDIB, &nWidth, &nHeight, &nColor);
		m_sPictWidth.Format("%d", nWidth);
		m_sPictHeight.Format("%d", nHeight);
		UpdateData(FALSE);

		m_bUpdatePict = TRUE;
		SetUpdateData();
	}
}

void CDataDlg::OnOK()
{
	ExitDialog();
}

void CDataDlg::OnCancel()
{
	ExitDialog();
}

void CDataDlg::ExitDialog()
{
	if (m_bUpdateData) {
		UINT nMsg;

		if (m_pItemData == NULL)
			nMsg = IDS_INPUT_DATA_SAVE;
		else
			nMsg = IDS_UPDATE_DATA_SAVE;

		const int rc = ::AfxMessageBox(nMsg, MB_ICONQUESTION | MB_YESNOCANCEL);
		switch (rc) {
		case IDYES:
			OnSave();
			EndDialog(IDOK);
			break;
		case IDNO:
			EndDialog(IDCANCEL);
			break;
		}
	} else
		EndDialog(IDCANCEL);
}

void CDataDlg::SetUpdateData()
{
	if (!m_bUpdateData) {
		m_bUpdateData = TRUE;
		m_cSave.EnableWindow(TRUE);
	}
}

void CDataDlg::ResetUpdateData()
{
	if (m_bUpdateData) {
		m_bUpdateData = FALSE;
		m_bUpdatePict = FALSE;
		m_bUpdateSound = FALSE;
		m_bUpdateText = FALSE;
		m_cSave.EnableWindow(FALSE);
	}
}

void CDataDlg::OnChangeTitle()
{
	SetUpdateData();
}

void CDataDlg::OnChangeCode()
{
	SetUpdateData();
}

void CDataDlg::OnChangeCreator()
{
	SetUpdateData();
}

void CDataDlg::OnChangeRemark()
{
	SetUpdateData();
}

void CDataDlg::OnChangeKeyword()
{
	SetUpdateData();
}

void CDataDlg::EnableButton()
{
	m_cPictRotation.EnableWindow(m_hDIB != NULL);
	m_cPictErase.EnableWindow(m_hDIB != NULL);
}

void CDataDlg::OnAutoNumbering()
{
	CAutoNumDlg dlg;

	if (m_pItemFolder->m_oFolderRec.sCodeDef.IsEmpty()) {
		UpdateData(TRUE);
		dlg.m_bAutoNumbering = FALSE;
		dlg.m_sCode = m_sCode;
		dlg.m_lStep = 10;
	} else {
		dlg.m_bAutoNumbering = TRUE;
		dlg.m_sCode = m_pItemFolder->m_oFolderRec.sCodeDef;
		dlg.m_lStep = m_pItemFolder->m_oFolderRec.nCodeStep;
	}

	if (dlg.DoModal() == IDCANCEL)
		return;

	if (dlg.m_bAutoNumbering) {
		m_pItemFolder->m_oFolderRec.sCodeDef = dlg.m_sCode;
		m_pItemFolder->m_oFolderRec.nCodeStep = dlg.m_lStep;
		AutoNumbering(FALSE);
	} else {
		m_pItemFolder->m_oFolderRec.sCodeDef.Empty();
		m_pItemFolder->m_oFolderRec.nCodeStep = 0;
	}
	m_pItemFolder->m_pPcsDB->UpdateFolderEx(&m_pItemFolder->m_oFolderRec);
}

void CDataDlg::OnAutoSave()
{
	BOOL bAutoSave = m_cAutoSave;
	theApp.WriteProfileInt(g_sSectionSettings, "AutoSave", bAutoSave);
	m_cAutoRotation.EnableWindow(bAutoSave);

	if (bAutoSave)
		m_cPictClipBoard.EnableWindow(TRUE);
	else {
		m_cPictClipBoard.EnableWindow(CheckClipBoard());
		m_cPictClipBoard.SetCheck(0);
	}
}

void CDataDlg::AutoNumbering(BOOL bNext)
{
	if (m_pItemFolder->m_oFolderRec.sCodeDef.IsEmpty())
		return;

	if (bNext)
		AutoNextNumbering(m_pItemFolder->m_oFolderRec.sCodeDef, m_pItemFolder->m_oFolderRec.nCodeStep);

	m_sCode = m_pItemFolder->m_oFolderRec.sCodeDef;

	m_pItemFolder->m_pPcsDB->UpdateFolderEx(&m_pItemFolder->m_oFolderRec);

	UpdateData(FALSE);
}

void CDataDlg::OnTextData()
{
	CTextDlg dlg;

	dlg.m_sText = m_sTextData;

	if (dlg.DoModal() == IDOK) {
		m_sTextData = dlg.m_sText;
		m_bUpdateText = TRUE;
		SetUpdateData();
	}
}

void CDataDlg::OnCopyData()
{
	m_sCode = m_oDataRec.sCode;
	m_sCreator = m_oDataRec.sCreator;
	m_sRemark = m_oDataRec.sRemark;
	m_sKeyword = m_oDataRec.sKeyword;
	m_sTitle = m_oDataRec.sTitle;
	UpdateData(FALSE);

	SetUpdateData();
}

void CDataDlg::OnPictClipboard()
{
	if (!m_cPictClipBoard.GetCheck()) {
		if (m_cAutoSave)
			m_cPictClipBoard.SetCheck(1);
		else
			GetPicture(PHOTO_CLIPBOARD);
	} else
		m_cPictClipBoard.SetCheck(0);
}

void CDataDlg::OnDrawClipboard()
{
	if (m_cPictClipBoard.GetCheck()) {
		GetPicture(PHOTO_CLIPBOARD);
		OnSave();
	} else
		m_cPictClipBoard.EnableWindow(CheckClipBoard());

	if (m_hWndNext)
		::SendMessage(m_hWndNext, WM_DRAWCLIPBOARD, 0, 0);
}

BOOL CDataDlg::CheckClipBoard()
{
	BOOL bOK = FALSE;

	if (OpenClipboard()) {
		UINT cf = 0;
		while ((cf = ::EnumClipboardFormats(cf)) != 0) {
			if (cf == CF_BITMAP || cf == CF_DIB || cf == CF_TEXT) {
				bOK = TRUE;
				break;
			}
		}
		::CloseClipboard();
	}

	return bOK;
}

void CDataDlg::OnChangeCbChain(HWND hWndRemove, HWND hWndAfter)
{
	if (hWndRemove == m_hWndNext)
		m_hWndNext = hWndAfter;

	if (m_hWndNext)
		::SendMessage(m_hWndNext, WM_CHANGECBCHAIN, (WPARAM)hWndRemove, (LPARAM)hWndAfter);
}

void CDataDlg::OnDestroy()
{
	CDialogAF::OnDestroy();

	ChangeClipboardChain(m_hWndNext);
}

HDIB CDataDlg::GetDIBFromBmp(HBITMAP hBmp)
{
	HDIB hDIB8, hDIB24;
	MMPictureGetBitmap8(&hDIB8, hBmp);
	MMPictureGetBitmap(&hDIB24, hBmp);

	long nSize8, nSize24;
	MMPictureWriteNull(hDIB8, MMTYPE_PNG, &nSize8);
	MMPictureWriteNull(hDIB24, MMTYPE_JPG, &nSize24);

	if (nSize8 < nSize24) {
		MMPictureFree(hDIB24);
		return hDIB8;
	} else {
		MMPictureFree(hDIB8);
		return hDIB24;
	}
}

LRESULT CDataDlg::OnPictWindow(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (!m_cPictWindow.GetCheck()) {
		SetCapture();
		HCURSOR hCursor = theApp.LoadCursor(IDC_SELECT_WINDOW);
		SetCursor(hCursor);
		m_cPictWindow.SetCheck(1);
		::AfxGetMainWnd()->ShowWindow(SW_HIDE);
		m_hWndSel = NULL;
	} else {
		ReleaseCapture();
		m_cPictWindow.SetCheck(0);
	}

	return 0;
}

void CDataDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_cPictWindow.GetCheck()) {
		ClientToScreen(&point);

		CWnd *pWnd = WindowFromPoint(point);

		if (pWnd) {
			CWnd *pWndParent = GetTopParentWindow(pWnd);
			if (pWndParent->m_hWnd == ::AfxGetMainWnd()->m_hWnd)
				pWnd = NULL;
		}
		if (m_hWndSel != pWnd->GetSafeHwnd()) {
			if (m_hWndSel != NULL)
				DrawWindowFrame(FromHandle(m_hWndSel));

			if (pWnd != NULL)
				DrawWindowFrame(pWnd);

			m_hWndSel = pWnd->GetSafeHwnd();
		}
	}

	CDialogAF::OnMouseMove(nFlags, point);
}

void CDataDlg::DrawWindowFrame(CWnd *pWnd)
{
	CPen pen(PS_SOLID, 6, RGB(0, 0, 255));

	CDC *pDC = pWnd->GetWindowDC();

	CRect rect;
	pWnd->GetWindowRect(rect);

	const int nDrawModeOrg = pDC->SetROP2(R2_NOTXORPEN);
	CPen *pPenOrg = pDC->SelectObject(&pen);

	pDC->Rectangle(0, 0, rect.Width(), rect.Height());

	pDC->SelectObject(pPenOrg);
	pDC->SetROP2(nDrawModeOrg);

	pWnd->ReleaseDC(pDC);
}

void CDataDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_cPictWindow.GetCheck()) {
		ReleaseCapture();

		if (m_hWndSel != NULL) {
			GetPicture(PHOTO_WINDOW);
			if (m_cAutoSave.GetCheck())
				OnSave();
		}

		m_cPictWindow.SetCheck(0);
		::AfxGetMainWnd()->ShowWindow(SW_SHOW);
		SetForegroundWindow();
	}

	CDialogAF::OnLButtonUp(nFlags, point);
}

HDIB CDataDlg::GetWindowDIB(CWnd *pWnd)
{
	CRect rect;
	pWnd->GetWindowRect(&rect);
	const int nWidth = rect.Width();
	const int nHeight = rect.Height();

	CDC *pDC = pWnd->GetWindowDC();
	CDC DCMem;
	DCMem.CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, nWidth, nHeight);

	CGdiObject *pOrg = DCMem.SelectObject(&bmp);
	DCMem.BitBlt(0, 0, nWidth, nHeight, pDC, 0, 0, SRCCOPY);
	DCMem.SelectObject(pOrg);

	pWnd->ReleaseDC(pDC);

	HDIB hDIB;
	MMPictureGetBitmap8(&hDIB, bmp);

	return hDIB;
}

CWnd *CDataDlg::GetTopParentWindow(CWnd *pWnd)
{
	CWnd *pWndParent = pWnd;
	while (CWnd *pWnd2 = pWndParent->GetParent())
		pWndParent = pWnd2;

	return pWndParent;
}

BOOL CDataDlg::GetWindowTitle(CWnd *pWnd, CString &sTitle)
{
	CString sText;

	while (true) {
		pWnd->GetWindowText(sText);
		if (!sText.IsEmpty()) {
			sTitle = sText;
			return TRUE;
		}

		if ((pWnd = pWnd->GetParent()) == NULL)
			return FALSE;
	}
}