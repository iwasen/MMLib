/****************************************************************************
 *                                                                          *
 *  ファイル名: DIB.H                                                   *
 *                                                                          *
 *                                                                          *
 ****************************************************************************/

/* 指定された値を範囲内に収めるマクロ */
#define BOUND(x,min,max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

/* 2つの値を交換するマクロ */
#define SWAP(x,y)   ((x)^=(y)^=(x)^=(y))

/* 2つの値の最小値を取得するマクロ */
#define MIN(x,y) (((x) <= (y)) : x ? y)

/*
 * 時間のかかる処理の場合に砂時計カーソルを表示し、
 * 処理終了後に消去するマクロ
 */
#define StartWait() hcurSave = SetCursor(LoadCursor(NULL,IDC_WAIT))
#define EndWait()   SetCursor(hcurSave)

#define MINBAND         50     /* このプログラムで使われる最小のバンド幅  */
#define BANDINCREMENT   20     /* 最適なバンド幅を決定するときに使われる、*/
                               /* バンド幅の差分値                        */

/*
 * DIB属性フラグ(fFileOptions)のビット値。[ファイルを開く]ダイアログ
 * のDIBビット数ラジオボタンのコントロールIDとしても使われる。
 */
#define F_1BPP            DLGOPEN_1BPP
#define F_4BPP            DLGOPEN_4BPP
#define F_8BPP            DLGOPEN_8BPP
#define F_24BPP     DLGOPEN_24BPP

/*
 * DIB属性フラグ(fFileOptions)のビット値。[ファイルを開く]ダイアログ
 * のDIB圧縮タイプラジオボタンのコントロールIDとしても使われる。
 */
#define F_RLE4            DLGOPEN_RLE4
#define F_RLE8            DLGOPEN_RLE8
#define F_RGB            DLGOPEN_RGB

/* _lseekのフラグ*/
#define  SEEK_CUR 1
#define  SEEK_END 2
#define  SEEK_SET 0

/* SETDIBSCALINGプリンタエスケープで渡される構造体 */
typedef struct {
        short ScaleMode;
        short dx, dy;
} DIBPARAMS;

extern HWND hWndApp;              /* アプリケーションウィンドウのハンドル   */

#define MAXREAD  32768            /* 読み取り処理時の読み取り可能なバイト数 */

/* さまざまなリソースのヘッダー記号 */
#define BFT_ICON   0x4349   /* 'IC' */
#define BFT_BITMAP 0x4d42   /* 'BM' */
#define BFT_CURSOR 0x5450   /* 'PT' */

/* リソースがDIBかどうかを判断するマクロ */
#define ISDIB(bft) ((bft) == BFT_BITMAP)

/* 指定された値を最も近いDWORD (unsigned long )境界に整列させるマクロ */
#define ALIGNULONG(i)        ((i+3)/4*4)

/* 指定された値を最も近いバイト境界に整列させるマクロ */
#define WIDTHBYTES(i)        ((i+31)/32*4)

#define PALVERSION      0x300
#define MAXPALETTE        256   /* サポートするパレットエントリの最大数 */

/********** 以下の定義はDLGOPEN.Cで使用 ************************/

/* DlgOpenダイアログのコントロールID */
#define DLGOPEN_OPTION           0xF000
#define DLGOPEN_1BPP             0x0001
#define DLGOPEN_4BPP             0x0002
#define DLGOPEN_8BPP             0x0004
#define DLGOPEN_24BPP            0x0008
#define DLGOPEN_RLE4             0x0010
#define DLGOPEN_RLE8             0x0020
#define DLGOPEN_RGB              0x0040

#define DLGOPEN_OPTION8          0x0080

/*
 * フラグ:
 *     LOWORDは標準的なFileOpen()のフラグ(OF_*)
 *     HIWORDは以下のいずれか
 */
#define OF_MUSTEXIST    0x00010000  /* ユーザーが[OK]を押した場合は         */
                                    /* ファイルが必ず存在する               */
