#ifndef __OUNEWMAPDLG_H__
#define __OUNEWMAPDLG_H__
#pragma once


// OUNewMapDlg �Ի���

class OUNewMapDlg : public CDialog
{
	DECLARE_DYNAMIC(OUNewMapDlg)

public:
	OUNewMapDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~OUNewMapDlg();

// �Ի�������
	enum { IDD = IDD_NEWSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CString m_szMapName;
    DWORD m_dwWidth;
    DWORD m_dwHeight;
    virtual BOOL OnInitDialog();
};

#endif
