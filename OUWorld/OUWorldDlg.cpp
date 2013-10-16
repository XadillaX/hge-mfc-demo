// OUWorldDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OUWorld.h"
#include "OUWorldDlg.h"
#include "CHGEThread.h"

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


// COUWorldDlg �Ի���




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


// COUWorldDlg ��Ϣ�������

BOOL COUWorldDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    MoveWindow(0, 0, 800, 600);

    /** ��ʼ��HGE */
    InitializeHGE();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

#include "../OUPlazaGUISystem/OUFont.h"
void COUWorldDlg::InitializeHGE()
{
    m_pHGE = hgeCreate(HGE_VERSION);
    if(m_pHGE == NULL)
    {
        ::AfxMessageBox(L"�޷�����������Ⱦ�ں˶���");
        exit(0);
    }

    m_pHGE->System_SetState(HGE_HWNDPARENT, this->m_hWnd);
    m_pHGE->System_SetState(HGE_FRAMEFUNC, __HGEUpdate);
    m_pHGE->System_SetState(HGE_RENDERFUNC, __HGERender);
    m_pHGE->System_SetState(HGE_FPS, 100);
    m_pHGE->System_SetState(HGE_ZBUFFER, true);
    m_pHGE->System_SetState(HGE_HIDEMOUSE, false);
    
    /** ���ڿ�� */
    CRect rcWin;
    GetWindowRect(&rcWin);
    m_pHGE->System_SetState(HGE_SCREENWIDTH, rcWin.Width());
    m_pHGE->System_SetState(HGE_SCREENHEIGHT, rcWin.Height());
    
    if(!m_pHGE->System_Initiate())
    {
        ::AfxMessageBox(L"�޷���ʼ��������Ⱦ�ں˶���");
        exit(0);
    }

    /** ������Դ */
    OUFontLoader::instance().AddXmlLoader(string("GUI/FontFamily.xml"));

    /** ������Ϸ���� */
    g_pGameWorld = new OURenderWorld();
    if(NULL == g_pGameWorld)
    {
        ::AfxMessageBox(L"�޷������������硣");
        exit(0);
    }

    g_pGameWorld->SetGameSize(rcWin.Width(), rcWin.Height());

    m_hHGEThread = ::CreateThread(NULL, 0, __HGEThread, (LPVOID)m_pHGE, 0, &m_dwHGEThreadID);
    if(0 == m_hHGEThread)
    {
        ::AfxMessageBox(L"�޷�����������Ⱦ�̡߳�");
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void COUWorldDlg::OnPaint()
{
    return;
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

