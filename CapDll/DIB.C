/******************************************************************************
 *                                                                            *
 *  モジュール: DIB.C                                                         *
 *                                                                            *
 *  説明:       DIBを処理するルーチン                                         *
 *                                                                            *
 *  関数:                                                                     *
 *                                                                            *
 *  OpenDIB()           - DIBファイルを開いてメモリDIBを作成します。          *
 *  WriteDIB()          - グローバルハンドルをCF_DIB形式でファイルに書き込み  *
 *                        ます。                                              *
 *  DibInfo()           - CF_DIB形式のメモリブロックに関連付けられている      *
 *                        情報ブロックを取得します。                          *
 *  CreateBIPalette()   - BITMAPINFO構造体を指すポインタからGDIパレットを     *
 *                        作成します。                                        *
 *  CreateDibPalette()  - BITMAPINFO構造体のハンドルからGDIパレットを作成     *
 *                        します。                                            *
 *  ReadDibBitmapInfo() - DIB形式のファイルを読み取り、そのBITMAPINFOの       *
 *                        グローバルハンドルを返します。                      *
 *  GetPaletteSize()       - 指定されたDIBのパレットのバイト数を計算します。     *
 *  GetDibNumColors()      - DIBの色数を判断します。                             *
 *  BitmapFromDib()     - CF_DIB形式のブロックを識別するグローバルハンドル    *
 *                        から、DDBを作成します。                             *
 *  DibFromBitmap()     - 渡されたDDBと同じ内容のDIBを作成します。            *
 *  DrawBitmap()        - DC内の指定された位置にビットマップを描画します。    *
 *  DibBlt()            - SetDIBitsToDevice()を使って、CIF_DIB形式でビット    *
 *                        マップを描画します。                                *
 *  StretchDibBlt()     - StretchDIBits()を使って、CIF_DIB形式でビットマップ  *
 *                        を描画します。                                      *
 *  lread()             - 64Kバイト以上を読み出すためのプライベートルーチン   *
 *  lwrite()            - 64Kバイト以上を書き込むためのプライベートルーチン   *
 *                                                                            *
 ******************************************************************************/

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "dib.h"

static   HCURSOR hcurSave;

/****************************************************************************
 *                                                                          *
 *  関数:   OpenDIB(LPSTR szFile)                                           *
 *                                                                          *
 *  目的:   DIBファイルを開いてメモリDIBを作成します。また、BITMAPINFO、    *
 *          パレットデータ、ビットを含むメモリハンドルを作成します。        *
 *                                                                          *
 *  戻り値: DIBを識別するハンドル                                           *
 *                                                                          *
 ****************************************************************************/
HANDLE OpenDIB (szFile)
LPSTR szFile;
{
    unsigned            fh;
    BITMAPINFOHEADER    bi;
    LPBITMAPINFOHEADER  lpbi;
    DWORD               dwLen = 0;
    DWORD               dwBits;
    HANDLE              hdib;
    HANDLE              h;
    OFSTRUCT            of;

    /* ファイルを開いてDIB情報を読み取る */
    fh = OpenFile(szFile, &of, OF_READ);
    if (fh == -1)
        return NULL;

    hdib = ReadDibBitmapInfo(fh);
    if (!hdib)
        return NULL;
    DibInfo(hdib,&bi);

    /* DIBの保持に必要なメモリ量を計算 */
    dwBits = bi.biSizeImage;
    dwLen  = bi.biSize + (DWORD)GetPaletteSize (&bi) + dwBits;

    /* DIBを保持するビットマップ情報バッファサイズを増やす */
    h = GlobalReAlloc(hdib, dwLen, GHND);
    if (!h){
        GlobalFree(hdib);
        hdib = NULL;
    }
    else
        hdib = h;

    /* ビットを読み取る */
    if (hdib){

        lpbi = (VOID FAR *)GlobalLock(hdib);
        lread(fh, (LPSTR)lpbi + (WORD)lpbi->biSize + GetPaletteSize(lpbi), dwBits);
        GlobalUnlock(hdib);
    }
    _lclose(fh);

    return hdib;
}

