// OUMapMakerView.cpp : OUMapMakerView 类的实现
//

#include "stdafx.h"
#include "OUMapMaker.h"

#include "OUMapMakerDoc.h"
#include "OUMapMakerView.h"

#include "OUHGEThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// OUMapMakerView

IMPLEMENT_DYNCREATE(OUMapMakerView, CView)

BEGIN_MESSAGE_MAP(OUMapMakerView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
    ON_WM_CLOSE()
    ON_WM_SIZE()
END_MESSAGE_MAP()

// OUMapMakerView 构造/析构

OUMapMakerView::OUMapMakerView() :
    m_pHGE(NULL)
{
	// TODO: 在此处添加构造代码
    m_pHGE = hgeCreate(HGE_VERSION);

    ::InitializeCriticalSection(&g_csRenderLock);
}

OUMapMakerView::~OUMapMakerView()
{
    m_pHGE->Release();
    m_pHGE = NULL;
}

BOOL OUMapMakerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// OUMapMakerView 绘制

void OUMapMakerView::OnDraw(CDC* /*pDC*/)
{
	OUMapMakerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	/** 初始化HGE */
    static bool bFirst = true;
    if(bFirst)
    {
        bFirst = false;

        /** 窗体宽高 */
        RECT rc;
        GetClientRect(&rc);
        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;

        m_pHGE->System_SetState(HGE_SCREENWIDTH, width);
        m_pHGE->System_SetState(HGE_SCREENHEIGHT, height);
        m_pHGE->System_SetState(HGE_FPS, 100);
        m_pHGE->System_SetState(HGE_SCREENBPP, 32);
        m_pHGE->System_SetState(HGE_HIDEMOUSE, false);
        m_pHGE->System_SetState(HGE_SHOWSPLASH, false);
        m_pHGE->System_SetState(HGE_HWNDPARENT, GetSafeHwnd());
        m_pHGE->System_SetState(HGE_USESOUND, false);
        m_pHGE->System_SetState(HGE_WINDOWED, true);

        //m_pHGE->System_SetState(HGE_FRAMEFUNC, __HGEUpdate);
        //m_pHGE->System_SetState(HGE_RENDERFUNC, __HGERender);

        if(!m_pHGE->System_Initiate())
        {
            ::MessageBox(NULL, "内核引擎初始化失败。", "错误", MB_ICONSTOP | MB_OK);
            SendMessage(WM_CLOSE);
            return;
        }

        /** 创建引擎线程 */
        m_hThreadHandle = ::CreateThread(NULL, 0, __HGETHREAD, (LPVOID)m_pHGE, 0, NULL);
    }
}


// OUMapMakerView 打印

BOOL OUMapMakerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void OUMapMakerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void OUMapMakerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// OUMapMakerView 诊断

#ifdef _DEBUG
void OUMapMakerView::AssertValid() const
{
	CView::AssertValid();
}

void OUMapMakerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

OUMapMakerDoc* OUMapMakerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(OUMapMakerDoc)));
	return (OUMapMakerDoc*)m_pDocument;
}
#endif //_DEBUG


// OUMapMakerView 消息处理程序

void OUMapMakerView::OnClose()
{
    SendMessage(WM_QUIT);
    WaitForSingleObject(m_hThreadHandle, INFINITE);
    CloseHandle(m_hThreadHandle);
    m_hThreadHandle = NULL;

    CView::OnClose();
}

void OUMapMakerView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    if(m_pHGE->System_GetState(HGE_HWND))
    {
        if(cx == 0 || cy == 0) return;

        ::EnterCriticalSection(&g_csRenderLock);

        m_pHGE->System_SetState(HGE_SCREENWIDTH, cx);
        m_pHGE->System_SetState(HGE_SCREENHEIGHT, cy);

        ::MoveWindow(m_pHGE->System_GetState(HGE_HWND),0, 0, cx, cy, true);

        ::LeaveCriticalSection(&g_csRenderLock);
    }
}
