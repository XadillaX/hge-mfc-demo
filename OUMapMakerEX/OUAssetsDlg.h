#pragma once
#include "afxcmn.h"
#include "OUAssetsTabDlg.h"


// OUAssetsDlg �Ի���

class OUAssetsDlg : public CDialog
{
	DECLARE_DYNAMIC(OUAssetsDlg)

public:
	OUAssetsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~OUAssetsDlg();

// �Ի�������
	enum { IDD = IDD_OUASSETSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
    CTabCtrl                m_AssetsTab;
    OUAssetsTabDlg          m_AssetsTabDlg[OUAT_MAX];

public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();
    afx_msg void OnTcnSelchangeTabassetstab(NMHDR *pNMHDR, LRESULT *pResult);
};
