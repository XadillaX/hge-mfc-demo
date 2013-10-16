/***********************************
 *
 * O游棋牌大厅世界
 * - 渲染世界 -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OURENDERWORLD_H__
#define __OURENDERWORLD_H__
#pragma once

#include "../OUPlazaGUISystem/OUFont.h"
#include <vector>
#include "../OUPlazaGUISystem/OUGUIManager.h"
#include "../OUPlazaGUISystem/OUGUICallBackObject.h"
#include "OUMapTester.h"
using namespace std;

struct coor
{
    int x, y;
};

#define FMTCALLBACKFUNC(p) (CALLBACK_FUNC)(&OURenderWorld::p)
#define GENCALLBACKPTR(obj, p) (obj->*p)

#define ADDCALLBACKEX(name, func) OUGUICallBackObject::AddCallBack(name, FMTCALLBACKFUNC(func), this)
#define RUNCALLBACKEX(name, sender, e) OUGUICallBackObject::RunCallBack(name, sender, e)

class OURenderWorld : public OUGUICallBackObject
{
public:
    OURenderWorld(void);
    virtual ~OURenderWorld(void);

    void                        OnRender();
    void                        OnUpdate(float fDeltaTime);

    void                        Shutdown();
    bool                        ShouldShutdown();
    HGE*                        GetHGE();

    void                        SetGameSize(DWORD dwWidth, DWORD dwHeight);
    void                        GetGameSize(DWORD& dwWidth, DWORD& dwHeight);

    void                        EchoName(OUGUIObject* obj, OUGUIEvent* e)
    {
        ::MessageBoxA(NULL, obj->GetKey().c_str(), "echo", 0);
    }

    void                        DisableIt(OUGUIObject* obj, OUGUIEvent* e)
    {
        obj->SetEnableMode(false);
    }

private:
    OUSafeObj<bool>             m_bShouldShutdown;
    HGE*                        m_pHGE;

    /** 世界宽高 */
    DWORD                       m_dwGameWidth;
    DWORD                       m_dwGameHeight;

    TCHAR                       m_szTopText[256];

    bool                        m_bLastLButton;
    bool                        m_bLastRButton;
    vector<coor>                m_Coor;
    coor                        m_Mouse;

    OUGUIManager*               m_pGUIManager;

    OUMapTester*                m_pMap;
};

#endif
