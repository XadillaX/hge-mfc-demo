#pragma once
#include "afxcmn.h"
#include "OUAssetsTabDlg.h"


// OUAssetsDlg 对话框

class OUAssetsDlg : public CDialog
{
	DECLARE_DYNAMIC(OUAssetsDlg)

public:
	OUAssetsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~OUAssetsDlg();

// 对话框数据
	enum { IDD = IDD_OUASSETSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
    CTabCtrl                m_AssetsTab;
    OUAssetsTabDlg          m_AssetsTabDlg[OUAT_MAX];

public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();
    afx_msg void OnTcnSelchangeTabassetstab(NMHDR *pNMHDR, LRESULT *pResult);
};
