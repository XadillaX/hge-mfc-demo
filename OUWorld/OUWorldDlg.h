// OUWorldDlg.h : 头文件
//
#ifndef __OUWORLDDLG_H__
#define __OUWORLDDLG_H__
#pragma once

// COUWorldDlg 对话框
class COUWorldDlg : public CDialog
{
// 构造
public:
	COUWorldDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_OUWORLD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
