#ifndef __OUMAPVIEW_H__
#define __OUMAPVIEW_H__
#pragma once
#include "../OUPlazaRender/hge.h"
#include "../OUPlazaGUISystem/Global.h"
#include <map>
#include <set>
#include <vector>
#include "../OUPlazaGUISystem/OUFont.h"
#include "utils.h"
using namespace std;

class OUMapView
{
public:
    OUMapView(void);
    virtual ~OUMapView(void);

    void                        OnUpdate(float fDelta);
    void                        Render();

private:
    HGE*                        m_pHGE;

    DWORD                       m_dwWidth;
    DWORD                       m_dwHeight;
    DWORD                       m_dwTrueWidth;
    DWORD                       m_dwTrueHeight;
};
#endif
