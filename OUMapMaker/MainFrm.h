// MainFrm.h : OUMainFrame 类的接口
//
#include "OUAssetsDock.h"
#pragma once

class OUMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	OUMainFrame();
	DECLARE_DYNCREATE(OUMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~OUMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
    OUAssetsDock m_wndAssetsDock;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnClickSetSkin();
};
