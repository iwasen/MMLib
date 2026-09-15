// Item.cpp: CItem クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MMLib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CItem クラス
//////////////////////////////////////////////////////////////////////

CObList CItem::m_oItemList;
POSITION CItem::m_oPosition;

CItem::CItem()
{
	m_hItem = NULL;
	m_bReadChildren = FALSE;

	m_oItemList.AddTail(this);
}

CItem::~CItem()
{
	POSITION pos;
	HANDLE hMem;

	if ((pos = m_oItemList.Find(this)) != NULL)
		m_oItemList.RemoveAt(pos);

	if (::AfxGetApp()->m_pMainWnd->OpenClipboard()) {
		if ((hMem = ::GetClipboardData(g_nCFMMLibrary)) != NULL) {
			CItem *pItem = *(CItem **)::GlobalLock(hMem);
			::GlobalUnlock(hMem);
			if (pItem == this) {
				::EmptyClipboard();
				::CloseClipboard();
			}
		}
	}
}

void CItem::RemoveAll()
{
	while (!m_oItemList.IsEmpty()) {
		CItem *pItem = (CItem *)m_oItemList.GetHead();
		delete pItem;
	}
}

CItem *CItem::GetParent()
{
	HTREEITEM hItem = g_pTreeCtrl->GetParentItem(m_hItem);
	if (hItem == NULL)
		return NULL;

	return (CItem *)g_pTreeCtrl->GetItemData(hItem);
}

CItem *CItem::GetHeadItem()
{
	m_oPosition = m_oItemList.GetHeadPosition();
	return GetNextItem();
}

CItem *CItem::GetNextItem()
{
	if (m_oPosition == NULL)
		return NULL;

	return (CItem *)m_oItemList.GetNext(m_oPosition);
}

//////////////////////////////////////////////////////////////////////
// CItemRoot クラス
//////////////////////////////////////////////////////////////////////

CItemRoot::CItemRoot()
{
	m_nItemType = ITEM_ROOT;
}

//////////////////////////////////////////////////////////////////////
// CItemInbox クラス
//////////////////////////////////////////////////////////////////////

CItemInbox::CItemInbox(CMMPcsDB *pPcsDB) : CItemFolder(pPcsDB)
{
	m_nItemType = ITEM_INBOX;
}

//////////////////////////////////////////////////////////////////////
// CItemFolder クラス
//////////////////////////////////////////////////////////////////////

CItemFolder::CItemFolder(CMMPcsDB *pPcsDB)
{
	m_nItemType = ITEM_FOLDER;
	m_pPcsDB = pPcsDB;
}

CString CItemFolder::GetFirstData()
{
	if (m_sFirstData.IsEmpty()) {
		if (m_oFolderRec.sFolderType == FTYPE_DFOLDER) {
			DATAREC oDataRec;
			if (m_pPcsDB->ReadChildDataEx(m_oFolderRec.sFolderID, &oDataRec, TRUE))
				m_sFirstData = oDataRec.sTitle;
		} else {
			FOLDERREC oFolderRec;
			if (m_pPcsDB->ReadChildFolderEx(m_oFolderRec.sFolderID, &oFolderRec, TRUE))
				m_sFirstData = oFolderRec.sFolderName;
		}
	}

	return m_sFirstData;
}

//////////////////////////////////////////////////////////////////////
// CItemData クラス
//////////////////////////////////////////////////////////////////////

CItemData::CItemData(CMMPcsDB *pPcsDB)
{
	m_nItemType = ITEM_DATA;
	m_pPcsDB = pPcsDB;
	m_hDIB = NULL;
}

CItemData::~CItemData()
{
	if (m_hDIB != NULL)
		MMPictureFree(m_hDIB);
}

//////////////////////////////////////////////////////////////////////
// CItemFind クラス
//////////////////////////////////////////////////////////////////////

CItemFind::CItemFind()
{
	m_nItemType = ITEM_FIND;
}

//////////////////////////////////////////////////////////////////////
// CItemPending クラス
//////////////////////////////////////////////////////////////////////

CItemPending::CItemPending()
{
	m_nItemType = ITEM_PENDING;
}
