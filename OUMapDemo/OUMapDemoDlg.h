
// OUMapDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "global.h"

// COUMapDemoDlg �Ի���
class COUMapDemoDlg : public CDialog
{
// ����
public:
	COUMapDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��
    virtual ~COUMapDemoDlg();

// �Ի�������
	enum { IDD = IDD_OUMAPDEMO_DIALOG };

protected:
    bool                        InitHGEDlg();
    void                        ReleaseHGE();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
    HGE*                        m_pHGE;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnClose();
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
