#pragma once

#include "OUAssetsDlg.h"
// OUAssetsPane

class OUAssetsPane : public CDockablePane
{
	DECLARE_DYNAMIC(OUAssetsPane)

public:
	OUAssetsPane();
	virtual ~OUAssetsPane();

protected:
    OUAssetsDlg             m_AssetsDlg;

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
};


