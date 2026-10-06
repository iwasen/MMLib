// Item.cpp: CItem クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MMView.h"
#include "Item.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CItem::CItem()
{
	m_bRead = FALSE;
	m_hDIB1 = NULL;
	m_hDIB2 = NULL;
}

CItem::~CItem()
{
	if (m_hDIB1 != NULL)
		::MMPictureFree(m_hDIB1);

	if (m_hDIB2 != NULL)
		::MMPictureFree(m_hDIB2);
}

void CItem::ReadItem(CMmlRead *pMmlRead, int nPage)
{
	char aItemName[11];
	LPVOID pItemData;
	long nItemLen;

	if (m_bRead)
		return;

	if (!pMmlRead->SetPage(nPage))
		return;

	while (pMmlRead->ReadItem(aItemName, &pItemData, &nItemLen)) {
		if (strcmp(aItemName, "CDATE") == 0)
			m_sCreateDate = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "UDATE") == 0)
			m_sUpdateDate = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "CREATOR") == 0)
			m_sCreator = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "CODE") == 0)
			m_sCode = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "TITLE") == 0)
			m_sTitle = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "REMARK1") == 0)
			m_sRemark1 = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "REMARK2") == 0)
			m_sRemark2 = CString((LPCSTR)pItemData, nItemLen);
		else if (strcmp(aItemName, "PICT1") == 0) {
			MMPictureReadMem(&m_hDIB1, pItemData, nItemLen);
			if (m_hDIB1 != NULL)
				m_hDIB2 = MakeSmallPicture(m_hDIB1, PICT_WIDTH, PICT_HEIGHT);
		} else if (strcmp(aItemName, "TEXT") == 0)
			m_sText = CString((LPCSTR)pItemData, nItemLen);

		delete pItemData;
	}

	m_bRead = TRUE;
}
