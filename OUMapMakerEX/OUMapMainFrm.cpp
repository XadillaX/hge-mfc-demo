
// OUMapMainFrm.cpp : OUMapMainFrame ���ʵ��
//

#include "stdafx.h"
#include "OUMapMakerEX.h"

#include "OUMapMainFrm.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// OUMapMainFrame

IMPLEMENT_DYNCREATE(OUMapMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(OUMapMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &OUMapMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &OUMapMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &OUMapMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &OUMapMainFrame::OnUpdateApplicationLook)
    ON_COMMAND(ID_EXTSKIN, &OUMapMainFrame::OnExtSkin)
    ON_COMMAND(ID_INNERSTATE, &OUMapMainFrame::OnInnerState)
    ON_UPDATE_COMMAND_UI(ID_INNERSTATE, &OUMapMainFrame::OnUpdateInnerState)
    ON_UPDATE_COMMAND_UI(ID_INNERGRID, &OUMapMainFrame::OnUpdateInnergrid)
    ON_COMMAND(ID_INNERGRID, &OUMapMainFrame::OnInnergrid)
    ON_UPDATE_COMMAND_UI(ID_EDITMODE_HAND, &OUMapMainFrame::OnUpdateEditmodeHand)
    ON_COMMAND(ID_EDITMODE_HAND, &OUMapMainFrame::OnEditmodeHand)
    ON_COMMAND(ID_EDITMODE_ADD, &OUMapMainFrame::OnEditmodeAdd)
    ON_UPDATE_COMMAND_UI(ID_EDITMODE_ADD, &OUMapMainFrame::OnUpdateEditmodeAdd)
    ON_COMMAND(ID_EDITMODE_FILL, &OUMapMainFrame::OnEditmodeFill)
    ON_UPDATE_COMMAND_UI(ID_EDITMODE_FILL, &OUMapMainFrame::OnUpdateEditmodeFill)
    ON_COMMAND(ID_EDITMODE_ERASE, &OUMapMainFrame::OnEditmodeErase)
    ON_UPDATE_COMMAND_UI(ID_EDITMODE_ERASE, &OUMapMainFrame::OnUpdateEditmodeErase)
    ON_WM_SETCURSOR()
    ON_COMMAND(ID_EDITMODE_ARROW, &OUMapMainFrame::OnEditmodeArrow)
    ON_UPDATE_COMMAND_UI(ID_EDITMODE_ARROW, &OUMapMainFrame::OnUpdateEditmodeArrow)
    ON_COMMAND(ID_UPPERGRID, &OUMapMainFrame::OnUpperGrid)
    ON_UPDATE_COMMAND_UI(ID_UPPERGRID, &OUMapMainFrame::OnUpdateUpperGrid)
    ON_COMMAND(ID_EDITMODE_GROUND, &OUMapMainFrame::OnEditmodeGround)
    ON_UPDATE_COMMAND_UI(ID_EDITMODE_GROUND, &OUMapMainFrame::OnUpdateEditmodeGround)
    ON_COMMAND(ID_EDITMODE_BLOCK, &OUMapMainFrame::OnEditmodeBlock)
    ON_UPDATE_COMMAND_UI(ID_EDITMODE_BLOCK, &OUMapMainFrame::OnUpdateEditmodeBlock)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// OUMapMainFrame ����/����

OUMapMainFrame::OUMapMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2005);
}

OUMapMainFrame::~OUMapMainFrame()
{
}

int OUMapMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("δ�ܴ����˵���\n");
		return -1;      // δ�ܴ���
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// ��ֹ�˵����ڼ���ʱ��ý���
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_MAINFRAME_256) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

    /** �༭ģʽ������ */
	if (!m_wndEditModeToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_EDITMODE) ||
        !m_wndEditModeToolBar.LoadToolBar(IDR_EDITMODE))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}
	CString strEditModeToolBarName;
	bNameValid = strEditModeToolBarName.LoadString(IDS_TOOLBAR_EDITMODE);
	ASSERT(bNameValid);
    m_wndEditModeToolBar.SetWindowText(strEditModeToolBarName);
    m_wndEditModeToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// �����û�����Ĺ���������:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

    if (!m_AssetsPane.Create("��Դ��ͼ", this, CRect(0, 0, 400, 100), TRUE, IDD_OUASSETSDLG, 
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
    {
        TRACE0("δ�ܴ�����Դ��ͼ\nn");
        return FALSE;
    }
    m_AssetsPane.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);

	// TODO: �������ϣ���������Ͳ˵�����ͣ������ɾ��������
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    m_wndEditModeToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
    DockPane(&m_AssetsPane, AFX_IDW_DOCKBAR_RIGHT);
    DockPane(&m_wndEditModeToolBar);

	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���ù�������ͣ�����ڲ˵��滻
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ���ÿ���(��ס Alt �϶�)�������Զ���
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// �����û�����Ĺ�����ͼ��
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			m_UserImages.SetImageSize(CSize(16, 16), FALSE);
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// ���ò˵����Ի�(���ʹ�õ�����)
	// TODO: �������Լ��Ļ������ȷ��ÿ�������˵�������һ���������
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL OUMapMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

// OUMapMainFrame ���

#ifdef _DEBUG
void OUMapMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void OUMapMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// OUMapMainFrame ��Ϣ�������

void OUMapMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT OUMapMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void OUMapMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void OUMapMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL OUMapMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// ���ཫִ�������Ĺ���

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// Ϊ�����û������������Զ��尴ť
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void OUMapMainFrame::OnExtSkin()
{
    // TODO: �ڴ���������������
    OUChooseSkinDlg dlg;
    if(dlg.DoModal() == IDOK)
    {
        g_szCurSkinName = dlg.GetSelectedSkin();
        if(g_szCurSkinName != "")
        {
            ::skinppRemoveSkin();
            ::skinppLoadSkin(FormatSkinFilename(g_szCurSkinName.c_str()));
        }
        else ::skinppRemoveSkin();

        g_IniFile.SetString("system", "skin", g_szCurSkinName.c_str());
    }
}

#include "OUHGEThread.h"
bool __InnerStateChecked = true;
void OUMapMainFrame::OnInnerState()
{
    // TODO: �ڴ���������������
    if(g_pView != NULL) g_pView->SetShowState(!__InnerStateChecked);
    __InnerStateChecked = !__InnerStateChecked;
}

void OUMapMainFrame::OnUpdateInnerState(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    pCmdUI->SetCheck(__InnerStateChecked);
}

bool __InnerGridhecked = false;

void OUMapMainFrame::OnUpdateInnergrid(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    pCmdUI->SetCheck(__InnerGridhecked);
}

void OUMapMainFrame::OnInnergrid()
{
    if(g_pView != NULL) g_pView->SetShowGrid(!__InnerGridhecked);
    __InnerGridhecked = !__InnerGridhecked;
    // TODO: �ڴ���������������
}

bool __handedit = false;
bool __addedit = false;
bool __filledit = false;
bool __eraseedit = false;
bool __chooseedit = true;
bool __groundedit = false;
bool __blockedit = false;

void OUMapMainFrame::OnUpdateEditmodeHand(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    pCmdUI->SetRadio(__handedit);
}

void OUMapMainFrame::OnEditmodeHand()
{
    // TODO: �ڴ���������������
    __handedit = true;
    __addedit = false;
    __filledit = false;
    __eraseedit = false;
    __chooseedit = false;
    __groundedit = false;
    __blockedit = false;

    if(g_pView)
    {
        OM_CHANGEEDITMODE_PARAM* param = new OM_CHANGEEDITMODE_PARAM();
        param->m_emEditMode = OUMEM_MOVE;
        g_pView->SendMessage(OM_CHANGEEDITMODE, param);
    }
}

void OUMapMainFrame::OnEditmodeAdd()
{
    // TODO: �ڴ���������������
    __handedit = false;
    __addedit = true;
    __filledit = false;
    __eraseedit = false;
    __chooseedit = false;
    __groundedit = false;
    __blockedit = false;

    if(g_pView)
    {
        OM_CHANGEEDITMODE_PARAM* param = new OM_CHANGEEDITMODE_PARAM();
        param->m_emEditMode = OUMEM_ADD;
        g_pView->SendMessage(OM_CHANGEEDITMODE, param);
    }
}

void OUMapMainFrame::OnUpdateEditmodeAdd(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    pCmdUI->SetRadio(__addedit);
}

void OUMapMainFrame::OnEditmodeFill()
{
    // TODO: �ڴ���������������
    __handedit = false;
    __addedit = false;
    __filledit = true;
    __eraseedit = false;
    __chooseedit = false;
    __groundedit = false;
    __blockedit = false;

    if(g_pView)
    {
        OM_CHANGEEDITMODE_PARAM* param = new OM_CHANGEEDITMODE_PARAM();
        param->m_emEditMode = OUMEM_FILL;
        g_pView->SendMessage(OM_CHANGEEDITMODE, param);
    }
}

void OUMapMainFrame::OnUpdateEditmodeFill(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    pCmdUI->SetRadio(__filledit);
}

void OUMapMainFrame::OnEditmodeErase()
{
    // TODO: �ڴ���������������
    __handedit = false;
    __addedit = false;
    __filledit = false;
    __eraseedit = true;
    __chooseedit = false;
    __groundedit = false;
    __blockedit = false;

    if(g_pView)
    {
        OM_CHANGEEDITMODE_PARAM* param = new OM_CHANGEEDITMODE_PARAM();
        param->m_emEditMode = OUMEM_ERASE;
        g_pView->SendMessage(OM_CHANGEEDITMODE, param);
    }
}

void OUMapMainFrame::OnUpdateEditmodeErase(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    pCmdUI->SetRadio(__eraseedit);
}

BOOL OUMapMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    return CFrameWndEx::OnSetCursor(pWnd, nHitTest, message);
}

