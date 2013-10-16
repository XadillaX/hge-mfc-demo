// OUWorldDlg.h : ͷ�ļ�
//
#ifndef __OUWORLDDLG_H__
#define __OUWORLDDLG_H__
#pragma once

// COUWorldDlg �Ի���
class COUWorldDlg : public CDialog
{
// ����
public:
	COUWorldDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_OUWORLD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
    void                        InitializeHGE();

private:
    HGE*                        m_pHGE;
    HANDLE                      m_hHGEThread;
    DWORD                       m_dwHGEThreadID;

public:
    afx_msg void OnClose();
//    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
#endif
