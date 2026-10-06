// FindDB.cpp: CFindDB クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MMLib.h"
#include "FindDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CFindDB::CFindDB()
{
	m_oPcserve.Connect(CHT_LOCAL, "", "MMLIB");
}

CFindDB::~CFindDB()
{
	m_oFindDB.DBClose();
	m_oPcserve.Disconnect();
}

BOOL CFindDB::Open(BOOL bCreate)
{
	static constexpr DBF_I field[] = {
		{"FINDID",	    'C', LEN_ID,       0},	// 検索フォルダＩＤ
		{"LIBRARYID",	'C', LEN_ID,       0},	// 検索対象ライブラリＩＤ
		{"FINDTYPE",	'C', LEN_FINDTYPE, 0},	// 検索種別
		{"FNAME",		'C', LEN_TITLE,    0},	// フォルダ名称

		{"CDATEFROM1",	'C', LEN_DATE,     0},	// 撮影年月日時分From
		{"CDATETO1",	'C', LEN_DATE,	   0},	// 撮影年月日時分To
		{"CDATEBEFO1",	'C', LEN_DATE,     0},	// 撮影年月日時分Before
		{"UDATEFROM1",	'C', LEN_DATE,     0},	// 更新年月日時分From
		{"UDATETO1",	'C', LEN_DATE,     0},	// 更新年月日時分To
		{"UDATEBEFO1",	'C', LEN_DATE,     0},	// 更新年月日時分Before
		{"CREATOR1",	'C', LEN_CREATOR,  0},	// 作成者
		{"CODE1",		'C', LEN_CODE,     0},	// コード
		{"TITLE1",		'C', LEN_TITLE,    0},	// 画像タイトル
		{"REMARK11",	'C', LEN_REMARK,   0},	// 備考
		{"REMARK21",	'C', LEN_KEYWORD,  0},	// キーワード
		{"TEXT1",		'C', LEN_TEXT,	   0},	// テキストデータ
		{"MATCHING1",	'C', LEN_MATCHING, 0},	// マッチングオプション

		{"CDATEFROM2",	'C', LEN_DATE,     0},	// 撮影年月日時分From
		{"CDATETO2",	'C', LEN_DATE,	   0},	// 撮影年月日時分To
		{"CDATEBEFO2",	'C', LEN_DATE,     0},	// 撮影年月日時分Before
		{"UDATEFROM2",	'C', LEN_DATE,     0},	// 更新年月日時分From
		{"UDATETO2",	'C', LEN_DATE,     0},	// 更新年月日時分To
		{"UDATEBEFO2",	'C', LEN_DATE,     0},	// 更新年月日時分Before
		{"CREATOR2",	'C', LEN_CREATOR,  0},	// 作成者
		{"CODE2",		'C', LEN_CODE,     0},	// コード
		{"TITLE2",		'C', LEN_TITLE,    0},	// 画像タイトル
		{"REMARK12",	'C', LEN_REMARK,   0},	// 備考
		{"REMARK22",	'C', LEN_KEYWORD,  0},	// キーワード
		{"TEXT2",		'C', LEN_TEXT,	   0},	// テキストデータ
		{"MATCHING2",	'C', LEN_MATCHING, 0},	// マッチングオプション

		{"CDATEFROM3",	'C', LEN_DATE,     0},	// 撮影年月日時分From
		{"CDATETO3",	'C', LEN_DATE,	   0},	// 撮影年月日時分To
		{"CDATEBEFO3",	'C', LEN_DATE,     0},	// 撮影年月日時分Before
		{"UDATEFROM3",	'C', LEN_DATE,     0},	// 更新年月日時分From
		{"UDATETO3",	'C', LEN_DATE,     0},	// 更新年月日時分To
		{"UDATEBEFO3",	'C', LEN_DATE,     0},	// 更新年月日時分Before
		{"CREATOR3",	'C', LEN_CREATOR,  0},	// 作成者
		{"CODE3",		'C', LEN_CODE,     0},	// コード
		{"TITLE3",		'C', LEN_TITLE,    0},	// 画像タイトル
		{"REMARK13",	'C', LEN_REMARK,   0},	// 備考
		{"REMARK23",	'C', LEN_KEYWORD,  0},	// キーワード
		{"TEXT3",		'C', LEN_TEXT,	   0},	// テキストデータ
		{"MATCHING3",	'C', LEN_MATCHING, 0},	// マッチングオプション

		{"CDATEFROM4",	'C', LEN_DATE,     0},	// 撮影年月日時分From
		{"CDATETO4",	'C', LEN_DATE,	   0},	// 撮影年月日時分To
		{"CDATEBEFO4",	'C', LEN_DATE,     0},	// 撮影年月日時分Before
		{"UDATEFROM4",	'C', LEN_DATE,     0},	// 更新年月日時分From
		{"UDATETO4",	'C', LEN_DATE,     0},	// 更新年月日時分To
		{"UDATEBEFO4",	'C', LEN_DATE,     0},	// 更新年月日時分Before
		{"CREATOR4",	'C', LEN_CREATOR,  0},	// 作成者
		{"CODE4",		'C', LEN_CODE,     0},	// コード
		{"TITLE4",		'C', LEN_TITLE,    0},	// 画像タイトル
		{"REMARK14",	'C', LEN_REMARK,   0},	// 備考
		{"REMARK24",	'C', LEN_KEYWORD,  0},	// キーワード
		{"TEXT4",		'C', LEN_TEXT,	   0},	// テキストデータ
		{"MATCHING4",	'C', LEN_MATCHING, 0},	// マッチングオプション

		{"CDATEFROM5",	'C', LEN_DATE,     0},	// 撮影年月日時分From
		{"CDATETO5",	'C', LEN_DATE,	   0},	// 撮影年月日時分To
		{"CDATEBEFO5",	'C', LEN_DATE,     0},	// 撮影年月日時分Before
		{"UDATEFROM5",	'C', LEN_DATE,     0},	// 更新年月日時分From
		{"UDATETO5",	'C', LEN_DATE,     0},	// 更新年月日時分To
		{"UDATEBEFO5",	'C', LEN_DATE,     0},	// 更新年月日時分Before
		{"CREATOR5",	'C', LEN_CREATOR,  0},	// 作成者
		{"CODE5",		'C', LEN_CODE,     0},	// コード
		{"TITLE5",		'C', LEN_TITLE,    0},	// 画像タイトル
		{"REMARK15",	'C', LEN_REMARK,   0},	// 備考
		{"REMARK25",	'C', LEN_KEYWORD,  0},	// キーワード
		{"TEXT5",		'C', LEN_TEXT,	   0},	// テキストデータ
		{"MATCHING5",	'C', LEN_MATCHING, 0}	// マッチングオプション
	};
	BOOL bCreateIndex = FALSE;
	int err;

	CString sDirName = g_pMMLibDB->m_sCacheDir;
open:
	CString sPathName = sDirName + FN_FIND_DBF;
	if ((err = m_oFindDB.DBOpen(m_oPcserve, sPathName)) != 0) {
		bCreateIndex = TRUE;
		if (err == ERROR_DB_NOFILE && bCreate)
			err = m_oFindDB.DBCreate(m_oPcserve, sPathName, field, ARRAY_SIZE(field));
		if (err != 0)
			return FALSE;
	} else {
		int nRecSize;
		CPSDB oNewDB;

		if ((err = m_oFindDB.DBRecSize(&nRecSize)) != 0)
			return err;

		if (nRecSize - 1 < sizeof(FINDBUF)) {
			CString sPathName2 = sDirName + "FINDTMP.DBF";
			if ((err = oNewDB.DBCreate(m_oPcserve, sPathName2, field, ARRAY_SIZE(field))) == 0) {
				if ((err = oNewDB.DBCopy(m_oFindDB)) != 0)
					return err;
				m_oFindDB.DBClose();
				oNewDB.DBClose();
				m_oPcserve.CFDelete(sPathName);
				m_oPcserve.CFRename(sPathName2, sPathName);
				goto open;
			}
		}
	}

	int nIdxNo;
	sPathName = sDirName + FN_FIND_NDX;
	if (bCreateIndex || m_oFindDB.DBIndex(sPathName, &nIdxNo) != 0) {
		if ((err = m_oFindDB.DBIdxCreate(sPathName, "FINDID", 1, &nIdxNo)) != 0)
			return FALSE;
	}

	return TRUE;
}

