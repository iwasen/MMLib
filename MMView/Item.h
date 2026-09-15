// Item.h: CItem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CItem
{
public:
	CItem();
	virtual ~CItem();

	BOOL m_bRead;
	CString	m_sCreateDate;		// 撮影年月日時分
	CString	m_sUpdateDate;		// 更新年月日時分
	CString	m_sCreator;			// 作成者
	CString	m_sCode;			// コード
	CString	m_sTitle;			// 画像タイトル
	CString	m_sRemark1;			// 備考１
	CString	m_sRemark2;			// 備考２
	HDIB m_hDIB1;
	HDIB m_hDIB2;
	CString m_sText;			// テキストデータ
	CRect m_rcIcon;

	void ReadItem(CMmlRead *pMmlRead, int nPage);
};
