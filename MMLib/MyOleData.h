// MyOleData.h: CMyOleDataSource クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Item.h"

class CMyOleDataSource : public COleDataSource
{
public:
	CItem *m_pItem;

protected:
	CString m_sTempFileName;

	virtual BOOL OnRenderGlobalData(LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal);
};