void OUMapMainFrame::OnEditmodeArrow()
{
    // TODO: �ڴ���������������
    __handedit = false;
    __addedit = false;
    __filledit = false;
    __eraseedit = false;
    __chooseedit = true;
    __groundedit = false;
    __blockedit = false;

    if(g_pView)
    {
        OM_CHANGEEDITMODE_PARAM* param = new OM_CHANGEEDITMODE_PARAM();
        param->m_emEditMode = OUMEM_CHOOSE;
        g_pView->SendMessage(OM_CHANGEEDITMODE, param);
    }
}

void OUMapMainFrame::OnUpdateEditmodeArrow(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    pCmdUI->SetRadio(__chooseedit);
}

bool __UpperGridhecked = true;

void OUMapMainFrame::OnUpperGrid()
{
    // TODO: �ڴ���������������
    if(g_pView != NULL) g_pView->SetUpperGrid(!__UpperGridhecked);
    __UpperGridhecked = !__UpperGridhecked;
}

void OUMapMainFrame::OnUpdateUpperGrid(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    pCmdUI->SetCheck(__UpperGridhecked);
}

void OUMapMainFrame::OnEditmodeGround()
{
    // TODO: �ڴ���������������
    __handedit = false;
    __addedit = false;
    __filledit = false;
    __eraseedit = false;
    __chooseedit = false;
    __groundedit = true;
    __blockedit = false;

    if(g_pView)
    {
        OM_CHANGEEDITMODE_PARAM* param = new OM_CHANGEEDITMODE_PARAM();
        param->m_emEditMode = OUMEM_GROUND;
        g_pView->SendMessage(OM_CHANGEEDITMODE, param);
    }
}

void OUMapMainFrame::OnUpdateEditmodeGround(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    pCmdUI->SetRadio(__groundedit);
}

void OUMapMainFrame::OnEditmodeBlock()
{
    // TODO: �ڴ���������������
    __handedit = false;
    __addedit = false;
    __filledit = false;
    __eraseedit = false;
    __chooseedit = false;
    __groundedit = false;
    __blockedit = true;

    if(g_pView)
    {
        OM_CHANGEEDITMODE_PARAM* param = new OM_CHANGEEDITMODE_PARAM();
        param->m_emEditMode = OUMEM_BLOCK;
        g_pView->SendMessage(OM_CHANGEEDITMODE, param);
    }
}

void OUMapMainFrame::OnUpdateEditmodeBlock(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    pCmdUI->SetRadio(__blockedit);
}
