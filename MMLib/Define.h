#pragma once

#define CACHEMODE_DIRECT	0
#define	CACHEMODE_CACHE		1
#define CACHEMODE_OFFLINE	2

#define ITEM_NONE		0x00
#define ITEM_ROOT		0x01
#define ITEM_INBOX		0x02
#define ITEM_FOLDER		0x04
#define ITEM_DATA		0x08
#define ITEM_PENDING	0x10
#define ITEM_FIND		0x20

#define IMAGE_ROOT			0
#define IMAGE_LIBRARY		1
#define IMAGE_PENDING		2
#define IMAGE_INBOX			3
#define IMAGE_INBOX_OPEN	4
#define IMAGE_GFOLDER		5
#define IMAGE_GFOLDER_OPEN	6
#define IMAGE_DFOLDER		7
#define IMAGE_DFOLDER_OPEN	8
#define IMAGE_FFOLDER		9
#define IMAGE_FFOLDER_OPEN	10
#define IMAGE_FDATA			11
#define IMAGE_FDATA_OPEN	12
#define IMAGE_DATA			13

#define ADDITEM_LIBRARY		1
#define ADDITEM_FOLDER		2
#define ADDITEM_DATA		3
#define ADDITEM_PENDING		4
#define ADDITEM_FIND		5

#define	FN_FOLDER_DBF	"FOLDER3.DBF"		// フォルダ管理 data
#define FN_FOLDER_BIN	"FOLDER3.BIN"		// フォルダ管理バイナリファイル
#define	FN_FOLDER_NDX1	"FOLDER1.EDX"		// フォルダ管理 index
#define	FN_FOLDER_NDX2	"FOLDER2.EDX"		// フォルダ管理 index

#define	FN_DATA_DBF		"MMDATA2.DBF"		// データファイル管理 data
#define	FN_DATA_BIN		"MMDATA2.BIN"		// データバイナリファイル
#define	FN_DATA_NDX1	"MMDATA1.EDX"		// データファイル管理 index
#define	FN_DATA_NDX2	"MMDATA2.EDX"		// データファイル管理 index

#define	FN_FIND_DBF		"FIND1.DBF"			// 検索フォルダ管理 data
#define	FN_FIND_NDX		"FIND1.EDX"			// 検索フォルダ管理 index

#define FN_KEYWORD_DBF	"KEYWORD.DBF"		// 検索用キーワード

#define FN_PICTURE_DBF	"PICTURE.DBF"		// 画像検索DB
#define FN_PICTURE_NDX1	"PICTURE1.EDX"		// 画像検索index
#define FN_PICTURE_NDX2	"PICTURE2.EDX"		// 画像検索index
#define FN_PICTURE_NDX3	"PICTURE3.EDX"		// 画像検索index
#define FN_PICTURE_NDX4	"PICTURE4.EDX"		// 画像検索index

#define FN_ACCESS_LOG_DBF	"ACCLOG.DBF"		// アクセス履歴DB
#define FN_ACCESS_LOG_NDX1	"ACCLOG1.EDX"		// アクセス履歴index
#define FN_ACCESS_LOG_NDX2	"ACCLOG2.EDX"		// アクセス履歴index

#define FTYPE_LIBRARY	'L'
#define FTYPE_GFOLDER	'G'
#define FTYPE_DFOLDER	'D'

#define FIND_TYPE_FOLDER	'F'
#define FIND_TYPE_DATA		'D'
#define FIND_TYPE_SIMPLE	'S'

#define FID_LIBRARY		"F0000000"

#define LEN_CREATOR		20
#define	LEN_ID			8
#define	LEN_FOLDERNAME	40
#define LEN_FOLDERTYPE	1
#define	LEN_RECT		4
#define	LEN_ICON		1
#define	LEN_DATE		12
#define	LEN_CODE		20
#define	LEN_TITLE		40
#define	LEN_REMARK		60
#define	LEN_KEYWORD		100
#define LEN_TEXT		100
#define LEN_DPI			4
#define LEN_WIDTH		5
#define LEN_HEIGHT		5
#define LEN_BIT			2
#define LEN_UCOUNT		6
#define LEN_MATCHING	8
#define LEN_FINDTYPE	1
#define LEN_CODESTEP	8
#define LEN_PASSWORD	8
#define LEN_PERMISSION	1
#define LEN_PFACTOR		8
#define LEN_SEQ_NO		10
#define LEN_TIME		19
#define LEN_FOLDER_PATH	100

#define FORM_FIELD		"FORM"
#define PICT1_FIELD		"PICT1"
#define PICT2_FIELD		"PICT2"
#define TEXT_FIELD		"TEXT"
#define SOUND_FIELD		"SOUND"
#define MOVIE_FIELD		"MOVIE"

