#ifndef __OUMAPITEMOBJECT_H__
#define __OUMAPITEMOBJECT_H__
#pragma once
#include "global.h"
#include "oumapresourceobject.h"

enum OUMAPITEM_TYPE
{
    OUMIT_NONE,
    OUMIT_BUILDING,
    OUMIT_TABLE,
    OUMIT_ROLE,

    OUMIT_MAX
};

class OUMapItemObject
{
public:
    OUMapItemObject(void);
    virtual ~OUMapItemObject(void);

    virtual bool                IsInsideScreen(float fStartX, float fStartY, float fScreenWidth, float fScreenHeight);

    virtual void                ResponseInput(float itemX, float itemY)     {  }
    virtual void                Update(float fDelta)                        {  }
    virtual void                Render(float fStartX, float fStartY)        {  }

    virtual void                SetZ(float z)                               {  }
    virtual float               GetZ()                                      { return 1.0f; }

    float                       GetX()                                      { return m_fWorldX; }
    float                       GetY()                                      { return m_fWorldY; }
    void                        SetX(float x)                               { m_fWorldX = x; }
    void                        SetY(float y)                               { m_fWorldY = y; }

    OUMAPITEM_TYPE              GetObjectType()                             { return m_emClassType; }

protected:
    OUMAPITEM_TYPE              m_emClassType;

    HGE*                        m_pHGE;
    OUMapResourceObject*        m_pSprite;

    float                       m_fWorldX;
    float                       m_fWorldY;
    float                       m_fWidth;
    float                       m_fHeight;
};

#endif
