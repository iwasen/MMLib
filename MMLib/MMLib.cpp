// MMLib.cpp : アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"
#include "MMLib.h"
#include "MainFrm.h"
#include "MMLibDoc.h"
#include "MMLibTreeView.h"
#include "MMLibDB.h"
#include "UpdateAp.h"
#include "PictFrame.h"
#include "PictDoc.h"
#include "PictView.h"
#include "FindKeywordFrame.h"
#include "FindKeywordDoc.h"
#include "FindKeywordView.h"
#include "VerRes.h"
#include "HyperLink.h"
#include "Regist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DDE_TEXT	1
#define DDE_DATA	2

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogAF
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	m_cProductURL;
	CHyperLink	m_cEmailAddr;
	CHyperLink	m_cCompanyURL;

	// ClassWizard 仮想関数のオーバーライドを生成します。
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // // DDX/DDV のサポート

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogAF(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRODUCT_URL, m_cProductURL);
	DDX_Control(pDX, IDC_EMAIL_ADDR, m_cEmailAddr);
	DDX_Control(pDX, IDC_COMPANY_URL, m_cCompanyURL);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogAF)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialogAF::OnInitDialog();

	CVerRes res;
	CString sVersion = res.GetProductName() + "  Version " + res.GetProductVersion();
#ifdef _WIN64
	sVersion += " (64bit)";
#else
	sVersion += " (32bit)";
#endif
	SetDlgItemText(IDC_PRODUCT, sVersion);
	SetDlgItemText(IDC_COPYRIGHT, res.GetLegalCopyright());

	m_cProductURL.SetHyperLinkURL(NULL);
	m_cCompanyURL.SetHyperLinkURL(NULL);
	m_cEmailAddr.SetHyperLinkURL("mailto:shop@ymec.com");

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMMLibApp

BEGIN_MESSAGE_MAP(CMMLibApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(IDM_ONLINE_UPDATE, OnOnlineUpdate)
	ON_COMMAND(IDM_ONLINE_MANUAL, OnOnlineManual)
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 標準の印刷セットアップ コマンド
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMMLibApp クラスの構築

CMMLibApp::CMMLibApp()
{
	m_dwIdInst = 0;

	m_nCheckData = GET_CHECK_DATA;
	if (GetCheckData2() == m_nCheckData)
		m_pRegistryName = g_sRegistryName;
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CMMLibApp オブジェクト

CMMLibApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMMLibApp クラスの初期化

BOOL CMMLibApp::InitInstance()
{
	//* プログラムの複数起動抑止
/*
	{
		CWnd *pWnd = CWnd::FindWindow(NULL, "Multimedia Library Manager");
		if (pWnd != NULL) {
			pWnd->ShowWindow(SW_RESTORE);
			pWnd->SetForegroundWindow();
			return FALSE;
		}
	}
*/
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	AfxEnableControlContainer();

	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さく
	//  したければ以下の特定の初期化ルーチンの中から不必要なもの
	//  を削除してください。
	CDpi::InitDpi();

	g_hIcon = LoadIcon(IDR_MAINFRAME);

	if (strstr(m_lpCmdLine, "/C") == NULL) {
		CVerRes res;
		if (!CHECK_LICENSE0(0x01, res.GetProductVersion() + "/MMLIB"))
			return FALSE;
	} else
		AllowSetForegroundWindow(ASFW_ANY);

	g_oImageList.Create(IDB_IMAGE_LIST_SMALL, 16, 1, RGB(255, 255, 255));
	g_oImageList2.Create(IDB_IMAGE_LIST_LARGE, 32, 1, RGB(255, 255, 255));

	// 設定が保存される下のレジストリ キーを変更します。
	// 会社名または所属など、適切な文字列に
	// 変更してください。
	SetRegistryKey(_T("Yoshimasa Electronic Inc.\\Multimedia Library"));

	// 旧バージョンのiniファイルからレジストリへコピー
	if (GetProfileInt(g_sSectionMainWindow, "Left", -1) == -1)
		CopyInifileToRegistory();

	LoadStdProfileSettings();  // 標準の INI ファイルのオプションをローﾄﾞします (MRU を含む)

	::AfxOleInit();
//	::AfxInitRichEdit();

	g_pMMLibDB = new CMMLibDB;
	g_nCFMMLibrary = RegisterClipboardFormat("MMLibrary");

	// カーソルのロード
	m_hCursorArrow = LoadStandardCursor(IDC_ARROW);
	m_hCursorHandOpen = LoadCursor(IDC_HANDOPEN);
	m_hCursorHandClose = LoadCursor(IDC_HANDCLOSE);

	g_oDefaultFont.CreatePointFont(90, GetString(IDS_DEFAULT_FONT));

	m_hIconApp = LoadIcon(IDR_MAINFRAME);

	g_sViewFontName = GetProfileString(g_sSectionOptions, "ViewFontName", GetString(IDS_DEFAULT_FONT));
	g_nViewFontSize = GetProfileInt(g_sSectionOptions, "ViewFontSize", 90);
	g_nFontSize = GetProfileInt(g_sSectionOptions, "FontSize", 0);
	g_sImageEditor = GetProfileString(g_sSectionOptions, "ImageEditor");
	g_rcPrintMargin.left = GetProfileInt(g_sSectionSettings, "PrintMarginLeft", 0);
	g_rcPrintMargin.top = GetProfileInt(g_sSectionSettings, "PrintMarginTop", 0);
	g_rcPrintMargin.right= GetProfileInt(g_sSectionSettings, "PrintMarginRight", 0);
	g_rcPrintMargin.bottom = GetProfileInt(g_sSectionSettings, "PrintMarginBottom", 0);
	g_bPrintMargin = GetProfileInt(g_sSectionSettings, "PrintMargin", 1);

	::GetModuleFileName(m_hInstance, m_sExeFileName.GetBuffer(MAX_PATH), MAX_PATH);
	m_sExeFileName.ReleaseBuffer();

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMMLibDoc),
		RUNTIME_CLASS(CMainFrame),       // メイン SDI フレーム ウィンドウ
		RUNTIME_CLASS(CMMLibTreeView));
	AddDocTemplate(pDocTemplate);

	m_pPictDocTemplate = new CSingleDocTemplate(
		IDR_PICTFRAME,
		RUNTIME_CLASS(CPictDoc),
		RUNTIME_CLASS(CPictFrame),       // 画像 SDI フレーム ウィンドウ
		RUNTIME_CLASS(CPictView));
	AddDocTemplate(m_pPictDocTemplate);

	m_pFindDocTemplate = new CSingleDocTemplate(
		IDR_FINDFRAME,
		RUNTIME_CLASS(CFindKeywordDoc),
		RUNTIME_CLASS(CFindKeywordFrame),       // 検索結果 SDI フレーム ウィンドウ
		RUNTIME_CLASS(CFindKeywordView));
	AddDocTemplate(m_pFindDocTemplate);

	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CMyCmdLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// コマンドラインでディスパッチ コマンドを指定します。
//	if (!ProcessShellCommand(cmdInfo))
//		return FALSE;
	pDocTemplate->OpenDocumentFile(NULL);

	CString sTitle;
	m_pMainWnd->GetWindowText(sTitle);
#ifdef _WIN64
	sTitle += " (64bit)";
#else
	sTitle += " (32bit)";
#endif
	m_pMainWnd->SetWindowText(sTitle);

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	int nShow = GetProfileInt(g_sSectionMainWindow, "Show", SW_SHOW);
	if (nShow == SW_SHOWMINIMIZED)
		nShow = SW_SHOW;
	m_pMainWnd->ShowWindow(nShow);
	m_pMainWnd->UpdateWindow();

	OpenDocumentFile("Current");

	InitDDE();

	return TRUE;
}

