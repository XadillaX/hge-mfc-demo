#include "StdAfx.h"
#include "OUMapResourceRole.h"

map<DWORD, HTEXTURE> OUMapResourceRole::m_hStandingMap;
map<DWORD, HTEXTURE> OUMapResourceRole::m_hRunMap;

OUMapResourceRole::OUMapResourceRole(void) :
    m_pCur(NULL),
    m_hStanding(0),
    m_hRun(0)
{
    memset(m_pStanding, 0, sizeof(m_pStanding));
    memset(m_pRun, 0, sizeof(m_pRun));
}

OUMapResourceRole::~OUMapResourceRole(void)
{
    Release();
}

void OUMapResourceRole::LoadGlobalTexture(DWORD dwID)
{
    char filename[512];
    sprintf(filename, "%s%d.ini", OUMAP_RES_PATH_ROLE, dwID);

    string sfilename, rfilename;
    OUIniFile ini(filename);

    sfilename = ini.GetString("role", "standing");
    rfilename = ini.GetString("role", "run");

    OUPACKFILEDATA data = OUResourcePool::instance().GetResource(sfilename);
    if(data.m_dwFileSize == 0) return;
    HTEXTURE s = g_pHGE->Texture_Load((char*)data.m_pBuf, data.m_dwFileSize);
    data = OUResourcePool::instance().GetResource(rfilename);
    if(data.m_dwFileSize == 0)
    {
        g_pHGE->Texture_Free(s);
        return;
    }
    HTEXTURE r = g_pHGE->Texture_Load((char*)data.m_pBuf, data.m_dwFileSize);

    m_hStandingMap[dwID] = s;
    m_hRunMap[dwID] = r;
}

bool OUMapResourceRole::Load(DWORD dwID)
{
    Release();

    char filename[512];
    sprintf(filename, "%s%d.ini", OUMAP_RES_PATH_ROLE, dwID);

    string sfilename, rfilename;
    OUIniFile ini(filename);

    //sfilename = ini.GetString("role", "standing");
    //rfilename = ini.GetString("role", "run");

    //OUPACKFILEDATA data = OUResourcePool::instance().GetResource(sfilename);
    //if(data.m_dwFileSize == 0) return false;
    //m_hStanding = m_pHGE->Texture_Load((char*)data.m_pBuf, data.m_dwFileSize, true);
    //data = OUResourcePool::instance().GetResource(rfilename);
    //m_hRun = m_pHGE->Texture_Load((char*)data.m_pBuf, data.m_dwFileSize, true);

    m_hStanding = GetGlobalStandingTexture(dwID);
    m_hRun = GetGlobalRunTexture(dwID);
    if(m_hStanding == 0 || m_hRun == 0)
    {
        m_hStanding = 0;
        m_hRun = 0;
        return false;
    }

    int sw, sh;
    int rw, rh;
    sw = m_pHGE->Texture_GetWidth(m_hStanding, true);
    sh = m_pHGE->Texture_GetHeight(m_hStanding, true);
    rw = m_pHGE->Texture_GetWidth(m_hRun, true);
    rh = m_pHGE->Texture_GetHeight(m_hRun, true);

    if(!m_hStanding || !m_hRun)
    {
        Release();
        return false;
    }

    int fps = ini.GetInt("role", "fps", 25);

    int spercount = ini.GetInt("role", "standingpercount", 4);
    int sspotx = ini.GetInt("role", "standingspotx", 0);
    int sspoty = ini.GetInt("role", "standingspoty", 0);
    for(int i = 0; i < 8; i++)
    {
        int x = 0;
        int y = i * (sh / 8);
        m_pStanding[i] = new hgeAnimation(m_hStanding, spercount, fps, x, y, sw / spercount, sh / 8);
        m_pStanding[i]->SetSpeed(fps);
        m_pStanding[i]->SetHotSpot(sspotx, sspoty);
        //m_pStanding[i]->Play();
    }

    int rpercount = ini.GetInt("role", "runpercount", 4);
    int rspotx = ini.GetInt("role", "runspotx", 0);
    int rspoty = ini.GetInt("role", "runspoty", 0);
    for(int i = 0; i < 8; i++)
    {
        int x = 0;
        int y = i * (rh / 8);
        m_pRun[i] = new hgeAnimation(m_hRun, rpercount, fps, x, y, rw / rpercount, rh / 8);
        m_pRun[i]->SetSpeed(fps);
        m_pRun[i]->SetHotSpot(rspotx, rspoty);
        //m_pRun[i]->Play();
    }

    SetCurAnimation(OURAS_STANDING, OURFD_UP);

    return true;
}

void OUMapResourceRole::Release()
{
    for(int i = 0; i < 8; i++)
    {
        SAFEDEL(m_pStanding[i]);
        SAFEDEL(m_pRun[i]);
    }

    memset(m_pStanding, 0, sizeof(m_pStanding));
    memset(m_pRun, 0, sizeof(m_pRun));

    //if(m_hStanding) m_pHGE->Texture_Free(m_hStanding);
    //if(m_hRun) m_pHGE->Texture_Free(m_hRun);

    m_hStanding = 0;
    m_hRun = 0;
    m_pCur = NULL;
}

void OUMapResourceRole::ReleaseAll()
{
    for(map<DWORD, HTEXTURE>::iterator it = m_hStandingMap.begin(); it != m_hStandingMap.end(); it++)
    {
        g_pHGE->Texture_Free(it->second);
    }

    for(map<DWORD, HTEXTURE>::iterator it = m_hRunMap.begin(); it != m_hRunMap.end(); it++)
    {
        g_pHGE->Texture_Free(it->second);
    }

    m_hStandingMap.clear();
    m_hRunMap.clear();
}

bool OUMapResourceRole::SetCurAnimation(OUMAPROLE_ACT_STATE state, OUMAPROLE_FACE_DIR dir)
{
    if(state >= OURAS_MAX) return false;
    if(dir >= OURFD_MAX) return false;

    if(state == OURAS_STANDING)
    {
        m_pCur = m_pStanding[dir];
        m_fWidth = m_pCur->GetWidth();
        m_fHeight = m_pCur->GetHeight();
        return true;
    }
    else
    if(state == OURAS_RUN)
    {
        m_pCur = m_pRun[dir];
        m_fWidth = m_pCur->GetWidth();
        m_fHeight = m_pCur->GetHeight();
        return true;
    }

    return false;
}

void OUMapResourceRole::Update(float fDeltaTime)
{
    if(!m_pCur) return;
    m_pCur->Update(fDeltaTime);
}

void OUMapResourceRole::Render(float fX, float fY)
{
    if(!m_pCur) return;
    m_pCur->Render(fX, fY);
}

hgeAnimation* OUMapResourceRole::GetAnimation(OUMAPROLE_ACT_STATE state, OUMAPROLE_FACE_DIR dir)
{
    if(state >= OURAS_MAX) return NULL;
    if(dir >= OURFD_MAX) return NULL;

    if(state == OURAS_STANDING)
    {
        return m_pStanding[dir];
    }
    else
    if(state == OURAS_RUN)
    {
        return m_pRun[dir];
    }

    return NULL;
}

hgeAnimation** OUMapResourceRole::GetAnimationArray(OUMAPROLE_ACT_STATE state)
{
    if(state >= OURAS_MAX) return NULL;

    if(state == OURAS_STANDING)
    {
        return m_pStanding;
    }
    else
    if(state == OURAS_RUN)
    {
        return m_pRun;
    }

    return NULL;
}
