// MainFrm.cpp : OUMainFrame ���ʵ��
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
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// OUMainFrame ����/����

OUMainFrame::OUMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
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
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

    if(!m_wndAssetsDock.Create("��Դ�����", this, CRect(0, 0, 100, 100), TRUE, IDD_OUASSETSDLG,
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
    {
        TRACE0("δ�ܴ�����Դ�������\n");
        return FALSE;
    }
    m_wndAssetsDock.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);

	// TODO: �������Ҫ��ͣ������������ɾ��������
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX;

    /** �����ڿ���Լ�λ�� */
    cs.cx = 800;
    cs.cy = 600;
    cs.x = (GetSystemMetrics(SM_CXFULLSCREEN) - 800) / 2;
    cs.y = (GetSystemMetrics(SM_CYFULLSCREEN) - 600) / 2;

	return TRUE;
}


// OUMainFrame ���

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


// OUMainFrame ��Ϣ�������



#include "OUChooseSkinDlg.h"
void OUMainFrame::OnClickSetSkin()
{
    // TODO: �ڴ���������������
    OUChooseSkinDlg dlg;
    if(dlg.DoModal() == IDOK)
    {
        g_szCurSkinName = dlg.GetSelectedSkin();
        skinppLoadSkin(FormatSkinFilename(g_szCurSkinName.c_str()));

        g_IniFile.SetString("system", "skin", g_szCurSkinName.c_str());
    }
}
