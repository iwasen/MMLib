#pragma once

// CFindKeywordReBar

class CFindKeywordReBar : public CReBar
{
protected:
	DECLARE_DYNAMIC(CFindKeywordReBar)

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