BOOL CFindDB::ReadFind(LPCSTR pLibraryID, FINDREC *pFindRec)
{
	int err;

	if (pLibraryID != NULL) {
		CString sFilter;
		sFilter.Format("*FLIBRARYID=%s", pLibraryID);
		if ((err = m_oFindDB.DBSetFilter(sFilter)) != 0)
			return FALSE;
	}

	int nRead;
	FINDBUF oFindBuf;
	if ((err = m_oFindDB.DBReadNext(1, &oFindBuf, &nRead)) != 0)
		return FALSE;

	if (nRead == 0)
		return FALSE;

	m_oFindDB.DBGetFieldBuf(&oFindBuf, pFindRec);

	return TRUE;
}

BOOL CFindDB::AddFind(FINDREC *pFindRec)
{
	int err;

	if ((err = MakeFindID(pFindRec->sFindID)) != 0)
		return FALSE;

	FINDBUF oFindBuf;
	m_oFindDB.DBSetFieldBuf(&oFindBuf, pFindRec);

	if ((err = m_oFindDB.DBSetFilter(NULL)) != 0)
		return FALSE;

	if ((err = m_oFindDB.DBStoreUniq(&oFindBuf)) != 0)
		return FALSE;

	return TRUE;
}

BOOL CFindDB::UpdateFind(FINDREC *pFindRec)
{
	int err;

	FINDBUF oFindBuf;
	m_oFindDB.DBSetFieldBuf(&oFindBuf, pFindRec);

	if ((err = m_oFindDB.DBSetFilter(NULL)) != 0)
		return FALSE;

	if ((err = m_oFindDB.DBUpdateKey(pFindRec->sFindID, LEN_ID, &oFindBuf)) != 0)
		return FALSE;

	return TRUE;
}

BOOL CFindDB::DeleteFind(LPCSTR pDID)
{
	int err;

	if ((err = m_oFindDB.DBSetFilter(NULL)) != 0)
		return FALSE;

	if ((err = m_oFindDB.DBDeleteKey(pDID, LEN_ID, 1)) != 0)
		return FALSE;

	return TRUE;
}

int CFindDB::MakeFindID(CString &sID)
{
	int err;

	time_t ltime;
	time(&ltime);

	if ((err = m_oFindDB.DBSetFilter(NULL)) != 0)
		return err;

	int nFind;
	for (;;) {
		MakeID('S', (DWORD)ltime, sID);

		if ((err = m_oFindDB.DBSearch2(sID, LEN_ID, &nFind)) != 0)
			return err;

		if (nFind != 1)
			break;

		ltime++;
	}

	return 0;
}
