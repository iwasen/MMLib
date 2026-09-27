// Item.h: CItem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MMLibDB.h"

class CItem : public CObject
{
public:
	CItem();
	virtual ~CItem();

	int m_nItemType;
	HTREEITEM m_hItem;
	BOOL m_bReadChildren;
	BOOL m_bCheck;
	CItem *m_pNextSelected;

	static void RemoveAll();
	CItem *GetParent() const;
	virtual LPCTSTR GetID() = 0;
	virtual LPCTSTR GetTitle() = 0;
	static CItem *GetHeadItem();
	static CItem *GetNextItem();

protected:
	static CObList m_oItemList;
	static POSITION m_oPosition;
};

class CItemRoot : public CItem
{
public:
	CItemRoot();
	virtual LPCTSTR GetID() { return ""; }
	virtual LPCTSTR GetTitle() { return ""; }
};

class CItemFolder : public CItem
{
public:
	CItemFolder(CMMPcsDB *pPcsDB);

	CMMPcsDB *m_pPcsDB;
	FOLDERREC m_oFolderRec;
	int m_nFolderType;
	CString m_sFirstData;

	virtual LPCTSTR GetID() { return m_oFolderRec.sFolderID; }
	virtual LPCTSTR GetTitle() { return m_oFolderRec.sFolderName; }
	CString GetFirstData();
};

class CItemInbox : public CItemFolder
{
public:
	CItemInbox(CMMPcsDB *pPcsDB);
};

class CItemData : public CItem
{
public:
	CItemData(CMMPcsDB *pPcsDB);
	virtual ~CItemData();

	CMMPcsDB *m_pPcsDB;
	DATAREC m_oDataRec;
	HDIB m_hDIB;
	CSize m_oSize;
	CRect m_oRect;

	virtual LPCTSTR GetID() { return m_oDataRec.sDataID; }
	virtual LPCTSTR GetTitle() { return m_oDataRec.sTitle; }
};

class CItemFind : public CItem
{
public:
	CItemFind();

	FINDREC m_oFindRec;
	int m_nFindType;
	BOOL m_bSave;

	virtual LPCTSTR GetID() { return m_oFindRec.sFindID; }
	virtual LPCTSTR GetTitle() { return m_oFindRec.sFolderName; }
};

class CItemPending : public CItem
{
public:
	CItemPending();

	CString m_sTitle;
	CString m_sServerName;
	int m_nChType;
	CString m_sDirName;
	CString m_sLibraryID;
	int m_nCacheMode;

	virtual LPCTSTR GetID() { return m_sLibraryID; }
	virtual LPCTSTR GetTitle() { return m_sTitle; }
};

class CAddItem : public CObject
{
public:
	int m_nAddItemType;
};

class CAddItemLibrary : public CAddItem
{
public:
	CAddItemLibrary() { m_nAddItemType = ADDITEM_LIBRARY; }

	CMMPcsDB *m_pPcsDB;
};

class CAddItemPending : public CAddItem
{
public:
	CAddItemPending() { m_nAddItemType = ADDITEM_PENDING; }

	CString m_sTitle;
	CString m_sServerName;
	int m_nChType;
	CString m_sDirName;
	CString m_sLibraryID;
	int m_nCacheMode;
};

class CAddItemFolder : public CAddItem
{
public:
	CAddItemFolder() { m_nAddItemType = ADDITEM_FOLDER; }

	CItemFolder *m_pItemFolder;
	FOLDERREC *m_pFolderRec;
	CItemFolder *m_pItemAddFolder;
};

class CAddItemData : public CAddItem
{
public:
	CAddItemData() { m_nAddItemType = ADDITEM_DATA; }

	CItemFolder *m_pItemFolder;
	DATAREC *m_pDataRec;
	CItemData *m_pItemAddData;
};

class CAddItemFind : public CAddItem
{
public:
	CAddItemFind() { m_nAddItemType = ADDITEM_FIND; }

	CItem *m_pItemParent;
	FINDREC *m_pFindRec;
	BOOL m_bSelect;
	BOOL m_bSave;
};

class CSelectItem : public CObject
{
public:
	CString m_sLibraryID;
	CString m_sFolderID;
	CString m_sDataID;
};
