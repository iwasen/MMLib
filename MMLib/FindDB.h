// FindDB.h: CFindDB クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CFindDB
{
public:
	CFindDB();
	virtual ~CFindDB();

	BOOL Open(BOOL bCreate);
	BOOL ReadFind(LPCTSTR pLibraryID, FINDREC *pFindRec);
	BOOL AddFind(FINDREC *pFindRec);
	BOOL UpdateFind(FINDREC *pFindRec);
	BOOL DeleteFind(LPCTSTR pDID);

private:
	CPcserve m_oPcserve;
	CPSDB m_oFindDB;

	int MakeFindID(CString &sID);
};
