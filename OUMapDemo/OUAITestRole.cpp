#include "StdAfx.h"
#include "OUAITestRole.h"
#include <cstdlib>
#include "OUMapView.h"

OUAITestRole::OUAITestRole(void)
{
    m_pAStar = NULL;
    m_pMap = NULL;
}

OUAITestRole::~OUAITestRole(void)
{
    SAFEDEL(m_pAStar);
}

void OUAITestRole::SetMap(OUMap* map)
{
    m_pMap = map;
    OUMapAStar* star = new OUMapAStar();
    star->SetMap(m_pMap->GetWidth(), m_pMap->GetHeight(), m_pMap);

    m_pAStar = star;
}

void OUAITestRole::RandomPos()
{
    VERIFY(m_pMap != NULL);

    int x = rand() % m_pMap->GetWidth();
    int y = rand() % m_pMap->GetHeight();
    while(!m_pAStar->CheckCoor(x, y))
    {
        x = rand() % m_pMap->GetWidth();
        y = rand() % m_pMap->GetHeight();
    }

    OUPOINT pt = m_pMap->GetCenterOfCoor(x, y);
    m_fWorldX = pt.m_fX;
    m_fWorldY = pt.m_fY;

    SetStateAndFaceDir(OUMAPROLE_ACT_STATE::OURAS_STANDING, (OUMAPROLE_FACE_DIR)(rand() % 8));

    this->CreateState();
}

void OUAITestRole::Update(float fDelta)
{
    OUBaseRole::Update(fDelta);

    if(m_fTargetX < 0 && m_fTargetY < 0 && !m_bWaiting)
    {
        CreateState();
    }
    else
    if(m_bWaiting)
    {
        m_fNowWaitTime += fDelta;
        if(m_fNowWaitTime >= m_fWaitTime)
        {
            CreateState();
        }
    }
}

void OUAITestRole::CreateState()
{
    int prob = rand() % 100;
    if(prob < 50)
    {
        m_bWaiting = true;
        m_fWaitTime = g_pHGE->Random_Float(1.0, 5.0f);
        m_fNowWaitTime = 0.0f;
    }
    else
    {
        m_bWaiting = false;

        int x = rand() % m_pMap->GetWidth();
        int y = rand() % m_pMap->GetHeight();
        while(!m_pAStar->CheckCoor(x, y))
        {
            x = rand() % m_pMap->GetWidth();
            y = rand() % m_pMap->GetHeight();
        }

        OUPOINT pt = m_pMap->GetCenterOfCoor(x, y);
        Goto(pt.m_fX, pt.m_fY);
    }
}
