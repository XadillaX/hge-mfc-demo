#pragma once


// OUNewMapDlg �Ի���

class OUNewMapDlg : public CDialog
{
	DECLARE_DYNAMIC(OUNewMapDlg)

public:
	OUNewMapDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~OUNewMapDlg();

// �Ի�������
	enum { IDD = IDD_OUNEWMAPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CString m_szMapName;
    DWORD m_dwTileWidth;
    DWORD m_dwTileHeight;
    DWORD m_dwWidth;
    DWORD m_dwHeight;
protected:
    virtual void OnOK();
public:
    virtual BOOL OnInitDialog();
};
