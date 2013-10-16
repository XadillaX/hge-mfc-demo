
// OUMapDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OUMapDemo.h"
#include "OUMapDemoDlg.h"
#include "OURender.h"

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


// COUMapDemoDlg 对话框




COUMapDemoDlg::COUMapDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COUMapDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pHGE = NULL;
}

COUMapDemoDlg::~COUMapDemoDlg()
{
}

void COUMapDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COUMapDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_CLOSE()
    ON_WM_DESTROY()
    ON_WM_TIMER()
END_MESSAGE_MAP()


// COUMapDemoDlg 消息处理程序

BOOL COUMapDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	// TODO: 在此添加额外的初始化代码
    MoveWindow(0, 0, OUSCREEN_WIDTH, OUSCREEN_HEIGHT);
    if(!InitHGEDlg())
    {
        this->MessageBox(L"内核引擎初始化失败。", L"失败", MB_ICONERROR);
        this->SendMessage(WM_CLOSE);
    }

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

int hCrt = 0;

bool COUMapDemoDlg::InitHGEDlg()
{
#ifdef _DEBUG
    FreeConsole();
    if(AllocConsole()) 
    { 
        hCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT); 
        *stdout = *(::_fdopen(hCrt, "w"));
        ::setvbuf(stdout, NULL, _IONBF, 0);
        *stderr = *(::_fdopen(hCrt, "w"));
        ::setvbuf(stderr, NULL, _IONBF, 0);
    }
#endif

    m_pHGE = hgeCreate(HGE_VERSION);
    g_pHGE = m_pHGE;

    m_pHGE->System_SetState(HGE_HWNDPARENT, GetSafeHwnd());
    m_pHGE->System_SetState(HGE_SCREENWIDTH, OUSCREEN_WIDTH);
    m_pHGE->System_SetState(HGE_SCREENHEIGHT, OUSCREEN_HEIGHT);
    m_pHGE->System_SetState(HGE_FPS, 100);
    m_pHGE->System_SetState(HGE_SCREENBPP, 32);
    m_pHGE->System_SetState(HGE_WINDOWED, true);
    m_pHGE->System_SetState(HGE_HIDEMOUSE, false);

    m_pHGE->System_SetState(HGE_FRAMEFUNC, OUHGE_UPDATE);
    m_pHGE->System_SetState(HGE_RENDERFUNC, OUHGE_RENDER);
    m_pHGE->System_SetState(HGE_GFXRESTOREFUNC, OUHGE_RESTORETARGET);

    /** 字体 */
    OUFontLoader::instance().AddXmlLoader(string("GUI/FontFamily.xml"));

    /** 初始化 */
    if(!m_pHGE->System_Initiate()) return false;

    /** 动画环境 */
    OUAniFile::InitAll();

    /** 渲染对象 */
    g_pRender = new OURender();

    /** 开始 */
    if(!m_pHGE->System_Start()) return false;

    SetTimer(OUIDE_HGE_TIMER, 10, NULL);

    return true;
}

void COUMapDemoDlg::ReleaseHGE()
{
#ifdef _DEBUG
    FreeConsole();
#endif

    if(m_pHGE)
    {
        SAFEDEL(g_pRender);

        OUAniFile::ReleaseAll();
        g_pHGE = NULL;
        m_pHGE->System_Shutdown();
        m_pHGE->Release();
        m_pHGE = NULL;
    }
}

void COUMapDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void COUMapDemoDlg::OnPaint()
{
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
HCURSOR COUMapDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void COUMapDemoDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    

    CDialog::OnClose();
}

void COUMapDemoDlg::OnDestroy()
{
    KillTimer(OUIDE_HGE_TIMER);
    ReleaseHGE();
    CDialog::OnDestroy();

    // TODO: 在此处添加消息处理程序代码
}

void COUMapDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    switch(nIDEvent)
    {
    case OUIDE_HGE_TIMER:
        {
            if(!m_pHGE) break;
            m_pHGE->System_Start();

            break;
        }
    }

    CDialog::OnTimer(nIDEvent);
}
