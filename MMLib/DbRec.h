#pragma once

#pragma pack(4)

struct FOLDERBUF {
	char	folderID[LEN_ID];			// フォルダＩＤ
	char	parentFolderID[LEN_ID];		// 親フォルダＩＤ
	char	folderType[LEN_FOLDERTYPE];	// フォルダタイプ
	char	folderName[LEN_FOLDERNAME];	// フォルダ名
	char	createDate[LEN_DATE];		// フォルダ作成年月日時分
	char	updateDate[LEN_DATE];		// フォルダ更新年月日時分
	char	creator[LEN_CREATOR];		// 作成者
	char	remark[LEN_REMARK];			// 備考
	char	keyword[LEN_KEYWORD];		// キーワード
	char	updateCount[LEN_UCOUNT];	// 更新カウンタ
	char	codeDef[LEN_CODE];			// デフォルトデータコード
	char	codeStep[LEN_CODESTEP];		// コード更新ステップ
};

struct FOLDERREC {
	CString	sFolderID;				// フォルダＩＤ
	CString	sParentFolderID;		// 親フォルダＩＤ
	CString	sFolderType;			// フォルダタイプ
	CString	sFolderName;			// フォルダ名
	CString	sCreateDate;			// フォルダ作成年月日時分
	CString	sUpdateDate;			// フォルダ更新年月日時分
	CString	sCreator;				// 作成者
	CString	sRemark;				// 備考
	CString	sKeyword;				// キーワード
	long nUpdateCount;				// 更新カウンタ
	CString	sCodeDef;				// デフォルトデータコード
	long nCodeStep;					// コード更新ステップ
};

struct DATABUF {
	char	folderID[LEN_ID];			// フォルダＩＤ
	char	dataID[LEN_ID];				// 画像ＩＤ
	char	createDate[LEN_DATE];		// 撮影年月日時分
	char	updateDate[LEN_DATE];		// 更新年月日時分
	char	creator[LEN_CREATOR];		// 作成者
	char	code[LEN_CODE];				// コード
	char	title[LEN_TITLE];			// 画像タイトル
	char	remark[LEN_REMARK];			// 備考
	char	keyword[LEN_KEYWORD];		// キーワード
	char	dpi[LEN_DPI];				// DPI
	char	width[LEN_WIDTH];			// 画像の幅
	char	height[LEN_HEIGHT];			// 画像の高さ
	char	bit[LEN_BIT];				// 画像のビット数
	char	updateCount[LEN_UCOUNT];	// 更新カウンタ
};

struct DATAREC {
	CString	sFolderID;				// フォルダＩＤ
	CString	sDataID;				// 画像ＩＤ
	CString	sCreateDate;			// 撮影年月日時分
	CString	sUpdateDate;			// 更新年月日時分
	CString	sCreator;				// 作成者
	CString	sCode;					// コード
	CString	sTitle;					// 画像タイトル
	CString	sRemark;				// 備考
	CString	sKeyword;				// キーワード
	long nDpi;						// DPI
	long nWidth;					// 画像の幅
	long nHeight;					// 画像の高さ
	long nBit;						// 画像のビット数
	long nUpdateCount;				// 更新カウンタ
};

struct FINDBUF {
	char	findID[LEN_ID];				// 検索データＩＤ
	char	libraryID[LEN_ID];			// 検索対象ライブラリＩＤ
	char	findType[LEN_FINDTYPE];		// 検索種別
	char	folderName[LEN_TITLE];		// フォルダ名称
	struct {
		char	createDateFrom[LEN_DATE];	// 撮影年月日時分From
		char	createDateTo[LEN_DATE];		// 撮影年月日時分To
		char	createDateBefore[LEN_DATE];	// 撮影年月日時分Before
		char	updateDateFrom[LEN_DATE];	// 更新年月日時分From
		char	updateDateTo[LEN_DATE];		// 更新年月日時分To
		char	updateDateBefore[LEN_DATE];	// 更新年月日時分Before
		char	creator[LEN_CREATOR];		// 作成者
		char	code[LEN_CODE];				// コード
		char	title[LEN_TITLE];			// 画像タイトル
		char	remark[LEN_REMARK];			// 備考
		char	keyword[LEN_KEYWORD];		// キーワード
		char	text[LEN_TEXT];				// テキストデータ
		char	matching[LEN_MATCHING];		// マッチングオプション
	} conditions[N_FIND_CONDITIONS];
};

struct FIND_CONDITION {
	CString	sCreateDateFrom;				// 撮影年月日時分From
	CString	sCreateDateTo;				// 撮影年月日時分To
	CString	sCreateDateBefore;			// 撮影年月日時分Before
	CString	sUpdateDateFrom;				// 更新年月日時分From
	CString	sUpdateDateTo;				// 更新年月日時分To
	CString	sUpdateDateBefore;			// 更新年月日時分Before
	CString	sCreator;					// 作成者
	CString	sCode;						// コード
	CString	sTitle;						// 画像タイトル
	CString	sRemark;						// 備考
	CString	sKeyword;					// キーワード
	CString	sText;						// テキストデータ
	CString sMatching;					// マッチングオプション
};

struct FINDREC {
	CString sFindID;						// 検索データＩＤ
	CString	sLibraryID;					// 検索対象ライブラリＩＤ
	CString	sFindType;					// 検索種別
	CString	sFolderName;					// フォルダ名称
	FIND_CONDITION conditions[N_FIND_CONDITIONS];	// 検索条件
};

struct KEYWORDBUF {
	char	libraryID[LEN_ID];
	char	libraryName[LEN_FOLDERNAME];
	char	idType[1];
	char	id[LEN_ID];
	char	folderType[LEN_FOLDERTYPE];
	char	title[LEN_TITLE];
	char	creator[LEN_CREATOR];
	char	createDate[LEN_DATE];
	char	remark[LEN_REMARK];
	char	keyword[LEN_KEYWORD];
};

struct KEYWORDREC {
	CString	sLibraryID;
	CString sLibraryName;
	CString	sIdType;
	CString	sId;
	CString	sFolderType;
	CString	sTitle;
	CString sCreator;
	CString	sCreateDate;
	CString	sRemark;
	CString	sKeyword;
	CString	sText;
};

struct FINDPICTBUF {
	char	dataID[LEN_ID];
	char	factor_r[LEN_PFACTOR];
	char	factor_g[LEN_PFACTOR];
	char	factor_b[LEN_PFACTOR];
};

struct FINDPICTREC {
	CString	sDataID;
	CString sFactor_r;
	CString sFactor_g;
	CString sFactor_b;
};

struct ACCESSLOGBUF {
	char	seqNo[LEN_SEQ_NO];
	char	libraryID[LEN_ID];
	char	dataID[LEN_ID];
	char	AccessTime[LEN_TIME];
	char	title[LEN_TITLE];
	char	folderPath[LEN_FOLDER_PATH];
	char	createDate[LEN_DATE];
};

struct ACCESSLOGREC {
	int	nSeqNo;
	CString	sLibraryID;
	CString	sDataID;
	CString	sAccessTime;
	CString sTitle;
	CString sFolderPath;
	CString	sCreateDate;
};

#pragma pack()
