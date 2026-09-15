// CheckPwdDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MMLib.h"
#include "CheckPwdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckPwdDlg ダイアログ

CCheckPwdDlg::CCheckPwdDlg(CWnd* pParent /*=NULL*/)
	: CDialogAF(CCheckPwdDlg::IDD, pParent)
{
}

void CCheckPwdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogAF::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LIBRARY, m_sLibrary);
	DDX_Text(pDX, IDC_PASSWORD, m_sPassword);
}

BEGIN_MESSAGE_MAP(CCheckPwdDlg, CDialogAF)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckPwdDlg メッセージ ハンドラ