int CMMLibApp::ExitInstance()
{
	if (g_pMMLibDB != NULL) {
		g_pMMLibDB->ExitMMLibrary();
		delete g_pMMLibDB;
	}

	::DeleteTempFile(g_sTempFile);

	UninitDDE();

	WriteProfileInt(g_sSectionSettings, "PrintMarginLeft", g_rcPrintMargin.left);
	WriteProfileInt(g_sSectionSettings, "PrintMarginTop", g_rcPrintMargin.top);
	WriteProfileInt(g_sSectionSettings, "PrintMarginRight", g_rcPrintMargin.right);
	WriteProfileInt(g_sSectionSettings, "PrintMarginBottom", g_rcPrintMargin.bottom);
	WriteProfileInt(g_sSectionSettings, "PrintMargin", g_bPrintMargin);

	return CWinApp::ExitInstance();
}

static HDDEDATA CALLBACK DdeCallback(
	UINT uType,     // トランザクション型
	UINT /*uFmt*/,      // クリップボードデータ形式
	HCONV /*hconv*/,    // 対話のハンドル
	HSZ hsz1,       // 文字列のハンドル
	HSZ /*hsz2*/,       // 文字列のハンドル
	HDDEDATA hdata, // グローバルメモリオブジェクトのハンドル
	DWORD /*dwData1*/,  // トランザクション固有データ
	DWORD /*dwData2*/   // トランザクション固有データ
	)
{
	DWORD	cbData;
	LPSTR lpStr;
	struct MMLIBDATA {
		struct {
			char	title[64];
			char	comment[64];
		} folder;
		struct {
			char	title[64];
			char	comment[64];
		} data;
		char	text[65536];
		DWORD	size;
		char	dib[0];
	} *pMmlibData;

	switch (uType) {
	case XTYP_CONNECT:
		if (hsz1 == theApp.m_hTopicText) {
			theApp.m_nDdeMode = DDE_TEXT;
			return (HDDEDATA)TRUE;
		} else if (hsz1 == theApp.m_hTopicData) {
			theApp.m_nDdeMode = DDE_DATA;
			return (HDDEDATA)TRUE;
		}
		break;
	case XTYP_EXECUTE:
		switch (theApp.m_nDdeMode) {
		case DDE_TEXT:
			lpStr = (LPSTR)DdeAccessData(hdata, &cbData);
			{
				CStdioFile file;
				if (file.Open(lpStr, CFile::modeRead | CFile::shareDenyNone)) {
					theApp.m_pDocument->UpdateAllViews(NULL, VIEWHINT_TEXTFILE, &file);
					file.Close();
				}
			}
			DdeUnaccessData(hdata);
			return (HDDEDATA)DDE_FACK;
		case DDE_DATA:
			pMmlibData = (MMLIBDATA *)DdeAccessData(hdata, &cbData);
			HDIB hDIB = ::GlobalAlloc(GMEM_MOVEABLE, pMmlibData->size);
			LPBYTE pDib = (LPBYTE)GlobalLock(hDIB);
			memcpy(pDib, pMmlibData->dib, pMmlibData->size);
			::GlobalUnlock(hDIB);
			((CMMLibDoc *)theApp.m_pDocument)->InsertData(
					pMmlibData->folder.title, pMmlibData->folder.comment,
					pMmlibData->data.title, pMmlibData->data.comment, hDIB, pMmlibData->text);
			::GlobalFree(hDIB);
			DdeUnaccessData(hdata);
			return (HDDEDATA)DDE_FACK;
		}
		break;
	}
	return 0;
}

