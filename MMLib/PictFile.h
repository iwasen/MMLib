// PictFile.h: CPictFile クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CPictFile
{
public:
	CPictFile();
	virtual ~CPictFile();

	void ReadIniFile();
	LPCSTR ReadPict(CItemData *pItemData, int nSupportFile, long *pSize);

protected:
	LPSTR m_pPict;
	HANDLE m_hMem;
	int m_nActual;
	int m_nScale;
	int m_nHLimit;
	int m_nHPixel;
	int m_nVLimit;
	int m_nVPixel;

	void FreeMem();
	SHORT ConvertMMType1(LPCSTR pPict, int nSupportFile, long *pSize);
	SHORT ConvertMMType2(LPCSTR pPict, int nSupportFile, long *pSize);
};
