// Mml.cpp: CMmlWrite クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define FILE_TYPE	"MML1"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CMmlWrite::CMmlWrite()
{
	m_pPageListTop = NULL;
	m_pPageListBottom = NULL;
	m_pItemNameListTop = NULL;
	m_pItemNameListBottom = NULL;
	m_bNewPage = FALSE;
}

CMmlWrite::~CMmlWrite()
{
	Close();
}

BOOL CMmlWrite::Create(LPCSTR pFileName)
{
	if (!m_oFile.Open(pFileName, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive))
		return FALSE;

	m_bNewPage = TRUE;

	FILE_HEADER oFileHeader{};
	m_oFile.Write(&oFileHeader, sizeof(oFileHeader));

	return TRUE;
}

BOOL CMmlWrite::WriteItem(LPCSTR pItemName, LPCVOID pData, long len)
{
	if (m_oFile.m_hFile == CFile::hFileNull)
		return FALSE;

	if (len == 0)
		return TRUE;

	if (m_bNewPage) {
		NewPage();
		m_bNewPage = FALSE;
	}

	m_pPageListBottom->nItem++;

	ITEM_HEADER oItemHeader{
		.nItemNo = (short)GetItemNo(pItemName),
		.nLength = len
	};
	m_oFile.Write(&oItemHeader, sizeof(oItemHeader));

	m_oFile.Write(pData, len);

	return TRUE;
}

void CMmlWrite::NextPage()
{
	if (!m_bNewPage)
		m_bNewPage = TRUE;
}

void CMmlWrite::Close()
{
	FILE_HEADER oFileHeader;
	PageList *pPageList, *pPageListNext;
	ItemNameList *pItemNameList, *pItemNameListNext;
	int n;
	char aItemName[10];

	if (m_oFile.m_hFile == CFile::hFileNull)
		return;

	memcpy(oFileHeader.fileType, FILE_TYPE, 4);
	oFileHeader.nPageTableOffset = (long)m_oFile.Seek(0, CFile::current);

	for (pPageList = m_pPageListTop, n = 0; pPageList != NULL; pPageList = pPageListNext, n++) {
		pPageListNext = pPageList->pChain;
		PAGE_TABLE oPageTable{
			.nOffset = pPageList->nOffset,
			.nItem = pPageList->nItem
		};
		m_oFile.Write(&oPageTable, sizeof(oPageTable));
		delete pPageList;
	}

	oFileHeader.nPageTableNum = n;
	oFileHeader.nItemTableOffset = (long)m_oFile.Seek(0, CFile::current);

	for (pItemNameList = m_pItemNameListTop, n = 0; pItemNameList != NULL; pItemNameList = pItemNameListNext, n++) {
		pItemNameListNext = pItemNameList->pChain;
		memset(aItemName, ' ', sizeof(aItemName));
		memcpy(aItemName, (LPCSTR)pItemNameList->sItemName, min(pItemNameList->sItemName.GetLength(), sizeof(aItemName)));
		m_oFile.Write(aItemName, sizeof(aItemName));
		delete pItemNameList;
	}

	oFileHeader.nItemTableNum = n;

	m_oFile.SeekToBegin();
	m_oFile.Write(&oFileHeader, sizeof(oFileHeader));

	m_oFile.Close();
}

void CMmlWrite::NewPage()
{
	PageList *pPageList = new PageList;

	pPageList->pChain = NULL;
	pPageList->nOffset = (long)m_oFile.Seek(0, CFile::current);
	pPageList->nItem = 0;

	if (m_pPageListTop == NULL)
		m_pPageListTop = pPageList;

	if (m_pPageListBottom != NULL)
		m_pPageListBottom->pChain = pPageList;
	m_pPageListBottom = pPageList;
}

