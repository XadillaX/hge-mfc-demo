#include "StdAfx.h"
#include "OURenderWorld.h"
#include <stdio.h> 
#include <io.h> 
#include <fcntl.h> 
#include "../OUPlazaRender/OUAniFile.h"

int hCrt = 0;

OURenderWorld::OURenderWorld(void) :
    m_bShouldShutdown(false),
    m_bLastLButton(false),
    m_bLastRButton(false)
{
    //FreeConsole(); 
    //if(AllocConsole()) 
    //{ 
    //    hCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT); 
    //    *stdout = *(::_fdopen(hCrt, "w")); 
    //    ::setvbuf(stdout, NULL, _IONBF, 0); 
    //    *stderr = *(::_fdopen(hCrt, "w")); 
    //    ::setvbuf(stderr, NULL, _IONBF, 0); 
    //} 

    m_pHGE = hgeCreate(HGE_VERSION);

    m_szTopText[0] = '\0';

    /** 初始化GUI环境 */
    OUGUIObject::InitializeGUIEnviroment();
    /** 初始化动画环境 */
    OUAniFile::InitAll();

    /** 载入GUI */
    m_pGUIManager = new OUGUIManager();
    m_pGUIManager->SetSize(m_pHGE->System_GetState(HGE_SCREENWIDTH), m_pHGE->System_GetState(HGE_SCREENHEIGHT));
    m_pGUIManager->LoadXml(string("GUI/DefaultGUI.xml"));

    /** 注册委托回调函数 */
    ::ADDCALLBACKEX("EchoName", EchoName);
    ::ADDCALLBACKEX("DisableIt", DisableIt);

    /** 初始化地图环境 */
    OUMapTester::InitAll();

    m_pMap = new OUMapTester();
    if(!m_pMap->LoadMap("map"))
    {
        SAFEDEL(m_pMap);
    }
    else m_pMap->MoveArea(0, 0, 0);
}

OURenderWorld::~OURenderWorld(void)
{
    SAFEDEL(m_pMap);

    OUMapTester::ReleaseAll();
    OUGUIObject::ReleaseGUIEnviroment();
    OUAniFile::ReleaseAll();

    delete m_pGUIManager;
    m_pHGE->Release();
    OUGUICallBackObject::Clear();

    //FreeConsole();
}

void OURenderWorld::OnRender()
{
    m_pHGE->Gfx_Clear(0xffffffff);

    m_pMap->GetMapSprite()->Render(0, 0);

    m_pGUIManager->RenderGUI();

    for(int i = 1; i < m_Coor.size(); i++)
    {
        m_pHGE->Gfx_RenderLine(m_Coor[i - 1].x, m_Coor[i - 1].y, m_Coor[i].x, m_Coor[i].y, 0xffff0000);
    }

    //m_pHGE->Gfx_RenderLine(0, m_Mouse.y, m_dwGameWidth, m_Mouse.y, 0xff00ff00);
    //m_pHGE->Gfx_RenderLine(m_Mouse.x, 0, m_Mouse.x, m_dwGameHeight, 0xff00ff00);

    OUFontLoader::instance().GetFont(string("defultGUI"))->SetColor(0xff00ff00);
    OUFontLoader::instance().GetFont(string("defultGUI"))->Printf(10, 10, m_szTopText);
}

void OURenderWorld::OnUpdate(float fDeltaTime)
{
    /** 退出线程 */
    if(m_bShouldShutdown.GetValue()) return;

    /** 获取鼠标位置 */
    float mx, my;
    m_pHGE->Input_GetMousePos(&mx, &my);

    /** 一些状态 */
    int lstate = m_pHGE->Input_GetKeyState(HGEK_LBUTTON);
    int rstate = m_pHGE->Input_GetKeyState(HGEK_RBUTTON);
    int fps = m_pHGE->Timer_GetFPS();
    float dt = m_pHGE->Timer_GetDelta();

    /** GUI系统 */
    m_pGUIManager->SetSize(m_pHGE->System_GetState(HGE_SCREENWIDTH), m_pHGE->System_GetState(HGE_SCREENHEIGHT));
    m_pGUIManager->UpdateGUI(dt, mx, my);

    /** 地图世界大小 */
    OUPOINT wsize = m_pMap->GetSize();
    OUPOINT wtsize = m_pMap->GetTrueSize();

    /** 更新地图 */
    static int dirx[4] = { -1, 0, 1, 0 };
    static int diry[4] = { 0, -1, 0, 1 };
    OUPOINT pos = m_pMap->GetTruePosOfScreen();
    bool mousemoved = false;
    if(mx <= 3)
    {
        pos.m_fX -= 10;
        mousemoved = true;
        mx = 3;
    }
    if(my <= 5)
    {
        pos.m_fY -= 10;
        mousemoved = true;
        my = 3;
    }
    if(mx >= 797)
    {
        pos.m_fX += 10;
        mousemoved = true;
        mx = 797;
    }
    if(my >= 597)
    {
        pos.m_fY += 10;
        mousemoved = true;
        my = 597;
    }
    if(mousemoved) m_pHGE->Input_SetMousePos(mx, my);

    if(!mousemoved)
    {
        for(int i = HGEK_LEFT; i <= HGEK_DOWN; i++)
        {
            if(m_pHGE->Input_GetKeyState(i))
            {
                pos.m_fX += (dirx[i - HGEK_LEFT] * 5);
                pos.m_fY += (diry[i - HGEK_LEFT] * 5);
            }   
        }
    }
    static bool lastspace = false;
    if(!m_pHGE->Input_GetKeyState(HGEK_SPACE) && lastspace) OUMapTester::SetShowGrid(!OUMapTester::GetShowGrid());
    lastspace = m_pHGE->Input_GetKeyState(HGEK_SPACE);
    m_pMap->MoveArea(pos.m_fX, pos.m_fY, fDeltaTime);

    /** 左上角坐标 */
    OUPOINT mpos = m_pMap->GetTruePosOfScreen();

    /** Debug字 */
    wsprintf(m_szTopText, L"FPS: %d\n鼠标位置: (%d, %d)\n地图大小：%d x %d\n世界大小: %d x %d\n左上角坐标: (%d, %d)\n",
        fps,
        (int)mx, (int)my,
        (int)wsize.m_fX, (int)wsize.m_fY,
        (int)wtsize.m_fX, (int)wtsize.m_fY,
        (int)mpos.m_fX, (int)mpos.m_fY
        );

    /** 鼠标痕迹线 */
    m_Mouse.x = mx;
    m_Mouse.y = my;
    if(lstate)
    {
        if(m_Coor.size() > 100) m_Coor.erase(m_Coor.begin());
        m_Coor.push_back(m_Mouse);
    }
    else
    if(!lstate)
    {
        m_Coor.clear();
    }

    m_bLastLButton = lstate;
    m_bLastRButton = rstate;

    return;
}

void OURenderWorld::Shutdown()
{
    m_bShouldShutdown.SetValue(true);
}

bool OURenderWorld::ShouldShutdown()
{
    return m_bShouldShutdown.GetValue();
}

HGE* OURenderWorld::GetHGE()
{
    return m_pHGE;
}

void OURenderWorld::SetGameSize(DWORD dwWidth, DWORD dwHeight)
{
    m_dwGameWidth = dwWidth;
    m_dwGameHeight = dwHeight;
}

void OURenderWorld::GetGameSize(DWORD &dwWidth, DWORD &dwHeight)
{
    dwWidth = m_dwGameWidth;
    dwHeight = m_dwGameHeight;
}
