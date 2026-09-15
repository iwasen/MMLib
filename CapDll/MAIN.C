#include "windows.h"		/* すべてのWindowsアプリケーションに必要 */
#include <string.h>
#include "generic.h"		/* このプログラムに必要なヘッダー        */
#include "cap.h"

HANDLE hInst;			/* 現在のインスタンス                    */

/****************************************************************************

    関数: WinMain(HANDLE, HANDLE, LPSTR, int)

    目的: 初期化関数の呼び出しとメッセージループ処理

    コメント:

        Windowsは、この関数の名前をプログラムの初期エントリポイントとして認識
        します。WinMain関数は、同じプログラムの別のインスタンスが実行中でなけ
        ればアプリケーション初期化ルーチンを呼び出します。また、つねにインスタ
        ンス初期化ルーチンを呼び出します。その後、最上位の制御構造である「メッ
        セージの取得とディスパッチを行うループ」を実行して、残りの処理を行いま
        す。このループはWM_QUITメッセージを受け取ったときに終了します。このと
        きWinMain関数はPostQuitMessage()の戻り値を返し、アプリケーションのイン
        スタンスを終了させます。

        通常、メッセージループに入る前に関数の処理を中止しなければならない場合
        には、NULLを返します。

****************************************************************************/

int PASCAL WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow)
HANDLE hInstance;			     /* 現在のインスタンス        */
HANDLE hPrevInstance;			     /* 以前のインスタンス        */
LPSTR lpCmdLine;			     /* コマンドライン            */
int nCmdShow;				     /* 表示のタイプ              */
                                             /* (通常またはアイコン)      */
{
    MSG msg;				     /* メッセージ                */

    if (!hPrevInstance)			 /* ほかのインスタンスが実行中か? */
	if (!InitApplication(hInstance)) /* 共通の初期化処理              */
	    return (FALSE);		 /* 初期化に失敗した場合は終了    */

    /* インスタンスごとに初期化処理を実行 */

    if (!InitInstance(hInstance, nCmdShow))
        return (FALSE);

    /* WM_QUITメッセージを受け取るまでメッセージの取得とディスパッチを行う */

    while (GetMessage(&msg,	  /* メッセージ構造体                       */
	    NULL,		  /* メッセージを受け取るウィンドウハンドル */
	    NULL,		  /* 調べるメッセージの下限値               */
	    NULL))		  /* 調べるメッセージの上限値               */
	{
	TranslateMessage(&msg);	  /* 仮想キーコードの変換                   */
	DispatchMessage(&msg);	  /* メッセージをウィンドウにディスパッチ   */
    }
    return (msg.wParam);	  /* PostQuitMessage関数の戻り値を返す      */
}


/****************************************************************************

    関数: InitApplication(HANDLE)

    目的: ウィンドウデータの初期化とウィンドウクラスの登録

    コメント:

        この関数は、このアプリケーションのインスタンスがまだ1つも実行されてい
        ないときだけ、アプリケーションの初期化処理時に呼び出されます。この関数
        が初期化処理を実行するのは、実行中のインスタンスの数にかかわらず最初の
        1度だけです。

        ここでは、WNDCLASS構造体に必要な値を設定し、WindowsのRegisterClass()関
        数を呼び出すことによりウィンドウクラスを初期化します。このアプリケー
        ションのすべてのインスタンスは同じウィンドウクラスを使用するため、ウィ
        ンドウクラスの初期化が必要なのは最初のインスタンスを初期化するときだけ
        です。

****************************************************************************/

BOOL InitApplication(hInstance)
HANDLE hInstance;			       /* 現在のインスタンス       */
{
    WNDCLASS  wc;

    /*
     * ウィンドウクラス構造体に、メインウィンドウの
     * 内容を記述するパラメータを設定
     */

    wc.style = NULL;                    /* クラススタイル                   */
    wc.lpfnWndProc = MainWndProc;       /* このクラスのウィンドウに送られる */
                                        /* メッセージを取得する関数         */
    wc.cbClsExtra = 0;                  /* クラスごとの補足データなし       */
    wc.cbWndExtra = 0;                  /* ウィンドウごとの補足データなし   */
    wc.hInstance = hInstance;           /* クラスを所有するアプリケーション */
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH); 
    wc.lpszMenuName =  "GenericMenu";   /*
                                         * リソースファイル(.rc)で指定した
                                         * メニュー名
                                         */
    wc.lpszClassName = "GenericWClass"; /* CreateWindowで使われる名前     */

    /* ウィンドウクラスを登録し、成功または失敗を示すコードを返す */
    return (RegisterClass(&wc));
}


