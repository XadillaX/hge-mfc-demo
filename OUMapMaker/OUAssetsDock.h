#pragma once

#include "OUAssetsDlg.h"
// OUAssetsDock

class OUAssetsDock : public CDockablePane
{
	DECLARE_DYNAMIC(OUAssetsDock)

public:
	OUAssetsDock();
	virtual ~OUAssetsDock();

protected:
	DECLARE_MESSAGE_MAP()

protected:
    OUAssetsDlg                 m_AssetsDlg;
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
};


