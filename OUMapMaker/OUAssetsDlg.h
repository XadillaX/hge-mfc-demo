#pragma once


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
};
