
// OUMapDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OUMapDemo.h"
#include "OUMapDemoDlg.h"
#include "OURender.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// COUMapDemoDlg �Ի���




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


// COUMapDemoDlg ��Ϣ�������

BOOL COUMapDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    MoveWindow(0, 0, OUSCREEN_WIDTH, OUSCREEN_HEIGHT);
    if(!InitHGEDlg())
    {
        this->MessageBox(L"�ں������ʼ��ʧ�ܡ�", L"ʧ��", MB_ICONERROR);
        this->SendMessage(WM_CLOSE);
    }

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

    /** ���� */
    OUFontLoader::instance().AddXmlLoader(string("GUI/FontFamily.xml"));

    /** ��ʼ�� */
    if(!m_pHGE->System_Initiate()) return false;

    /** �������� */
    OUAniFile::InitAll();

    /** ��Ⱦ���� */
    g_pRender = new OURender();

    /** ��ʼ */
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void COUMapDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR COUMapDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void COUMapDemoDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    

    CDialog::OnClose();
}

void COUMapDemoDlg::OnDestroy()
{
    KillTimer(OUIDE_HGE_TIMER);
    ReleaseHGE();
    CDialog::OnDestroy();

    // TODO: �ڴ˴������Ϣ����������
}

void COUMapDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
