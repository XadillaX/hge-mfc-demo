#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "string"
using namespace std;


// OUChooseSkinDlg �Ի���

class OUChooseSkinDlg : public CDialog
{
	DECLARE_DYNAMIC(OUChooseSkinDlg)

public:
	OUChooseSkinDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~OUChooseSkinDlg();

// �Ի�������
	enum { IDD = IDD_CHOOSE_SKIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
