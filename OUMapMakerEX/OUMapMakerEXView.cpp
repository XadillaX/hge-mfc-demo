
// OUMapMakerEXView.cpp : OUMapMakerEXView 类的实现
//

#include "stdafx.h"
#include "OUMapMakerEX.h"

#include "OUMapMakerEXDoc.h"
#include "OUMapMakerEXView.h"
#include "OUHGEThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// OUMapMakerEXView

IMPLEMENT_DYNCREATE(OUMapMakerEXView, CScrollView)

BEGIN_MESSAGE_MAP(OUMapMakerEXView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &OUMapMakerEXView::OnFilePrintPreview)
    ON_WM_CLOSE()
    ON_WM_SIZE()
//    ON_WM_PAINT()
//ON_WM_ERASEBKGND()
ON_WM_PAINT()
ON_WM_SETCURSOR()
ON_WM_TIMER()
END_MESSAGE_MAP()

// OUMapMakerEXView 构造/析构

OUMapMakerEXView::OUMapMakerEXView() :
    m_pHGE(NULL),
    m_bHGEDlgCreated(false)
{
	// TODO: 在此处添加构造代码
    m_pHGE = hgeCreate(HGE_VERSION);
    ::InitializeCriticalSection(&g_csRenderLock);

    m_Size.used = true;
}

OUMapMakerEXView::~OUMapMakerEXView()
{
    delete g_pView;

    m_pHGE->Release();
    m_pHGE = NULL;
}

BOOL OUMapMakerEXView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
    SetScrollSizes(MM_TEXT, CSize(0, 0));

	return CScrollView::PreCreateWindow(cs);
}

// OUMapMakerEXView 绘制

void OUMapMakerEXView::OnDraw(CDC* /*pDC*/)
{
	OUMapMakerEXDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

    if(!m_bHGEDlgCreated)
    {
        //g_pDlgView = (char*)this;
        g_pViewDlg = this;

        RECT rc;
        GetClientRect(&rc);

        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;

        m_pHGE->System_SetState(HGE_SCREENWIDTH, width);
        m_pHGE->System_SetState(HGE_SCREENHEIGHT, height);
        m_pHGE->System_SetState(HGE_FPS, 100);
        m_pHGE->System_SetState(HGE_SCREENBPP, 32);
        //m_pHGE->System_SetState(HGE_HIDEMOUSE, false);
        m_pHGE->System_SetState(HGE_SHOWSPLASH, false);
        m_pHGE->System_SetState(HGE_HWNDPARENT, GetSafeHwnd());
        m_pHGE->System_SetState(HGE_USESOUND, false);
        m_pHGE->System_SetState(HGE_WINDOWED, true);
        m_pHGE->System_SetState(HGE_ZBUFFER, true);
        m_pHGE->System_SetState(HGE_DONTSUSPEND, true);

        m_pHGE->System_SetState(HGE_FRAMEFUNC, __HGEUpdate);
        //m_pHGE->System_SetState(HGE_RENDERFUNC, __HGERender);

        if(!m_pHGE->System_Initiate())
        {
            ::MessageBox(NULL, "内核引擎初始化失败。", "错误", MB_ICONSTOP | MB_OK);
            SendMessage(WM_CLOSE);
            return;
        }

        //::SetFocus(m_pHGE->System_GetState(HGE_HWND));

        /** 创建引擎线程 */
        //m_hThreadHandle = ::CreateThread(NULL, 0, __HGETHREAD, (LPVOID)m_pHGE, 0, NULL);
        
        /** 计时器 */
        ::g_pView = new OUMapView();
        SetTimer(55555, 10, NULL);

        m_bHGEDlgCreated = true;
    }

    if(m_pHGE->System_GetState(HGE_HWND) && g_pView)
    {
        ::MoveWindow(m_pHGE->System_GetState(HGE_HWND), 0, 0, g_pView->GetScreenWidth(), g_pView->GetScreenHeight(), 1);
    }
}


// OUMapMakerEXView 打印


void OUMapMakerEXView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL OUMapMakerEXView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void OUMapMakerEXView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void OUMapMakerEXView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void OUMapMakerEXView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void OUMapMakerEXView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// OUMapMakerEXView 诊断

#ifdef _DEBUG
void OUMapMakerEXView::AssertValid() const
{
	CScrollView::AssertValid();
}

