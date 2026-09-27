// MMLib.h : MMLIB アプリケーションのメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

#include "pcscl.h"
#include "mmdb.h"

#include "Define.h"
#include "DbRec.h"
#include "Global.h"
#include "MyCtrl.h"
#include "ddeml.h"
#include "Dpi.h"

/////////////////////////////////////////////////////////////////////////////
// CMMLibApp:
// このクラスの動作の定義に関しては MMLib.cpp ファイルを参照してください。
//

class CMMLibApp : public CWinApp
{
public:
	CMMLibApp();

	HCURSOR	m_hCursorArrow;
	HCURSOR	m_hCursorHandOpen;
	HCURSOR	m_hCursorHandClose;
	CSingleDocTemplate *m_pPictDocTemplate;
	CSingleDocTemplate *m_pFindDocTemplate;
	HSZ m_hTopicText;
	HSZ m_hTopicData;
	CDocument *m_pDocument;
	int m_nDdeMode;
	DWORD m_dwIdInst;

protected:
	HICON m_hIconApp;
	CString m_sExeFileName;
	HSZ m_hService;
	ULONG m_nCheckData;
	const char *m_pRegistryName;

	void InitDDE();
	void UninitDDE() const;
	void CopyInifileToRegistory();

	DECLARE_MESSAGE_MAP()
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnOnlineUpdate();
	afx_msg void OnOnlineManual();
};

extern CMMLibApp theApp;

/////////////////////////////////////////////////////////////////////////////

class CMyCmdLineInfo : public CCommandLineInfo
{
public:
	void ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast );
};