/****************************************************************************
 *                                                                          *
 *  関数:   OpenMemDIB(VOID)                                           *
 *                                                                          *
 *  目的:   クリップボードのDIBからメモリDIBを作成します。また、BITMAPINFO、    *
 *          パレットデータ、ビットを含むメモリハンドルを作成します。        *
 *                                                                          *
 *  戻り値: DIBを識別するハンドル                                           *
 *                                                                          *
 ****************************************************************************/
HANDLE OpenMemDIB (HWND hWnd)
{
    LPBITMAPINFOHEADER  lpbi;
    DWORD	i, dwLen = 0;
	HANDLE	h = NULL, hbi = NULL;
	LPSTR	p;

    /* ファイルを開いてDIB情報を読み取る */
	if (! OpenClipboard(hWnd))
        return NULL;

	if ((h = GetClipboardData(CF_DIB)) == NULL)
		goto end;

	dwLen = (int)GlobalSize(h);
	if ((hbi = GlobalAlloc(GHND, dwLen)) == NULL)
		goto end;

	p = (LPSTR)GlobalLock(hbi);
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(h);

	for (i = 0; i < dwLen;i++)
		*p++ = *((LPSTR)lpbi + i);

	GlobalUnlock(h);
	GlobalUnlock(hbi);

end:
	EmptyClipboard();
	CloseClipboard();
	return(hbi);
}

/****************************************************************************
 *                                                                          *
 *  関数:   WriteDIB(LPSTR szFile,HANDLE hdib)                              *
 *                                                                          *
 *  目的:   グローバルハンドルをDF_DIB形式でファイルに書き込みます。        *
 *                                                                          *
 *  戻り値: TRUE  - 正常に終了した場合                                      *
 *          FALSE - それ以外の場合                                          *
 *                                                                          *
 ****************************************************************************/
BOOL WriteDIB (szFile, hdib)
LPSTR szFile;
HANDLE hdib;
{
    BITMAPFILEHEADER    hdr;
    LPBITMAPINFOHEADER  lpbi;
    int                 fh;
    OFSTRUCT            of;

    if (!hdib)
        return FALSE;

    fh = OpenFile (szFile, &of, OF_CREATE|OF_READWRITE);
    if (fh == -1)
        return FALSE;

    lpbi = (VOID FAR *)GlobalLock (hdib);

    /* ファイルヘッダーの各メンバを設定 */
    hdr.bfType          = BFT_BITMAP;
    hdr.bfSize          = (int)GlobalSize (hdib) + sizeof (BITMAPFILEHEADER);
    hdr.bfReserved1     = 0;
    hdr.bfReserved2     = 0;
    hdr.bfOffBits       = (DWORD)sizeof(BITMAPFILEHEADER) + lpbi->biSize +
                          GetPaletteSize(lpbi);

    /* ファイルヘッダーを書き込む */
    _lwrite (fh, (LPSTR)&hdr, sizeof (BITMAPFILEHEADER));

    /* DIBヘッダーとビットを書き込む */
    lwrite (fh, (LPSTR)lpbi, (int)GlobalSize(hdib));

    GlobalUnlock (hdib);
    _lclose (fh);
    return TRUE;
}

/****************************************************************************
 *                                                                          *
 *  関数:   DibInfo(HANDLE hbi,LPBITMAPINFOHEADER lpbi)                     *
 *                                                                          *
 *  目的:   CF_DIB形式のメモリブロックに関連付けられているDIB情報を         *
 *          取得します。                                                    *
 *                                                                          *
 *  戻り値: TRUE  - 正常に終了した場合                                      *
 *          FALSE - それ以外の場合                                          *
 *                                                                          *
 ****************************************************************************/
BOOL DibInfo (hbi, lpbi)
HANDLE hbi;
LPBITMAPINFOHEADER lpbi;
{
    if (hbi){
        *lpbi = *(LPBITMAPINFOHEADER)GlobalLock (hbi);

        /* デフォルトのメンバ設定 */
        if (lpbi->biSize != sizeof (BITMAPCOREHEADER)){
            if (lpbi->biSizeImage == 0L)
                lpbi->biSizeImage =
                    WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount) * lpbi->biHeight;

            if (lpbi->biClrUsed == 0L)
                lpbi->biClrUsed = GetDibNumColors (lpbi);
        }
        GlobalUnlock (hbi);
        return TRUE;
    }
    return FALSE;
}

