// Mml.h: CMmlWrite クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

struct PageList {
	PageList *pChain;
	long nOffset;
	long nItem;
};

struct ItemNameList {
	ItemNameList *pChain;
	CString sItemName;
	int nItemNo;
};

#pragma pack(1)
struct FILE_HEADER {
	char fileType[4];
	long nPageTableOffset;
	long nPageTableNum;
	long nItemTableOffset;
	long nItemTableNum;
};

struct PAGE_TABLE {
	long nOffset;
	long nItem;
};

struct ITEM_HEADER {
	short nItemNo;
	long nLength;
};
#pragma pack()

class CMmlWrite : public CObject
{
public:
	CMmlWrite();
	virtual ~CMmlWrite();

	BOOL Create(LPCSTR pFileName);
	BOOL WriteItem(LPCSTR pItemName, LPCVOID pData, long len);
	void NextPage();
	void Close();

protected:
	CFile m_oFile;
	PageList *m_pPageListTop;
	PageList *m_pPageListBottom;
	ItemNameList *m_pItemNameListTop;
	ItemNameList *m_pItemNameListBottom;
	BOOL m_bNewPage;

	void NewPage();
	int GetItemNo(LPCSTR pItemName);
};

class CMmlRead : public CObject
{
public:
	CMmlRead();
	virtual ~CMmlRead();

	int m_nPage;

	BOOL Open(LPCSTR pFileName);
	BOOL SetPage(int nPage);
	BOOL ReadItem(LPSTR pItemName, LPVOID *pData, long *pLen);
	void Close();

protected:
	CFile m_oFile;
	FILE_HEADER m_oFileHeader;
	PAGE_TABLE *m_pPageTable;
	CString *m_pItemTable;
	int m_nMaxItem;
	int m_nCurrentItem;
};