#define VIEWHINT_ADDITEM		1
#define VIEWHINT_SELECTITEM		2
#define VIEWHINT_OPENITEM		3
#define VIEWHINT_CLOSEITEM		4
#define VIEWHINT_REMOVEITEM		5
#define VIEWHINT_UPDATEITEM		6
#define VIEWHINT_RELOADITEM		7
#define VIEWHINT_REFINDITEM		8
#define VIEWHINT_DRAWALL		9
#define VIEWHINT_SETFONT		10
#define VIEWHINT_UPDATEWINDOW	11
#define VIEWHINT_SELECTALL		12
#define VIEWHINT_DRAWLIST		13
#define VIEWHINT_ENLARGE		14
#define VIEWHINT_REDUCE			15
#define VIEWHINT_RROTATION		16
#define VIEWHINT_LROTATION		17
#define VIEWHINT_DRAWPICT		18
#define VIEWHINT_SETPICTRATIO	19
#define VIEWHINT_SETSMALLSIZE	20
#define VIEWHINT_PICTFRAME		21
#define VIEWHINT_SORTITEM		22
#define VIEWHINT_SETTEXT		23
#define VIEWHINT_TEXTFILE		24
#define VIEWHINT_MODIFYDATA		25
#define VIEWHINT_SAVEDATA		26
#define VIEWHINT_SELECTDATA		27
#define VIEWHINT_EDITPICT		28
#define VIEWHINT_CHECKSAVE		29
#define VIEWHINT_WORDWRAP		30
#define VIEWHINT_OPENALLLIBRARY	31
#define VIEWHINT_KEYWORDFIND	32
#define VIEWHINT_KEYWORDPREV	33
#define VIEWHINT_KEYWORDNEXT	34
#define VIEWHINT_GETSEARCHCOUNT	35
#define VIEWHINT_SETSEL			36
#define VIEWHINT_RESETSEL		37

#define VIEWMODE_PICT		0
#define VIEWMODE_LIST		1
#define VIEWMODE_DETAIL		2

#define WM_GETSELECTEDITEM	(WM_APP + 0)
#define WM_UPDATEPICT		(WM_APP + 1)
#define WM_MYCOMMAND		(WM_APP + 2)
#define WM_BUTTON_DOWN		(WM_APP + 3)
#define WM_CHECK_LICENSE1	(WM_APP + 4)
#define WM_CHECK_LICENSE2	(WM_APP + 5)
#define WM_REGIST_LICENSE	(WM_APP + 6)
#define WM_NOTIFY_UPDATE	(WM_APP + 7)
#define	WM_END_DIALOG		(WM_APP + 8)
#define	WM_GETFINDTEXT		(WM_APP + 9)

#define PICT_WIDTH			80
#define PICT_HEIGHT			60
#define PICT_FRAME			5
#define PICT_TEXT			30
#define PICT_MARGIN			20

#define SMALL_NORMAL		0
#define SMALL_HISPD			1
#define SMALL_HIRES			2

#define PHOTO_NULL			0
#define PHOTO_CAMERA		1
#define	PHOTO_SCANNER		2
#define	PHOTO_FILE			3
#define PHOTO_CLIPBOARD		4
#define PHOTO_WINDOW		5

#define DETAIL_NAME			0
#define DETAIL_CODE			1
#define DETAIL_CDATE		2
#define DETAIL_UDATE		3
#define DETAIL_CREATOR		4
#define DETAIL_REMARK		5
#define DETAIL_KEYWORD		6
#define DETAIL_SERVER		7
#define DETAIL_DIRECTORY	8
#define DETAIL_FIRSTDATA	9
#define DETAIL_NUM			10

#define DETAIL_NONE			-1
#define DETAIL_LIBRARY		0
#define DETAIL_FOLDER		1
#define DETAIL_DATA			2

#define AUTOSIZE_NONE		0
#define AUTOSIZE_WIDTH		1
#define AUTOSIZE_HEIGHT		2
#define AUTOSIZE_BOTH		3

#define MAXUSERNAME	64

#define MAX_RECENT	10

#define N_FIND_CONDITIONS	5

#define DIALOGFONT_NORMAL	0
#define DIALOGFONT_LARGE	1

#define SRCFILETYPE_MML		0x01
#define SRCFILETYPE_PICT	0x02
#define SRCFILETYPE_MMDB	0x03
#define SRCFILETYPE_PDF		0x04

#define TEXTMODE_TITLE_CODE	0
#define TEXTMODE_TITLE_ONLY	1

#define PERMISSION_READ		0x01
#define PERMISSION_ADD		0x02
#define PERMISSION_UPDATE	0x04
#define PERMISSION_DELETE	0x08
#define PERMISSION_FULL		0x0f

#define PICTFILE_BMP		0x01
#define PICTFILE_GIF		0x02
#define PICTFILE_JPG		0x04
#define PICTFILE_FAX		0x08
#define PICTFILE_PNG		0x10
#define PICTFILE_ALL		0xff
#define PICTFILE_TRANSDEF	(PICTFILE_BMP | PICTFILE_JPG)
#define PICTFILE_DEFAULT	(PICTFILE_BMP | PICTFILE_PNG | PICTFILE_JPG)

#define PROGRAM_ERROR		{CString msg; msg.Format("Program Error. File=%s Line=%d", __FILE__, __LINE__); ::AfxMessageBox(msg);}

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define PRODUCT_CODE	"MMLIB"

// ライセンスチェック
#define GET_CHECK_DATA \
		((GetCheckData1() + 19580414) ^ 0x3f81f9ca)
#define CHECK_LICENSE0(_app_id, _product_version) \
		(CheckLicense1(g_sProgName, g_hIcon, m_pRegistryName, (_app_id << 8), TRUE, PRODUCT_CODE, _product_version) == m_nCheckData ? TRUE : FALSE)
#define CHECK_LICENSE1(_app_id) \
		::AfxGetMainWnd()->SendMessage(WM_CHECK_LICENSE1, (WPARAM)(_app_id << 8))
#define CHECK_LICENSE2(_app_id) \
		::AfxGetMainWnd()->SendMessage(WM_CHECK_LICENSE2, (WPARAM)(_app_id << 8))
//#define REGIST_LICENSE() \
//		::AfxGetMainWnd()->SendMessage(WM_REGIST_LICENSE)

#define PDF_TEXT_DLL	"xd2txlib.dll"
