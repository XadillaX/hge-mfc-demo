
// OUMapMakerEXView.cpp : OUMapMakerEXView ���ʵ��
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
	// ��׼��ӡ����
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

// OUMapMakerEXView ����/����

OUMapMakerEXView::OUMapMakerEXView() :
    m_pHGE(NULL),
    m_bHGEDlgCreated(false)
{
	// TODO: �ڴ˴���ӹ������
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
    SetScrollSizes(MM_TEXT, CSize(0, 0));

	return CScrollView::PreCreateWindow(cs);
}

// OUMapMakerEXView ����

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
            ::MessageBox(NULL, "�ں������ʼ��ʧ�ܡ�", "����", MB_ICONSTOP | MB_OK);
            SendMessage(WM_CLOSE);
            return;
        }

        //::SetFocus(m_pHGE->System_GetState(HGE_HWND));

        /** ���������߳� */
        //m_hThreadHandle = ::CreateThread(NULL, 0, __HGETHREAD, (LPVOID)m_pHGE, 0, NULL);
        
        /** ��ʱ�� */
        ::g_pView = new OUMapView();
        SetTimer(55555, 10, NULL);

        m_bHGEDlgCreated = true;
    }

    if(m_pHGE->System_GetState(HGE_HWND) && g_pView)
    {
        ::MoveWindow(m_pHGE->System_GetState(HGE_HWND), 0, 0, g_pView->GetScreenWidth(), g_pView->GetScreenHeight(), 1);
    }
}


// OUMapMakerEXView ��ӡ


void OUMapMakerEXView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL OUMapMakerEXView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void OUMapMakerEXView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void OUMapMakerEXView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// OUMapMakerEXView ���

#ifdef _DEBUG
void OUMapMakerEXView::AssertValid() const
{
	CScrollView::AssertValid();
}

void OUMapMakerEXView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

OUMapMakerEXDoc* OUMapMakerEXView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(OUMapMakerEXDoc)));
	return (OUMapMakerEXDoc*)m_pDocument;
}
#endif //_DEBUG


// OUMapMakerEXView ��Ϣ�������

void OUMapMakerEXView::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    SendMessage(WM_QUIT);
    WaitForSingleObject(m_hThreadHandle, INFINITE);
    CloseHandle(m_hThreadHandle);
    m_hThreadHandle = NULL;

    CScrollView::OnClose();
}

void OUMapMakerEXView::OnSize(UINT nType, int cx, int cy)
{
    CScrollView::OnSize(nType, cx, cy);

    /** �ı�HGE�Ӵ��ڴ�С */
    if(m_pHGE->System_GetState(HGE_HWND))
    {
        if(cx == 0 || cy == 0) return;

        /** ����ͼ��ͼ���͸ı��С����Ϣ */
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
//    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
    // TODO: �ڴ����ר�ô����/����û���
    SIZE sz = GetDocument()->GetDocumentSize();
    SetScrollSizes(MM_TEXT, CSize(sz.cx, sz.cy));
}

BOOL OUMapMakerEXView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
    // TODO: �ڴ����ר�ô����/����û���
    if(m_pHGE->System_GetState(HGE_HWND) && g_pView)
    {
        /** �ƶ� */
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
    // TODO: �ڴ����ר�ô����/����û���
    if(m_pHGE->System_GetState(HGE_HWND) && g_pView)
    {
        ::MoveWindow(m_pHGE->System_GetState(HGE_HWND), 0, 0, g_pView->GetScreenWidth(), g_pView->GetScreenHeight(), 1);
    }

    return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}

void OUMapMakerEXView::OnPaint()
{
    //CPaintDC dc(this); // device context for painting
    // TODO: �ڴ˴������Ϣ����������

    //if(m_pHGE->System_GetState(HGE_HWND) && g_pView)
    //{
    //    ::MoveWindow(m_pHGE->System_GetState(HGE_HWND), 0, 0, g_pView->GetScreenWidth(), g_pView->GetScreenHeight(), 1);
    //}

    CScrollView::OnPaint();
}

BOOL OUMapMakerEXView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void OUMapMakerEXView::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
