// OUWorldDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OUWorld.h"
#include "OUWorldDlg.h"
#include "CHGEThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// COUWorldDlg 对话框




COUWorldDlg::COUWorldDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COUWorldDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COUWorldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COUWorldDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_CLOSE()
//    ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// COUWorldDlg 消息处理程序

BOOL COUWorldDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    MoveWindow(0, 0, 800, 600);

    /** 初始化HGE */
    InitializeHGE();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

#include "../OUPlazaGUISystem/OUFont.h"
void COUWorldDlg::InitializeHGE()
{
    m_pHGE = hgeCreate(HGE_VERSION);
    if(m_pHGE == NULL)
    {
        ::AfxMessageBox(L"无法创建房间渲染内核对象。");
        exit(0);
    }

    m_pHGE->System_SetState(HGE_HWNDPARENT, this->m_hWnd);
    m_pHGE->System_SetState(HGE_FRAMEFUNC, __HGEUpdate);
    m_pHGE->System_SetState(HGE_RENDERFUNC, __HGERender);
    m_pHGE->System_SetState(HGE_FPS, 100);
    m_pHGE->System_SetState(HGE_ZBUFFER, true);
    m_pHGE->System_SetState(HGE_HIDEMOUSE, false);
    
    /** 窗口宽高 */
    CRect rcWin;
    GetWindowRect(&rcWin);
    m_pHGE->System_SetState(HGE_SCREENWIDTH, rcWin.Width());
    m_pHGE->System_SetState(HGE_SCREENHEIGHT, rcWin.Height());
    
    if(!m_pHGE->System_Initiate())
    {
        ::AfxMessageBox(L"无法初始化房间渲染内核对象。");
        exit(0);
    }

    /** 字体资源 */
    OUFontLoader::instance().AddXmlLoader(string("GUI/FontFamily.xml"));

    /** 配置游戏世界 */
    g_pGameWorld = new OURenderWorld();
    if(NULL == g_pGameWorld)
    {
        ::AfxMessageBox(L"无法创建房间世界。");
        exit(0);
    }

    g_pGameWorld->SetGameSize(rcWin.Width(), rcWin.Height());

    m_hHGEThread = ::CreateThread(NULL, 0, __HGEThread, (LPVOID)m_pHGE, 0, &m_dwHGEThreadID);
    if(0 == m_hHGEThread)
    {
        ::AfxMessageBox(L"无法创建房间渲染线程。");
        exit(0);
    }
}

void COUWorldDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COUWorldDlg::OnPaint()
{
    return;
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COUWorldDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void COUWorldDlg::OnClose()
{
    g_pGameWorld->Shutdown();
    ::WaitForSingleObject(m_hHGEThread, INFINITE);
    ::CloseHandle(m_hHGEThread);

    delete g_pGameWorld;

    m_pHGE->System_Shutdown();
    m_pHGE->Release();

    ::OUFontLoader::instance().Release();

    CDialog::OnClose();
}

