#include "StdAfx.h"
#include "OUHGEThread.h"

HGE* __hge = NULL;
::CRITICAL_SECTION g_csRenderLock;

bool __HGEUpdate()
{
    static float dt;
    dt = __hge->Timer_GetDelta();

    if(g_pView) g_pView->OnUpdate(dt);

    return false;
}

bool __HGERender()
{
    __hge->Gfx_BeginScene(0);
    __hge->Gfx_Clear(0xffcccccc);

    if(g_pView)
    {
        g_pView->Render();
        
        /** 状态 */
        if(g_pView->GetShowState()) g_pView->RenderState();
    }

    __hge->Gfx_EndScene();

    return false;
}

void __HGETimerFunc(LPVOID lpParam)
{
    __hge = (HGE*)lpParam;

    if(__hge->System_GetState(HGE_HWND))
    {
        __hge->System_Start();
        __HGERender();
    }
}

DWORD WINAPI __HGETHREAD(LPVOID lpParam)
{
    return 0;
    __hge = (HGE*)lpParam;
    MSG msg;

    g_pView = new OUMapView();

    for(;;)
    {
        if(__hge->System_GetState(HGE_HWND))
        {
            //::EnterCriticalSection(&g_csRenderLock);

            __hge->System_Start();
            __HGERender();

            //::LeaveCriticalSection(&g_csRenderLock);
        }

        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT) break;
            DispatchMessage(&msg);
        }
    }

    delete g_pView;
    ::DeleteCriticalSection(&g_csRenderLock);

    return 0;
}
