#include "stdafx.h"
#include "CHGEThread.h"

OURenderWorld* g_pGameWorld = NULL;

DWORD WINAPI __HGEThread(LPVOID lpParam)
{
    HGE* hge = (HGE*)lpParam;
    MSG msg;

    while(g_pGameWorld && !g_pGameWorld->ShouldShutdown())
    {
        hge->System_Start();

        if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT) break;
            ::DispatchMessage(&msg);
        }
    }

    return 0;
}

bool __HGEUpdate()
{
    if(NULL == g_pGameWorld) return true;
    
    static float fDeltaTime;
    fDeltaTime = g_pGameWorld->GetHGE()->Timer_GetDelta();
    g_pGameWorld->OnUpdate(fDeltaTime);

    return false;
}

bool __HGERender()
{
    if(NULL == g_pGameWorld) return true;

    g_pGameWorld->GetHGE()->Gfx_BeginScene();
    g_pGameWorld->GetHGE()->Gfx_Clear(0xff000000);
    g_pGameWorld->OnRender();
    g_pGameWorld->GetHGE()->Gfx_EndScene();

    return false;
}
