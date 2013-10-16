
// OUMapMainFrm.h : OUMapMainFrame 类的接口
//

#pragma once

#include "OUChooseSkinDlg.h"
#include "OUAssetsPane.h"

class OUMapMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	OUMapMainFrame();
	DECLARE_DYNCREATE(OUMapMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 实现
public:
	virtual ~OUMapMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
    CMFCToolBar       m_wndEditModeToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
    OUAssetsPane      m_AssetsPane;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnExtSkin();
    afx_msg void OnInnerState();
    afx_msg void OnUpdateInnerState(CCmdUI *pCmdUI);
    afx_msg void OnUpdateInnergrid(CCmdUI *pCmdUI);
    afx_msg void OnInnergrid();
    afx_msg void OnUpdateEditmodeHand(CCmdUI *pCmdUI);
    afx_msg void OnEditmodeHand();
    afx_msg void OnEditmodeAdd();
    afx_msg void OnUpdateEditmodeAdd(CCmdUI *pCmdUI);
    afx_msg void OnEditmodeFill();
    afx_msg void OnUpdateEditmodeFill(CCmdUI *pCmdUI);
    afx_msg void OnEditmodeErase();
    afx_msg void OnUpdateEditmodeErase(CCmdUI *pCmdUI);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnEditmodeArrow();
    afx_msg void OnUpdateEditmodeArrow(CCmdUI *pCmdUI);
    afx_msg void OnUpperGrid();
    afx_msg void OnUpdateUpperGrid(CCmdUI *pCmdUI);
    afx_msg void OnEditmodeGround();
    afx_msg void OnUpdateEditmodeGround(CCmdUI *pCmdUI);
    afx_msg void OnEditmodeBlock();
    afx_msg void OnUpdateEditmodeBlock(CCmdUI *pCmdUI);
};
