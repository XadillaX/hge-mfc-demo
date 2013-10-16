#include "StdAfx.h"
#include "Global.h"
#include "OURender.h"

HGE* g_pHGE = NULL;
OURender* g_pRender = NULL;

bool operator < (const OUMapCoor a, const OUMapCoor b)
{
    if(a.y < b.y) return true;
    if(a.y > b.y) return false;
    return a.x < b.x;
}

bool OUHGE_UPDATE()
{
    if(!g_pHGE) return false;

    float fDelta = g_pHGE->Timer_GetDelta();
    if(g_pRender) g_pRender->Update(fDelta);

    return false;
}

bool OUHGE_RENDER()
{
    if(!g_pHGE) return false;
    OUFont* fnt = OUFontLoader::instance().GetFont(string("ËÎÌו"));
    fnt->SetColor(0xff00ff00);

    static char s[2048];
    OUPOINT rpt = g_pRender->GetRoleTruePos();
    sprintf(s, "FPS: %d\nRole: (%.0f, %.0f)\n", g_pHGE->Timer_GetFPS(), rpt.m_fX, rpt.m_fY);

    g_pHGE->Gfx_BeginScene();
    g_pHGE->Gfx_Clear(0xff000000);

    if(g_pRender) g_pRender->Render();
    fnt->Print(10, 10, s);

    g_pHGE->Gfx_EndScene();

    return false;
}

bool OUHGE_RESTORETARGET()
{
    if(!g_pHGE) return false;
    if(!g_pRender) return false;

    g_pRender->RestoreTarget();
    return false;
}
