#ifndef __OUAITESTROLE_H__
#define __OUAITESTROLE_H__
#pragma once
#include "oubaserole.h"

class OUAITestRole : public OUBaseRole
{
public:
    OUAITestRole(void);
    virtual ~OUAITestRole(void);

    virtual void                SetMap(OUMap* map);
    void                        RandomPos();

    virtual void                Update(float fDelta);

private:
    void                        CreateState();

private:
    bool                        m_bWaiting;
    float                       m_fWaitTime;
    float                       m_fNowWaitTime;
};

#endif