void OUMapMakerEXView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

OUMapMakerEXDoc* OUMapMakerEXView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(OUMapMakerEXDoc)));
	return (OUMapMakerEXDoc*)m_pDocument;
}
#endif //_DEBUG


// OUMapMakerEXView 消息处理程序

void OUMapMakerEXView::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    SendMessage(WM_QUIT);
    WaitForSingleObject(m_hThreadHandle, INFINITE);
    CloseHandle(m_hThreadHandle);
    m_hThreadHandle = NULL;

    CScrollView::OnClose();
}

void OUMapMakerEXView::OnSize(UINT nType, int cx, int cy)
{
    CScrollView::OnSize(nType, cx, cy);

    /** 改变HGE子窗口大小 */
    if(m_pHGE->System_GetState(HGE_HWND))
    {
        if(cx == 0 || cy == 0) return;

        /** 给地图视图发送改变大小的消息 */
        if(g_pView)
        {
            OM_SIZE_PARAM* param = new OM_SIZE_PARAM();
            param->m_nScreenWidth = cx;
            param->m_nScreenHeight = cy;

            g_pView->SendMessage(OM_SIZE, param);
        }
    }
}



//BOOL OUMapMakerEXView::OnEraseBkgnd(CDC* pDC)
//{
//    // TODO: 在此添加消息处理程序代码和/或调用默认值
//
//    return CScrollView::OnEraseBkgnd(pDC);
//}

void OUMapMakerEXView::OnInitialUpdate()
{
    CScrollView::OnInitialUpdate();
}

void OUMapMakerEXView::SetScrollSize(int nWidth, int nHeight)
{
    //SetScrollSizes(MM_TEXT, CSize(nWidth, nHeight));
    m_Size.size.cx = nWidth;
    m_Size.size.cy = nHeight;
    m_Size.used = false;
}

void OUMapMakerEXView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
    // TODO: 在此添加专用代码和/或调用基类
    SIZE sz = GetDocument()->GetDocumentSize();
    SetScrollSizes(MM_TEXT, CSize(sz.cx, sz.cy));
}

BOOL OUMapMakerEXView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
    // TODO: 在此添加专用代码和/或调用基类
    if(m_pHGE->System_GetState(HGE_HWND) && g_pView)
    {
        /** 移动 */
        OUPOINT pt = g_pView->GetTruePosOfScreen();
        pt.m_fX += sizeScroll.cx;
        pt.m_fY += sizeScroll.cy;
        g_pView->MoveArea(pt.m_fX, pt.m_fY, true);
        //OM_SCROLLMOVEAREA_PARAM* param = new OM_SCROLLMOVEAREA_PARAM();
        //param->m_nToX = pt.m_fX;
        //param->m_nToY = pt.m_fY;
        //g_pView->SendMessage(OM_SCROLLMOVEAREA, param);

        ::MoveWindow(m_pHGE->System_GetState(HGE_HWND), 0, 0, g_pView->GetScreenWidth(), g_pView->GetScreenHeight(), 1);
    }

    return CScrollView::OnScrollBy(sizeScroll, bDoScroll);
}

BOOL OUMapMakerEXView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
    // TODO: 在此添加专用代码和/或调用基类
    if(m_pHGE->System_GetState(HGE_HWND) && g_pView)
    {
        ::MoveWindow(m_pHGE->System_GetState(HGE_HWND), 0, 0, g_pView->GetScreenWidth(), g_pView->GetScreenHeight(), 1);
    }

    return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}

void OUMapMakerEXView::OnPaint()
{
    //CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码

    //if(m_pHGE->System_GetState(HGE_HWND) && g_pView)
    //{
    //    ::MoveWindow(m_pHGE->System_GetState(HGE_HWND), 0, 0, g_pView->GetScreenWidth(), g_pView->GetScreenHeight(), 1);
    //}

    CScrollView::OnPaint();
}

BOOL OUMapMakerEXView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void OUMapMakerEXView::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if(nIDEvent == 55555)
    {
        //if(g_bFirstWillOpen)
        //{
        //    GetDocument()->OnOpenDocument(g_szFirstWillOpenName.c_str());
        //    g_bFirstWillOpen = false;
        //}

        __HGETimerFunc(m_pHGE);
    }

    CScrollView::OnTimer(nIDEvent);
}
