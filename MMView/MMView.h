// MMView.h : MMVIEW アプリケーションのメイン ヘッダー ファイル
//

#pragma once

#include "resource.h"       // メイン シンボル
#include "pcscl.h"
#include "mmdb.h"
#include "Mml.h"
#include "Item.h"
#include "Global.h"
#include "Dpi.h"

/////////////////////////////////////////////////////////////////////////////
// CMMViewApp:
// このクラスの動作の定義に関しては MMView.cpp ファイルを参照してください。
//

class CMMViewApp : public CWinApp
{
public:
	HCURSOR	m_hCursorArrow;
	HCURSOR	m_hCursorHandOpen;
	HCURSOR	m_hCursorHandClose;

protected:
	HICON m_hIconApp;
	CString m_sExeFileName;

	DECLARE_MESSAGE_MAP()
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	afx_msg void OnAppAbout();
	afx_msg void OnOnlineUpdate();
	afx_msg void OnOnlineManual();
};
