#ifndef __OUBASEROLE_H__
#define __OUBASEROLE_H__
#pragma once
#include "oumapitemrole.h"
#include "oumapastar.h"

class OUMapView;
class OUMapAStar;
typedef OUMapView OUMap;

#define OUROLE_DEFAULT_SPEEDX   100.0f
#define OUROLE_DEFAULT_SPEEDY   50.0f

class OUBaseRole : public OUMapItemRole
{
public:
    OUBaseRole(void);
    virtual ~OUBaseRole(void);

    virtual void                SetMap(OUMap* map);
    void                        Goto(float truex, float truey);

    virtual void                Update(float fDelta);
    virtual void                UpdateMove(float fDelta);

    void                        SetRoleCoor(int x, int y);

protected:
    virtual void                MoveToCoorThisFrame(float x, float y);

protected:
    OUMap*                      m_pMap;
    OUMapAStar*                 m_pAStar;

    float                       m_fTargetX;
    float                       m_fTargetY;

    float                       m_fNextX;
    float                       m_fNextY;
};

#endif