void CMMLibApp::InitDDE()
{
	if (DdeInitialize(&m_dwIdInst,
			(PFNCALLBACK)MakeProcInstance((FARPROC)DdeCallback, hInst),
			APPCMD_FILTERINITS, NULL) != 0)
		return;

	m_hService = DdeCreateStringHandle(m_dwIdInst, "MMLIB", CP_WINANSI);
	m_hTopicText = DdeCreateStringHandle(m_dwIdInst, "text", 0);
	m_hTopicData = DdeCreateStringHandle(m_dwIdInst, "data", 0);

	DdeNameService(m_dwIdInst, m_hService, NULL, DNS_REGISTER);
}

void CMMLibApp::UninitDDE()
{
	if (m_hService)
		DdeFreeStringHandle(m_dwIdInst, m_hService);

	if (m_hTopicText)
		DdeFreeStringHandle(m_dwIdInst, m_hTopicText);

	if (m_hTopicData)
		DdeFreeStringHandle(m_dwIdInst, m_hTopicData);

	if (m_dwIdInst)
		DdeUninitialize(m_dwIdInst);
}

void CMMLibApp::OnOnlineUpdate()
{
	OnlineUpdateFunc pOnlineUpdate;
	HINSTANCE hModule;

#ifdef _DEBUG
	if ((hModule = ::LoadLibrary("YMUPDATED.DLL")) == NULL) {
#else
	if ((hModule = ::LoadLibrary("YMUPDATE.DLL")) == NULL) {
#endif
		::AfxMessageBox(IDS_NOT_FOUND_YMUPDATE_DLL);
		return;
	}

	if ((pOnlineUpdate = (OnlineUpdateFunc)GetProcAddress(hModule, "OnlineUpdate")) == NULL) {
		::AfxMessageBox(IDS_ONLINE_UPDATE_FAIL);
		::FreeLibrary(hModule);
		return;
	}

	CString sOnlineUpdate;
	sOnlineUpdate.LoadString(IDS_ONLINE_UPDATE);
#ifdef _WIN64
	sOnlineUpdate += "64";
#else
	sOnlineUpdate += "32";
#endif

	pOnlineUpdate(sOnlineUpdate, m_sExeFileName, m_lpCmdLine, m_hIconApp, "Multimedia\nLibrary", g_sRegistryName);
}

void CMMLibApp::OnOnlineManual()
{
	ShellExecute(NULL, "open", GetString(IDS_HELP_URL), NULL, NULL, SW_SHOWNORMAL);
}

void CMMLibApp::CopyInifileToRegistory()
{
	static const char sIniFile[] = "MMLib.ini";
	char sSections[1024];
	char sEntries[1024];

	GetPrivateProfileSectionNames(sSections, sizeof(sSections), sIniFile);
	for (char *pSection = sSections; *pSection != '\0'; pSection += strlen(pSection) + 1) {
		GetPrivateProfileSection(pSection, sEntries, sizeof(sEntries), sIniFile);
		for (char *pEntry = sEntries; *pEntry != '\0'; pEntry += strlen(pEntry) + 1) {
			char *pValue;
			if ((pValue = strchr(pEntry, '=')) != NULL) {
				*pValue++ = '\0';
				if (strspn(pValue, "0123456789-") == strlen(pValue))
					WriteProfileInt(pSection, pEntry, atoi(pValue));
				else
					WriteProfileString(pSection, pEntry, pValue);
			}
		}
	}
}

// ダイアログを実行するためのアプリケーション コマンド
void CMMLibApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMMLibApp コマンド

void CMMLibApp::OnFileNew()
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CWinApp::OnFileNew();
}

//////////////////////////////////////////////////////////////////////
// CMyCmdLineInfo クラス
//////////////////////////////////////////////////////////////////////

void CMyCmdLineInfo::ParseParam(LPCTSTR lpszParam, BOOL /*bFlag*/, BOOL /*bLast*/)
{
	if (_stricmp(lpszParam, "offline") == 0)
		g_bParamOffline = TRUE;
}
