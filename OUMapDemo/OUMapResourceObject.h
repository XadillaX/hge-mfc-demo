#ifndef __OUMAPRESOURCEOBJECT_H__
#define __OUMAPRESOURCEOBJECT_H__
#pragma once
#include "global.h"

class OUMapResourceObject
{
public:
    OUMapResourceObject(void);
    virtual ~OUMapResourceObject(void);

    virtual bool                Load(DWORD dwID)                = 0;
    virtual void                Update(float fDeltaTime)        = 0;
    virtual void                Render(float fX, float fY)      = 0;
    virtual void                RenderStretch(float x1, float y1, float w, float h) {  }
    virtual void                RenderCover(float fX, float fY) {  }

    virtual void                SetColor(DWORD dwColor)         {  }
    virtual DWORD               GetColor()                      { return 0; }
    virtual void                SetZ(float z)                   {  }
    virtual float               GetZ()                          { return 1.0f; }

    DWORD                       GetResourceID()                 { return m_dwID; }
    float                       GetWidth()                      { return m_fWidth; }
    float                       GetHeight()                     { return m_fHeight; }
    float                       GetSize(float* w, float* h)     { *w = m_fWidth, *h = m_fHeight; }

protected:
    DWORD                       m_dwID;

    float                       m_fWidth;
    float                       m_fHeight;

    HGE*                        m_pHGE;
};

#endif