#define OF_NOSHOWSPEC   0x00020000  /* 編集ボックスに検索方法を表示しない   */
#define OF_SHOWSPEC     0x00000000  /* 編集ボックスに検索方法を表示する     */
#define OF_SAVE         0x00040000  /* [OK]ボタンが「保存」を意味する       */
#define OF_OPEN         0x00080000  /* [OK]ボタンが「開く」を意味する       */
#define OF_NOOPTIONS    0x00100000  /* オプションを非表示にする             */

/*  DlgDirLst() の属性 */
#define ATTRFILELIST    0x0000         /* ファイルのみ               */
#define ATTRDIRLIST     0xC010         /* ディレクトリとドライブのみ */
#define CBEXTMAX        6              /* "\*.txt"のバイト数         */


#define IDF(id)  ((id) & ~DLGOPEN_OPTION)  /* コントロールIDからフラグを取得 */
#define FID(f)   ((f)  |  DLGOPEN_OPTION)  /* フラグからコントロールIDを取得 */

/***************** グローバル変数 *************************/

extern char        achFileName[128]; /* ファイルのパス名                    */
extern DWORD       dwOffset;         /* DIBファイルの現在位置を指すポインタ */
extern RECT        rcClip;           /* 現在のクリッピング長方形            */
extern BOOL        fPalColors;       /* 現在のDIBカラーテーブルがRGB値では  */
                                     /* なくパレットインデックスならばTRUE  */
extern BOOL        bDIBToDevice;     /* SetDIBitsToDevice()を使ってデータを */
                                     /* 転送                                */
extern BOOL        bLegitDraw;       /* 描画可能なビットマップがあるか?     */
extern WORD        wTransparent;     /* DCのモード　　　　　                */
extern char        szAppName[];      /* アプリケーション名　　　　　　      */
extern HPALETTE    hpalCurrent;      /* 現在のパレットを識別するハンドル    */
extern HANDLE      hdibCurrent;      /* 現在のメモリDIBを識別するハンドル   */
extern HBITMAP     hbmCurrent;       /* 現在のメモリビットマップを識別する  */
                                     /* ハンドル                            */
extern HANDLE      hbiCurrent;       /* 現在のビットマップ情報構造体を      */
                                     /* 識別するハンドル                    */
extern DWORD       dwStyle;          /* アプリケーションウインドウの        */
                                     /* スタイルビット                      */

/***************** 関数の宣言 *******************/

DWORD PASCAL lread(int, VOID FAR *, DWORD);
DWORD PASCAL lwrite(int, VOID FAR *, DWORD);

/***********************************************************/
/* dib.cモジュールで使われる関数の宣言　　　　　           */
/***********************************************************/

extern	HANDLE      OpenDIB (LPSTR);
extern	HANDLE		OpenMemDIB(HWND);
extern	BOOL        WriteDIB (LPSTR,HANDLE);
extern	WORD        GetPaletteSize (VOID FAR *);
extern	WORD		GetDibNumColorsByHandle(HANDLE);
extern	WORD        GetDibNumColors (VOID FAR *);
extern	HPALETTE    CreateDibPalette (HANDLE);
extern	HPALETTE    CreateBIPalette (LPBITMAPINFOHEADER);
extern	HANDLE      DibFromBitmap (HBITMAP, DWORD, WORD, HPALETTE);
extern	HBITMAP     BitmapFromDib (HANDLE, HPALETTE);
extern	BOOL        DibBlt (HDC, int, int, int, int, HANDLE, int, int, LONG);
extern	BOOL        StretchDibBlt (HDC, int, int, int, int, HANDLE, int, int, int, int, LONG);
extern	BOOL        DibInfo (HANDLE,LPBITMAPINFOHEADER);
extern	HANDLE      ReadDibBitmapInfo (int);
extern	BOOL        DrawBitmap (HDC, int, int, HBITMAP, DWORD);

