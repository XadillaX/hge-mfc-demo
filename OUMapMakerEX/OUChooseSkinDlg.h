#pragma once
#include "afxwin.h"


// OUChooseSkinDlg 对话框

class OUChooseSkinDlg : public CDialog
{
	DECLARE_DYNAMIC(OUChooseSkinDlg)

public:
	OUChooseSkinDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~OUChooseSkinDlg();

// 对话框数据
	enum { IDD = IDD_CHOOSESKIN };

public:
    string                      GetSelectedSkin();

protected:
    string                      m_szCurSelected;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnLbnDblclkSkinlist();
    afx_msg void OnBnClickedClean();
protected:
    CListBox m_ListBox;
};
