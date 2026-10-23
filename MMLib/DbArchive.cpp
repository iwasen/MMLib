#include "stdafx.h"
#include "MMLib.h"
#include "DbArchive.h"

#define ARCHIVE_VERSION	1

BOOL CDbArchive::Export(const CString &sFilePath, CMMPcsDB *pPcsDB, const FOLDERREC &oFolderRec)
{
	if (!m_oFile.Open(sFilePath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
		return FALSE;

	m_pPcsDB = pPcsDB;

	WriteLong(ARCHIVE_VERSION);

	ExportFolder(oFolderRec);

	return TRUE;
}

void CDbArchive::ExportFolder(const FOLDERREC &oFolderRec)
{
	WriteFolder(oFolderRec);

	if (oFolderRec.sFolderType[0] == FTYPE_DFOLDER) {
		ExportData(oFolderRec);
	} else {
		CStringArray aFolderID;
		FOLDERREC oFolderRecChild;
		BOOL bFirst = TRUE;
		while (m_pPcsDB->ReadChildFolderEx(oFolderRec.sFolderID, &oFolderRecChild, bFirst)) {
			bFirst = FALSE;

			aFolderID.Add(oFolderRecChild.sFolderID);
		}

		for (int i = 0; i < aFolderID.GetCount(); i++) {
			m_pPcsDB->ReadFolderEx(aFolderID[i], &oFolderRecChild);

			ExportFolder(oFolderRecChild);
		}
	}
}

void CDbArchive::ExportData(const FOLDERREC &oFolderRec)
{
	CStringArray aDataID;
	DATAREC oDataRec;
	BOOL bFirst = TRUE;

	while (m_pPcsDB->ReadChildDataEx(oFolderRec.sFolderID, &oDataRec, bFirst)) {
		bFirst = FALSE;

		aDataID.Add(oDataRec.sDataID);
	}

	for (int i = 0; i < aDataID.GetCount(); i++) {
		m_pPcsDB->ReadDataEx(aDataID[i], &oDataRec);

		WriteData(oDataRec);
	}
}

void CDbArchive::WriteFolder(const FOLDERREC &oFolderRec)
{
	WriteString("FOLDER");

	WriteString(oFolderRec.sFolderID);
	WriteString(oFolderRec.sParentFolderID);
	WriteString(oFolderRec.sFolderType);
	WriteString(oFolderRec.sFolderName);
	WriteString(oFolderRec.sCreateDate);
	WriteString(oFolderRec.sUpdateDate);
	WriteString(oFolderRec.sCreator);
	WriteString(oFolderRec.sRemark);
	WriteString(oFolderRec.sKeyword);
	WriteLong(oFolderRec.nUpdateCount);
	WriteString(oFolderRec.sCodeDef);
	WriteLong(oFolderRec.nCodeStep);
	WriteBinary(m_pPcsDB->m_oFolderDB, FORM_FIELD);
}

void CDbArchive::WriteData(const DATAREC &oDataRec)
{
	WriteString("DATA");

	WriteString(oDataRec.sFolderID);
	WriteString(oDataRec.sDataID);
	WriteString(oDataRec.sCreateDate);
	WriteString(oDataRec.sUpdateDate);
	WriteString(oDataRec.sCreator);
	WriteString(oDataRec.sCode);
	WriteString(oDataRec.sTitle);
	WriteString(oDataRec.sRemark);
	WriteString(oDataRec.sKeyword);
	WriteLong(oDataRec.nDpi);
	WriteLong(oDataRec.nWidth);
	WriteLong(oDataRec.nHeight);
	WriteLong(oDataRec.nBit);
	WriteLong(oDataRec.nUpdateCount);
	WriteBinary(m_pPcsDB->m_oDataDB, PICT1_FIELD);
	WriteBinary(m_pPcsDB->m_oDataDB, PICT2_FIELD);
	WriteBinary(m_pPcsDB->m_oDataDB, TEXT_FIELD);
	WriteBinary(m_pPcsDB->m_oDataDB, SOUND_FIELD);
}

void CDbArchive::WriteString(LPCSTR pData)
{
	UINT32 len = (UINT)strlen(pData);

	m_oFile.Write(&len, sizeof(UINT32));
	m_oFile.Write(pData, len);
}

void CDbArchive::WriteLong(UINT32 nData)
{
	m_oFile.Write(&nData, sizeof(UINT32));
}

void CDbArchive::WriteBinary(CPSDB &oPsDb, LPCSTR pFieldName)
{
	long nSize;

	if (oPsDb.DBGetBinarySize(pFieldName, &nSize) == 0 && nSize != 0) {
		char *pBuf = new char[nSize];
		oPsDb.DBReadBinary(pFieldName, pBuf, nSize, &nSize);
		m_oFile.Write(&nSize, sizeof(UINT32));
		m_oFile.Write(pBuf, nSize);
		delete[] pBuf;
	} else {
		nSize = 0;
		m_oFile.Write(&nSize, sizeof(UINT32));
	}
}

BOOL CDbArchive::Import(const CString &sFilePath, CMMPcsDB *pPcsDB, const FOLDERREC &oFolderRec)
{
	if (!m_oFile.Open(sFilePath, CFile::modeRead | CFile::shareDenyNone))
		return FALSE;

	m_pPcsDB = pPcsDB;

	ReadLong(m_nVersion);

	const CString *pFolderID = &oFolderRec.sFolderID;
	CString sType;
	while (true) {
		if (!ReadString(sType))
			break;

		if (sType == "FOLDER")
			ReadFolder(pFolderID);
		else if (sType == "DATA")
			ReadData();

		pFolderID = NULL;
	}

	return TRUE;
}

void CDbArchive::ReadFolder(const CString *pFolderID)
{
	FOLDERREC oFolderRec;

	ReadString(oFolderRec.sFolderID);
	ReadString(oFolderRec.sParentFolderID);
	ReadString(oFolderRec.sFolderType);
	ReadString(oFolderRec.sFolderName);
	ReadString(oFolderRec.sCreateDate);
	ReadString(oFolderRec.sUpdateDate);
	ReadString(oFolderRec.sCreator);
	ReadString(oFolderRec.sRemark);
	ReadString(oFolderRec.sKeyword);
	ReadLong(oFolderRec.nUpdateCount);
	ReadString(oFolderRec.sCodeDef);
	ReadLong(oFolderRec.nCodeStep);

	if (oFolderRec.sFolderType == FTYPE_LIBRARY)
		oFolderRec.sFolderType = FTYPE_GFOLDER;

	CString sOldFolderID = oFolderRec.sFolderID;

	if (pFolderID != NULL)
		oFolderRec.sParentFolderID = *pFolderID;
	else
		m_oFolderIDMap.Lookup(oFolderRec.sParentFolderID, oFolderRec.sParentFolderID);

	m_pPcsDB->AddFolderEx(&oFolderRec);

	ReadBinary(m_pPcsDB->m_oFolderDB, FORM_FIELD);

	m_oFolderIDMap.SetAt(sOldFolderID, oFolderRec.sFolderID);
}

void CDbArchive::ReadData()
{
	DATAREC oDataRec;

	ReadString(oDataRec.sFolderID);
	ReadString(oDataRec.sDataID);
	ReadString(oDataRec.sCreateDate);
	ReadString(oDataRec.sUpdateDate);
	ReadString(oDataRec.sCreator);
	ReadString(oDataRec.sCode);
	ReadString(oDataRec.sTitle);
	ReadString(oDataRec.sRemark);
	ReadString(oDataRec.sKeyword);
	ReadLong(oDataRec.nDpi);
	ReadLong(oDataRec.nWidth);
	ReadLong(oDataRec.nHeight);
	ReadLong(oDataRec.nBit);
	ReadLong(oDataRec.nUpdateCount);

	m_oFolderIDMap.Lookup(oDataRec.sFolderID, oDataRec.sFolderID);

	m_pPcsDB->AddDataEx(&oDataRec);

	ReadBinary(m_pPcsDB->m_oDataDB, PICT1_FIELD);
	ReadBinary(m_pPcsDB->m_oDataDB, PICT2_FIELD);
	ReadBinary(m_pPcsDB->m_oDataDB, TEXT_FIELD);
	ReadBinary(m_pPcsDB->m_oDataDB, SOUND_FIELD);
}

BOOL CDbArchive::ReadString(CString &sData)
{
	UINT32 len = 0;

	if (m_oFile.Read(&len, sizeof(UINT32)) != sizeof(UINT32))
		return FALSE;

	if (m_oFile.Read(sData.GetBuffer(len), len) != len)
		return FALSE;

	sData.ReleaseBuffer(len);

	return TRUE;
}

BOOL CDbArchive::ReadLong(long &nData)
{
	return m_oFile.Read(&nData, sizeof(UINT32)) == sizeof(UINT32);
}

BOOL CDbArchive::ReadBinary(CPSDB &oPsDb, LPCSTR pFieldName)
{
	UINT32 len = 0;

	m_oFile.Read(&len, sizeof(UINT32));

	if (len != 0) {
		char *pBuf = new char[len];
		if (m_oFile.Read(pBuf, len) != len) {
			delete[] pBuf;
			return FALSE;
		}

		oPsDb.DBWriteBinary(pFieldName, pBuf, len);

		delete[] pBuf;
	}

	return TRUE;
}
