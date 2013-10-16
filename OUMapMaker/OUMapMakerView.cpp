// OUMapMakerView.cpp : OUMapMakerView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
    ON_WM_CLOSE()
    ON_WM_SIZE()
END_MESSAGE_MAP()

// OUMapMakerView ����/����

OUMapMakerView::OUMapMakerView() :
    m_pHGE(NULL)
{
	// TODO: �ڴ˴���ӹ������
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// OUMapMakerView ����

void OUMapMakerView::OnDraw(CDC* /*pDC*/)
{
	OUMapMakerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	/** ��ʼ��HGE */
    static bool bFirst = true;
    if(bFirst)
    {
        bFirst = false;

        /** ������ */
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
            ::MessageBox(NULL, "�ں������ʼ��ʧ�ܡ�", "����", MB_ICONSTOP | MB_OK);
            SendMessage(WM_CLOSE);
            return;
        }

        /** ���������߳� */
        m_hThreadHandle = ::CreateThread(NULL, 0, __HGETHREAD, (LPVOID)m_pHGE, 0, NULL);
    }
}


// OUMapMakerView ��ӡ

BOOL OUMapMakerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void OUMapMakerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void OUMapMakerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// OUMapMakerView ���

#ifdef _DEBUG
void OUMapMakerView::AssertValid() const
{
	CView::AssertValid();
}

void OUMapMakerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

OUMapMakerDoc* OUMapMakerView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(OUMapMakerDoc)));
	return (OUMapMakerDoc*)m_pDocument;
}
#endif //_DEBUG


// OUMapMakerView ��Ϣ�������

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
