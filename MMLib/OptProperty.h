#pragma once

// OptProperty.h : ヘッダー ファイル
//

#include "OptFolder.h"
#include "OptFont.h"
#include "OptOrder.h"
#include "OptTwain.h"
#include "OptImageEditor.h"

/////////////////////////////////////////////////////////////////////////////
// COptProperty

class COptProperty : public CPropertySheet
{
public:
	COptProperty(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	COptProperty(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	COptFolder m_oOptFolder;
	COptFont m_oOptFont;
	COptOrder m_oOptOrder;
	COptTwain m_oOptTwain;
	COptImageEditor m_oOptImageEditor;

protected:
	void AddPropertyPage();

	DECLARE_MESSAGE_MAP()
};
