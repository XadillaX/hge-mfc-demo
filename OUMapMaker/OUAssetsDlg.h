#pragma once


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
};
