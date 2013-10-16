#ifndef __OURENDER_H__
#define __OURENDER_H__
#pragma once
#include "global.h"
#include "OUMapView.h"
#include <vector>
using namespace std;

class OURender
{
public:
    OURender(void);
    ~OURender(void);

    void                        Update(float fDelta);
    void                        Render();

    /** 重新获取双重缓冲 */
    void                        RestoreTarget()                     { if(m_pMap) m_pMap->RestoreTarget(); }

    OUPOINT                     GetRoleTruePos()                    { OUPOINT pt; pt.m_fX = m_pMyRole->GetX(), pt.m_fY = m_pMyRole->GetY(); return pt; }

private:
    HGE*                        m_pHGE;
    OUMapView*                  m_pMap;

    OUBaseRole*                 m_pMyRole;
    vector<OUBaseRole*>         m_pRoles;
};

#endif
