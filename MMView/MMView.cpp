// MMView.cpp : アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"
#include "MMView.h"
#include "MainFrm.h"
#include "MMViewDoc.h"
#include "MMPictView.h"
#include "UpdateAp.h"
#include "VerRes.h"
#include "HyperLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート

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

	CVerRes		res;
	CString sVersion;
	sVersion = res.GetProductName() + "  Version " + res.GetProductVersion();
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
// CMMViewApp

BEGIN_MESSAGE_MAP(CMMViewApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(IDM_ONLINE_UPDATE, OnOnlineUpdate)
	ON_COMMAND(IDM_ONLINE_MANUAL, OnOnlineManual)
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 標準の印刷セットアップ コマンド
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// 唯一の CMMViewApp オブジェクト

CMMViewApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMMViewApp クラスの初期化

BOOL CMMViewApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	AfxEnableControlContainer();

	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さく
	// したければ以下の特定の初期化ルーチンの中から不必要なもの
	// を削除してください。

	// 設定が保存される下のレジストリ キーを変更します。
	// TODO: この文字列を、会社名または所属など適切なものに
	// 変更してください。
	SetRegistryKey(_T("Yoshimasa Electronic Inc.\\Multimedia Library"));

	LoadStdProfileSettings();  // 標準の INI ファイルのオプションをローﾄﾞします (MRU を含む)

	g_ImageList.Create(IDB_IMAGE_LIST_SMALL, 16, 1, RGB(255, 255, 255));
	g_ImageList2.Create(IDB_IMAGE_LIST_LARGE, 32, 1, RGB(255, 255, 255));

	g_rcPrintMargin.left = GetProfileInt(g_sSectionSettings, "PrintMarginLeft", 0);
	g_rcPrintMargin.top = GetProfileInt(g_sSectionSettings, "PrintMarginTop", 0);
	g_rcPrintMargin.right= GetProfileInt(g_sSectionSettings, "PrintMarginRight", 0);
	g_rcPrintMargin.bottom = GetProfileInt(g_sSectionSettings, "PrintMarginBottom", 0);

	CDpi::InitDpi();

	// カーソルのロード
	m_hCursorArrow = LoadStandardCursor(IDC_ARROW);
	m_hCursorHandOpen = LoadCursor(IDC_HANDOPEN);
	m_hCursorHandClose = LoadCursor(IDC_HANDCLOSE);

	m_hIconApp = LoadIcon(IDR_MAINFRAME);

	::GetModuleFileName(m_hInstance, m_sExeFileName.GetBuffer(MAX_PATH), MAX_PATH);
	m_sExeFileName.ReleaseBuffer();

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMMViewDoc),
		RUNTIME_CLASS(CMainFrame),       // メイン SDI フレーム ウィンドウ
		RUNTIME_CLASS(CMMPictView));
	AddDocTemplate(pDocTemplate);

	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// コマンドラインでディスパッチ コマンドを指定します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	CString sTitle;
	m_pMainWnd->GetWindowText(sTitle);
#ifdef _WIN64
	sTitle += " (64bit)";
#else
	sTitle += " (32bit)";
#endif
	m_pMainWnd->SetWindowText(sTitle);

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	m_pMainWnd->ShowWindow(GetProfileInt(g_sSectionMainWindow, "Show", SW_SHOW));
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

int CMMViewApp::ExitInstance()
{
	WriteProfileInt(g_sSectionSettings, "PrintMarginLeft", g_rcPrintMargin.left);
	WriteProfileInt(g_sSectionSettings, "PrintMarginTop", g_rcPrintMargin.top);
	WriteProfileInt(g_sSectionSettings, "PrintMarginRight", g_rcPrintMargin.right);
	WriteProfileInt(g_sSectionSettings, "PrintMarginBottom", g_rcPrintMargin.bottom);

	return CWinApp::ExitInstance();
}

// ダイアログを実行するためのアプリケーション コマンド
void CMMViewApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMMViewApp メッセージ ハンドラ

void CMMViewApp::OnOnlineUpdate()
{
	OnlineUpdateFunc pOnlineUpdate;
	HINSTANCE hModule;

	if ((hModule = LoadLibrary("YMUPDATE.DLL")) == NULL) {
		AfxMessageBox(IDS_NOT_FOUND_YMUPDATE_DLL);
		return;
	}

	if ((pOnlineUpdate = (OnlineUpdateFunc)GetProcAddress(hModule, "OnlineUpdate")) == NULL) {
		AfxMessageBox(IDS_ONLINE_UPDATE_FAIL);
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
	pOnlineUpdate(sOnlineUpdate, m_sExeFileName, m_lpCmdLine, m_hIconApp, "MMViewer", "Multimedia Library");
}

void CMMViewApp::OnOnlineManual()
{
	ShellExecute(NULL, "open", GetString(IDS_HELP_URL), NULL, NULL, SW_SHOWNORMAL);
}