/****************************************************************************
 *                                                                          *
 *  関数:   CreateBIPalette(LPBITMAPINFOHEADER lpbi)                        *
 *                                                                          *
 *  目的:   BITMAPINFO構造体を指すポインタを指定すると、そのカラーテーブル  *
 *          からGDIパレットオブジェクトを作成します。                       *
 *                                                                          *
 *  戻り値: パレットを識別するハンドル                                      *
 *                                                                          *
 ****************************************************************************/
HPALETTE CreateBIPalette (lpbi)
LPBITMAPINFOHEADER lpbi;
{
    LOGPALETTE          *pPal;
    HPALETTE            hpal = NULL;
    WORD                nNumColors;
    BYTE                red;
    BYTE                green;
    BYTE                blue;
    WORD                i;
    RGBQUAD        FAR *pRgb;

    if (!lpbi)
        return NULL;

    if (lpbi->biSize != sizeof(BITMAPINFOHEADER))
        return NULL;

    /* カラーテーブルを指すポインタと色数を取得 */
    pRgb = (RGBQUAD FAR *)((LPSTR)lpbi + (WORD)lpbi->biSize);
    nNumColors = GetDibNumColors(lpbi);

    if (nNumColors){
        /* 論理パレット構造体を割り当てる */
        pPal = (LOGPALETTE*)LocalAlloc(LPTR,sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
        if (!pPal)
            return NULL;

        pPal->palNumEntries = nNumColors;
        pPal->palVersion    = PALVERSION;

        /*
         * DIBカラーテーブルからパレットエントリを設定し、
         * 論理カラーパレットを作成する
         */
        for (i = 0; i < nNumColors; i++){
            pPal->palPalEntry[i].peRed   = pRgb[i].rgbRed;
            pPal->palPalEntry[i].peGreen = pRgb[i].rgbGreen;
            pPal->palPalEntry[i].peBlue  = pRgb[i].rgbBlue;
            pPal->palPalEntry[i].peFlags = (BYTE)0;
        }
        hpal = CreatePalette(pPal);
        LocalFree((HANDLE)pPal);
    }
    else if (lpbi->biBitCount == 24){
        /*
         * 24ビットのDIBにはカラーテーブルエントリがないため、
         * 最大値(256)を設定する
         */
        nNumColors = MAXPALETTE;
        pPal = (LOGPALETTE*)LocalAlloc(LPTR,sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
        if (!pPal)
            return NULL;

        pPal->palNumEntries = nNumColors;
        pPal->palVersion    = PALVERSION;

        red = green = blue = 0;

        /*
         * 256 (= 8*8*4)通りのRGBの組み合わせを生成して
         * パレットに設定する
         */
        for (i = 0; i < pPal->palNumEntries; i++){
            pPal->palPalEntry[i].peRed   = red;
            pPal->palPalEntry[i].peGreen = green;
            pPal->palPalEntry[i].peBlue  = blue;
            pPal->palPalEntry[i].peFlags = (BYTE)0;

            if (!(red += 32))
                if (!(green += 32))
                    blue += 64;
        }
        hpal = CreatePalette(pPal);
        LocalFree((HANDLE)pPal);
    }
    return hpal;
}

/****************************************************************************
 *                                                                          *
 *  関数:   CreateDibPalette(HANDLE hbi)                                    *
 *                                                                          *
 *  目的:   BITMAPINFO構造体を指すグローバルハンドルを指定すると、その      *
 *          カラーテーブルからGDIパレットオブジェクトを作成します。         *
 *          (BITMAPINFOHEADER形式のDIB専用)                                 *
 *                                                                          *
 *  戻り値: パレットを識別するハンドル                                      *
 *                                                                          *
 ****************************************************************************/
HPALETTE CreateDibPalette (hbi)
HANDLE hbi;
{
    HPALETTE hpal;

    if (!hbi)
        return NULL;
    hpal = CreateBIPalette((LPBITMAPINFOHEADER)GlobalLock(hbi));
    GlobalUnlock(hbi);
    return hpal;
}

/****************************************************************************
 *                                                                          *
 *  関数:   ReadDibBitmapInfo(int fh)                                       *
 *                                                                          *
 *  目的:   DIB形式のファイルを読み取り、そのBITMAPINFOのグローバルハン     *
 *          ドルを返します。この関数は以前の(BITMAPCOREHEADER)形式と新し    *
 *          いBITMAPINFOHEADER)形式の両方を処理できますが、返すのはつね     *
 *          に新しいBITMAPINFOです。                                        *
 *                                                                          *
 *  戻り値: ファイル内のDIBのBITMAPINFOを識別するハンドル                   *
 *                                                                          *
 ****************************************************************************/
HANDLE ReadDibBitmapInfo (fh)
int fh;
{
    DWORD     off;
    HANDLE    hbi = NULL;
    int       size;
    int       i;
    WORD      nNumColors;

    RGBQUAD FAR       *pRgb;
    BITMAPINFOHEADER   bi;
    BITMAPCOREHEADER   bc;
    LPBITMAPINFOHEADER lpbi;
    BITMAPFILEHEADER   bf;
    DWORD              dwWidth = 0;
    DWORD              dwHeight = 0;
    WORD               wPlanes, wBitCount;

    if (fh == -1)
        return NULL;

    /* ファイルポインタをリセットし、ファイルヘッダーを読み取る */
    off = _llseek(fh, 0L, SEEK_CUR);
    if (sizeof (bf) != _lread (fh, (LPSTR)&bf, sizeof (bf)))
        return FALSE;

    /* RCヘッダーがあるか調べる */
    if (!ISDIB (bf.bfType)) {
        bf.bfOffBits = 0L;
        _llseek (fh, off, SEEK_SET);
    }
    if (sizeof (bi) != _lread (fh, (LPSTR)&bi, sizeof(bi)))
        return FALSE;

    nNumColors = GetDibNumColors (&bi);

    /*
     * 情報ブロックの内容(BITMAPINFOまたはBITMAPCORE)をチェックし、
     * それに従って情報を取得する。BITMAPCOREHEADERならば、情報を
     * BITMAPINFOHEADER形式のブロックに転送する。
     */
    switch (size = (int)bi.biSize){
        case sizeof (BITMAPINFOHEADER):
            break;

        case sizeof (BITMAPCOREHEADER):

            bc = *(BITMAPCOREHEADER*)&bi;

            dwWidth   = (DWORD)bc.bcWidth;
            dwHeight  = (DWORD)bc.bcHeight;
            wPlanes   = bc.bcPlanes;
            wBitCount = bc.bcBitCount;

            bi.biSize               = sizeof(BITMAPINFOHEADER);
            bi.biWidth              = dwWidth;
            bi.biHeight             = dwHeight;
            bi.biPlanes             = wPlanes;
            bi.biBitCount           = wBitCount;

            bi.biCompression        = BI_RGB;
            bi.biSizeImage          = 0;
            bi.biXPelsPerMeter      = 0;
            bi.biYPelsPerMeter      = 0;
            bi.biClrUsed            = nNumColors;
            bi.biClrImportant       = nNumColors;

            _llseek (fh, (LONG)sizeof (BITMAPCOREHEADER) - sizeof (BITMAPINFOHEADER), SEEK_CUR);
            break;

        default:
            /* DIBではない */
            return NULL;
    }

    /* 0ならばデフォルト値を設定 */
    if (bi.biSizeImage == 0){
        bi.biSizeImage = WIDTHBYTES ((DWORD)bi.biWidth * bi.biBitCount)
                         * bi.biHeight;
    }
    if (bi.biClrUsed == 0)
        bi.biClrUsed = GetDibNumColors(&bi);

    /* BITMAPINFO構造体とカラーテーブルを割り当てる */
    hbi = GlobalAlloc (GHND, (LONG)bi.biSize + nNumColors * sizeof(RGBQUAD));
    if (!hbi)
        return NULL;
    lpbi = (VOID FAR *)GlobalLock (hbi);
    *lpbi = bi;

    /* カラーテーブルを指すポインタを取得 */
    pRgb = (RGBQUAD FAR *)((LPSTR)lpbi + bi.biSize);
    if (nNumColors){
        if (size == sizeof(BITMAPCOREHEADER)){
            /*
             * 古いカラーテーブル(3バイトのRGBTRIPLE)を新しいカラーテー
             * ブル(4バイトのRGBQUAD)に変換
             */
            _lread (fh, (LPSTR)pRgb, nNumColors * sizeof(RGBTRIPLE));

            for (i = nNumColors - 1; i >= 0; i--){
                RGBQUAD rgb;

                rgb.rgbRed      = ((RGBTRIPLE FAR *)pRgb)[i].rgbtRed;
                rgb.rgbBlue     = ((RGBTRIPLE FAR *)pRgb)[i].rgbtBlue;
                rgb.rgbGreen    = ((RGBTRIPLE FAR *)pRgb)[i].rgbtGreen;
                rgb.rgbReserved = (BYTE)0;

                pRgb[i] = rgb;
            }
        }
        else
            _lread(fh,(LPSTR)pRgb,nNumColors * sizeof(RGBQUAD));
    }

    if (bf.bfOffBits != 0L)
        _llseek(fh,off + bf.bfOffBits,SEEK_SET);

    GlobalUnlock(hbi);
    return hbi;
}

/****************************************************************************
 *                                                                          *
 *  関数:   GetPaletteSize(VOID FAR * pv)                                      *
 *                                                                          *
 *  目的:   パレットのバイト数を計算します。情報ブロックがBITMAPCOREHEADER  *
 *          型ならば、色数の3倍がパレットサイズになります。それ以外の場合   *
 *          は、色数の4倍がパレットサイズになります。                       *
 *                                                                          *
 *  戻り値: パレットのバイト数                                              *
 *                                                                          *
 ****************************************************************************/
WORD GetPaletteSize (pv)
VOID FAR * pv;
{
    LPBITMAPINFOHEADER lpbi;
    WORD               NumColors;

    lpbi      = (LPBITMAPINFOHEADER)pv;
    NumColors = GetDibNumColors(lpbi);

    if (lpbi->biSize == sizeof(BITMAPCOREHEADER))
        return NumColors * sizeof(RGBTRIPLE);
    else
        return NumColors * sizeof(RGBQUAD);
}

/****************************************************************************
 *                                                                          *
 *  関数:   GetDibNumColorsByHandle(HANDLE h)                               *
 *                                                                          *
 *  目的:   情報ブロックのBitCountメンバを参照して、DIBの色数を判断します。 *
 *                                                                          *
 *  戻り値: DIBの色数                                                       *
 *                                                                          *
 ****************************************************************************/
WORD GetDibNumColorsByHandle(HANDLE h)
{
    LPBITMAPINFOHEADER  lpbi;
	WORD	n;


    lpbi = (LPBITMAPINFOHEADER)GlobalLock(h);
	n = GetDibNumColors((LPVOID)lpbi);
	GlobalUnlock(h);
	return(n);
}

/****************************************************************************
 *                                                                          *
 *  関数:   GetDibNumColors(VOID FAR * pv)                                     *
 *                                                                          *
 *  目的:   情報ブロックのBitCountメンバを参照して、DIBの色数を判断します。 *
 *                                                                          *
 *  戻り値: DIBの色数                                                       *
 *                                                                          *
 ****************************************************************************/
WORD GetDibNumColors (pv)
VOID FAR * pv;
{
    int                 bits;
    LPBITMAPINFOHEADER  lpbi;
    LPBITMAPCOREHEADER  lpbc;

    lpbi = ((LPBITMAPINFOHEADER)pv);
    lpbc = ((LPBITMAPCOREHEADER)pv);

    /*
     *  BITMAPINFO形式ヘッダーの場合、パレットのサイズはBITMAPCORE形式の
     *  ヘッダーのbiClrUsedが示している。パレットのサイズは、ピクセル当たり
     *  のビット数により異なる。
     */
    if (lpbi->biSize != sizeof(BITMAPCOREHEADER)){
        if (lpbi->biClrUsed != 0)
            return (WORD)lpbi->biClrUsed;
        bits = lpbi->biBitCount;
    }
    else
        bits = lpbc->bcBitCount;

    switch (bits){
        case 1:                return 2;
        case 4:
                return 16;
        case 8:
                return 256;
        default:
                /* 24ビットDIBにはカラーテーブルはない */
                return 0;
    }
}
/****************************************************************************
 *                                                                          *
 *  関数:   DibFromBitmap()                                                 *
 *                                                                          *
 *  目的:   指定されたDDB(デバイスに依存するビットマップ)と同じ内容のDIB    *
 *          を含むグローバルメモリブロックを作成します。                    *
 *                                                                          *
 *  戻り値: DIBを識別するハンドル                                           *
 *                                                                          *
 ****************************************************************************/
HANDLE DibFromBitmap (HBITMAP hbm, DWORD biStyle, WORD biBits, HPALETTE hpal)
{
    BITMAP               bm;
    BITMAPINFOHEADER     bi;
    BITMAPINFOHEADER FAR *lpbi;
    DWORD                dwLen;
    HANDLE               hdib;
    HANDLE               h;
    HDC                  hdc;

    if (!hbm)
        return NULL;

    if (hpal == NULL)
        hpal = GetStockObject(DEFAULT_PALETTE);

    GetObject(hbm,sizeof(bm),(LPSTR)&bm);

    if (biBits == 0)
        biBits =  bm.bmPlanes * bm.bmBitsPixel;

    bi.biSize               = sizeof(BITMAPINFOHEADER);
    bi.biWidth              = bm.bmWidth;
    bi.biHeight             = bm.bmHeight;
    bi.biPlanes             = 1;
    bi.biBitCount           = biBits;
    bi.biCompression        = biStyle;
    bi.biSizeImage          = 0;
    bi.biXPelsPerMeter      = 0;
    bi.biYPelsPerMeter      = 0;
    bi.biClrUsed            = 0;
    bi.biClrImportant       = 0;

    dwLen  = bi.biSize + GetPaletteSize(&bi);

    hdc = GetDC(NULL);
    hpal = SelectPalette(hdc,hpal,FALSE);
         RealizePalette(hdc);

    hdib = GlobalAlloc(GHND,dwLen);

    if (!hdib){
        SelectPalette(hdc,hpal,FALSE);
        ReleaseDC(NULL,hdc);
        return NULL;
    }

    lpbi = (VOID FAR *)GlobalLock(hdib);

    *lpbi = bi;

    /*
     *  lpBitsパラメータにNULLを指定してGetDIBitsを呼び出し、
     *  biSizeImageメンバを計算させる
     */
    GetDIBits(hdc, hbm, 0, (WORD)bi.biHeight,
        NULL, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);

    bi = *lpbi;
    GlobalUnlock(hdib);

    /* ドライバがbiSizeImageメンバを設定しなかった場合はここで補う */
    if (bi.biSizeImage == 0){
        bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * biBits) * bm.bmHeight;

        if (biStyle != BI_RGB)
            bi.biSizeImage = (bi.biSizeImage * 3) / 2;
    }

    /* ビット全体を保持するのに十分な大きさのバッファを再割り当てする */
    dwLen = bi.biSize + GetPaletteSize(&bi) + bi.biSizeImage;
    if (h = GlobalReAlloc(hdib,dwLen,0))
        hdib = h;
    else{
        GlobalFree(hdib);
        hdib = NULL;

        SelectPalette(hdc,hpal,FALSE);
        ReleaseDC(NULL,hdc);
        return hdib;
    }
    /*
     * lpBitsパラメータにNULL以外の値を設定してGetDIBitsを呼び出し、
     * 実際にビットデータを取得する
     */
    lpbi = (VOID FAR *)GlobalLock(hdib);

    if (GetDIBits( hdc,
                   hbm,
                   0,
                   (WORD)bi.biHeight,
                   (LPSTR)lpbi + (WORD)lpbi->biSize + GetPaletteSize(lpbi),
                   (LPBITMAPINFO)lpbi, DIB_RGB_COLORS) == 0){
         GlobalUnlock(hdib);
         hdib = NULL;
         SelectPalette(hdc,hpal,FALSE);
         ReleaseDC(NULL,hdc);
         return NULL;
    }

    bi = *lpbi;
    GlobalUnlock(hdib);

    SelectPalette(hdc,hpal,FALSE);
    ReleaseDC(NULL,hdc);
    return hdib;
}

/****************************************************************************
 *                                                                          *
 *  関数:   BitmapFromDib(HANDLE hdib, HPALETTE hpal)                       *
 *                                                                          *
 *  目的:   CF_DIB形式のメモリブロックのグローバルハンドルを指定すると、    *
 *          DDB (デバイスに依存しないビットマップ)を作成します。            *
 *                                                                          *
 *  戻り値: DDBを識別するハンドル                                           *
 *                                                                          *
 ****************************************************************************/
HBITMAP BitmapFromDib (HANDLE hdib, HPALETTE hpal)
{
    LPBITMAPINFOHEADER  lpbi;
    HPALETTE            hpalT;
    HDC                 hdc;
    HBITMAP             hbm;

    StartWait();

    if (!hdib)
        return NULL;

    lpbi = (VOID FAR *)GlobalLock(hdib);

    if (!lpbi)
        return NULL;

    hdc = GetDC(NULL);

    if (hpal){
        hpalT = SelectPalette(hdc,hpal,FALSE);
        RealizePalette(hdc);     // GDIのバグ????
    }

    hbm = CreateDIBitmap(hdc,
                (LPBITMAPINFOHEADER)lpbi,
                (LONG)CBM_INIT,
                (LPSTR)lpbi + lpbi->biSize + GetPaletteSize(lpbi),
                (LPBITMAPINFO)lpbi,
                DIB_RGB_COLORS );

    if (hpal)
        SelectPalette(hdc,hpalT,FALSE);

    ReleaseDC(NULL,hdc);
    GlobalUnlock(hdib);

    EndWait();

    return hbm;
}
/****************************************************************************
 *                                                                          *
 *  関数:   DrawBitmap(HDC hdc, int x, int y, HBITMAP hbm, DWORD rop)       *
 *                                                                          *
 *  目的:   DCのhdc内の指定された位置にビットマップhbmを描画します。        *
 *                                                                          *
 *  戻り値: BitBlt()の戻り値                                                *
 *                                                                          *
 ****************************************************************************/
BOOL DrawBitmap (hdc, x, y, hbm, rop)
HDC        hdc;
int        x, y;
HBITMAP    hbm;
DWORD      rop;
{
    HDC       hdcBits;
    BITMAP    bm;
    BOOL      f;

    if (!hdc || !hbm)
        return FALSE;

    hdcBits = CreateCompatibleDC(hdc);
    GetObject(hbm,sizeof(BITMAP),(LPSTR)&bm);
    SelectObject(hdcBits,hbm);
    f = BitBlt(hdc,0,0,bm.bmWidth,bm.bmHeight,hdcBits,0,0,rop);
    DeleteDC(hdcBits);

    return f;
}
/****************************************************************************
 *                                                                          *
 *  関数:  DibBlt( HDC hdc,                                                 *
 *                 int x0, int y0,                                          *
 *                 int dx, int dy,                                          *
 *                 HANDLE hdib,                                             *
 *                 int x1, int y1,                                          *
 *                 LONG rop)                                                *
 *                                                                          *
 *  目的:   SetDIBitsToDeviceを使って、CF_DIB形式のビットマップを描画       *
 *          します。BitBltと同じパラメータを取ります。                      *
 *                                                                          *
 *  戻り値: TRUE  - 正常に終了した場合                                      *
 *          FALSE - それ以外の場合                                          *
 *                                                                          *
 ****************************************************************************/
BOOL DibBlt (hdc, x0, y0, dx, dy, hdib, x1, y1, rop)
HDC        hdc;
int        x0, y0, dx, dy;
HANDLE     hdib;
int        x1, y1;
LONG       rop;
{
    LPBITMAPINFOHEADER   lpbi;
    LPSTR                pBuf;

    if (!hdib)
        return PatBlt(hdc,x0,y0,dx,dy,rop);

    lpbi = (VOID FAR *)GlobalLock(hdib);

    if (!lpbi)
        return FALSE;

    pBuf = (LPSTR)lpbi + (WORD)lpbi->biSize + GetPaletteSize(lpbi);
    SetDIBitsToDevice (hdc, x0, y0, dx, dy,
                       x1,y1,
                       x1,
                       dy,
                       pBuf, (LPBITMAPINFO)lpbi,
                       DIB_RGB_COLORS );

    GlobalUnlock(hdib);
    return TRUE;
}
/****************************************************************************
 *                                                                          *
 *  関数:   StretchDibBlt( HDC hdc,                                         *
 *                         int x, int y,                                    *
 *                         int dx, int dy,                                  *
 *                         HANDLE hdib,                                     *
 *                         int x0, int y0,                                  *
 *                         int dx0, int dy0,                                *
 *                         LONG rop)                                        *
 *                                                                          *
 *  目的:   StrechDIBits()を使って、CF_DIB形式のビットマップを描画します。  *
 *          StretchBlt()と同じパラメータを取ります。                        *
 *                                                                          *
 *  戻り値: TRUE  - 正常に終了した場合                                      *
 *          FALSE - それ以外の場合                                          *
 *                                                                          *
 ****************************************************************************/
BOOL StretchDibBlt (hdc, x, y, dx, dy, hdib, x0, y0, dx0, dy0, rop)
HDC hdc;
int x, y;
int dx, dy;
HANDLE hdib;
int x0, y0;
int dx0, dy0;
LONG rop;

{
    LPBITMAPINFOHEADER lpbi;
    LPSTR        pBuf;
    BOOL         f;

    if (!hdib)
        return PatBlt(hdc,x,y,dx,dy,rop);

    lpbi = (VOID FAR *)GlobalLock(hdib);

    if (!lpbi)
        return FALSE;

    pBuf = (LPSTR)lpbi + (WORD)lpbi->biSize + GetPaletteSize(lpbi);

    f = StretchDIBits ( hdc,
                        x, y,
                        dx, dy,
                        x0, y0,
                        dx0, dy0,
                        pBuf, (LPBITMAPINFO)lpbi,
                        DIB_RGB_COLORS,
                        rop);

    GlobalUnlock(hdib);
    return f;
}

 /********** 64Kバイト以上の読み書きを行うプライベートルーチン ************/
/****************************************************************************
 *                                                                          *
 *  関数:   lread(int fh, VOID FAR *pv, DWORD ul)                           *
 *                                                                          *
 *  目的:   データをすべて読み取るまで32Kバイトずつデータを読み取ります。   *
 *                                                                          *
 *  戻り値: 0                  - 正常に読み取れなかった場合                 *
 *          読み取ったバイト数 - それ以外の場合                             *
 *                                                                          *
 ****************************************************************************/
DWORD PASCAL lread (fh, pv, ul)
int           fh;
VOID far      *pv;
DWORD         ul;
{
    DWORD     ulT = ul;
    BYTE	*hp = pv;

    while (ul > (DWORD)MAXREAD) {
        if (_lread(fh, (LPSTR)hp, (WORD)MAXREAD) != MAXREAD)
                return 0;
        ul -= MAXREAD;
        hp += MAXREAD;
    }
    if (_lread(fh, (LPSTR)hp, (WORD)ul) != (WORD)ul)
        return 0;
    return ulT;
}

/****************************************************************************
 *                                                                          *
 *  関数:   lwrite(int fh, VOID FAR *pv, DWORD ul)                          *
 *                                                                          *
 *  目的:   データをすべて書き込むまで32Kバイトずつデータを書き込みます。   *
 *                                                                          *
 *  戻り値: 0                  - 正常に書き込めなかった場合                 *
 *          書き込んだバイト数 - それ以外の場合                             *
 *                                                                          *
 ****************************************************************************/
DWORD PASCAL lwrite (fh, pv, ul)
int          fh;
VOID FAR     *pv;
DWORD        ul;
{
    DWORD     ulT = ul;
    BYTE	*hp = pv;

    while (ul > MAXREAD) {
        if (_lwrite(fh, (LPSTR)hp, (WORD)MAXREAD) != MAXREAD)
                return 0;
        ul -= MAXREAD;
        hp += MAXREAD;
    }
    if (_lwrite(fh, (LPSTR)hp, (WORD)ul) != (WORD)ul)
        return 0;
    return ulT;
}
