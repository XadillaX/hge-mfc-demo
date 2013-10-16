#ifndef __OUMAPITEMROLE_H__
#define __OUMAPITEMROLE_H__
#pragma once
#include "oumapitemobject.h"
#include "oumapresourcerole.h"

class OUMapItemRole : public OUMapItemObject
{
public:
    OUMapItemRole(void);
    virtual ~OUMapItemRole(void);

    /** 只是demo，具体请自行实现 */
    void                        Release();
    bool                        SetResource(DWORD dwID);

    virtual bool                IsInsideScreen(float fStartX, float fStartY, float fScreenWidth, float fScreenHeight);
    virtual void                Update(float fDelta);
    virtual void                Render(float fStartX, float fStartY);

    void                        SetState(OUMAPROLE_ACT_STATE state);
    void                        SetFaceDir(OUMAPROLE_FACE_DIR dir);
    void                        SetStateAndFaceDir(OUMAPROLE_ACT_STATE state, OUMAPROLE_FACE_DIR dir);

    void                        SetOrder(int o) { m_nOrder = o; }
    int                         GetOrder()      { return m_nOrder; }

protected:
    hgeAnimation*               GetCurAnimation();

protected:
    hgeAnimation*               m_pStanding[8];
    hgeAnimation*               m_pRun[8];

    OUMAPROLE_ACT_STATE         m_emState;
    OUMAPROLE_FACE_DIR          m_emFaceDir;

    int                         m_nOrder;
};

#endif
