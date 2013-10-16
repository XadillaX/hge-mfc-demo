#ifndef __OUNEWMAPDLG_H__
#define __OUNEWMAPDLG_H__
#pragma once


// OUNewMapDlg 对话框

class OUNewMapDlg : public CDialog
{
	DECLARE_DYNAMIC(OUNewMapDlg)

public:
	OUNewMapDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~OUNewMapDlg();

// 对话框数据
	enum { IDD = IDD_NEWSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CString m_szMapName;
    DWORD m_dwWidth;
    DWORD m_dwHeight;
    virtual BOOL OnInitDialog();
};

#endif