int CMmlWrite::GetItemNo(LPCSTR pItemName)
{
	ItemNameList *pItemNameList;
	int n;

	for (pItemNameList = m_pItemNameListTop, n = 0; pItemNameList != NULL; pItemNameList = pItemNameList->pChain, n++) {
		if (pItemNameList->sItemName == pItemName)
			return pItemNameList->nItemNo;
	}

	pItemNameList = new ItemNameList;
	pItemNameList->pChain = NULL;
	pItemNameList->sItemName = pItemName;
	pItemNameList->nItemNo = n;

	if (m_pItemNameListTop == NULL)
		m_pItemNameListTop = pItemNameList;

	if (m_pItemNameListBottom != NULL)
		m_pItemNameListBottom->pChain = pItemNameList;
	m_pItemNameListBottom = pItemNameList;

	return n;
}

//////////////////////////////////////////////////////////////////////
// CMmlRead クラス
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CMmlRead::CMmlRead()
{
	m_pPageTable = NULL;
	m_pItemTable = NULL;
	m_nPage = 0;
}

CMmlRead::~CMmlRead()
{
	Close();
}

BOOL CMmlRead::Open(LPCSTR pFileName)
{
	if (!m_oFile.Open(pFileName, CFile::modeRead | CFile::shareDenyWrite))
		return FALSE;

	m_oFile.Read(&m_oFileHeader, sizeof(m_oFileHeader));

	if (memcmp(m_oFileHeader.fileType, FILE_TYPE, 4) != 0) {
		m_oFile.Close();
		return FALSE;
	}

	m_nPage = m_oFileHeader.nPageTableNum;

	char (*pItemTable)[10];
	m_pPageTable = new PAGE_TABLE[m_oFileHeader.nPageTableNum];
	m_oFile.Seek(m_oFileHeader.nPageTableOffset, CFile::begin);
	m_oFile.Read(m_pPageTable, m_oFileHeader.nPageTableNum * sizeof(PAGE_TABLE));

	pItemTable = new char[m_oFileHeader.nItemTableNum][10];
	m_oFile.Seek(m_oFileHeader.nItemTableOffset, CFile::begin);
	m_oFile.Read(pItemTable, m_oFileHeader.nItemTableNum * 10);

	m_pItemTable = new CString[m_oFileHeader.nItemTableNum];
	for (int i = 0; i < m_oFileHeader.nItemTableNum; i++) {
		m_pItemTable[i] = CString(pItemTable[i], 10);
		m_pItemTable[i].TrimRight();
	}

	delete [] pItemTable;

	SetPage(0);

	return TRUE;
}

BOOL CMmlRead::SetPage(int nPage)
{
	if (m_oFile.m_hFile == CFile::hFileNull)
		return FALSE;

	if (nPage >= m_oFileHeader.nPageTableNum)
		return FALSE;

	m_oFile.Seek(m_pPageTable[nPage].nOffset, CFile::begin);
	m_nMaxItem = m_pPageTable[nPage].nItem;
	m_nCurrentItem = 0;

	return TRUE;
}

BOOL CMmlRead::ReadItem(LPSTR pItemName, LPVOID *pData, long *pLen)
{
	if (m_oFile.m_hFile == CFile::hFileNull)
		return FALSE;

	if (m_nCurrentItem == m_nMaxItem)
		return FALSE;

	m_nCurrentItem++;

	ITEM_HEADER oItemHeader;
	m_oFile.Read(&oItemHeader, sizeof(oItemHeader));

	strcpy(pItemName, m_pItemTable[oItemHeader.nItemNo]);

	*pData = new char[oItemHeader.nLength];
	*pLen = oItemHeader.nLength;
	m_oFile.Read(*pData, oItemHeader.nLength);

	return TRUE;
}

void CMmlRead::Close()
{
	if (m_oFile.m_hFile != CFile::hFileNull)
		m_oFile.Close();

	if (m_pPageTable != NULL) {
		delete [] m_pPageTable;
		m_pPageTable = NULL;
	}

	if (m_pItemTable != NULL) {
		delete [] m_pItemTable;
		m_pItemTable = NULL;
	}
}

