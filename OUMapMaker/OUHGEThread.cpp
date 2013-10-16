#include "StdAfx.h"
#include "OUHGEThread.h"
#include "OUMapView.h"

HGE* __hge = NULL;
OUMapView* g_pView;
::CRITICAL_SECTION g_csRenderLock;

bool __HGEUpdate()
{
    static float dt;
    dt = __hge->Timer_GetDelta();
    g_pView->OnUpdate(dt);

    return false;
}

bool __HGERender()
{
    __hge->Gfx_BeginScene(0);
    __hge->Gfx_Clear(0xff000000);

    if(g_pView) g_pView->Render();

    __hge->Gfx_EndScene();

    return false;
}

DWORD WINAPI __HGETHREAD(LPVOID lpParam)
{
    __hge = (HGE*)lpParam;
    MSG msg;

    g_pView = new OUMapView();

    for(;;)
    {
        if(__hge->System_GetState(HGE_HWND))
        {
            ::EnterCriticalSection(&g_csRenderLock);

            __hge->System_Start();
            __HGEUpdate();
            __HGERender();

            ::LeaveCriticalSection(&g_csRenderLock);
        }

        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT) break;
            DispatchMessage(&msg);
        }

        Sleep(1);
    }

    delete g_pView;
    ::DeleteCriticalSection(&g_csRenderLock);

    return 0;
}