/****************************************************************************

    関数: InitInstance(HANDLE, int)

    目的: インスタンスハンドルの保存とメインウィンドウの作成

    コメント:

        この関数は、このアプリケーションのすべてのインスタンスの初期化時に呼び
        出され、インスタンス間で共有できない初期化処理を実行します。

        ここでは、インスタンスハンドルをスタティック変数に保存して、メイン
        プログラムのウィンドウを作成し、表示します。

****************************************************************************/

BOOL InitInstance(hInstance, nCmdShow)
    HANDLE          hInstance;       /* 現在のインスタンス               */
    int             nCmdShow;        /* 最初のShowWindow()呼び出しの引数 */
{
    HWND            hWnd;            /* メインウィンドウのハンドル       */

    /*
     * インスタンスハンドルをスタティック変数に保存する。このハンドルは、
     * InitInstanceの実行後にアプリケーションからWindowsを呼び出すときに
     * 頻繁に使われる
     */

    hInst = hInstance;

    /* このアプリケーションインスタンスのメインウィンドウを作成 */

    hWnd = CreateWindow(
        "GenericWClass",               /* RegisterClass()呼び出しを参照    */
        "Generic",                     /* ウィンドウタイトルバーのテキスト */
        WS_OVERLAPPEDWINDOW,           /* ウィンドウスタイル               */
        CW_USEDEFAULT,                 /* デフォルトの水平位置             */
        CW_USEDEFAULT,                 /* デフォルトの垂直位置             */
        CW_USEDEFAULT,                 /* デフォルトの幅                   */
        CW_USEDEFAULT,                 /* デフォルトの高さ                 */
        NULL,                          /*
                                        * オーバーラップウィンドウは
                                        * 親ウィンドウを持たない
                                        */
        NULL,                          /* ウィンドウクラスのメニューを使う */
        hInstance,                     /*
                                        * このインスタンスが
                                        * このウィンドウを所有
                                        */
        NULL                           /* ポインタは不要                   */
    );

    /* ウィンドウを作成できなかったときは「失敗」を示すFALSEを返す */
    if (!hWnd)
        return (FALSE);

    /* ウィンドウを表示し、そのクライアント領域を更新してTRUEを返す */

    ShowWindow(hWnd, nCmdShow);  /* ウィンドウを表示              */
    UpdateWindow(hWnd);          /* WM_PAINTメッセージを送る      */
    return (TRUE);               /* PostQuitMessageの戻り値を返す */
}

/****************************************************************************

    関数: MainWndProc(HWND, UINT, WPARAM, LPARAM)

    目的: メッセージ処理

    メッセージ:

        WM_COMMAND - アプリケーションメニュー
                     ([バージョン情報]ダイアログボックス)
        WM_DESTROY - ウィンドウの破棄

    コメント:

        IDM_ABOUTメッセージを処理するため、MakeProcInstance()を呼び出して
        About()関数の現在のインスタンスアドレスを取得します。その後、
        Dialogbox関数を呼び出して、generic.rcファイルの情報に従ってダイアログ
        ボックスを作成し、About()関数に制御を渡します。About()関数が制御を戻
        した時点で、インスタンスアドレスを解放します。

****************************************************************************/

long CALLBACK __export MainWndProc(hWnd, message, wParam, lParam)
HWND hWnd;				  /* ウィンドウハンドル */
UINT message;				  /* メッセージのタイプ */
WPARAM wParam;				  /* 追加情報           */
LPARAM lParam;				  /* 追加情報           */
{
    switch (message) {
        case WM_COMMAND:
            if (wParam == IDM_ABOUT){
	    	{
				CapPachinko(hWnd, (LPSTR)"www");

			}
		break;

	    } 
               return (DefWindowProc(hWnd, message, wParam, lParam));
        case WM_DESTROY:             /* メッセージ: ウィンドウを破棄  */
            PostQuitMessage(0);
            break;

        default:                     /* 未処理メッセージはそのまま渡す */
            return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (NULL);
}


