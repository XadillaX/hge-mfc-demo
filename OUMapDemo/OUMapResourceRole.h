#ifndef __OUMAPRESOURCEROLE_H__
#define __OUMAPRESOURCEROLE_H__
#pragma once
#include "oumapresourceobject.h"

#define OUMAP_RES_PATH_ROLE  "role\\role"

enum OUMAPROLE_FACE_DIR
{
    OURFD_UP                    = 0,
    OURFD_UPRIGHT,
    OURFD_RIGHT,
    OURFD_DOWNRIGHT,
    OURFD_DOWN,
    OURFD_DOWNLEFT,
    OURFD_LEFT,
    OURFD_UPLEFT,

    OURFD_MAX
};

enum OUMAPROLE_ACT_STATE
{
    OURAS_STANDING,
    OURAS_RUN,

    OURAS_MAX
};

class OUMapResourceRole : public OUMapResourceObject
{
public:
    OUMapResourceRole(void);
    virtual ~OUMapResourceRole(void);

    virtual bool                Load(DWORD dwID);
    virtual void                Render(float fX, float fY);
    virtual void                Update(float fDeltaTime);

    void                        Release();
    bool                        SetCurAnimation(OUMAPROLE_ACT_STATE state, OUMAPROLE_FACE_DIR dir);

    hgeAnimation*               GetAnimation(OUMAPROLE_ACT_STATE state, OUMAPROLE_FACE_DIR dir);
    hgeAnimation**              GetAnimationArray(OUMAPROLE_ACT_STATE state);

    static void                 ReleaseAll();
    static HTEXTURE             GetGlobalStandingTexture(DWORD dwID)
    {
        if(m_hStandingMap.find(dwID) == m_hStandingMap.end())
        {
            LoadGlobalTexture(dwID);
        }
        if(m_hStandingMap.find(dwID) == m_hStandingMap.end()) return 0;

        return m_hStandingMap[dwID];
    }

    static HTEXTURE             GetGlobalRunTexture(DWORD dwID)
    {
        if(m_hRunMap.find(dwID) == m_hRunMap.end())
        {
            LoadGlobalTexture(dwID);
        }
        if(m_hRunMap.find(dwID) == m_hRunMap.end()) return 0;

        return m_hRunMap[dwID];
    }

private:
    static void                 LoadGlobalTexture(DWORD dwID);

private:
    hgeAnimation*               m_pStanding[8];
    hgeAnimation*               m_pRun[8];

    hgeAnimation*               m_pCur;

    HTEXTURE                    m_hStanding;
    HTEXTURE                    m_hRun;

    static map<DWORD, HTEXTURE> m_hStandingMap;
    static map<DWORD, HTEXTURE> m_hRunMap;
};
#endif
