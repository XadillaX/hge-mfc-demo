// MainFrm.cpp : OUMainFrame 类的实现
//

#include "stdafx.h"
#include "OUMapMaker.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// OUMainFrame

IMPLEMENT_DYNCREATE(OUMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(OUMainFrame, CFrameWnd)
	ON_WM_CREATE()
    ON_COMMAND(ID_32771, &OUMainFrame::OnClickSetSkin)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// OUMainFrame 构造/析构

OUMainFrame::OUMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

OUMainFrame::~OUMainFrame()
{
}


int OUMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

    if(!m_wndAssetsDock.Create("资源侧边栏", this, CRect(0, 0, 100, 100), TRUE, IDD_OUASSETSDLG,
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
    {
        TRACE0("未能创建资源侧边栏。\n");
        return FALSE;
    }
    m_wndAssetsDock.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
    DockPane(&m_wndAssetsDock);

	return 0;
}

BOOL OUMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX;

    /** 主窗口宽高以及位置 */
    cs.cx = 800;
    cs.cy = 600;
    cs.x = (GetSystemMetrics(SM_CXFULLSCREEN) - 800) / 2;
    cs.y = (GetSystemMetrics(SM_CYFULLSCREEN) - 600) / 2;

	return TRUE;
}


// OUMainFrame 诊断

#ifdef _DEBUG
void OUMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void OUMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// OUMainFrame 消息处理程序



#include "OUChooseSkinDlg.h"
void OUMainFrame::OnClickSetSkin()
{
    // TODO: 在此添加命令处理程序代码
    OUChooseSkinDlg dlg;
    if(dlg.DoModal() == IDOK)
    {
        g_szCurSkinName = dlg.GetSelectedSkin();
        skinppLoadSkin(FormatSkinFilename(g_szCurSkinName.c_str()));

        g_IniFile.SetString("system", "skin", g_szCurSkinName.c_str());
    }
}
