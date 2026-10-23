// MyOleData.cpp: CMyOleDataSource クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MMLib.h"
#include "MyOleData.h"

BOOL CMyOleDataSource::OnRenderGlobalData(LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal)
{
	DROPFILES *pDropFiles;
	int nLength;
	LPSTR pFileName;

	switch (lpFormatEtc->cfFormat) {
	case CF_HDROP:
		if (*phGlobal == NULL) {
			if (m_sTempFileName.IsEmpty()) {
				if (!::MakeTempFile(m_pItem, m_sTempFileName, PICTFILE_ALL, FALSE))
					return FALSE;
			}

			if (m_sTempFileName.IsEmpty())
				return FALSE;

			nLength = m_sTempFileName.GetLength();
			if ((*phGlobal = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,
						sizeof(DROPFILES) + nLength + 1)) != NULL) {
				pDropFiles = (DROPFILES *)::GlobalLock(*phGlobal);
				pDropFiles->pFiles = sizeof(DROPFILES);
				pDropFiles->fWide = FALSE;
				pFileName = (char *)pDropFiles + sizeof(DROPFILES);
				strcpy(pFileName, m_sTempFileName);
				for (int i = 0; i < nLength; i++) {
					if (pFileName[i] == ';')
						pFileName[i] = '\0';
				}
				pFileName[nLength] = '\0';

				::GlobalUnlock(*phGlobal);
			}
		}
		return TRUE;
	}

	return FALSE;
}
