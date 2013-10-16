#pragma once
#include "afxwin.h"


// OUChooseSkinDlg �Ի���

class OUChooseSkinDlg : public CDialog
{
	DECLARE_DYNAMIC(OUChooseSkinDlg)

public:
	OUChooseSkinDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~OUChooseSkinDlg();

// �Ի�������
	enum { IDD = IDD_CHOOSESKIN };

public:
    string                      GetSelectedSkin();

protected:
    string                      m_szCurSelected;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnLbnDblclkSkinlist();
    afx_msg void OnBnClickedClean();
protected:
    CListBox m_ListBox;
};
