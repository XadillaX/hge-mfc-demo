
// OUMapDemoDlg.h : 头文件
//

#pragma once
#include "global.h"

// COUMapDemoDlg 对话框
class COUMapDemoDlg : public CDialog
{
// 构造
public:
	COUMapDemoDlg(CWnd* pParent = NULL);	// 标准构造函数
    virtual ~COUMapDemoDlg();

// 对话框数据
	enum { IDD = IDD_OUMAPDEMO_DIALOG };

protected:
    bool                        InitHGEDlg();
    void                        ReleaseHGE();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
    HGE*                        m_pHGE;

	// 生成的消息映射函数
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
