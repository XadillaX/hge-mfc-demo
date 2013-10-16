#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "string"
using namespace std;


// OUChooseSkinDlg 对话框

class OUChooseSkinDlg : public CDialog
{
	DECLARE_DYNAMIC(OUChooseSkinDlg)

public:
	OUChooseSkinDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~OUChooseSkinDlg();

// 对话框数据
	enum { IDD = IDD_CHOOSE_SKIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    virtual BOOL OnInitDialog();
    CListBox m_ListBox;

protected:
    string                      m_szCurSelected;

public:
    string                      GetSelectedSkin();
    afx_msg void OnLbnSelchangeList1();
    afx_msg void OnLbnDblclkList1();
protected:
    virtual void OnOK();
};
